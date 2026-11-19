//======================================================================
// EALauncher.cpp  -  Endless Ages launcher / patcher
//
// The only program that downloads anything. It runs before the game:
//   1. asks the content server for its manifest (every client file with
//      size and CRC32, plus the content version)
//   2. checks the files in this folder (CRCs are cached in EALauncher.crc so
//      only files that changed since last time are re-read)
//   3. downloads whatever is missing or different (resumes partial downloads,
//      verifies every file's CRC before replacing the old one)
//   4. updates itself if a newer EALauncher.exe is published
//   5. writes the game server address from the content server (GameServer=
//      in ContentServer.cfg) into coinit.tmp, which the client reads
//   6. PLAY starts EndlessAges.exe; login happens inside the game.
//
// Files that are not in the manifest (settings, logs, coinit.tmp ...) are
// never touched.
//
// EALauncher.cfg (written on first run):
//   Server=127.0.0.1:25858      content server address
//======================================================================
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <windows.h>
#include <windowsx.h>
#include <wininet.h>
#include <shlobj.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <objidl.h>
#include <gdiplus.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "resource.h"
extern "C" {
#include "axcrypt.h"
}

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "advapi32.lib")

// Where a fresh install goes, and the server a fresh install talks to.
#define INSTALL_FOLDER   "EndlessAgesGold"
#define GAME_TITLE       "Endless Ages Gold"
#define DEFAULT_SERVER   "127.0.0.1:25858"   // OVH dedicated server (ContentServer, TCP)

using namespace Gdiplus;

//----------------------------------------------------------------------
// state shared between the UI and the worker thread
//----------------------------------------------------------------------
enum Phase { PH_CONNECT, PH_CHECK, PH_DOWNLOAD, PH_READY, PH_ERROR, PH_OFFLINE, PH_INSTALL, PH_INSTALLING };

static CRITICAL_SECTION g_lock;
static Phase       g_phase = PH_CONNECT;
static std::string g_status = "Connecting to the update server...";
static std::string g_detail;
static double      g_progress = 0.0;      // 0..1
static int         g_version = 0;
static volatile LONG g_workerRunning = 0;
static volatile LONG g_quit = 0;

static HWND        g_wnd = 0;
static std::string g_exeDir, g_exePath, g_server = DEFAULT_SERVER;
static bool        g_installMode = false;   // running from a download, game not installed yet
static FILE *      g_logFile = 0;
static std::string g_manifestGameServer;   // "gameserver" line from the manifest
static std::string g_mirror;               // "mirror" line: fast file host (e.g. Cloudflare R2), files fall back to the content server
static int         g_mirrorFailures = 0;   // stop using the mirror after several failures in a row
static std::string g_updateName;           // "name" line: e.g. "Version 1.0.3" (from Patch\UpdateNotes.txt)
static std::vector<std::string> g_notes;   // "notes" lines: what's new in this update
// update-notes box scrolling (UI thread only)
static int  g_notesScroll = 0, g_notesMax = 0;          // pixels scrolled / max scroll
static RECT g_notesBox = { 0, 0, 0, 0 };                // whole notes box (mouse wheel works over it)
static RECT g_notesTrack = { 0, 0, 0, 0 };              // scroll bar track
static int  g_notesThumbH = 0;                          // scroll bar thumb height
static bool g_notesDrag = false; static int g_notesDragY = 0, g_notesDragStart = 0;

static Image *     g_bg = 0;
static Image *     g_btnUp = 0;
static Image *     g_btnHover = 0;

#define WND_W 800
#define WND_H 600
#define TIMER_REPAINT 1

struct Button { RECT rc; const char * text; bool hover, down, enabled; };
static Button g_play = { { 560, 528, 690, 578 }, "PLAY",  false, false, false };
static Button g_exit = { { 696, 528, 786, 578 }, "EXIT",  false, false, true };
static Button g_retry = { { 424, 528, 554, 578 }, "RETRY", false, false, false };
static Button g_account = { { 288, 528, 418, 578 }, "NEW ACCOUNT", false, false, false };
static Button g_settings = { { 152, 528, 282, 578 }, "SETTINGS", false, false, false };

//----------------------------------------------------------------------
// helpers
//----------------------------------------------------------------------
static void LogLine(const char * fmt, ...)
{
	if (!g_logFile) return;
	char body[1024];
	va_list ap; va_start(ap, fmt);
	_vsnprintf(body, sizeof(body) - 1, fmt, ap); body[sizeof(body) - 1] = 0;
	va_end(ap);
	SYSTEMTIME st; GetLocalTime(&st);
	fprintf(g_logFile, "%04d-%02d-%02d [%02d:%02d:%02d] %s\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, body);
	fflush(g_logFile);
}

static void SetStatus(Phase ph, const std::string & status, const std::string & detail, double progress)
{
	EnterCriticalSection(&g_lock);
	g_phase = ph; g_status = status; g_detail = detail; g_progress = progress;
	LeaveCriticalSection(&g_lock);
}
static void SetDetail(const std::string & detail, double progress)
{
	EnterCriticalSection(&g_lock);
	g_detail = detail; g_progress = progress;
	LeaveCriticalSection(&g_lock);
}

static std::string Lower(std::string s) { for (size_t i = 0; i < s.size(); i++) s[i] = (char)tolower((unsigned char)s[i]); return s; }

static std::string Fmt(const char * fmt, ...)
{
	char b[1024];
	va_list ap; va_start(ap, fmt);
	_vsnprintf(b, sizeof(b) - 1, fmt, ap); b[sizeof(b) - 1] = 0;
	va_end(ap);
	return b;
}

static std::string MB(ULONGLONG bytes) { return Fmt("%.1f MB", bytes / 1048576.0); }

//----------------------------------------------------------------------
// CRC32 (same as the content server)
//----------------------------------------------------------------------
static DWORD g_crcTable[256];
static void InitCrc()
{
	for (DWORD i = 0; i < 256; i++)
	{
		DWORD c = i;
		for (int k = 0; k < 8; k++) c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
		g_crcTable[i] = c;
	}
}
static DWORD CrcUpdate(DWORD crc, const unsigned char * p, DWORD n)
{
	for (DWORD i = 0; i < n; i++) crc = g_crcTable[(crc ^ p[i]) & 0xFF] ^ (crc >> 8);
	return crc;
}
static bool CrcFile(const std::string & path, DWORD & out)
{
	HANDLE h = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
	if (h == INVALID_HANDLE_VALUE) return false;
	std::vector<unsigned char> buf(1 << 20);
	DWORD crc = 0xFFFFFFFFu, got = 0;
	while (!g_quit && ReadFile(h, &buf[0], (DWORD)buf.size(), &got, 0) && got > 0) crc = CrcUpdate(crc, &buf[0], got);
	CloseHandle(h);
	out = crc ^ 0xFFFFFFFFu;
	return !g_quit;
}

//----------------------------------------------------------------------
// CRC cache (EALauncher.crc): "<crc> <size> <mtime> <path>"
//----------------------------------------------------------------------
struct CacheEntry { DWORD crc; ULONGLONG size; ULONGLONG mtime; };
static std::map<std::string, CacheEntry> g_cache;

static void LoadCache()
{
	FILE * f = fopen((g_exeDir + "EALauncher.crc").c_str(), "r");
	if (!f) return;
	char line[1200];
	while (fgets(line, sizeof(line), f))
	{
		unsigned long crc; unsigned long long size, mt; int n = 0;
		if (sscanf(line, "%lx %llu %llu %n", &crc, &size, &mt, &n) >= 3 && n > 0)
		{
			std::string p = line + n;
			while (!p.empty() && (p.back() == '\n' || p.back() == '\r')) p.pop_back();
			CacheEntry e = { (DWORD)crc, size, mt };
			g_cache[Lower(p)] = e;
		}
	}
	fclose(f);
}
static void SaveCache()
{
	FILE * f = fopen((g_exeDir + "EALauncher.crc").c_str(), "w");
	if (!f) return;
	for (std::map<std::string, CacheEntry>::iterator it = g_cache.begin(); it != g_cache.end(); ++it)
		fprintf(f, "%08lX %llu %llu %s\n", (unsigned long)it->second.crc, (unsigned long long)it->second.size,
		        (unsigned long long)it->second.mtime, it->first.c_str());
	fclose(f);
}
static bool FileInfo(const std::string & path, ULONGLONG & size, ULONGLONG & mtime)
{
	WIN32_FILE_ATTRIBUTE_DATA fa;
	if (!GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &fa)) return false;
	if (fa.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) return false;
	size = ((ULONGLONG)fa.nFileSizeHigh << 32) | fa.nFileSizeLow;
	mtime = ((ULONGLONG)fa.ftLastWriteTime.dwHighDateTime << 32) | fa.ftLastWriteTime.dwLowDateTime;
	return true;
}
// CRC of a local file, from the cache when size and time haven't changed
static bool LocalCrc(const std::string & rel, const std::string & full, ULONGLONG size, ULONGLONG mtime, DWORD & crc)
{
	std::string key = Lower(rel);
	std::map<std::string, CacheEntry>::iterator it = g_cache.find(key);
	if (it != g_cache.end() && it->second.size == size && it->second.mtime == mtime) { crc = it->second.crc; return true; }
	if (!CrcFile(full, crc)) return false;
	CacheEntry e = { crc, size, mtime };
	g_cache[key] = e;
	return true;
}

//----------------------------------------------------------------------
// HTTP (WinINet)
//----------------------------------------------------------------------
static HINTERNET g_net = 0;

static std::string UrlEncodePath(const std::string & rel)
{
	std::string out;
	for (size_t i = 0; i < rel.size(); i++)
	{
		unsigned char c = (unsigned char)rel[i];
		if (isalnum(c) || c == '/' || c == '.' || c == '-' || c == '_') out += (char)c;
		else out += Fmt("%%%02X", c);
	}
	return out;
}

static HINTERNET OpenUrl(const std::string & path, const std::string & extraHeaders, DWORD & status)
{
	// a full URL (the file mirror) or a path on the content server
	std::string url = (path.compare(0, 7, "http://") == 0 || path.compare(0, 8, "https://") == 0) ? path : "http://" + g_server + path;
	HINTERNET h = InternetOpenUrlA(g_net, url.c_str(), extraHeaders.empty() ? 0 : extraHeaders.c_str(),
	                               extraHeaders.empty() ? 0 : (DWORD)-1,
	                               INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE |
	                               INTERNET_FLAG_NO_UI | INTERNET_FLAG_KEEP_CONNECTION, 0);
	status = 0;
	if (h)
	{
		DWORD len = sizeof(status);
		HttpQueryInfoA(h, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &status, &len, 0);
	}
	return h;
}

static bool HttpGetText(const std::string & path, std::string & out)
{
	DWORD status = 0;
	HINTERNET h = OpenUrl(path, "", status);
	if (!h) return false;
	if (status != 200) { InternetCloseHandle(h); return false; }
	char buf[16384]; DWORD got = 0;
	out.clear();
	while (InternetReadFile(h, buf, sizeof(buf), &got) && got > 0) out.append(buf, got);
	InternetCloseHandle(h);
	return true;
}

//----------------------------------------------------------------------
// manifest
//----------------------------------------------------------------------
struct RemoteFile { std::string rel; ULONGLONG size; DWORD crc; };

// the player's own files - never replaced even if a server lists them
static bool IsPlayerFile(const std::string & rel)
{
	std::string n = Lower(rel);
	static const char * exact[] = { "coinit.tmp", "ealauncher.cfg", "ealauncher.crc", "ealauncher.log", "ealauncher.old", "musicsetting.cfg", 0 };
	for (int i = 0; exact[i]; i++) if (n == exact[i]) return true;
	size_t dot = n.find_last_of('.');
	std::string ext = dot == std::string::npos ? "" : n.substr(dot);
	return ext == ".log" || ext == ".part" || ext == ".tmp";
}

static bool SafeRelPath(const std::string & rel)
{
	if (rel.empty() || rel[0] == '/' || rel[0] == '\\') return false;
	if (rel.find(':') != std::string::npos) return false;
	if (("/" + rel + "/").find("/../") != std::string::npos) return false;
	return true;
}

static bool ParseManifest(const std::string & text, int & version, std::vector<RemoteFile> & files)
{
	version = 0; files.clear();
	std::string upName; std::vector<std::string> notes;
	size_t pos = 0; int lineNo = 0;
	while (pos < text.size())
	{
		size_t eol = text.find('\n', pos);
		if (eol == std::string::npos) eol = text.size();
		std::string line = text.substr(pos, eol - pos);
		pos = eol + 1;
		if (!line.empty() && line.back() == '\r') line.pop_back();
		if (lineNo++ == 0) { if (line.compare(0, 9, "EACONTENT") != 0) return false; continue; }
		if (line.compare(0, 8, "version ") == 0) { version = atoi(line.c_str() + 8); continue; }
		if (line.compare(0, 11, "gameserver ") == 0) { g_manifestGameServer = line.substr(11); continue; }
		if (line.compare(0, 7, "mirror ") == 0) { g_mirror = line.substr(7); while (!g_mirror.empty() && g_mirror.back() == '/') g_mirror.pop_back(); continue; }
		if (line.compare(0, 5, "name ") == 0) { upName = line.substr(5); continue; }
		if (line.compare(0, 6, "notes ") == 0 || line == "notes") { if (notes.size() < 1000) notes.push_back(line.size() > 6 ? line.substr(6) : ""); continue; }
		if (line.compare(0, 6, "files ") == 0 || line.empty()) continue;
		unsigned long crc; unsigned long long size; int n = 0;
		if (sscanf(line.c_str(), "%lx %llu %n", &crc, &size, &n) >= 2 && n > 0)
		{
			RemoteFile f; f.crc = (DWORD)crc; f.size = size; f.rel = line.substr(n);
			if (IsPlayerFile(f.rel)) continue;   // never overwrite the player's settings/logs
			if (SafeRelPath(f.rel)) files.push_back(f);
			else LogLine("Ignoring unsafe path in manifest: %s", f.rel.c_str());
		}
	}
	EnterCriticalSection(&g_lock); g_updateName = upName; g_notes.swap(notes); LeaveCriticalSection(&g_lock);
	return true;
}

static std::string LocalPath(const std::string & rel)
{
	std::string p = g_exeDir + rel;
	for (size_t i = 0; i < p.size(); i++) if (p[i] == '/') p[i] = '\\';
	return p;
}

static void MakeDirsFor(const std::string & full)
{
	for (size_t i = 3; i < full.size(); i++)
		if (full[i] == '\\') CreateDirectoryA(full.substr(0, i).c_str(), 0);
}

//----------------------------------------------------------------------
// download one file (resumes <file>.part, verifies CRC)
//----------------------------------------------------------------------
enum DlResult { DL_OK, DL_FAIL, DL_BADCRC, DL_QUIT };

static DlResult DownloadFile(const RemoteFile & f, const std::string & url, const std::string & partPath,
                             ULONGLONG doneBefore, ULONGLONG totalAll, int index, int count)
{
	MakeDirsFor(partPath);
	ULONGLONG have = 0, mt = 0;
	if (!FileInfo(partPath, have, mt) || have > f.size) { DeleteFileA(partPath.c_str()); have = 0; }

	DWORD crc = 0xFFFFFFFFu;
	if (have > 0)
	{//re-hash what we already have so the final CRC covers the whole file
		HANDLE h = CreateFileA(partPath.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
		if (h == INVALID_HANDLE_VALUE) { have = 0; DeleteFileA(partPath.c_str()); }
		else
		{
			std::vector<unsigned char> b(1 << 20); DWORD got = 0;
			while (ReadFile(h, &b[0], (DWORD)b.size(), &got, 0) && got > 0) crc = CrcUpdate(crc, &b[0], got);
			CloseHandle(h);
		}
	}

	if (f.size == 0)
	{//empty file: nothing to download, but the .part must exist for the move
		HANDLE e = CreateFileA(partPath.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (e == INVALID_HANDLE_VALUE) { LogLine("Cannot write %s (error %lu)", partPath.c_str(), GetLastError()); return DL_FAIL; }
		CloseHandle(e);
	}
	if (have < f.size)
	{
		std::string hdr = have > 0 ? Fmt("Range: bytes=%llu-\r\n", (unsigned long long)have) : "";
		DWORD status = 0;
		HINTERNET h = OpenUrl(url, hdr, status);
		if (!h) { LogLine("Cannot connect to download %s", f.rel.c_str()); return DL_FAIL; }
		if (have > 0 && status == 200)
		{//server ignored the range - start over
			have = 0; crc = 0xFFFFFFFFu; DeleteFileA(partPath.c_str());
		}
		else if (status != 200 && status != 206)
		{
			LogLine("Server answered %lu for %s", status, f.rel.c_str());
			InternetCloseHandle(h);
			return DL_FAIL;
		}
		HANDLE out = CreateFileA(partPath.c_str(), GENERIC_WRITE, 0, 0, OPEN_ALWAYS, 0, 0);
		if (out == INVALID_HANDLE_VALUE) { InternetCloseHandle(h); LogLine("Cannot write %s (error %lu)", partPath.c_str(), GetLastError()); return DL_FAIL; }
		LARGE_INTEGER p; p.QuadPart = (LONGLONG)have; SetFilePointerEx(out, p, 0, FILE_BEGIN); SetEndOfFile(out);

		std::vector<unsigned char> buf(256 * 1024);
		DWORD got = 0, lastUi = 0;
		DWORD t0 = GetTickCount(); ULONGLONG startHave = have;
		bool ok = true;
		while (have < f.size)
		{
			if (g_quit) { ok = false; break; }
			if (!InternetReadFile(h, &buf[0], (DWORD)buf.size(), &got)) { ok = false; break; }
			if (got == 0) break;
			DWORD wr = 0;
			if (!WriteFile(out, &buf[0], got, &wr, 0) || wr != got) { ok = false; break; }
			crc = CrcUpdate(crc, &buf[0], got);
			have += got;
			DWORD now = GetTickCount();
			if (now - lastUi > 100)
			{
				lastUi = now;
				double secs = (now - t0) / 1000.0;
				double rate = secs > 0.5 ? (have - startHave) / secs : 0;
				std::string d = Fmt("File %d of %d: %s  -  %s of %s", index, count, f.rel.c_str(), MB(have).c_str(), MB(f.size).c_str());
				if (rate > 0) d += Fmt("  (%.1f MB/s)", rate / 1048576.0);
				SetDetail(d, totalAll ? (double)(doneBefore + have) / totalAll : 0);
			}
		}
		CloseHandle(out);
		InternetCloseHandle(h);
		if (g_quit) return DL_QUIT;
		if (!ok || have != f.size) { LogLine("Download of %s stopped at %llu of %llu bytes", f.rel.c_str(), (unsigned long long)have, (unsigned long long)f.size); return DL_FAIL; }
	}
	if ((crc ^ 0xFFFFFFFFu) != f.crc)
	{
		LogLine("CRC mismatch on %s (got %08lX, expected %08lX) - deleting the download", f.rel.c_str(), (unsigned long)(crc ^ 0xFFFFFFFFu), (unsigned long)f.crc);
		DeleteFileA(partPath.c_str());
		return DL_BADCRC;
	}
	return DL_OK;
}

//----------------------------------------------------------------------
// self update
//----------------------------------------------------------------------
static bool IsLauncherExe(const std::string & rel) { return Lower(rel) == "ealauncher.exe"; }

static bool InstallNewLauncher(const std::string & partPath)
{
	std::string oldPath = g_exeDir + "EALauncher.old";
	DeleteFileA(oldPath.c_str());
	if (!MoveFileExA(g_exePath.c_str(), oldPath.c_str(), MOVEFILE_REPLACE_EXISTING)) { LogLine("Self-update: cannot move the running launcher aside (error %lu)", GetLastError()); return false; }
	if (!MoveFileExA(partPath.c_str(), g_exePath.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
	{
		LogLine("Self-update: cannot put the new launcher in place (error %lu)", GetLastError());
		MoveFileExA(oldPath.c_str(), g_exePath.c_str(), MOVEFILE_REPLACE_EXISTING);
		return false;
	}
	STARTUPINFOA si; PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si)); si.cb = sizeof(si); ZeroMemory(&pi, sizeof(pi));
	std::string cmd = "\"" + g_exePath + "\"";
	if (!CreateProcessA(0, &cmd[0], 0, 0, FALSE, 0, 0, g_exeDir.c_str(), &si, &pi)) { LogLine("Self-update: cannot start the new launcher (error %lu)", GetLastError()); return false; }
	CloseHandle(pi.hThread); CloseHandle(pi.hProcess);
	LogLine("Self-update: started the new launcher");
	return true;
}

//----------------------------------------------------------------------
// coinit.tmp - the client reads the game server address from it.
// Five encrypted fields (user, password, game, content server, game server),
// each written as base64 that wraps onto a second line, then fullscreen,
// remember-login, width, height, depth. Only the game server field is
// replaced; everything else is kept byte for byte.
//----------------------------------------------------------------------
#define COINIT_KEY "qwertyuiopoiuytrewqwertyuiopoiuy"

// same line reader as CMainFrame::ParseLine in the client
static std::string CoinitField(const std::string & buf, size_t & pos, int sqCount)
{
	std::string build; int lines = 0;
	size_t len = buf.size();
	while (len > 0 && pos < len - 1)
	{
		build += buf[pos]; pos++;
		if (pos + 1 < len && buf[pos] == '\r' && buf[pos + 1] == '\n')
		{
			lines++;
			if (lines > sqCount) break;
		}
	}
	pos += 2;
	return build;
}

static std::string Encrypt(const std::string & text)
{
	char out[512];
	if (!ax_encrypt(COINIT_KEY, text.c_str(), out, sizeof(out))) return "";
	return out;
}

static bool ServerIsThisPC()
{
	std::string h = Lower(g_server.substr(0, g_server.find(':')));
	return h == "localhost" || h.compare(0, 4, "127.") == 0;
}

static void ApplyGameServer(const std::string & addr)
{
	if (addr.empty()) return;
	if (ServerIsThisPC())
	{//the server PC itself plays on 127.0.0.1 (a router often can't loop back to its own public address)
		LogLine("Content server is on this PC - leaving the game server address in coinit.tmp alone");
		return;
	}
	if (addr.size() > 64) { LogLine("Game server address too long for coinit.tmp: %s", addr.c_str()); return; }
	std::string path = g_exeDir + "coinit.tmp";
	std::string buf;
	FILE * f = fopen(path.c_str(), "rb");
	if (f)
	{
		char b[4096]; size_t n;
		while ((n = fread(b, 1, sizeof(b), f)) > 0) buf.append(b, n);
		fclose(f);
	}

	std::string out;
	if (!buf.empty())
	{
		size_t pos = 0;
		std::string fields[5];
		for (int i = 0; i < 5; i++) fields[i] = CoinitField(buf, pos, 1);
		std::string rest = pos < buf.size() ? buf.substr(pos) : "";
		char cur[256] = "";
		ax_decrypt(COINIT_KEY, fields[4].c_str(), cur, sizeof(cur));
		if (addr == cur) return;   // already right
		std::string enc = Encrypt(addr);
		if (enc.empty()) { LogLine("Could not encrypt the game server address"); return; }
		for (int i = 0; i < 4; i++) out += fields[i] + "\r\n";
		out += enc + "\r\n" + rest;
		LogLine("Game server address set to %s (was %s)", addr.c_str(), cur[0] ? cur : "empty");
	}
	else
	{//no coinit.tmp yet (new install): write one with defaults.
	 //The client never logs in from these credentials - login is in-game.
		const char * vals[5] = { "player", "player", "ENDLESS", g_server.c_str(), addr.c_str() };
		for (int i = 0; i < 5; i++)
		{
			std::string enc = Encrypt(vals[i]);
			if (enc.empty()) { LogLine("Could not encrypt coinit.tmp"); return; }
			out += enc + "\r\n";
		}
		out += "0\r\n1\r\n1024\r\n768\r\n32\r\n";
		LogLine("Created coinit.tmp with game server %s", addr.c_str());
	}

	std::string tmp = path + ".new";
	f = fopen(tmp.c_str(), "wb");
	if (!f) { LogLine("Cannot write %s", tmp.c_str()); return; }
	fwrite(out.data(), 1, out.size(), f);
	fclose(f);
	SetFileAttributesA(path.c_str(), FILE_ATTRIBUTE_NORMAL);   // a read-only coinit.tmp can't be replaced
	if (!MoveFileExA(tmp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
		LogLine("Cannot replace coinit.tmp (error %lu)", GetLastError());
}

//----------------------------------------------------------------------
// install (a player runs the downloaded EALauncher.exe)
//----------------------------------------------------------------------
static bool Exists(const std::string & p) { return GetFileAttributesA(p.c_str()) != INVALID_FILE_ATTRIBUTES; }

static std::string InstallDir()
{
	char b[MAX_PATH] = "";
	DWORD n = GetEnvironmentVariableA("ProgramW6432", b, MAX_PATH);       // real C:\Program Files, also from a 32-bit exe
	if (!n || n >= MAX_PATH) n = GetEnvironmentVariableA("ProgramFiles", b, MAX_PATH);
	std::string d = (n && n < MAX_PATH) ? b : "C:\\Program Files";
	if (d.back() != '\\') d += "\\";
	return d + INSTALL_FOLDER "\\";
}

static bool IsGameFolder(const std::string & dir)
{
	return Exists(dir + "EndlessAges.exe") || Exists(dir + "EALauncher.cfg") || Exists(dir + "VERSIONINF.VER");
}

static bool MakeShortcut(const std::string & lnk, const std::string & target, const std::string & workDir)
{
	IShellLinkA * sl = 0;
	if (FAILED(CoCreateInstance(CLSID_ShellLink, 0, CLSCTX_INPROC_SERVER, IID_IShellLinkA, (void **)&sl))) return false;
	sl->SetPath(target.c_str());
	sl->SetWorkingDirectory(workDir.c_str());
	sl->SetDescription(GAME_TITLE);
	sl->SetIconLocation(target.c_str(), 0);
	IPersistFile * pf = 0;
	bool ok = false;
	if (SUCCEEDED(sl->QueryInterface(IID_IPersistFile, (void **)&pf)))
	{
		wchar_t w[MAX_PATH]; MultiByteToWideChar(CP_ACP, 0, lnk.c_str(), -1, w, MAX_PATH);
		ok = SUCCEEDED(pf->Save(w, TRUE));
		pf->Release();
	}
	sl->Release();
	return ok;
}

// Runs elevated ("EALauncher.exe /install <server>"): makes the folder, lets
// normal users write to it (so patching never needs admin), copies the
// launcher in, writes its cfg, adds shortcuts. The game files themselves are
// downloaded afterwards by the installed launcher, as a normal update.
static int RunInstall(const std::string & server)
{
	std::string dir = InstallDir();
	std::string dirNoSlash = dir.substr(0, dir.size() - 1);
	SHCreateDirectoryExA(0, dirNoSlash.c_str(), 0);
	if (!Exists(dirNoSlash)) return 2;

	g_logFile = fopen((dir + "EALauncher.log").c_str(), "a");
	LogLine("Installing to %s (content server %s)", dir.c_str(), server.c_str());

	// BUILTIN\Users (S-1-5-32-545) may modify everything in the game folder
	std::string cmd = "icacls \"" + dirNoSlash + "\" /grant *S-1-5-32-545:(OI)(CI)M /T /Q";
	STARTUPINFOA si; PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si)); si.cb = sizeof(si); si.dwFlags = STARTF_USESHOWWINDOW; si.wShowWindow = SW_HIDE;
	ZeroMemory(&pi, sizeof(pi));
	if (CreateProcessA(0, &cmd[0], 0, 0, FALSE, CREATE_NO_WINDOW, 0, 0, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, 60000);
		CloseHandle(pi.hThread); CloseHandle(pi.hProcess);
	}
	else LogLine("icacls failed to start (error %lu)", GetLastError());

	if (!CopyFileA(g_exePath.c_str(), (dir + "EALauncher.exe").c_str(), FALSE))
	{
		LogLine("Cannot copy the launcher into %s (error %lu)", dir.c_str(), GetLastError());
		return 3;
	}
	FILE * f = fopen((dir + "EALauncher.cfg").c_str(), "w");
	if (f) { fprintf(f, "; Endless Ages launcher\n; Content server (address:port) the launcher updates from.\nServer=%s\n", server.c_str()); fclose(f); }

	CoInitialize(0);
	char path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(0, CSIDL_COMMON_DESKTOPDIRECTORY, 0, 0, path)))
		MakeShortcut(std::string(path) + "\\" GAME_TITLE ".lnk", dir + "EALauncher.exe", dirNoSlash);
	if (SUCCEEDED(SHGetFolderPathA(0, CSIDL_COMMON_PROGRAMS, 0, 0, path)))
		MakeShortcut(std::string(path) + "\\" GAME_TITLE ".lnk", dir + "EALauncher.exe", dirNoSlash);
	CoUninitialize();

	HKEY k;
	if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\" INSTALL_FOLDER, 0, 0, 0, KEY_WRITE | KEY_WOW64_64KEY, 0, &k, 0) == ERROR_SUCCESS)
	{
		RegSetValueExA(k, "InstallPath", 0, REG_SZ, (const BYTE *)dir.c_str(), (DWORD)dir.size() + 1);
		RegCloseKey(k);
	}
	LogLine("Install step done - the launcher will now download the game");
	return 0;
}

// non-elevated side: ask Windows for admin rights, run the install, then start
// the installed launcher (as the normal user) and close this one.
static DWORD WINAPI InstallThread(LPVOID)
{
	SetStatus(PH_INSTALLING, "Installing " GAME_TITLE "...", "Windows will ask for permission to install into Program Files.", 0);
	std::string params = "/install " + g_server;
	SHELLEXECUTEINFOA sei; ZeroMemory(&sei, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd = g_wnd;
	sei.lpVerb = "runas";
	sei.lpFile = g_exePath.c_str();
	sei.lpParameters = params.c_str();
	sei.nShow = SW_HIDE;
	if (!ShellExecuteExA(&sei) || !sei.hProcess)
	{
		DWORD err = GetLastError();
		LogLine("Install not started (error %lu)", err);
		SetStatus(PH_INSTALL, "Install cancelled.", err == ERROR_CANCELLED ? "Installing into Program Files needs administrator permission. Press INSTALL to try again." : Fmt("Could not start the installer (error %lu).", err), 0);
		return 0;
	}
	WaitForSingleObject(sei.hProcess, INFINITE);
	DWORD code = 1; GetExitCodeProcess(sei.hProcess, &code);
	CloseHandle(sei.hProcess);
	std::string installed = InstallDir() + "EALauncher.exe";
	if (code != 0 || !Exists(installed))
	{
		LogLine("Install failed (code %lu)", code);
		SetStatus(PH_INSTALL, "The install did not finish.", Fmt("Could not set up %s (code %lu). Press INSTALL to try again.", InstallDir().c_str(), code), 0);
		return 0;
	}
	STARTUPINFOA si; PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si)); si.cb = sizeof(si); ZeroMemory(&pi, sizeof(pi));
	std::string cmd = "\"" + installed + "\"";
	std::string dir = InstallDir();
	if (CreateProcessA(0, &cmd[0], 0, 0, FALSE, 0, 0, dir.c_str(), &si, &pi)) { CloseHandle(pi.hThread); CloseHandle(pi.hProcess); }
	PostMessageA(g_wnd, WM_CLOSE, 0, 0);
	return 0;
}

//----------------------------------------------------------------------
// the update pass (worker thread)
//----------------------------------------------------------------------
static DWORD UpdateBody()
{
	LogLine("Checking for updates at %s", g_server.c_str());
	SetStatus(PH_CONNECT, "Connecting to the update server...", g_server, 0);

	std::string manifest;
	std::vector<RemoteFile> files;
	int version = 0;
	g_manifestGameServer.clear();
	g_mirror.clear();
	g_mirrorFailures = 0;
	if (!HttpGetText("/manifest", manifest) || !ParseManifest(manifest, version, files))
	{
		LogLine("Could not get the manifest from %s", g_server.c_str());
		SetStatus(PH_OFFLINE, "Could not reach the update server.", "Check your connection and press RETRY, or PLAY without updating.", 0);
		InterlockedExchange(&g_workerRunning, 0);
		return 0;
	}
	EnterCriticalSection(&g_lock); g_version = version; LeaveCriticalSection(&g_lock);
	LogLine("Server content version %d, %u files%s%s", version, (unsigned)files.size(),
	        g_mirror.empty() ? "" : ", file mirror ", g_mirror.c_str());

	// --- check local files
	ULONGLONG totalSize = 0;
	for (size_t i = 0; i < files.size(); i++) totalSize += files[i].size;
	std::vector<size_t> need;
	ULONGLONG checked = 0, needBytes = 0;
	SetStatus(PH_CHECK, "Checking your game files...", "", 0);
	for (size_t i = 0; i < files.size() && !g_quit; i++)
	{
		const RemoteFile & f = files[i];
		std::string full = LocalPath(f.rel);
		ULONGLONG size = 0, mt = 0;
		bool good = false;
		if (FileInfo(full, size, mt) && size == f.size)
		{
			DWORD crc = 0;
			if (LocalCrc(f.rel, full, size, mt, crc)) good = (crc == f.crc);
		}
		if (!good) { need.push_back(i); needBytes += f.size; }
		checked += f.size;
		SetDetail(Fmt("%u of %u  -  %s", (unsigned)(i + 1), (unsigned)files.size(), f.rel.c_str()), totalSize ? (double)checked / totalSize : 1);
	}
	SaveCache();
	if (g_quit) return 0;

	// launcher itself goes last so everything else is done before it restarts
	for (size_t k = 0; k < need.size(); k++)
		if (IsLauncherExe(files[need[k]].rel) && k + 1 < need.size()) { std::swap(need[k], need.back()); break; }

	// --- download
	if (!need.empty())
	{
		LogLine("%u files need updating (%s)", (unsigned)need.size(), MB(needBytes).c_str());
		SetStatus(PH_DOWNLOAD, g_updateName.empty() ? Fmt("Downloading update - %u files, %s", (unsigned)need.size(), MB(needBytes).c_str())
		                                           : Fmt("Updating to %s - %u files, %s", g_updateName.c_str(), (unsigned)need.size(), MB(needBytes).c_str()), "", 0);
		ULONGLONG done = 0;
		for (size_t k = 0; k < need.size(); k++)
		{
			const RemoteFile & f = files[need[k]];
			std::string full = LocalPath(f.rel);
			std::string part = full + ".part";
			DlResult r = DL_FAIL;
			// fast mirror first (if the content server names one), then the content server itself
			if (!g_mirror.empty() && g_mirrorFailures < 3)
			{
				for (int attempt = 0; attempt < 2 && r != DL_OK && r != DL_QUIT && r != DL_BADCRC; attempt++)
					r = DownloadFile(f, g_mirror + "/" + UrlEncodePath(f.rel), part, done, needBytes, (int)k + 1, (int)need.size());
				if (r == DL_OK) g_mirrorFailures = 0;
				else if (r != DL_QUIT)
				{
					g_mirrorFailures++;
					LogLine("Mirror could not supply %s - using the content server%s", f.rel.c_str(),
					        g_mirrorFailures >= 3 ? " (mirror turned off for this run)" : "");
				}
			}
			for (int attempt = 0; attempt < 3 && r != DL_OK && r != DL_QUIT; attempt++)
			{
				if (attempt > 0) { LogLine("Retrying %s", f.rel.c_str()); Sleep(1000); }
				r = DownloadFile(f, "/files/" + UrlEncodePath(f.rel), part, done, needBytes, (int)k + 1, (int)need.size());
			}
			if (r == DL_QUIT) return 0;
			if (r != DL_OK)
			{
				SetStatus(PH_ERROR, "The update could not be downloaded.", "Lost the connection to the update server. Press RETRY to continue where it stopped.", 0);
				InterlockedExchange(&g_workerRunning, 0);
				return 0;
			}

			if (IsLauncherExe(f.rel))
			{
				SetStatus(PH_DOWNLOAD, "Updating the launcher...", "", 1);
				if (InstallNewLauncher(part)) { PostMessageA(g_wnd, WM_CLOSE, 0, 0); return 0; }
				SetStatus(PH_ERROR, "The launcher could not update itself.", "See EALauncher.log. Press RETRY to try again.", 0);
				InterlockedExchange(&g_workerRunning, 0);
				return 0;
			}

			if (!MoveFileExA(part.c_str(), full.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
			{
				DWORD err = GetLastError();
				LogLine("Cannot replace %s (error %lu)", full.c_str(), err);
				if (err == ERROR_SHARING_VIOLATION || err == ERROR_ACCESS_DENIED)
					SetStatus(PH_ERROR, "A game file is in use.", "Close Endless Ages (and the editor/servers using this folder), then press RETRY.", 0);
				else
					SetStatus(PH_ERROR, "A game file could not be replaced.", Fmt("%s (error %lu) - see EALauncher.log", f.rel.c_str(), err), 0);
				InterlockedExchange(&g_workerRunning, 0);
				return 0;
			}
			ULONGLONG size = 0, mt = 0;
			if (FileInfo(full, size, mt)) { CacheEntry e = { f.crc, size, mt }; g_cache[Lower(f.rel)] = e; }
			done += f.size;
			LogLine("Updated %s (%s)", f.rel.c_str(), MB(f.size).c_str());
		}
		SaveCache();
		LogLine("Update complete");
	}
	else
		LogLine("All files up to date");

	ApplyGameServer(g_manifestGameServer);
	SetStatus(PH_READY, g_updateName.empty() ? Fmt("Up to date  -  version %d", version) : Fmt("Up to date  -  %s", g_updateName.c_str()), "Press PLAY to enter Endless Ages.", 1);
	InterlockedExchange(&g_workerRunning, 0);
	return 0;
}

static DWORD WINAPI UpdateThread(LPVOID)
{
	UpdateBody();
	InterlockedExchange(&g_workerRunning, 0);   // every exit path, including quitting mid-download
	return 0;
}

static void StartUpdate()
{
	if (InterlockedCompareExchange(&g_workerRunning, 1, 0) != 0) return;
	HANDLE t = CreateThread(0, 0, UpdateThread, 0, 0, 0);
	if (t) CloseHandle(t); else InterlockedExchange(&g_workerRunning, 0);
}

//----------------------------------------------------------------------
// config / images
//----------------------------------------------------------------------
static void LoadConfig()
{
	std::string cfg = g_exeDir + "EALauncher.cfg";
	FILE * f = fopen(cfg.c_str(), "r");
	if (!f)
	{
		if (g_installMode) return;
		f = fopen(cfg.c_str(), "w");
		if (f) { fprintf(f, "; Endless Ages launcher\n; Content server (address:port) the launcher updates from.\n; On the server PC itself use Server=127.0.0.1:25858\nServer=%s\n", g_server.c_str()); fclose(f); }
		return;
	}
	char line[512];
	while (fgets(line, sizeof(line), f))
	{
		std::string l = line;
		while (!l.empty() && (l.back() == '\n' || l.back() == '\r' || l.back() == ' ')) l.pop_back();
		if (l.empty() || l[0] == ';' || l[0] == '#') continue;
		size_t eq = l.find('=');
		if (eq != std::string::npos && Lower(l.substr(0, eq)) == "server" && eq + 1 < l.size()) g_server = l.substr(eq + 1);
	}
	fclose(f);

	// One-time move off the old playit.gg tunnels: rewrite stale configs to the new server.
	if (Lower(g_server).find("tun.ply.gg") != std::string::npos)
	{
		g_server = DEFAULT_SERVER;
		f = fopen(cfg.c_str(), "w");
		if (f) { fprintf(f, "; Endless Ages launcher\n; Content server (address:port) the launcher updates from.\n; On the server PC itself use Server=127.0.0.1:25858\nServer=%s\n", g_server.c_str()); fclose(f); }
	}
}

static Image * LoadImageRes(int id)
{
	HRSRC r = FindResourceA(0, MAKEINTRESOURCEA(id), (LPCSTR)RT_RCDATA);
	if (!r) return 0;
	DWORD size = SizeofResource(0, r);
	void * data = LockResource(LoadResource(0, r));
	if (!data || !size) return 0;
	IStream * s = SHCreateMemStream((const BYTE *)data, size);
	if (!s) return 0;
	Image * img = Image::FromStream(s);
	s->Release();
	if (img && img->GetLastStatus() != Ok) { delete img; img = 0; }
	return img;
}

//----------------------------------------------------------------------
// painting
//----------------------------------------------------------------------
static const Color GOLD(255, 226, 190, 96);
static const Color TEAL(255, 150, 210, 230);

static void DrawButton(Graphics & g, const Button & b)
{
	RectF r((REAL)b.rc.left, (REAL)b.rc.top, (REAL)(b.rc.right - b.rc.left), (REAL)(b.rc.bottom - b.rc.top));
	Image * img = (b.hover && b.enabled && g_btnHover) ? g_btnHover : g_btnUp;
	if (img)
	{
		ImageAttributes ia;
		if (!b.enabled)
		{
			ColorMatrix cm = { { { 1, 0, 0, 0, 0 }, { 0, 1, 0, 0, 0 }, { 0, 0, 1, 0, 0 }, { 0, 0, 0, 0.45f, 0 }, { 0, 0, 0, 0, 1 } } };
			ia.SetColorMatrix(&cm);
		}
		g.DrawImage(img, r, 0, 0, (REAL)img->GetWidth(), (REAL)img->GetHeight(), UnitPixel, &ia);
	}
	else
	{
		SolidBrush bb(Color(220, 10, 10, 14)); g.FillRectangle(&bb, r);
		Pen p(Color(255, 90, 150, 200), 1.5f); g.DrawRectangle(&p, r);
	}
	FontFamily ff(L"Tahoma");
	Font font(&ff, 15, FontStyleBold, UnitPixel);
	StringFormat sf; sf.SetAlignment(StringAlignmentCenter); sf.SetLineAlignment(StringAlignmentCenter);
	Color c = !b.enabled ? Color(110, 200, 200, 200) : (b.down ? Color(255, 255, 255, 255) : (b.hover ? Color(255, 255, 230, 150) : GOLD));
	SolidBrush tb(c);
	wchar_t w[32]; MultiByteToWideChar(CP_ACP, 0, b.text, -1, w, 32);
	if (b.down) r.Offset(1, 1);
	g.DrawString(w, -1, &font, r, &sf, &tb);
}

static void Paint(HDC hdc)
{
	Bitmap back(WND_W, WND_H, PixelFormat32bppPARGB);
	Graphics g(&back);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	if (g_bg) g.DrawImage(g_bg, 0, 0, WND_W, WND_H);
	else { SolidBrush b(Color(255, 8, 8, 12)); g.FillRectangle(&b, 0, 0, WND_W, WND_H); }

	// bottom panel
	SolidBrush panel(Color(185, 4, 6, 10));
	g.FillRectangle(&panel, 0, 440, WND_W, WND_H - 440);
	Pen line(Color(160, 90, 150, 200), 1.0f);
	g.DrawLine(&line, 0, 440, WND_W, 440);

	EnterCriticalSection(&g_lock);
	std::string status = g_status, detail = g_detail;
	double prog = g_progress; Phase ph = g_phase;
	LeaveCriticalSection(&g_lock);

	FontFamily ff(L"Tahoma");
	Font big(&ff, 17, FontStyleBold, UnitPixel);
	Font fSmall(&ff, 12, FontStyleRegular, UnitPixel);
	SolidBrush gold(ph == PH_ERROR || ph == PH_OFFLINE ? Color(255, 240, 120, 90) : GOLD);
	SolidBrush teal(TEAL);
	wchar_t w[1024];
	MultiByteToWideChar(CP_ACP, 0, status.c_str(), -1, w, 1024);
	g.DrawString(w, -1, &big, PointF(24, 454), &gold);
	MultiByteToWideChar(CP_ACP, 0, detail.c_str(), -1, w, 1024);
	StringFormat clip; clip.SetTrimming(StringTrimmingEllipsisPath); clip.SetFormatFlags(StringFormatFlagsNoWrap);
	g.DrawString(w, -1, &fSmall, RectF(24, 480, 752, 18), &clip, &teal);

	// progress bar
	RectF bar(24, 502, 752, 14);
	SolidBrush barBg(Color(255, 16, 18, 24)); g.FillRectangle(&barBg, bar);
	if (prog > 0)
	{
		RectF fill(bar.X + 1, bar.Y + 1, (REAL)((bar.Width - 2) * (prog > 1 ? 1 : prog)), bar.Height - 2);
		LinearGradientBrush lg(PointF(0, bar.Y), PointF(0, bar.Y + bar.Height), Color(255, 245, 215, 120), Color(255, 150, 105, 30));
		g.FillRectangle(&lg, fill);
	}
	Pen barPen(Color(255, 90, 150, 200), 1.0f); g.DrawRectangle(&barPen, bar);

	// version, bottom-left
	EnterCriticalSection(&g_lock); int v = g_version; std::string upName = g_updateName; std::vector<std::string> notes = g_notes; LeaveCriticalSection(&g_lock);
	SolidBrush dim(Color(160, 180, 180, 190));
	std::string vs = !upName.empty() ? upName : (v ? Fmt("Content version %d", v) : std::string(""));
	MultiByteToWideChar(CP_ACP, 0, vs.c_str(), -1, w, 1024);
	g.DrawString(w, -1, &fSmall, RectF(24, 548, 124, 30), &clip, &dim);

	// what's new (Patch\UpdateNotes.txt on the content server): every update, newest at the top.
	// Scroll with the mouse wheel over the box, or drag the scroll bar.
	g_notesBox.left = g_notesBox.right = 0; g_notesMax = 0;
	if (!notes.empty() || !upName.empty())
	{
		RectF box(24, 24, 380, 396);
		Font fNotes(&ff, 13, FontStyleRegular, UnitPixel);
		Font fHead(&ff, 14, FontStyleBold, UnitPixel);
		const REAL textW = box.Width - 24 - 10;          // leave room for the scroll bar
		std::vector<std::wstring> wl(notes.size());
		std::vector<REAL> lh(notes.size());
		std::vector<bool> head(notes.size());
		REAL total = 0;
		for (size_t i = 0; i < notes.size(); i++)
		{
			head[i] = notes[i].compare(0, 8, "Version ") == 0;
			wl[i].assign(notes[i].size() + 1, L'\0');
			MultiByteToWideChar(CP_ACP, 0, notes[i].c_str(), -1, &wl[i][0], (int)wl[i].size());
			if (notes[i].empty()) { lh[i] = 8; }
			else { RectF need; g.MeasureString(wl[i].c_str(), -1, head[i] ? &fHead : &fNotes, RectF(0, 0, textW, 2000), &need); lh[i] = need.Height + (head[i] ? 2 : 0); }
			total += lh[i];
		}
		REAL h = 44 + total + 12; if (h > box.Height) h = box.Height;
		const REAL viewY = box.Y + 40, viewH = h - 48;
		g_notesMax = total > viewH ? (int)(total - viewH + 0.5f) : 0;
		if (g_notesScroll > g_notesMax) g_notesScroll = g_notesMax;
		if (g_notesScroll < 0) g_notesScroll = 0;
		g_notesBox.left = (LONG)box.X; g_notesBox.top = (LONG)box.Y; g_notesBox.right = (LONG)(box.X + box.Width); g_notesBox.bottom = (LONG)(box.Y + h);

		SolidBrush notesBg(Color(185, 4, 6, 10)); g.FillRectangle(&notesBg, box.X, box.Y, box.Width, h);
		g.DrawRectangle(&line, box.X, box.Y, box.Width, h);
		std::string title = upName.empty() ? "What's new" : "What's new  -  " + upName;
		MultiByteToWideChar(CP_ACP, 0, title.c_str(), -1, w, 1024);
		g.DrawString(w, -1, &big, RectF(box.X + 12, box.Y + 10, box.Width - 24, 24), &clip, &gold);

		SolidBrush noteText(Color(255, 220, 220, 225));
		g.SetClip(RectF(box.X + 1, viewY, box.Width - 2, viewH));
		REAL y = viewY - (REAL)g_notesScroll;
		for (size_t i = 0; i < notes.size(); i++)
		{
			if (!notes[i].empty() && y + lh[i] > viewY && y < viewY + viewH)
				g.DrawString(wl[i].c_str(), -1, head[i] ? &fHead : &fNotes, RectF(box.X + 12, y, textW, lh[i] + 2), 0, head[i] ? (Brush *)&gold : (Brush *)&noteText);
			y += lh[i];
		}
		g.ResetClip();

		g_notesTrack.left = g_notesTrack.right = 0;
		if (g_notesMax > 0)
		{
			RectF track(box.X + box.Width - 12, viewY, 6, viewH);
			REAL th = viewH * viewH / total; if (th < 24) th = 24;
			REAL ty = track.Y + (viewH - th) * g_notesScroll / g_notesMax;
			SolidBrush trackB(Color(140, 40, 60, 80)); g.FillRectangle(&trackB, track);
			SolidBrush thumbB(Color(230, 245, 215, 120)); g.FillRectangle(&thumbB, track.X, ty, track.Width, th);
			g_notesTrack.left = (LONG)track.X - 4; g_notesTrack.top = (LONG)track.Y; g_notesTrack.right = (LONG)(track.X + track.Width + 4); g_notesTrack.bottom = (LONG)(track.Y + track.Height);
			g_notesThumbH = (int)th;
		}
	}

	if (g_retry.enabled) DrawButton(g, g_retry);
	if (g_account.enabled) DrawButton(g, g_account);
	if (g_settings.enabled) DrawButton(g, g_settings);
	DrawButton(g, g_play);
	DrawButton(g, g_exit);

	Graphics screen(hdc);
	screen.DrawImage(&back, 0, 0);
}

static void UpdateButtons()
{
	EnterCriticalSection(&g_lock); Phase ph = g_phase; LeaveCriticalSection(&g_lock);
	g_play.enabled = (ph == PH_READY || ph == PH_OFFLINE || ph == PH_INSTALL);
	g_retry.enabled = (ph == PH_ERROR || ph == PH_OFFLINE);
	g_account.enabled = (ph == PH_READY || ph == PH_CHECK || ph == PH_DOWNLOAD);
	g_settings.enabled = (ph != PH_INSTALL && ph != PH_CONNECT);
}

static bool InRect(const RECT & r, int x, int y) { return x >= r.left && x < r.right && y >= r.top && y < r.bottom; }

//----------------------------------------------------------------------
// NEW ACCOUNT window -> POST /register on the content server
//----------------------------------------------------------------------
#define IDC_ACC_USER 301
#define IDC_ACC_PASS 302
#define IDC_ACC_PASS2 303
#define IDC_ACC_EMAIL 304
#define IDC_ACC_CREATE 305
#define IDC_ACC_CANCEL 306
#define WM_ACC_RESULT (WM_APP + 20)
static HWND g_accWnd = 0, g_accStatus = 0;
static HBRUSH g_accBrush = 0;

static std::string FormEncode(const std::string & v)
{
	std::string o;
	for (size_t i = 0; i < v.size(); i++)
	{
		unsigned char c = (unsigned char)v[i];
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') o += (char)c;
		else o += Fmt("%%%02X", c);
	}
	return o;
}

struct AccRequest { std::string body; };

static DWORD WINAPI RegisterThread(LPVOID p)
{
	AccRequest * r = (AccRequest *)p;
	std::string host = g_server, result;
	INTERNET_PORT port = 80;
	size_t c = host.find(':');
	if (c != std::string::npos) { port = (INTERNET_PORT)atoi(host.c_str() + c + 1); host = host.substr(0, c); }
	HINTERNET con = InternetConnectA(g_net, host.c_str(), port, 0, 0, INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET req = con ? HttpOpenRequestA(con, "POST", "/register", 0, 0, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_UI, 0) : 0;
	const char * hdr = "Content-Type: application/x-www-form-urlencoded\r\n";
	if (req && HttpSendRequestA(req, hdr, (DWORD)-1, (LPVOID)r->body.data(), (DWORD)r->body.size()))
	{
		char b[1024]; DWORD got = 0;
		while (InternetReadFile(req, b, sizeof(b), &got) && got > 0) result.append(b, got);
	}
	if (req) InternetCloseHandle(req);
	if (con) InternetCloseHandle(con);
	delete r;
	while (!result.empty() && (result.back() == '\n' || result.back() == '\r')) result.pop_back();
	if (result.empty()) result = "ERR Could not reach the server. Try again in a moment.";
	if (g_accWnd) PostMessageA(g_accWnd, WM_ACC_RESULT, 0, (LPARAM)new std::string(result));
	return 0;
}

static std::string GetText(int id)
{
	char b[256] = ""; GetDlgItemTextA(g_accWnd, id, b, sizeof(b));
	return b;
}

static LRESULT CALLBACK AccWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLOREDIT:
		SetBkColor((HDC)wp, RGB(16, 18, 24));
		SetTextColor((HDC)wp, (HWND)lp == g_accStatus ? RGB(240, 140, 100) : RGB(226, 190, 96));
		return (LRESULT)g_accBrush;
	case WM_COMMAND:
		if (LOWORD(wp) == IDC_ACC_CANCEL) { DestroyWindow(hwnd); return 0; }
		if (LOWORD(wp) == IDC_ACC_CREATE)
		{
			std::string u = GetText(IDC_ACC_USER), p1 = GetText(IDC_ACC_PASS), p2 = GetText(IDC_ACC_PASS2), e = GetText(IDC_ACC_EMAIL);
			if (u.size() < 3) { SetWindowTextA(g_accStatus, "Username must be at least 3 characters."); return 0; }
			if (p1.size() < 4) { SetWindowTextA(g_accStatus, "Password must be at least 4 characters."); return 0; }
			if (p1 != p2) { SetWindowTextA(g_accStatus, "The two passwords don't match."); return 0; }
			SetWindowTextA(g_accStatus, "Creating your account...");
			EnableWindow(GetDlgItem(hwnd, IDC_ACC_CREATE), FALSE);
			AccRequest * r = new AccRequest;
			r->body = "user=" + FormEncode(u) + "&pass=" + FormEncode(p1) + "&email=" + FormEncode(e);
			HANDLE t = CreateThread(0, 0, RegisterThread, r, 0, 0);
			if (t) CloseHandle(t); else delete r;
			return 0;
		}
		break;
	case WM_ACC_RESULT:
	{
		std::string * res = (std::string *)lp;
		EnableWindow(GetDlgItem(hwnd, IDC_ACC_CREATE), TRUE);
		if (res->compare(0, 2, "OK") == 0)
		{
			LogLine("New account created: %s", GetText(IDC_ACC_USER).c_str());
			MessageBoxA(hwnd, ("Account \"" + GetText(IDC_ACC_USER) + "\" created.\n\nPress PLAY and log in with it in the game.").c_str(), "Endless Ages", MB_ICONINFORMATION);
			DestroyWindow(hwnd);
		}
		else
			SetWindowTextA(g_accStatus, res->compare(0, 4, "ERR ") == 0 ? res->c_str() + 4 : res->c_str());
		delete res;
		return 0;
	}
	case WM_CLOSE: DestroyWindow(hwnd); return 0;
	case WM_DESTROY: g_accWnd = 0; EnableWindow(g_wnd, TRUE); SetForegroundWindow(g_wnd); return 0;
	}
	return DefWindowProcA(hwnd, msg, wp, lp);
}

static void ShowAccountWindow()
{
	if (g_accWnd) { SetForegroundWindow(g_accWnd); return; }
	static bool reg = false;
	if (!reg)
	{
		WNDCLASSA wc; ZeroMemory(&wc, sizeof(wc));
		wc.lpfnWndProc = AccWndProc; wc.hInstance = GetModuleHandleA(0);
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		g_accBrush = CreateSolidBrush(RGB(16, 18, 24));
		wc.hbrBackground = g_accBrush;
		wc.lpszClassName = "EALauncherAccount";
		RegisterClassA(&wc);
		reg = true;
	}
	RECT pr; GetWindowRect(g_wnd, &pr);
	int w = 380, h = 330;
	g_accWnd = CreateWindowExA(WS_EX_DLGMODALFRAME, "EALauncherAccount", "Create an Endless Ages account",
	                           WS_POPUP | WS_CAPTION | WS_SYSMENU, pr.left + (WND_W - w) / 2, pr.top + (WND_H - h) / 2, w, h, g_wnd, 0, GetModuleHandleA(0), 0);
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	const char * labels[4] = { "Username (3-16 letters, numbers, _)", "Password (4-20)", "Password again", "Email (optional)" };
	int ids[4] = { IDC_ACC_USER, IDC_ACC_PASS, IDC_ACC_PASS2, IDC_ACC_EMAIL };
	for (int i = 0; i < 4; i++)
	{
		HWND l = CreateWindowA("STATIC", labels[i], WS_CHILD | WS_VISIBLE, 20, 14 + i * 52, 330, 16, g_accWnd, 0, 0, 0);
		DWORD st = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER | (i == 1 || i == 2 ? ES_PASSWORD : 0);
		HWND e = CreateWindowExA(0, "EDIT", "", st, 20, 32 + i * 52, 330, 22, g_accWnd, (HMENU)(INT_PTR)ids[i], 0, 0);
		SendMessageA(e, EM_LIMITTEXT, i == 3 ? 100 : (i == 0 ? 16 : 20), 0);
		SendMessageA(l, WM_SETFONT, (WPARAM)font, 0); SendMessageA(e, WM_SETFONT, (WPARAM)font, 0);
	}
	g_accStatus = CreateWindowA("STATIC", "", WS_CHILD | WS_VISIBLE, 20, 222, 330, 32, g_accWnd, 0, 0, 0);
	HWND b1 = CreateWindowA("BUTTON", "Create account", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON, 130, 260, 120, 28, g_accWnd, (HMENU)IDC_ACC_CREATE, 0, 0);
	HWND b2 = CreateWindowA("BUTTON", "Cancel", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 260, 260, 90, 28, g_accWnd, (HMENU)IDC_ACC_CANCEL, 0, 0);
	SendMessageA(g_accStatus, WM_SETFONT, (WPARAM)font, 0); SendMessageA(b1, WM_SETFONT, (WPARAM)font, 0); SendMessageA(b2, WM_SETFONT, (WPARAM)font, 0);
	EnableWindow(g_wnd, FALSE);
	ShowWindow(g_accWnd, SW_SHOW);
	SetFocus(GetDlgItem(g_accWnd, IDC_ACC_USER));
}

//----------------------------------------------------------------------
// SETTINGS: screen size + fullscreen.  The client reads these from the
// plain-text lines after the 5 encrypted fields in coinit.tmp:
//   fullscreen(0/1), save-logon(0/1), width, height, depth
//----------------------------------------------------------------------
#define IDC_SET_RES 401
#define IDC_SET_FULL 402
#define IDC_SET_SAVE 403
#define IDC_SET_CANCEL 404
static HWND g_setWnd = 0;
static std::vector<std::pair<int,int> > g_modes;

static bool ReadCoinit(std::string & head, std::string tail[5])
{
	std::string path = g_exeDir + "coinit.tmp", buf;
	FILE * f = fopen(path.c_str(), "rb");
	if (!f) return false;
	char b[4096]; size_t n;
	while ((n = fread(b, 1, sizeof(b), f)) > 0) buf.append(b, n);
	fclose(f);
	size_t pos = 0;
	head.clear();
	for (int i = 0; i < 5; i++) head += CoinitField(buf, pos, 1) + "\r\n";
	const char * defs[5] = { "0", "1", "1024", "768", "32" };
	for (int i = 0; i < 5; i++)
	{
		tail[i] = pos < buf.size() ? CoinitField(buf, pos, 0) : "";
		while (!tail[i].empty() && (tail[i].back() == '\r' || tail[i].back() == '\n' || tail[i].back() == ' ')) tail[i].pop_back();
		if (tail[i].empty()) tail[i] = defs[i];
	}
	return true;
}

static bool WriteCoinitTail(const std::string & head, std::string tail[5])
{
	std::string path = g_exeDir + "coinit.tmp", tmp = path + ".new";
	std::string out = head;
	for (int i = 0; i < 5; i++) out += tail[i] + "\r\n";
	FILE * f = fopen(tmp.c_str(), "wb");
	if (!f) { LogLine("Cannot write %s", tmp.c_str()); return false; }
	fwrite(out.data(), 1, out.size(), f);
	fclose(f);
	SetFileAttributesA(path.c_str(), FILE_ATTRIBUTE_NORMAL);
	if (!MoveFileExA(tmp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
	{ LogLine("Cannot replace coinit.tmp (error %lu)", GetLastError()); return false; }
	return true;
}

static LRESULT CALLBACK SetWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CTLCOLORSTATIC:
		SetBkColor((HDC)wp, RGB(16, 18, 24));
		SetTextColor((HDC)wp, RGB(226, 190, 96));
		return (LRESULT)g_accBrush;
	case WM_COMMAND:
		if (LOWORD(wp) == IDC_SET_CANCEL) { DestroyWindow(hwnd); return 0; }
		if (LOWORD(wp) == IDC_SET_SAVE)
		{
			std::string head, tail[5];
			if (!ReadCoinit(head, tail)) { MessageBoxA(hwnd, "Press PLAY once first, then change the settings.", "Endless Ages", MB_ICONINFORMATION); return 0; }
			int sel = (int)SendDlgItemMessageA(hwnd, IDC_SET_RES, CB_GETCURSEL, 0, 0);
			if (sel >= 0 && sel < (int)g_modes.size())
			{
				tail[2] = Fmt("%d", g_modes[sel].first);
				tail[3] = Fmt("%d", g_modes[sel].second);
			}
			tail[0] = IsDlgButtonChecked(hwnd, IDC_SET_FULL) == BST_CHECKED ? "1" : "0";
			tail[4] = "32";
			if (WriteCoinitTail(head, tail))
			{
				LogLine("Settings saved: %sx%s %s", tail[2].c_str(), tail[3].c_str(), tail[0] == "1" ? "fullscreen" : "windowed");
				DestroyWindow(hwnd);
			}
			else MessageBoxA(hwnd, "Could not save the settings (see EALauncher.log).", "Endless Ages", MB_ICONWARNING);
			return 0;
		}
		break;
	case WM_CLOSE: DestroyWindow(hwnd); return 0;
	case WM_DESTROY: g_setWnd = 0; EnableWindow(g_wnd, TRUE); SetForegroundWindow(g_wnd); return 0;
	}
	return DefWindowProcA(hwnd, msg, wp, lp);
}

static void ShowSettingsWindow()
{
	if (g_setWnd) { SetForegroundWindow(g_setWnd); return; }
	std::string head, tail[5];
	if (!ReadCoinit(head, tail)) { MessageBoxA(g_wnd, "Press PLAY once first, then change the settings.", "Endless Ages", MB_ICONINFORMATION); return; }
	static bool reg = false;
	if (!reg)
	{
		WNDCLASSA wc; ZeroMemory(&wc, sizeof(wc));
		wc.lpfnWndProc = SetWndProc; wc.hInstance = GetModuleHandleA(0);
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		if (!g_accBrush) g_accBrush = CreateSolidBrush(RGB(16, 18, 24));
		wc.hbrBackground = g_accBrush;
		wc.lpszClassName = "EALauncherSettings";
		RegisterClassA(&wc);
		reg = true;
	}
	// every screen size this monitor supports (800x600 and up)
	g_modes.clear();
	DEVMODEA dm; ZeroMemory(&dm, sizeof(dm)); dm.dmSize = sizeof(dm);
	for (DWORD i = 0; EnumDisplaySettingsA(0, i, &dm); i++)
	{
		if (dm.dmPelsWidth < 800 || dm.dmPelsHeight < 600 || dm.dmBitsPerPel < 32) continue;
		std::pair<int,int> m((int)dm.dmPelsWidth, (int)dm.dmPelsHeight);
		if (std::find(g_modes.begin(), g_modes.end(), m) == g_modes.end()) g_modes.push_back(m);
	}
	std::sort(g_modes.begin(), g_modes.end());
	int curW = atoi(tail[2].c_str()), curH = atoi(tail[3].c_str());
	std::pair<int,int> cur(curW, curH);
	if (curW > 0 && curH > 0 && std::find(g_modes.begin(), g_modes.end(), cur) == g_modes.end()) { g_modes.push_back(cur); std::sort(g_modes.begin(), g_modes.end()); }

	RECT pr; GetWindowRect(g_wnd, &pr);
	int w = 320, h = 190;
	g_setWnd = CreateWindowExA(WS_EX_DLGMODALFRAME, "EALauncherSettings", "Endless Ages settings",
	                           WS_POPUP | WS_CAPTION | WS_SYSMENU, pr.left + (WND_W - w) / 2, pr.top + (WND_H - h) / 2, w, h, g_wnd, 0, GetModuleHandleA(0), 0);
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	HWND l = CreateWindowA("STATIC", "Screen size", WS_CHILD | WS_VISIBLE, 20, 16, 270, 16, g_setWnd, 0, 0, 0);
	HWND cb = CreateWindowA("COMBOBOX", "", WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST, 20, 34, 270, 300, g_setWnd, (HMENU)IDC_SET_RES, 0, 0);
	for (size_t i = 0; i < g_modes.size(); i++)
	{
		SendMessageA(cb, CB_ADDSTRING, 0, (LPARAM)Fmt("%d x %d", g_modes[i].first, g_modes[i].second).c_str());
		if (g_modes[i] == cur) SendMessageA(cb, CB_SETCURSEL, i, 0);
	}
	HWND chk = CreateWindowA("BUTTON", "Fullscreen", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, 20, 72, 270, 20, g_setWnd, (HMENU)IDC_SET_FULL, 0, 0);
	CheckDlgButton(g_setWnd, IDC_SET_FULL, tail[0] == "1" ? BST_CHECKED : BST_UNCHECKED);
	HWND b1 = CreateWindowA("BUTTON", "Save", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON, 100, 112, 90, 28, g_setWnd, (HMENU)IDC_SET_SAVE, 0, 0);
	HWND b2 = CreateWindowA("BUTTON", "Cancel", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 200, 112, 90, 28, g_setWnd, (HMENU)IDC_SET_CANCEL, 0, 0);
	HWND all[5] = { l, cb, chk, b1, b2 };
	for (int i = 0; i < 5; i++) SendMessageA(all[i], WM_SETFONT, (WPARAM)font, 0);
	EnableWindow(g_wnd, FALSE);
	ShowWindow(g_setWnd, SW_SHOW);
}

static void Play()
{
	std::string exe = g_exeDir + "EndlessAges.exe";
	STARTUPINFOA si; PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si)); si.cb = sizeof(si); ZeroMemory(&pi, sizeof(pi));
	std::string cmd = "\"" + exe + "\"";
	if (!CreateProcessA(0, &cmd[0], 0, 0, FALSE, 0, 0, g_exeDir.c_str(), &si, &pi))
	{
		LogLine("Cannot start %s (error %lu)", exe.c_str(), GetLastError());
		MessageBoxA(g_wnd, ("Could not start EndlessAges.exe.\n\n" + exe).c_str(), "Endless Ages", MB_ICONERROR);
		return;
	}
	CloseHandle(pi.hThread); CloseHandle(pi.hProcess);
	LogLine("Started EndlessAges.exe");
	DestroyWindow(g_wnd);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		SetTimer(hwnd, TIMER_REPAINT, 100, 0);
		return 0;
	case WM_TIMER:
		UpdateButtons();
		InvalidateRect(hwnd, 0, FALSE);
		return 0;
	case WM_ERASEBKGND:
		return 1;
	case WM_PAINT:
	{
		PAINTSTRUCT ps; HDC dc = BeginPaint(hwnd, &ps);
		Paint(dc);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		POINT pt = { GET_X_LPARAM(lp), GET_Y_LPARAM(lp) }; ScreenToClient(hwnd, &pt);
		if (g_notesMax > 0 && InRect(g_notesBox, pt.x, pt.y))
		{
			g_notesScroll -= GET_WHEEL_DELTA_WPARAM(wp) * 48 / WHEEL_DELTA;
			if (g_notesScroll < 0) g_notesScroll = 0;
			if (g_notesScroll > g_notesMax) g_notesScroll = g_notesMax;
			InvalidateRect(hwnd, 0, FALSE);
		}
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		int x = GET_X_LPARAM(lp), y = GET_Y_LPARAM(lp);
		if (g_notesDrag)
		{
			int range = (g_notesTrack.bottom - g_notesTrack.top) - g_notesThumbH;
			if (range > 0) g_notesScroll = g_notesDragStart + (y - g_notesDragY) * g_notesMax / range;
			if (g_notesScroll < 0) g_notesScroll = 0;
			if (g_notesScroll > g_notesMax) g_notesScroll = g_notesMax;
			InvalidateRect(hwnd, 0, FALSE);
			return 0;
		}
		g_play.hover = InRect(g_play.rc, x, y); g_exit.hover = InRect(g_exit.rc, x, y); g_retry.hover = InRect(g_retry.rc, x, y); g_account.hover = InRect(g_account.rc, x, y); g_settings.hover = InRect(g_settings.rc, x, y);
		TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, hwnd, 0 }; TrackMouseEvent(&tme);
		return 0;
	}
	case WM_MOUSELEAVE:
		g_play.hover = g_exit.hover = g_retry.hover = g_account.hover = g_settings.hover = false;
		return 0;
	case WM_LBUTTONDOWN:
	{
		int x = GET_X_LPARAM(lp), y = GET_Y_LPARAM(lp);
		if (InRect(g_play.rc, x, y) && g_play.enabled) { g_play.down = true; SetCapture(hwnd); }
		else if (InRect(g_exit.rc, x, y)) { g_exit.down = true; SetCapture(hwnd); }
		else if (InRect(g_retry.rc, x, y) && g_retry.enabled) { g_retry.down = true; SetCapture(hwnd); }
		else if (InRect(g_account.rc, x, y) && g_account.enabled) { g_account.down = true; SetCapture(hwnd); }
		else if (InRect(g_settings.rc, x, y) && g_settings.enabled) { g_settings.down = true; SetCapture(hwnd); }
		else if (g_notesMax > 0 && InRect(g_notesTrack, x, y)) { g_notesDrag = true; g_notesDragY = y; g_notesDragStart = g_notesScroll; SetCapture(hwnd); }
		else if (g_notesMax > 0 && InRect(g_notesBox, x, y)) { /* clicks inside the notes box don't drag the window */ }
		else { ReleaseCapture(); SendMessageA(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0); }   // drag the window
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = GET_X_LPARAM(lp), y = GET_Y_LPARAM(lp);
		ReleaseCapture();
		if (g_notesDrag) { g_notesDrag = false; return 0; }
		bool play = g_play.down && InRect(g_play.rc, x, y);
		bool ex = g_exit.down && InRect(g_exit.rc, x, y);
		bool retry = g_retry.down && InRect(g_retry.rc, x, y);
		bool account = g_account.down && InRect(g_account.rc, x, y);
		bool settings = g_settings.down && InRect(g_settings.rc, x, y);
		g_play.down = g_exit.down = g_retry.down = g_account.down = g_settings.down = false;
		if (ex) PostMessageA(hwnd, WM_CLOSE, 0, 0);
		else if (play && g_play.enabled)
		{
			EnterCriticalSection(&g_lock); Phase ph = g_phase; LeaveCriticalSection(&g_lock);
			if (ph == PH_INSTALL)
			{
				SetStatus(PH_INSTALLING, "Installing...", "", 0);
				HANDLE t = CreateThread(0, 0, InstallThread, 0, 0, 0);
				if (t) CloseHandle(t);
				return 0;
			}
			if (ph == PH_OFFLINE && MessageBoxA(hwnd, "The game could not be checked for updates.\n\nPlay anyway?", "Endless Ages", MB_YESNO | MB_ICONQUESTION) != IDYES)
				return 0;
			Play();
		}
		else if (retry && g_retry.enabled) StartUpdate();
		else if (account && g_account.enabled) ShowAccountWindow();
		else if (settings && g_settings.enabled) ShowSettingsWindow();
		return 0;
	}
	case WM_KEYDOWN:
		if (wp == VK_ESCAPE) PostMessageA(hwnd, WM_CLOSE, 0, 0);
		if (wp == VK_RETURN && g_play.enabled) { EnterCriticalSection(&g_lock); Phase ph = g_phase; LeaveCriticalSection(&g_lock); if (ph == PH_READY) Play(); }
		return 0;
	case WM_DESTROY:
		InterlockedExchange(&g_quit, 1);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcA(hwnd, msg, wp, lp);
}

//----------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int show)
{
	{
		char exe0[MAX_PATH] = { 0 };
		GetModuleFileNameA(0, exe0, MAX_PATH);
		g_exePath = exe0;
		g_exeDir = g_exePath.substr(0, g_exePath.find_last_of('\\') + 1);
	}
	// elevated install step (started by the download copy of the launcher)
	if (__argc >= 2 && _stricmp(__argv[1], "/install") == 0)
		return RunInstall(__argc >= 3 ? __argv[2] : DEFAULT_SERVER);

	// Run from a download (no game here, not the install folder)?
	if (!IsGameFolder(g_exeDir) && Lower(g_exeDir) != Lower(InstallDir()))
	{
		std::string installed = InstallDir() + "EALauncher.exe";
		if (Exists(installed))
		{//already installed - just start that launcher
			STARTUPINFOA si; PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si)); si.cb = sizeof(si); ZeroMemory(&pi, sizeof(pi));
			std::string cmd = "\"" + installed + "\"";
			std::string dir = InstallDir();
			if (CreateProcessA(0, &cmd[0], 0, 0, FALSE, 0, 0, dir.c_str(), &si, &pi)) { CloseHandle(pi.hThread); CloseHandle(pi.hProcess); return 0; }
		}
		g_installMode = true;
	}

	// only one launcher at a time (two would fight over the same files)
	HANDLE mutex = CreateMutexA(0, TRUE, "EndlessAgesLauncherSingleInstance");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// the previous launcher may be finishing a self-update; give it a moment
		if (WaitForSingleObject(mutex, 5000) == WAIT_TIMEOUT)
		{
			HWND other = FindWindowA("EALauncherWnd", 0);
			if (other) SetForegroundWindow(other);
			return 0;
		}
	}

	InitializeCriticalSection(&g_lock);
	InitCrc();
	SetCurrentDirectoryA(g_exeDir.c_str());

	// leftovers from a self-update
	for (int i = 0; i < 20 && GetFileAttributesA((g_exeDir + "EALauncher.old").c_str()) != INVALID_FILE_ATTRIBUTES; i++)
		if (!DeleteFileA((g_exeDir + "EALauncher.old").c_str())) Sleep(250);

	{//keep the log across runs (a self-update restarts the launcher), but not forever
		ULONGLONG ls = 0, lm = 0;
		std::string lp = g_exeDir + "EALauncher.log";
		if (g_installMode) { char t[MAX_PATH] = ""; GetTempPathA(MAX_PATH, t); lp = std::string(t) + "EALauncher-install.log"; }
		g_logFile = fopen(lp.c_str(), (FileInfo(lp, ls, lm) && ls > 1048576) ? "w" : "a");
	}
	LogLine("----------------------------------------------------------------");
	LogLine("Endless Ages launcher starting in %s", g_exeDir.c_str());
	LoadConfig();
	LoadCache();

	GdiplusStartupInput gsi; ULONG_PTR gtok = 0;
	GdiplusStartup(&gtok, &gsi, 0);
	g_bg = LoadImageRes(IDR_BG);
	g_btnUp = LoadImageRes(IDR_BTN_UP);
	g_btnHover = LoadImageRes(IDR_BTN_HOVER);

	g_net = InternetOpenA("EALauncher/1.0", INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	DWORD to = 15000;
	InternetSetOptionA(g_net, INTERNET_OPTION_CONNECT_TIMEOUT, &to, sizeof(to));
	InternetSetOptionA(g_net, INTERNET_OPTION_RECEIVE_TIMEOUT, &to, sizeof(to));

	WNDCLASSA wc; ZeroMemory(&wc, sizeof(wc));
	wc.lpfnWndProc = WndProc; wc.hInstance = hInst;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIconA(hInst, MAKEINTRESOURCEA(IDI_LAUNCHER));
	if (!wc.hIcon) wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.lpszClassName = "EALauncherWnd";
	RegisterClassA(&wc);
	int sx = (GetSystemMetrics(SM_CXSCREEN) - WND_W) / 2, sy = (GetSystemMetrics(SM_CYSCREEN) - WND_H) / 2;
	g_wnd = CreateWindowExA(WS_EX_APPWINDOW, wc.lpszClassName, "Endless Ages", WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX,
	                        sx, sy, WND_W, WND_H, 0, 0, hInst, 0);
	ShowWindow(g_wnd, show);
	UpdateWindow(g_wnd);

	if (g_installMode)
	{
		g_play.text = "INSTALL";
		SetStatus(PH_INSTALL, GAME_TITLE " is not installed yet.",
		          "INSTALL puts it in " + InstallDir() + " and downloads the game files.", 0);
	}
	else
		StartUpdate();

	MSG m;
	while (GetMessageA(&m, 0, 0, 0) > 0)
	{
		if (g_accWnd && IsDialogMessageA(g_accWnd, &m)) continue;   // Tab / Enter in the account window
		TranslateMessage(&m); DispatchMessageA(&m);
	}

	InterlockedExchange(&g_quit, 1);
	for (int i = 0; i < 50 && g_workerRunning; i++) Sleep(100);   // let a download stop cleanly
	SaveCache();
	if (g_net) InternetCloseHandle(g_net);
	delete g_bg; delete g_btnUp; delete g_btnHover;
	GdiplusShutdown(gtok);
	LogLine("Launcher closed");
	if (g_logFile) fclose(g_logFile);
	if (mutex) { ReleaseMutex(mutex); CloseHandle(mutex); }
	return 0;
}

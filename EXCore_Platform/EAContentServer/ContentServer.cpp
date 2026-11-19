//======================================================================
// ContentServer.cpp  -  Endless Ages content (patch) server
//
// Serves the game's client files to EALauncher over plain HTTP so the
// launcher can bring a player's install up to date before the game starts.
// The game client itself never downloads anything.
//
//   Patch\                 <- put the current client files here, laid out
//                             exactly like C:\Program Files\Core
//   Patch\VERSIONINF.VER   <- the content version (same 4-byte file the
//                             client uses); bump it when you publish
//   ContentServer.cfg      <- Port=25858, PatchFolder=Patch,
//                             GameServer=<public address players connect to>
//
// HTTP endpoints (GET or HEAD):
//   /status            "ok"
//   /version           the version number as text
//   /manifest          every file in Patch\ with size and CRC32
//   /files/<path>      the file itself (Range: bytes=N- supported for resume)
//   POST /register     user=&pass=&email=  -> "OK" or "ERR <reason>"  (new account
//                      in EARevolution.dbo.gmAccountBase; 3 per address per hour)
//
// Manifest format (text, CRLF):
//   EACONTENT 1
//   version <n>
//   mirror <url>             (only when FileMirror= is set)
//   gameserver <address>     (only when GameServer= is set)
//   files <count>
//   <crc32 as 8 hex digits> <size in bytes> <path relative to Patch\, using />
//
// Only files listed in the manifest are ever served, so a request can never
// reach anything outside the patch folder.
//======================================================================
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <shellapi.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "odbc32.lib")

//----------------------------------------------------------------------
// globals
//----------------------------------------------------------------------
static HWND          g_wnd = 0, g_log = 0, g_status = 0;
static HWND          g_btnRescan = 0, g_btnFolder = 0, g_btnLog = 0;
static std::string   g_exeDir;
static std::string   g_patchDir;          // with trailing backslash
static std::string   g_logPath;
static unsigned short g_port = 25858;
static bool          g_registration = true;  // Registration=on|off
static std::string   g_accountDb = "Driver={SQL Server};Server=.;Database=EARevolution;Trusted_Connection=Yes;";
static int           g_maxCharacters = 4;
static std::string   g_mirror;            // FileMirror= : fast host with a copy of Patch\ (e.g. Cloudflare R2)
static std::string   g_gameServer;        // public game server address handed to launchers (GameServer= in the cfg)
static SOCKET        g_listen = INVALID_SOCKET;
static volatile LONG g_running = 1;
static volatile LONG g_activeConnections = 0;
static volatile LONG g_activeDownloads = 0;
static volatile LONGLONG g_bytesSent = 0;
static volatile LONG g_requests = 0;
static DWORD         g_startTick = 0;
static CRITICAL_SECTION g_logLock;

#define WM_CS_LOGLINE   (WM_APP + 1)
#define WM_CS_RESCANNED (WM_APP + 2)
#define IDC_RESCAN  101
#define IDC_FOLDER  102
#define IDC_OPENLOG 103
#define TIMER_STATUS 1
#define TIMER_RESCAN 2
#define MAX_CONNECTIONS 64

struct FileEntry
{
	std::string rel;          // forward slashes, as served
	ULONGLONG   size;
	FILETIME    mtime;
	DWORD       crc;
};

// manifest (swapped in whole after each scan)
static SRWLOCK               g_manifestLock = SRWLOCK_INIT;
static std::map<std::string, FileEntry> g_files;   // key: lower-case rel path
static std::string           g_manifestText;
static int                   g_version = 0;
static ULONGLONG             g_totalBytes = 0;
static volatile LONG         g_scanning = 0;

//----------------------------------------------------------------------
// logging (any thread)
//----------------------------------------------------------------------
static void Log(const char * fmt, ...)
{
	char body[1024];
	va_list ap; va_start(ap, fmt);
	_vsnprintf(body, sizeof(body) - 1, fmt, ap);
	body[sizeof(body) - 1] = 0;
	va_end(ap);

	SYSTEMTIME st; GetLocalTime(&st);
	char line[1200];
	_snprintf(line, sizeof(line) - 1, "%04d-%02d-%02d [%02d:%02d:%02d] %s",
	          st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, body);
	line[sizeof(line) - 1] = 0;

	EnterCriticalSection(&g_logLock);
	FILE * f = fopen(g_logPath.c_str(), "a");
	if (f) { fputs(line, f); fputs("\n", f); fclose(f); }
	LeaveCriticalSection(&g_logLock);

	if (g_wnd)
	{
		std::string * copy = new std::string(line + 11);   // window shows time only
		if (!PostMessageA(g_wnd, WM_CS_LOGLINE, 0, (LPARAM)copy))
			delete copy;
	}
}

static void AppendToLogWindow(const std::string & line)
{
	if (!g_log) return;
	int len = GetWindowTextLengthA(g_log);
	if (len > 400000)
	{//trim the oldest half of the on-screen log (the file keeps everything)
		SendMessageA(g_log, EM_SETSEL, 0, len / 2);
		SendMessageA(g_log, EM_REPLACESEL, FALSE, (LPARAM)"");
		len = GetWindowTextLengthA(g_log);
	}
	std::string t = line + "\r\n";
	SendMessageA(g_log, EM_SETSEL, len, len);
	SendMessageA(g_log, EM_REPLACESEL, FALSE, (LPARAM)t.c_str());
}

//----------------------------------------------------------------------
// CRC32 (same polynomial as zip / zlib)
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
static bool CrcFile(const std::string & path, DWORD & crcOut)
{
	HANDLE h = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
	                       OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
	if (h == INVALID_HANDLE_VALUE) return false;
	static const DWORD BUF = 1 << 20;
	std::vector<unsigned char> buf(BUF);
	DWORD crc = 0xFFFFFFFFu, got = 0;
	while (ReadFile(h, &buf[0], BUF, &got, 0) && got > 0)
		for (DWORD i = 0; i < got; i++) crc = g_crcTable[(crc ^ buf[i]) & 0xFF] ^ (crc >> 8);
	CloseHandle(h);
	crcOut = crc ^ 0xFFFFFFFFu;
	return true;
}

//----------------------------------------------------------------------
// patch folder scan
//----------------------------------------------------------------------
static std::string Lower(std::string s) { for (size_t i = 0; i < s.size(); i++) s[i] = (char)tolower((unsigned char)s[i]); return s; }

// Files a game install creates for itself (settings, logs, caches). They are never
// published, even if they end up in the Patch folder (e.g. the game was run from it):
// sending them would overwrite every player's own settings.
static bool SkipName(const std::string & nameIn, bool isDir)
{
	std::string n = Lower(nameIn);
	if (isDir) return n == "cache" || (!n.empty() && n[0] == '_');
	if (n == "updatenotes.txt") return false;   // players get the update notes too
	static const char * exact[] = { "coinit.tmp", "ealauncher.cfg", "ealauncher.crc", "ealauncher.log", "ealauncher.old",
	                                 "musicsetting.cfg", "thumbs.db", "desktop.ini", 0 };
	for (int i = 0; exact[i]; i++) if (n == exact[i]) return true;
	static const char * ext[] = { ".log", ".txt", ".part", ".pdb", ".tmp", ".new", 0 };
	for (int i = 0; ext[i]; i++)
	{
		size_t l = strlen(ext[i]);
		if (n.size() > l && n.compare(n.size() - l, l, ext[i]) == 0) return true;
	}
	return n.find(".bak") != std::string::npos;
}

static void Walk(const std::string & dir, const std::string & relPrefix, std::vector<FileEntry> & out)
{
	WIN32_FIND_DATAA fd;
	HANDLE h = FindFirstFileA((dir + "*").c_str(), &fd);
	if (h == INVALID_HANDLE_VALUE) return;
	do
	{
		std::string name = fd.cFileName;
		if (name == "." || name == "..") continue;
		if (fd.dwFileAttributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)) continue;
		if (SkipName(name, (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)) continue;
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			Walk(dir + name + "\\", relPrefix + name + "/", out);
		else
		{
			FileEntry e;
			e.rel = relPrefix + name;
			e.size = ((ULONGLONG)fd.nFileSizeHigh << 32) | fd.nFileSizeLow;
			e.mtime = fd.ftLastWriteTime;
			e.crc = 0;
			out.push_back(e);
		}
	} while (FindNextFileA(h, &fd));
	FindClose(h);
}

static int ReadVersionFile(const std::string & path)
{
	HANDLE h = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (h == INVALID_HANDLE_VALUE) return 0;
	int v = 0; DWORD got = 0;
	ReadFile(h, &v, sizeof(v), &got, 0);
	CloseHandle(h);
	return got == sizeof(v) ? v : 0;
}

static DWORD WINAPI ScanThread(LPVOID)
{
	DWORD t0 = GetTickCount();
	std::vector<FileEntry> found;
	Walk(g_patchDir, "", found);
	std::sort(found.begin(), found.end(), [](const FileEntry & a, const FileEntry & b) { return Lower(a.rel) < Lower(b.rel); });

	// reuse CRCs of files that did not change since the last scan
	std::map<std::string, FileEntry> old;
	AcquireSRWLockShared(&g_manifestLock);
	old = g_files;
	ReleaseSRWLockShared(&g_manifestLock);

	int hashed = 0;
	ULONGLONG total = 0;
	std::map<std::string, FileEntry> fresh;
	for (size_t i = 0; i < found.size(); i++)
	{
		FileEntry & e = found[i];
		std::string key = Lower(e.rel);
		std::map<std::string, FileEntry>::iterator it = old.find(key);
		if (it != old.end() && it->second.size == e.size &&
		    CompareFileTime(&it->second.mtime, &e.mtime) == 0)
			e.crc = it->second.crc;
		else
		{
			if (!CrcFile(g_patchDir + e.rel, e.crc))
			{
				Log("WARNING: could not read %s - left out of the manifest", e.rel.c_str());
				continue;
			}
			hashed++;
		}
		total += e.size;
		fresh[key] = e;
	}

	int version = ReadVersionFile(g_patchDir + "VERSIONINF.VER");

	std::string text = "EACONTENT 1\r\n";
	char line[600];
	_snprintf(line, sizeof(line), "version %d\r\n", version); text += line;
	if (!g_mirror.empty()) { _snprintf(line, sizeof(line) - 1, "mirror %s\r\n", g_mirror.c_str()); line[sizeof(line) - 1] = 0; text += line; }
	if (!g_gameServer.empty()) { _snprintf(line, sizeof(line) - 1, "gameserver %s\r\n", g_gameServer.c_str()); line[sizeof(line) - 1] = 0; text += line; }
	// Patch\UpdateNotes.txt: first line = update name ("Version 1.0.3"), the rest = what's new.
	// (the file itself is also sent to players; the launcher shows these lines.)
	{
		FILE * nf = fopen((g_patchDir + "UpdateNotes.txt").c_str(), "rb");
		if (nf)
		{
			char nb[400]; bool first = true; int count = 0;
			while (fgets(nb, sizeof(nb), nf) && count < 1000)   // every update in the notes file, not just the latest
			{
				std::string l = nb;
				while (!l.empty() && (l.back() == '\n' || l.back() == '\r')) l.pop_back();
				if (first) { if (l.empty()) continue; text += "name " + l + "\r\n"; first = false; }
				else { text += "notes " + l + "\r\n"; count++; }
			}
			fclose(nf);
		}
	}
	_snprintf(line, sizeof(line), "files %u\r\n", (unsigned)fresh.size()); text += line;
	for (std::map<std::string, FileEntry>::iterator it = fresh.begin(); it != fresh.end(); ++it)
	{
		_snprintf(line, sizeof(line) - 1, "%08lX %I64u %s\r\n", it->second.crc, it->second.size, it->second.rel.c_str());
		line[sizeof(line) - 1] = 0;
		text += line;
	}

	AcquireSRWLockExclusive(&g_manifestLock);
	bool changed = (text != g_manifestText);
	g_files.swap(fresh);
	g_manifestText.swap(text);
	g_version = version;
	g_totalBytes = total;
	ReleaseSRWLockExclusive(&g_manifestLock);

	if (changed)
		Log("Patch folder scanned: %u files, %.1f MB, content version %d (%d checksummed, %.1fs)",
		    (unsigned)g_files.size(), total / 1048576.0, version, hashed, (GetTickCount() - t0) / 1000.0);
	if (version == 0)
		Log("WARNING: Patch\\VERSIONINF.VER is missing - copy the client's VERSIONINF.VER into the Patch folder.");

	InterlockedExchange(&g_scanning, 0);
	if (g_wnd) PostMessageA(g_wnd, WM_CS_RESCANNED, 0, 0);
	return 0;
}

static void StartScan()
{
	if (InterlockedCompareExchange(&g_scanning, 1, 0) != 0) return;   // one at a time
	HANDLE t = CreateThread(0, 0, ScanThread, 0, 0, 0);
	if (t) CloseHandle(t); else InterlockedExchange(&g_scanning, 0);
}

// Re-scan automatically a few seconds after anything in Patch\ changes.
static DWORD WINAPI WatchThread(LPVOID)
{
	HANDLE ch = FindFirstChangeNotificationA(g_patchDir.c_str(), TRUE,
		FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE);
	if (ch == INVALID_HANDLE_VALUE) return 0;
	while (g_running)
	{
		if (WaitForSingleObject(ch, 1000) == WAIT_OBJECT_0)
		{
			if (g_wnd) SetTimer(g_wnd, TIMER_RESCAN, 3000, 0);   // debounce: copying many files fires many times
			if (!FindNextChangeNotification(ch)) break;
		}
	}
	FindCloseChangeNotification(ch);
	return 0;
}

//----------------------------------------------------------------------
// HTTP
//----------------------------------------------------------------------
static bool SendAll(SOCKET s, const char * p, int n)
{
	while (n > 0)
	{
		int r = send(s, p, n, 0);
		if (r <= 0) return false;
		p += r; n -= r;
		InterlockedExchangeAdd64(&g_bytesSent, r);
	}
	return true;
}

static void SendSimple(SOCKET s, int code, const char * reason, const std::string & body, bool head, const char * type = "text/plain")
{
	char hdr[512];
	_snprintf(hdr, sizeof(hdr), "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nContent-Length: %u\r\n"
	          "Cache-Control: no-cache\r\nConnection: close\r\nServer: EAContentServer\r\n\r\n",
	          code, reason, type, (unsigned)body.size());
	SendAll(s, hdr, (int)strlen(hdr));
	if (!head && !body.empty()) SendAll(s, body.data(), (int)body.size());
}

static std::string UrlDecode(const std::string & in)
{
	std::string out;
	for (size_t i = 0; i < in.size(); i++)
	{
		if (in[i] == '%' && i + 2 < in.size() && isxdigit((unsigned char)in[i + 1]) && isxdigit((unsigned char)in[i + 2]))
		{
			char hex[3] = { in[i + 1], in[i + 2], 0 };
			out += (char)strtol(hex, 0, 16);
			i += 2;
		}
		else if (in[i] == '+') out += ' ';
		else out += in[i];
	}
	return out;
}

static void ServeFile(SOCKET s, const std::string & ip, const std::string & relIn, bool head, ULONGLONG rangeStart, bool hasRange)
{
	std::string rel = relIn;
	std::replace(rel.begin(), rel.end(), '\\', '/');

	FileEntry e;
	bool found = false;
	AcquireSRWLockShared(&g_manifestLock);
	std::map<std::string, FileEntry>::iterator it = g_files.find(Lower(rel));
	if (it != g_files.end()) { e = it->second; found = true; }
	ReleaseSRWLockShared(&g_manifestLock);
	if (!found)
	{
		Log("%s  404 not in patch: %s", ip.c_str(), rel.c_str());
		SendSimple(s, 404, "Not Found", "not found\n", head);
		return;
	}

	std::string full = g_patchDir + e.rel;
	std::replace(full.begin(), full.end(), '/', '\\');
	HANDLE h = CreateFileA(full.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
	if (h == INVALID_HANDLE_VALUE)
	{
		Log("%s  ERROR cannot open %s (error %lu)", ip.c_str(), full.c_str(), GetLastError());
		SendSimple(s, 500, "Internal Server Error", "cannot open file\n", head);
		return;
	}
	LARGE_INTEGER sz; GetFileSizeEx(h, &sz);
	ULONGLONG size = (ULONGLONG)sz.QuadPart;
	if (hasRange && rangeStart >= size && size > 0)
	{
		CloseHandle(h);
		SendSimple(s, 416, "Range Not Satisfiable", "", head);
		return;
	}
	if (!hasRange) rangeStart = 0;
	ULONGLONG len = size - rangeStart;

	char hdr[600];
	if (hasRange && rangeStart > 0)
		_snprintf(hdr, sizeof(hdr), "HTTP/1.1 206 Partial Content\r\nContent-Type: application/octet-stream\r\n"
		          "Content-Length: %I64u\r\nContent-Range: bytes %I64u-%I64u/%I64u\r\nX-CRC32: %08lX\r\n"
		          "Accept-Ranges: bytes\r\nConnection: close\r\nServer: EAContentServer\r\n\r\n",
		          len, rangeStart, size - 1, size, e.crc);
	else
		_snprintf(hdr, sizeof(hdr), "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\n"
		          "Content-Length: %I64u\r\nX-CRC32: %08lX\r\nAccept-Ranges: bytes\r\nConnection: close\r\n"
		          "Server: EAContentServer\r\n\r\n", len, e.crc);
	bool ok = SendAll(s, hdr, (int)strlen(hdr));

	if (ok && !head && len > 0)
	{
		InterlockedIncrement(&g_activeDownloads);
		LARGE_INTEGER pos; pos.QuadPart = (LONGLONG)rangeStart;
		SetFilePointerEx(h, pos, 0, FILE_BEGIN);
		std::vector<char> buf(256 * 1024);
		ULONGLONG left = len;
		DWORD got = 0;
		DWORD t0 = GetTickCount();
		while (left > 0 && g_running)
		{
			DWORD want = (DWORD)((ULONGLONG)buf.size() < left ? (ULONGLONG)buf.size() : left);
			if (!ReadFile(h, &buf[0], want, &got, 0) || got == 0) { ok = false; break; }
			if (!SendAll(s, &buf[0], (int)got)) { ok = false; break; }
			left -= got;
		}
		InterlockedDecrement(&g_activeDownloads);
		double secs = (GetTickCount() - t0) / 1000.0;
		if (ok)
		{
			if (len >= 1048576)   // only big files, so a full patch does not flood the log
				Log("%s  sent %s (%.1f MB in %.1fs%s)", ip.c_str(), e.rel.c_str(), len / 1048576.0, secs,
				    rangeStart ? ", resumed" : "");
		}
		else
			Log("%s  download of %s stopped after %I64u of %I64u bytes (client closed or network dropped)",
			    ip.c_str(), e.rel.c_str(), len - left, len);
	}
	CloseHandle(h);
}

//----------------------------------------------------------------------
// account sign-up (POST /register from the launcher)
//----------------------------------------------------------------------
static CRITICAL_SECTION g_regLock;
static std::map<std::string, std::vector<DWORD> > g_regTimes;   // ip -> times of recent sign-ups
static std::vector<DWORD> g_regAll;                              // all recent sign-ups

static std::string FormValue(const std::string & body, const char * name)
{
	std::string key = std::string(name) + "=";
	size_t pos = 0;
	while (pos < body.size())
	{
		size_t amp = body.find('&', pos);
		if (amp == std::string::npos) amp = body.size();
		if (body.compare(pos, key.size(), key) == 0)
			return UrlDecode(body.substr(pos + key.size(), amp - pos - key.size()));
		pos = amp + 1;
	}
	return "";
}

static bool AllowedChars(const std::string & v, const char * extra)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		unsigned char c = (unsigned char)v[i];
		if (isalnum(c)) continue;
		if (strchr(extra, c)) continue;
		return false;
	}
	return true;
}

// keep sign-ups to 3 per address per hour and 30 per hour in total
static bool RateOk(const std::string & ip)
{
	DWORD now = GetTickCount();
	EnterCriticalSection(&g_regLock);
	std::vector<DWORD> & v = g_regTimes[ip];
	v.erase(std::remove_if(v.begin(), v.end(), [now](DWORD t) { return now - t > 3600000; }), v.end());
	g_regAll.erase(std::remove_if(g_regAll.begin(), g_regAll.end(), [now](DWORD t) { return now - t > 3600000; }), g_regAll.end());
	bool ok = v.size() < 3 && g_regAll.size() < 30;
	if (ok) { v.push_back(now); g_regAll.push_back(now); }
	LeaveCriticalSection(&g_regLock);
	return ok;
}

static std::string OdbcError(SQLSMALLINT type, SQLHANDLE h)
{
	SQLCHAR state[8] = "", msg[512] = ""; SQLINTEGER native = 0; SQLSMALLINT len = 0;
	if (SQL_SUCCEEDED(SQLGetDiagRecA(type, h, 1, state, &native, msg, sizeof(msg), &len)))
		return std::string((char *)state) + " " + (char *)msg;
	return "unknown database error";
}

// returns "" on success, or the message to show the player
static std::string CreateAccount(const std::string & user, const std::string & pass, const std::string & email)
{
	SQLHENV env = 0; SQLHDBC dbc = 0; SQLHSTMT st = 0;
	std::string err;
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
	SQLSetConnectAttr(dbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)10, 0);
	SQLCHAR out[1024]; SQLSMALLINT outLen = 0;
	if (!SQL_SUCCEEDED(SQLDriverConnectA(dbc, 0, (SQLCHAR *)g_accountDb.c_str(), SQL_NTS, out, sizeof(out), &outLen, SQL_DRIVER_NOPROMPT)))
	{
		Log("ERROR: account database connection failed: %s", OdbcError(SQL_HANDLE_DBC, dbc).c_str());
		err = "The account server is not available right now. Try again later.";
		goto done;
	}
	SQLAllocHandle(SQL_HANDLE_STMT, dbc, &st);
	{
		// name taken?
		SQLLEN ulen = SQL_NTS;
		SQLBindParameter(st, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 64, 0, (SQLPOINTER)user.c_str(), 0, &ulen);
		if (!SQL_SUCCEEDED(SQLExecDirectA(st, (SQLCHAR *)"SELECT COUNT(*) FROM dbo.gmAccountBase WHERE Username = ?", SQL_NTS)))
		{
			Log("ERROR: account lookup failed: %s", OdbcError(SQL_HANDLE_STMT, st).c_str());
			err = "The account server had a problem. Try again later.";
			goto done;
		}
		SQLINTEGER count = 0; SQLLEN ind = 0;
		if (SQL_SUCCEEDED(SQLFetch(st))) SQLGetData(st, 1, SQL_C_SLONG, &count, 0, &ind);
		SQLCloseCursor(st);
		SQLFreeStmt(st, SQL_RESET_PARAMS);
		if (count > 0) { err = "That username is already taken."; goto done; }

		// is there an Email column? (the 2003/2008 schemas differ)
		bool hasEmail = false;
		if (SQL_SUCCEEDED(SQLExecDirectA(st, (SQLCHAR *)"SELECT COL_LENGTH('dbo.gmAccountBase','Email')", SQL_NTS)))
		{
			SQLINTEGER cl = 0; SQLLEN ci = 0;
			if (SQL_SUCCEEDED(SQLFetch(st)) && SQL_SUCCEEDED(SQLGetData(st, 1, SQL_C_SLONG, &cl, 0, &ci)) && ci != SQL_NULL_DATA && cl > 0) hasEmail = true;
			SQLCloseCursor(st);
		}

		char sql[1024];
		_snprintf(sql, sizeof(sql),
		    "INSERT INTO dbo.gmAccountBase (Username, Password, GameAccess, PortalAccess, active, accessLevel, maxCharacters, "
		    "LoggedOn, isadmin, muteDeaf, MinutesPlayed, Joined, recurringDate%s) "
		    "VALUES (?, ?, 1, 1, '1', 1, %d, 0, 0, 0, 0, GETDATE(), DATEADD(year, 10, GETDATE())%s)",
		    hasEmail ? ", Email" : "", g_maxCharacters, hasEmail ? ", ?" : "");
		SQLLEN l1 = SQL_NTS, l2 = SQL_NTS, l3 = SQL_NTS;
		SQLBindParameter(st, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 64, 0, (SQLPOINTER)user.c_str(), 0, &l1);
		SQLBindParameter(st, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 64, 0, (SQLPOINTER)pass.c_str(), 0, &l2);
		if (hasEmail) SQLBindParameter(st, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, (SQLPOINTER)email.c_str(), 0, &l3);
		if (!SQL_SUCCEEDED(SQLExecDirectA(st, (SQLCHAR *)sql, SQL_NTS)))
		{
			Log("ERROR: creating account %s failed: %s", user.c_str(), OdbcError(SQL_HANDLE_STMT, st).c_str());
			err = "The account could not be created (server error). Try again later.";
			goto done;
		}
	}
done:
	if (st) SQLFreeHandle(SQL_HANDLE_STMT, st);
	if (dbc) { SQLDisconnect(dbc); SQLFreeHandle(SQL_HANDLE_DBC, dbc); }
	if (env) SQLFreeHandle(SQL_HANDLE_ENV, env);
	return err;
}

static void HandleRegister(SOCKET s, const std::string & ip, const std::string & body)
{
	std::string user = FormValue(body, "user"), pass = FormValue(body, "pass"), email = FormValue(body, "email");
	std::string bad;
	if (!g_registration) bad = "New accounts are closed right now.";
	else if (user.size() < 3 || user.size() > 16 || !AllowedChars(user, "_")) bad = "Username must be 3-16 letters, numbers or _.";
	else if (pass.size() < 4 || pass.size() > 20 || !AllowedChars(pass, "!@#$%^&*()-_=+.,?~")) bad = "Password must be 4-20 characters (letters, numbers and !@#$%^&*()-_=+.,?~).";
	else if (!email.empty() && (email.size() > 100 || email.find('@') == std::string::npos || !AllowedChars(email, "@._-+"))) bad = "That email address doesn't look right.";
	else if (!RateOk(ip)) bad = "Too many new accounts from your connection. Try again in an hour.";
	if (bad.empty()) bad = CreateAccount(user, pass, email);
	if (bad.empty())
	{
		Log("%s  NEW ACCOUNT created: %s", ip.c_str(), user.c_str());
		SendSimple(s, 200, "OK", "OK\n", false);
	}
	else
	{
		Log("%s  sign-up refused (%s): %s", ip.c_str(), user.c_str(), bad.c_str());
		SendSimple(s, 200, "OK", "ERR " + bad + "\n", false);
	}
}

static DWORD WINAPI ClientThread(LPVOID param)
{
	SOCKET s = (SOCKET)param;
	sockaddr_in addr; int alen = sizeof(addr);
	getpeername(s, (sockaddr *)&addr, &alen);
	char ipbuf[64] = "?";
	inet_ntop(AF_INET, &addr.sin_addr, ipbuf, sizeof(ipbuf));
	std::string ip = ipbuf;

	DWORD timeout = 15000;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
	timeout = 60000;
	setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));

	// read the request header (we never need a body)
	std::string req;
	char buf[2048];
	while (req.find("\r\n\r\n") == std::string::npos && req.size() < 16384)
	{
		int r = recv(s, buf, sizeof(buf), 0);
		if (r <= 0) break;
		req.append(buf, r);
	}

	size_t eol = req.find("\r\n");
	if (eol != std::string::npos)
	{
		InterlockedIncrement(&g_requests);
		std::string first = req.substr(0, eol);
		char method[16] = "", target[2048] = "";
		sscanf(first.c_str(), "%15s %2047s", method, target);
		bool head = strcmp(method, "HEAD") == 0;

		ULONGLONG rangeStart = 0; bool hasRange = false;
		std::string lowerReq = Lower(req);
		size_t rp = lowerReq.find("\r\nrange: bytes=");
		if (rp != std::string::npos)
		{
			rangeStart = _strtoui64(req.c_str() + rp + 15, 0, 10);
			hasRange = true;
		}

		std::string path = target;
		size_t q = path.find('?');
		if (q != std::string::npos) path = path.substr(0, q);

		if (strcmp(method, "POST") == 0 && path == "/register")
		{
			size_t hdrEnd = req.find("\r\n\r\n");
			size_t cl = 0, cp = lowerReq.find("\r\ncontent-length:");
			if (cp != std::string::npos) cl = (size_t)atoi(req.c_str() + cp + 17);
			std::string body = hdrEnd != std::string::npos ? req.substr(hdrEnd + 4) : "";
			if (cl > 4096) SendSimple(s, 413, "Too Large", "too large\n", false);
			else
			{
				while (body.size() < cl)
				{
					int r = recv(s, buf, sizeof(buf), 0);
					if (r <= 0) break;
					body.append(buf, r);
				}
				HandleRegister(s, ip, body.substr(0, cl));
			}
		}
		else if (strcmp(method, "GET") != 0 && !head)
			SendSimple(s, 405, "Method Not Allowed", "GET only\n", false);
		else if (path == "/status")
			SendSimple(s, 200, "OK", "ok\n", head);
		else if (path == "/version")
		{
			AcquireSRWLockShared(&g_manifestLock);
			int v = g_version;
			ReleaseSRWLockShared(&g_manifestLock);
			char t[32]; _snprintf(t, sizeof(t), "%d\n", v);
			SendSimple(s, 200, "OK", t, head);
		}
		else if (path == "/manifest")
		{
			AcquireSRWLockShared(&g_manifestLock);
			std::string m = g_manifestText;
			ReleaseSRWLockShared(&g_manifestLock);
			Log("%s  checked for updates", ip.c_str());
			SendSimple(s, 200, "OK", m, head);
		}
		else if (path.compare(0, 7, "/files/") == 0)
			ServeFile(s, ip, UrlDecode(path.substr(7)), head, rangeStart, hasRange);
		else if (path == "/")
		{
			AcquireSRWLockShared(&g_manifestLock);
			char t[256];
			_snprintf(t, sizeof(t), "Endless Ages content server\nversion %d\nfiles %u\n", g_version, (unsigned)g_files.size());
			ReleaseSRWLockShared(&g_manifestLock);
			SendSimple(s, 200, "OK", t, head);
		}
		else
			SendSimple(s, 404, "Not Found", "not found\n", head);
	}

	shutdown(s, SD_SEND);
	closesocket(s);
	InterlockedDecrement(&g_activeConnections);
	return 0;
}

static DWORD WINAPI ListenThread(LPVOID)
{
	while (g_running)
	{
		SOCKET c = accept(g_listen, 0, 0);
		if (c == INVALID_SOCKET)
		{
			if (!g_running) break;
			Sleep(50);
			continue;
		}
		if (InterlockedIncrement(&g_activeConnections) > MAX_CONNECTIONS)
		{
			SendSimple(c, 503, "Service Unavailable", "busy, try again\n", false);
			closesocket(c);
			InterlockedDecrement(&g_activeConnections);
			continue;
		}
		HANDLE t = CreateThread(0, 0, ClientThread, (LPVOID)c, 0, 0);
		if (t) CloseHandle(t);
		else { closesocket(c); InterlockedDecrement(&g_activeConnections); }
	}
	return 0;
}

static bool StartListening()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { Log("ERROR: WSAStartup failed"); return false; }
	g_listen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_listen == INVALID_SOCKET) { Log("ERROR: cannot create socket (%d)", WSAGetLastError()); return false; }
	BOOL excl = TRUE;   // refuse to share the port with another server
	setsockopt(g_listen, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (const char *)&excl, sizeof(excl));
	sockaddr_in a; ZeroMemory(&a, sizeof(a));
	a.sin_family = AF_INET; a.sin_port = htons(g_port); a.sin_addr.s_addr = INADDR_ANY;
	if (bind(g_listen, (sockaddr *)&a, sizeof(a)) != 0)
	{
		int err = WSAGetLastError();
		if (err == WSAEADDRINUSE || err == WSAEACCES)
			Log("ERROR: TCP port %u is already in use - another content server (or the old EAServer) is probably running. Close it and start this again.", g_port);
		else
			Log("ERROR: cannot open TCP port %u (error %d)", g_port, err);
		closesocket(g_listen); g_listen = INVALID_SOCKET;
		return false;
	}
	listen(g_listen, SOMAXCONN);
	HANDLE t = CreateThread(0, 0, ListenThread, 0, 0, 0);
	if (t) CloseHandle(t);
	Log("Content server listening on TCP port %u. Launchers can now check for updates.", g_port);
	return true;
}

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------
static void LoadConfig()
{
	std::string folder = "Patch";
	std::string cfg = g_exeDir + "ContentServer.cfg";
	FILE * f = fopen(cfg.c_str(), "r");
	if (!f)
	{//write a default so it's easy to find and edit
		f = fopen(cfg.c_str(), "w");
		if (f)
		{
			fputs("; Endless Ages content server settings\n"
			      "; Port the launchers connect to (TCP). Open it in the firewall/router for internet players.\n"
			      "Port=25858\n"
			      "; Folder holding the current client files (relative to this exe, or a full path)\n"
			      "PatchFolder=Patch\n"
			      "; Game server address players connect to (your public IP or DNS name, e.g. endlessages.myddns.me).\n"
			      "; The launcher writes it into each player's coinit.tmp. Leave empty to not touch coinit.tmp.\n"
			      "GameServer=\n"
			      "; Optional fast download host holding a copy of the Patch folder (same paths), e.g. a\n"
			      "; Cloudflare R2 public URL. Launchers download from it and fall back to this server.\n"
			      "FileMirror=\n"
			      "; New accounts from the launcher's NEW ACCOUNT button: on/off\n"
			      "Registration=on\n"
			      "; How many characters a new account may have\n"
			      "MaxCharacters=4\n"
			      "; Database for accounts (ODBC). Default: this PC's SQL Server, EARevolution, Windows login.\n"
			      "; AccountDb=Driver={SQL Server};Server=127.0.0.1;Database=EARevolution;Uid=contentsvr;Pwd=...;\n", f);
			fclose(f);
		}
	}
	else
	{
		char line[1024];
		while (fgets(line, sizeof(line), f))
		{
			std::string l = line;
			while (!l.empty() && (l.back() == '\n' || l.back() == '\r' || l.back() == ' ')) l.pop_back();
			if (l.empty() || l[0] == ';' || l[0] == '#') continue;
			size_t eq = l.find('=');
			if (eq == std::string::npos) continue;
			std::string k = Lower(l.substr(0, eq)), v = l.substr(eq + 1);
			if (k == "port") { int p = atoi(v.c_str()); if (p > 0 && p < 65536) g_port = (unsigned short)p; }
			else if (k == "patchfolder" && !v.empty()) folder = v;
			else if (k == "registration") g_registration = !(Lower(v) == "off" || v == "0" || Lower(v) == "no");
			else if (k == "accountdb" && !v.empty()) g_accountDb = v;
			else if (k == "maxcharacters") { int m = atoi(v.c_str()); if (m > 0 && m < 50) g_maxCharacters = m; }
			else if (k == "filemirror") { while (!v.empty() && v[0] == ' ') v.erase(0, 1); g_mirror = v; }
			else if (k == "gameserver") { while (!v.empty() && v[0] == ' ') v.erase(0, 1); g_gameServer = v; }
		}
		fclose(f);
	}
	if (folder.size() > 1 && folder[1] == ':') g_patchDir = folder;   // full path
	else g_patchDir = g_exeDir + folder;
	if (g_patchDir.back() != '\\') g_patchDir += "\\";
	CreateDirectoryA(g_patchDir.c_str(), 0);
}

//----------------------------------------------------------------------
// window
//----------------------------------------------------------------------
static void UpdateStatus()
{
	AcquireSRWLockShared(&g_manifestLock);
	int v = g_version; unsigned n = (unsigned)g_files.size(); ULONGLONG tb = g_totalBytes;
	ReleaseSRWLockShared(&g_manifestLock);
	DWORD up = (GetTickCount() - g_startTick) / 1000;
	char s[512];
	if (g_listen == INVALID_SOCKET)
		_snprintf(s, sizeof(s), "NOT LISTENING - see log");
	else
		_snprintf(s, sizeof(s), "Port %u  |  Version %d  |  %u files (%.1f MB)  |  Downloads now: %ld  |  Sent %.1f MB  |  Requests %ld  |  Uptime %02u:%02u:%02u%s",
		          g_port, v, n, tb / 1048576.0, g_activeDownloads, g_bytesSent / 1048576.0, g_requests,
		          (unsigned)(up / 3600), (unsigned)((up / 60) % 60), (unsigned)(up % 60), g_scanning ? "  |  scanning..." : "");
	char cur[512]; GetWindowTextA(g_status, cur, sizeof(cur));
	if (strcmp(cur, s) != 0) SetWindowTextA(g_status, s);
}

static void Layout(int w, int h)
{
	int bw = 130, bh = 26, pad = 8;
	MoveWindow(g_status, pad, pad, w - 2 * pad, 20, TRUE);
	MoveWindow(g_log, pad, pad + 26, w - 2 * pad, h - (pad + 26) - bh - 2 * pad, TRUE);
	int y = h - bh - pad;
	MoveWindow(g_btnRescan, pad, y, bw, bh, TRUE);
	MoveWindow(g_btnFolder, pad + (bw + pad), y, bw, bh, TRUE);
	MoveWindow(g_btnLog, pad + 2 * (bw + pad), y, bw, bh, TRUE);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		HFONT ui = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		HFONT mono = CreateFontA(-13, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		                         CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");
		g_status = CreateWindowA("STATIC", "", WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP, 0, 0, 0, 0, hwnd, 0, 0, 0);
		g_log = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE |
		                        ES_AUTOVSCROLL | ES_READONLY, 0, 0, 0, 0, hwnd, 0, 0, 0);
		g_btnRescan = CreateWindowA("BUTTON", "Rescan Patch folder", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_RESCAN, 0, 0);
		g_btnFolder = CreateWindowA("BUTTON", "Open Patch folder", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_FOLDER, 0, 0);
		g_btnLog = CreateWindowA("BUTTON", "Open log file", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_OPENLOG, 0, 0);
		SendMessageA(g_status, WM_SETFONT, (WPARAM)ui, 0);
		SendMessageA(g_log, WM_SETFONT, (WPARAM)mono, 0);
		SendMessageA(g_log, EM_SETLIMITTEXT, 0, 0);
		SendMessageA(g_btnRescan, WM_SETFONT, (WPARAM)ui, 0);
		SendMessageA(g_btnFolder, WM_SETFONT, (WPARAM)ui, 0);
		SendMessageA(g_btnLog, WM_SETFONT, (WPARAM)ui, 0);
		SetTimer(hwnd, TIMER_STATUS, 1000, 0);
		return 0;
	}
	case WM_SIZE:
		Layout(LOWORD(lp), HIWORD(lp));
		return 0;
	case WM_GETMINMAXINFO:
		((MINMAXINFO *)lp)->ptMinTrackSize.x = 560;
		((MINMAXINFO *)lp)->ptMinTrackSize.y = 300;
		return 0;
	case WM_TIMER:
		if (wp == TIMER_STATUS) UpdateStatus();
		else if (wp == TIMER_RESCAN) { KillTimer(hwnd, TIMER_RESCAN); StartScan(); }
		return 0;
	case WM_CS_LOGLINE:
	{
		std::string * line = (std::string *)lp;
		if (line) { AppendToLogWindow(*line); delete line; }
		return 0;
	}
	case WM_CS_RESCANNED:
		UpdateStatus();
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case IDC_RESCAN: Log("Rescan requested"); StartScan(); break;
		case IDC_FOLDER: ShellExecuteA(hwnd, "open", g_patchDir.c_str(), 0, 0, SW_SHOWNORMAL); break;
		case IDC_OPENLOG: ShellExecuteA(hwnd, "open", g_logPath.c_str(), 0, 0, SW_SHOWNORMAL); break;
		}
		return 0;
	case WM_CLOSE:
		if (MessageBoxA(hwnd, "Shut down the content server?\n\nLaunchers will not be able to update while it is off.",
		                "Endless Ages Content Server", MB_YESNO | MB_ICONQUESTION) == IDYES)
			DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		InterlockedExchange(&g_running, 0);
		if (g_listen != INVALID_SOCKET) { closesocket(g_listen); g_listen = INVALID_SOCKET; }
		Log("Content server stopped.");
		g_wnd = 0;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcA(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int show)
{
	InitializeCriticalSection(&g_logLock);
	InitializeCriticalSection(&g_regLock);
	InitCrc();
	g_startTick = GetTickCount();

	char exe[MAX_PATH] = { 0 };
	GetModuleFileNameA(0, exe, MAX_PATH);
	g_exeDir = exe;
	g_exeDir = g_exeDir.substr(0, g_exeDir.find_last_of('\\') + 1);
	SetCurrentDirectoryA(g_exeDir.c_str());
	g_logPath = g_exeDir + "ContentServerLog.txt";

	WNDCLASSA wc; ZeroMemory(&wc, sizeof(wc));
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wc.lpszClassName = "EAContentServerWnd";
	RegisterClassA(&wc);
	g_wnd = CreateWindowA(wc.lpszClassName, "Endless Ages Content Server", WS_OVERLAPPEDWINDOW,
	                      CW_USEDEFAULT, CW_USEDEFAULT, 980, 560, 0, 0, hInst, 0);
	ShowWindow(g_wnd, show);
	UpdateWindow(g_wnd);

	Log("================================================================");
	Log("Endless Ages content server  (pid %lu)", GetCurrentProcessId());
	Log("Log file: %s", g_logPath.c_str());
	LoadConfig();
	Log("Patch folder: %s", g_patchDir.c_str());
	if (g_gameServer.empty())
		Log("GameServer= is not set in ContentServer.cfg - launchers will leave players' game server address alone.");
	else
		Log("Game server address handed to launchers: %s", g_gameServer.c_str());
	if (!g_mirror.empty())
		Log("File mirror handed to launchers: %s (keep it in sync with the Patch folder)", g_mirror.c_str());
	Log(g_registration ? "New accounts from launchers: ON" : "New accounts from launchers: OFF");

	// first scan runs before we accept launchers, so nobody gets an empty manifest
	InterlockedExchange(&g_scanning, 1);
	ScanThread(0);
	if (g_files.empty())
		Log("The Patch folder is empty. Copy the current client files into it (same layout as C:\\Program Files\\Core).");
	HANDLE w = CreateThread(0, 0, WatchThread, 0, 0, 0);
	if (w) CloseHandle(w);
	if (!StartListening())
		MessageBoxA(g_wnd, "The content server could not open its port - see the log window.", "Endless Ages Content Server", MB_ICONERROR);
	UpdateStatus();

	MSG m;
	while (GetMessageA(&m, 0, 0, 0) > 0) { TranslateMessage(&m); DispatchMessageA(&m); }
	WSACleanup();
	return 0;
}

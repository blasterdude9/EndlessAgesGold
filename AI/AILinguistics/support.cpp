/* Programming/Implimentation: Arron Boucher.
   Debugging: Sean Barnes.
   08/07/2004
*/

#include "stdafx.h"
#include "support.h"

static char filename[MAX_PATH_LEN];
static unsigned short wfilename[MAX_PATH_LEN];

static bool nocompress;

// operating system routines
static CString m_cSPTDir = "**default**";

int getclock()
{
	return GetTickCount();
}

char *getdts(char *buf)
{
	SYSTEMTIME temp;
	temp.wHour = 0;
	temp.wMinute = 0;
	temp.wSecond = 0;
	GetLocalTime(&temp);
	sprintf(buf,"%d/%d/%d %d:%02d:%02d",temp.wMonth,temp.wDay,temp.wYear,temp.wHour,temp.wMinute,temp.wSecond);
	return buf;
}

char *getdate(char *buf)
{
	SYSTEMTIME temp;
	temp.wHour = 0;
	temp.wMinute = 0;
	temp.wSecond = 0;
	GetLocalTime(&temp);
	sprintf(buf,"%d/%d/%d",temp.wMonth,temp.wDay,temp.wYear);
	return buf;
}

char *gettime(char *buf)
{
	SYSTEMTIME temp;
	temp.wHour = 0;
	temp.wMinute = 0;
	temp.wSecond = 0;
	GetLocalTime(&temp);
	sprintf(buf,"%d:%02d:%02d",temp.wHour,temp.wMinute,temp.wSecond);
	return buf;
}

void timestamp(CString str)
{
	SYSTEMTIME temp;
	temp.wHour = 0;
	temp.wMinute = 0;
	temp.wSecond = 0;
	GetLocalTime(&temp);
	str.Format(_T("%d/%d/%d %d:%02d:%02d"),temp.wMonth,temp.wDay,temp.wYear,temp.wHour,temp.wMinute,temp.wSecond);
}

void randomize()
{
	SYSTEMTIME ltime;
	GetLocalTime(&ltime);
	srand(getclock());
}


char * fullpath(char *a_File)
{
	/*GetModuleFileName(0,filename,MAX_PATH_LEN);
	int pos = (int)strlen(filename);
	while (--pos) if (filename[pos] == '\\') break;
	strcpy(filename + pos + 1,a_File);*/
	
	CString temp = m_cSPTDir;
	temp = temp + a_File;
    return temp.GetBuffer();
	//return filename;
}

void SetPath(char* a_File)
{
  //AfxMessageBox(a_File);
  m_cSPTDir = a_File;
}

LPWSTR fullpath(LPWSTR a_File)
{
 	USES_CONVERSION;
	GetModuleFileName(0,filename,MAX_PATH_LEN);
	wcscpy(wfilename,A2W(filename));
	int pos = (int)wcslen(wfilename);
	if (pos) while (--pos) if (wfilename[pos] == '\\') break;
	wcscpy(wfilename + pos + 1,a_File);
	return (LPWSTR)wfilename;
}

bool fexists(char* file)
{
	FILE *test = fopen(file,"r");
	if (!test) return false;
	fclose(test);
	return true;
}

int random(int max)
{
	return (int)((float)rand() * max / RAND_MAX);
}

// XML parsing functions

bool XMLtag(char *txt,char *tag)
{
	char *x;
	char *y;

	if (!(x = strstr(txt,"<"))) return false;
	if (!strstr(x,">")) return false;
    for (y = tag;(*y++ = *x) != '>';x++);
	*y = 0;
	return true;
}

bool XMLvalue(char *tag,char *attr,char *value)
{
	char buf[MAX_TAG_LEN];
	char *x;
	char *y;

	strcpy(buf,attr);
	strcat(buf,"=");
	if (!(y = strstr(tag,buf))) return false;
	x = y + strlen(buf);

	if (*x == '\"') {
		for (y = value,x++;*x && *x != '\"';x++) *y++ = *x;
	} else if (*x == '\'') {
		for (y = value,x++;*x && *x != '\'';x++) *y++ = *x;
	} else {
		for (y = value;*x && !strchr(" />",*x);x++) *y++ = *x;
	}
	*y = 0;
	return (*x != 0);
}

bool XMLnode(char *text,char *otag,char *data,char *ctag)
{
	char *x;
	char *y;
	char *z;
	char *at;
	bool fnd;
	bool q = false;   // adjusted to skip > in quoted attribute strings for stricter xml
	int len;

	if (*text != '<') return false;
	if (!strstr(text,">")) return false;
    for (x = text,y = otag;(*y++ = *x) != '>' || q;x++) if (*x == '\"') q = !q; *y = 0;

	if (*(y - 2) == '/') {
		*data = 0;
		*ctag = 0;
		return true;
	}
	at = x;
	while (z = strstr(at,"<")) {
		if (!strstr(z,">")) return false;
		if (*(z + 1) == '/' && *(z + 2) == *(text + 1)) break; // extra check also matches first letter of tag name
		char *otag1 = new char[MAX_TAG_LEN];
		char *data1 = new char[MAX_SIZE];
		char *ctag1 = new char[MAX_TAG_LEN];
		fnd = XMLnode(z,otag1,data1,ctag1);
		if (fnd) len = (int)(strlen(otag1) + strlen(data1) + strlen(ctag1));
		delete ctag1;
		delete data1;
		delete otag1;
		if (!fnd) return false;
		at = z + len - 1;
	}
	if (!z) return false;
	for (x++,y = data;x < z;x++) *y++ = *x; *y = 0;
	for (y = ctag;(*y++ = *z) != '>';z++);  *y = 0;
	return true;
}

// text manipulation

void remove(char *text,char *first,char *last)
{
	char *pos1, *pos2;
	if (!(pos1 = strstr(text,first))) return;
	if (!(pos2 = strstr(pos1,last))) return;
	*pos1 = 0;
	strcat(pos1,pos2 + strlen(last));
}

bool replace(char *line,char *string1,char *string2)
{
	char buf[MAX_SIZE];
	char *pos = strstr(line,string1);
	if (pos) {
		strcpy(buf,pos + strlen(string1));
		strcpy(pos,string2);
		strcat(pos,buf);
		return true;
	}
	return false;
}

void split(char *line,char *text,int cnt)
{
	char *at;
	char *pos = line;
	int i;

	if (cnt == 0) { strcpy(line,text); return; }

	for (at = text,i = 0;*at;at++) {
		if (*at == '.' || *at == ';' || *at == '?' || *at == '!') i++;
	}
	for (at--;at > text;at--) if (*at > ' ') break;
	if (*at != '.' && *at != ';' && *at != '?' && *at != '!') i++;
	for (at = text;*at && i >= cnt;at++) {
		if (*at == '.' || *at == ';' || *at == '?' || *at == '!') {
			i--;
			while (*at && *at <= ' ') at++;
			continue;
		}
		if (i == cnt) { if (*at >= ' ') *pos++ = *at; }
	}
	*pos = 0;
}

void cleanspace(char *text)
{
	char a = ' ';
	char *b = text;

	for (char *c = text;*c;c++) {
		if (*c == ' ') {
			if (a != *c) *b++ = *c;
			a = *c;
		} else if (*c < ' ') { a = ' ';
		} else a = *b++ = *c;
	}
	*b = 0;
}

void cleaner(char *text)
{
	char *b = text;

	for (char *c = text;*c;c++) {
		if (isalnum(*c) || strchr(" @-:\'",*c) || (isalnum(*(c + 1)) && *c == '.') ) *b++ = *c;
	}
	*b-- = 0;
	while (b >= text && *b <= ' ') *b-- = 0;
}

void rtrim(char *text)
{
	for (char *c = text + strlen(text);c >= text;c--) 
		if (*c < 32) *c = 0; 
		else break;
}

void fixnewlines(char *line)
{
	while (replace(line,"<br>","\r\n"));
	while (replace(line,"<br/>","\r\n"));
	while (replace(line,"<br />","\r\n"));
	while (replace(line,"<BR>","\r\n"));
	while (replace(line,"<BR/>","\r\n"));
	while (replace(line,"<BR />","\r\n"));
	while (replace(line,"<Br>","\r\n"));
	while (replace(line,"<Br/>","\r\n"));
	while (replace(line,"<Br />","\r\n"));
}

void uppercase(char *text)
{
	for (char *c = text;*c;c++)	if (islower(*c)) *c += 'A' - 'a';
}

void lowercase(char *text)
{
	for (char *c = text;*c;c++)	if (isupper(*c)) *c -= 'A' - 'a';
}

void propercase(char *text)
{
	bool flg = true;

	for (char *c = text;*c;c++)	{
		if (islower(*c)) {
			if (flg) *c += 'A' - 'a';
			flg = false;
		} else if (isupper(*c)) {
			if (flg) flg = false; else *c -= 'A' - 'a';
		} else flg = true;
	}
}

// text comparisons

int strpcmp(char *s1,char *s2)
{
	char *a = s1;
	char *b = s2;
	char c1, c1x;
	char c2, c2x;

	if (!*a && !*b) return 0;

	c1 = *a;
	c2 = *b;
	if (islower(c1)) c1 += 'A' - 'a';
	if (islower(c2)) c2 += 'A' - 'a';
	if (c1 != c2) {
		if (c1 == '_') return -1;
		if (c2 == '_') return 1;
		if (c1 == '*') return 1;
		if (c2 == '*') return -1;
		if (isalpha(c1)) {	if (isalpha(c2) && (c1 > c2)) return 1; }
		else if (isalpha(c2)) return 1;
		else if (c1 == '[') return -1;
		else if (c2 == '[') return 1;
		else if (c1 > c2) return 1;
		return -1;
	}
	c1x = c1;
	c2x = c2;
	a++;
	b++;

	while (*a || *b) {
		c1 = *a;
		c2 = *b;
		if (islower(c1)) c1 += 'A' - 'a';
		if (islower(c2)) c2 += 'A' - 'a';
		if (c1 != c2) {
			if (c1 == '_') return -1;
			if (c2 == '_') return 1;
			if (c1 == '[') return -1;
			if (c2 == '[') return 1;
			if (c1 == '*') return 1;
			if (c2 == '*') return -1;
			if (c1x == '*') if (c1 == '@' || c1 == '!' || c1 == '[') return 1;
			if (c2x == '*') if (c2 == '@' || c2 == '!' || c2 == '[') return -1;
			if (c1x == '_') if (c1 == '@' || c1 == '!' || c1 == '[') return 1;
			if (c2x == '_') if (c2 == '@' || c2 == '!' || c2 == '[') return -1;
			if (c1 > c2)   return 1;
			return -1;
		}
		a++;
		b++;
		c1x = c1;
		c2x = c2;
	}
	return 0;
}

int stricmp(char *s1,char *s2)
{
	char *a = s1;
	char *b = s2;
	char c1;
	char c2;

	while (*a || *b) {
		c1 = *a;
		c2 = *b;
		if (isupper(c1)) c1 -= 'A' - 'a';
		if (isupper(c2)) c2 -= 'A' - 'a';
		if (c1 != c2) { if (c1 < c2) return -1; else return 1; }
		a++;
		b++;
	}
	return 0;
}

int strnicmp(char *s1,char *s2,int cnt)
{
	char *a = s1;
	char *b = s2;
	char c1;
	char c2;

	for (int i = 0;*a || *b;i++) {
		if (i == cnt) break;
		c1 = *a;
		c2 = *b;
		if (isupper(c1)) c1 -= 'A' - 'a';
		if (isupper(c2)) c2 -= 'A' - 'a';
		if (c1 != c2) { if (c1 < c2) return -1; else return 1; }
		a++;
		b++;
	}
	return 0;
}

char *stristr(char *s1,char *s2)
{
	char *a;
	char *b;
	char *c;
	char c1;
	char c2;

	for (a = s1;*a;a++) {
		for (b = s2,c = a;*b && *c;b++,c++) {
			c1 = *b;
			c2 = *c;
			if (isupper(c1)) c1 -= 'A' - 'a';
			if (isupper(c2)) c2 -= 'A' - 'a';
			if (c1 != c2) break;
		}
		if (!*b) return a;
	}
	return 0;
}

bool patrns(char *line,char *best)
{
	if (*best == '_' && *line != '_') return true;
	if (*line == '*' && *best != '*') return true;
	if (*line == *best) {
		if (strstr(line,"*") && !strstr(best,"*") && !strstr(best,"_")) return true;
		if (strlen(line) < strlen(best)) { if (!strnicmp((char*)line,(char*)best,(int)strlen(line))) return true; }
		if (stricmp(line,best) < 0) return true;
	}
	return false;
}

// Space saving optimizations

void expand(char *line)
{
	if (nocompress) return;

	while (replace(line,"~}","</srai>"));
	while (replace(line,"~{","<srai>"));
	while (replace(line,"~%","<random><li>"));
	while (replace(line,"~&","</li><li>"));
	while (replace(line,"~@","</li></random>"));
	while (replace(line,"~Q","</set>"));
	while (replace(line,"~M"," can "));
	while (replace(line,"~L","index="));
	while (replace(line,"~K","name="));
	while (replace(line,"~J","ell me "));
	while (replace(line,"~I","<usesaved/>"));
	while (replace(line,"~H"," favorite "));
	while (replace(line,"~G"," about "));
	while (replace(line,"~F","WHAT "));
	while (replace(line,"~E","I WAS "));
	while (replace(line,"~D","IT IS "));
	while (replace(line,"~C","I HAVE "));
	while (replace(line,"~B","I DID "));
	while (replace(line,"~A","I AM "));
	while (replace(line,"~z","<ironic/>"));
	while (replace(line,"~y","<upset/>"));
	while (replace(line,"~x","<worried/>"));
	while (replace(line,"~w","<serious/>"));
	while (replace(line,"~U","</save>"));
	while (replace(line,"~u","<save>"));
	while (replace(line,"~t","<suspecting/>"));
	while (replace(line,"~s","<angry/>"));
	while (replace(line,"~r","<getname/>"));
	while (replace(line,"~q","<set "));
	while (replace(line,"~p","<get "));
	while (replace(line,"~o","</think>"));
	while (replace(line,"~n","<think>"));
	while (replace(line,"~m","<star/>"));
	while (replace(line,"~l","<thinking/>"));
	while (replace(line,"~k","<interested/>"));
	while (replace(line,"~j","<happy/>"));
	while (replace(line,"~i","<glad/>"));
	while (replace(line,"~h","<funny/>"));
	while (replace(line,"~g","<puzzled/>"));
	while (replace(line,"~f","I don't "));
	while (replace(line,"~e","I am "));
	while (replace(line,"~d","o you "));
	while (replace(line,"~c"," you"));
	while (replace(line,"~b","<sr/>"));
	while (replace(line,"~a","<person/>"));
	while (replace(line,"~)","</system>"));
	while (replace(line,"~(","<system>"));
}

void compress(char *line)
{
	if (nocompress) return;

	while (replace(line,"<srai>","~{"));
	while (replace(line,"</srai>","~}"));
	while (replace(line,"<random><li>","~%"));
	while (replace(line,"</li><li>","~&"));
	while (replace(line,"</li></random>","~@"));
	while (replace(line,"<system>","~("));
	while (replace(line,"</system>","~)"));
	while (replace(line,"<person/>","~a"));
	while (replace(line,"<sr/>","~b"));
	while (replace(line," you","~c"));
	while (replace(line,"o you ","~d"));
	while (replace(line,"I am ","~e"));
	while (replace(line,"I don't ","~f"));
	while (replace(line,"<puzzled/>","~g"));
	while (replace(line,"<funny/>","~h"));
	while (replace(line,"<glad/>","~i"));
	while (replace(line,"<happy/>","~j"));
	while (replace(line,"<interested/>","~k"));
	while (replace(line,"<thinking/>","~l"));
	while (replace(line,"<star/>","~m"));
	while (replace(line,"<think>","~n"));
	while (replace(line,"</think>","~o"));
	while (replace(line,"<get ","~p"));
	while (replace(line,"<set ","~q"));
	while (replace(line,"</set>","~Q"));
	while (replace(line,"<getname/>","~r"));
	while (replace(line,"<angry/>","~s"));
	while (replace(line,"<suspecting/>","~t"));
	while (replace(line,"<save>","~u"));
	while (replace(line,"</save>","~U"));
	while (replace(line,"<serious/>","~w"));
	while (replace(line,"<worried/>","~x"));
	while (replace(line,"<upset/>","~y"));
	while (replace(line,"<ironic/>","~z"));
	while (replace(line,"I AM ","~A"));
	while (replace(line,"I DID ","~B"));
	while (replace(line,"I HAVE ","~C"));
	while (replace(line,"IT IS ","~D"));
	while (replace(line,"I WAS ","~E"));
	while (replace(line,"WHAT ","~F"));
	while (replace(line," about ","~G"));
	while (replace(line," favorite ","~H"));
	while (replace(line,"<usesaved/>","~I"));
	while (replace(line,"ell me ","~J"));
	while (replace(line,"name=","~K"));
	while (replace(line,"index=","~L"));
	while (replace(line," can ","~M"));
}

void setcompress(bool val)
{
	nocompress = !val;
}

void newstr(char **s1,char *s2)
{
	*s1 = new char[strlen(s2) + 2];
	strcpy(*s1,s2);
}

bool isNumeric(CString s)
{
	int i;
	bool bPeriod = false;
	bool bSign = false;
	char c;

	for (i = 0;i < s.GetLength();i++) {
		c = (char) s[i];
		if (c >= '0' && c <= '9') { bSign = true; continue; }
		if (c == '.') {
			if (bPeriod) return false;
			bPeriod = true;
			continue;
		}
		if (c == '-') {
			if (bSign) return false;
			bSign = true;
			continue;
		}
		if (c == '+') {
			if (bSign) return false;
			bSign = true;
			continue;
		}
		return false;
	}
	return true;
}

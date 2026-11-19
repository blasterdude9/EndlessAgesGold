/* Programming/Implimentation: Arron Boucher.
   Debugging: Sean Barnes.
   08/07/2004
*/

// pu.cpp: implementation of the PUvm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pu.h"
#include "wn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DWORD   dwAccessType = PRE_CONFIG_INTERNET_ACCESS;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PUvm::PUvm()
{
	char *fnpose;

	blnInput = false;
	buf = new char[MAX_SIZE];

	xpt.kb = &kb;
	xpt.atr = &atr;

	kb.Add("New Cases");
	kb.curSub->m_cases.AddGbl();
	kb.curSub->m_cases.AddShare();

	kb.AddLoc("~~stack~~");
	kb.AddTag("~~events~~");

	cmdlist = CNIL;
	newstr(&jfrm,"Initialize");
	jflg = CNIL;
	newstr(&lastinput,"");
	newstr(&lastreply,"");

	newstr(&fnmname,fullpath("ref\\MaleName.txt"));
	newstr(&fnfname,fullpath("ref\\FemaleName.txt"));
	newstr(&fnbadwd,fullpath("ref\\badwords.txt"));
	newstr(&fnstop, fullpath("ref\\stoplist.txt"));
	newstr(&fnpgm,fullpath("data\\"));
	newstr(&fnrul,fullpath("data\\"));
	newstr(&fnkb,fullpath("data\\"));
	newstr(&fnpose,"pu");
	newstr(&startup,"");
	startbot = false;

	FILE *fdirs = fopen(fullpath("options.ini"),"r");
    if (fdirs) {
		while (!feof(fdirs)) {
			fgets(buf,MAX_BUF_LEN,fdirs);
			rtrim(buf);
			if (!strncmp(buf,"stoplist=",9))     { delete fnstop;  newstr(&fnstop,  buf + 9);  }
			if (!strncmp(buf,"malenames=",10))   { delete fnmname; newstr(&fnmname, buf + 10); }
			if (!strncmp(buf,"femalenames=",12)) { delete fnfname; newstr(&fnmname, buf + 12); }
			if (!strncmp(buf,"badwords=",9))     { delete fnbadwd; newstr(&fnbadwd, buf + 9);  }
			if (!strncmp(buf,"scripts=",8))      { delete fnpgm;   newstr(&fnpgm,   buf + 8);  }
			if (!strncmp(buf,"rules=",6))        { delete fnrul;   newstr(&fnrul,   buf + 6);  }
			if (!strncmp(buf,"facts=",6))        { delete fnkb;    newstr(&fnkb,    buf + 6);  }
			if (!strncmp(buf,"pose=",5))         { delete fnpose;  newstr(&fnpose,  buf + 5);  }
			if (!strncmp(buf,"startcmd=",9))     { delete startup; newstr(&startup, buf + 9);  }
			if (!strncmp(buf,(char*)"startbot=",9))     { startbot = !strnicmp(buf + 9,(char*)"true",4);     }
		}
		fclose(fdirs);
	}

	tmpcnt = 1;
	kcnt = 0;
	aflg = false;
	dflg = true;
	pflg = false;
	qflg = false;
	xflg = false;
	pinpos = -1;

	if (*fnpose) utpose(fnpose);
	delete fnpose;
}

PUvm::~PUvm()
{
	delete buf;
	delete lastinput;
	delete lastreply;
	delete fnmname;
	delete fnfname;
	delete fnbadwd;
	delete fnstop;
	delete fnpgm;
	delete fnrul;
	delete fnkb;
	delete startup;
	if (jfrm != CNIL) delete jfrm;
	closefps();
}

int PUvm::docmds(char *at)
{
	char *i = cmdlist;
	char *j;
	int m = FAILURE;

	strcpy(curfrm,"");
	strcpy(cursit,"");
	if (at != CNIL) i = at; else if (*i == '{') { while (*i && *i != '}') i++; if (*i) i++; }
	while (cmdlist != CNIL) {
		rtrim(i);
		jflg = CNIL;
		kflg = true;
		xflg = false;
		m = SUCCESS;

		for (j = nxtcmd(i);j != CNIL;i = j,j = nxtcmd(i)) {
			while (*i && isspace(*i)) i++;
			ptr = i;
			if (cmdhash(i) || blnInput) break;
			if (xflg) break;
			if (jflg != CNIL) { j = jflg; strcpy(curfrm,jfrm); jflg = CNIL; }
			if (kflg) strcpy(cursit,""); else kflg = true;
			if (*j == '?' && kcnt > 0) {
				kcnt--;
				j++;
				while (*j && isspace(*j)) j++;
				if (*j == ',') j++;
				while (*j && isspace(*j)) j++;
			}
		}

		delete cmdlist; cmdlist = CNIL;
		if (qflg || blnInput) break;

		if (stkret()) {
			if (jflg == CNIL) i = cmdlist; else i = jflg;
		} else break;
	}
	if (blnInput) {
		SetCaretBlinkTime(200);
		CEdit& edit = host->GetEditCtrl();
		dwAtSel = edit.GetSel();
	}
	return (m);
}

char *PUvm::nxtcmd(char *at)
{                    
	char *i;  

	i = at;
	if (!*i || *i == '{' || *i == '?' || *i == ';') return (CNIL);

	if (*i == '[') {
		while (*i && *i != '?') if (*i++ == '\"') while (*i && *i++ != '\"');
		if (*i) i++;
	}

	while (*i && *i != ',' && *i != '{' && *i != '?' && *i != ';') {
		if (*i == '(') while (*i && *i != ')') i++;
		if (*i++ == '\"') while (*i && *i++ != '\"');
	}
	if (*i == ',') i++;
	return i;
}

int PUvm::cmdhash(char *s)
{
	char *x = strstr(s,"_");
	if (x) {                                     // catches most facts: 20 commands still may mask parsing a fact
		if (!strchr("\'\"%$:([",*s)) {
			char *y = strstr(s," ");
			char *z = strstr(s,",");
			if  (!y || y > z || x < y) {
				dumm = unknown(s);
				while (*ptr && *ptr <= ' ') ptr++;
				return dumm;
			}
		}
	}

	switch (*s) {
		case '\'':             break; // comments
		case '\"': display(s); break;
		case '%':
		case '$':  assign(s);  break;
		case ':':  branch(s);  break;
		case '(':  situate(s); break;
		case '[':  condite(s); break;
		case 'a':  cmda(s);    break;
		case 'b':  cmdb(s);    break;
		case 'c':  cmdc(s);    break;
		case 'd':  cmdd(s);    break;
		case 'e':  cmde(s);    break;
		case 'f':  cmdf(s);    break;
		case 'g':  cmdg(s);    break;
		case 'h':  cmdh(s);    break;
		case 'i':  cmdi(s);    break;
		case 'k':  cmdk(s);    break;
		case 'l':  cmdl(s);    break;
		case 'm':  cmdm(s);    break;
		case 'n':  cmdn(s);    break;
		case 'o':  cmdo(s);    break;
		case 'p':  cmdp(s);    break;
		case 'q':  cmdq(s);    break;
		case 'r':  cmdr(s);    break;
		case 's':  cmds(s);    break;
		case 'u':  cmdu(s);    break;
		case 'w':  cmdw(s);    break;
		default:
			dumm = unknown(s);
			while (*ptr && *ptr <= ' ') ptr++;
			return dumm;
	}
	while (*ptr && *ptr <= ' ') ptr++;
	return SUCCESS;
}


void PUvm::stkpush(char *at)
{
	char savid[32];
	char tmp[80];

	savslot("%~~STKFRAME~~",curfrm);

	savslot("%~~STKSITUATION~~",cursit);

	savslot("%~~STKCMDS~~",cmdlist);

	sprintf(tmp,"%d",at - cmdlist);
	savslot("%~~STKPOSITION~~",tmp);

	sprintf(savid,"%s",kb.curLoc->m_cases.curCase->m_name);
	sprintf(tmpid,"~~cmd%d~~",tmpcnt++);

	kb.curLoc->m_cases.Add(tmpid);
	savslot("%~~STKRETURN~~",savid);
}

bool PUvm::stkpop()
{
	char wrkid[32];

	if (!getslot("%~~STKRETURN~~",tmpid)) return false;

	sprintf(wrkid,"%s",kb.curLoc->m_cases.curCase->m_name);
	kb.curLoc->m_cases.Remove(wrkid);
	kb.curLoc->m_cases.Find(tmpid);

	return true;
}

bool PUvm::stkret()
{
	char wrkid[32];
	int at;

	if (!getslot("%~~STKRETURN~~",tmpid)) return false;

	sprintf(wrkid,"%s",kb.curLoc->m_cases.curCase->m_name);
	kb.curLoc->m_cases.Remove(wrkid);
	kb.curLoc->m_cases.Find(tmpid);

	if (getslot("%~~STKFRAME~~",buf)) {
		strcpy(curfrm,buf);
		if (jfrm != CNIL) delete jfrm;
		newstr(&jfrm,buf);
	} else {
		strcpy(curfrm,"");
		jfrm = CNIL;
	}

	getslot("%~~STKSITUATION~~",cursit);
	if (*cursit) kflg = false;

	getslot("%~~STKCMDS~~",buf);
	if (cmdlist != CNIL) delete cmdlist;
	newstr(&cmdlist,buf);

	jflg = CNIL;
	getslot("%~~STKPOSITION~~",buf);
	if (*buf) { sscanf(buf,"%d",&at); if (at >= 0) jflg = cmdlist + at; }
	if (*jflg == '?') {
		jflg++;
		while (*jflg && isspace(*jflg)) jflg++;
		if (*jflg == ',') jflg++;
		while (*jflg && isspace(*jflg)) jflg++;
	}

	return true;
}

// script command type: situation tag () 
void PUvm::situate(char *at)
{
	char *i;
	int k;

	for (k = 0,i = at;k < 63 && (cursit[k++] = *i++) != ')';);
	cursit[k] = 0;
	kflg = false;
	while (*i && isspace(*i)) i++;
	jflg = i;

/* path mark stuff */
	if (*i != '[') {
		sprintf(buf,"(%s%s",curfrm,cursit);
		savslot(buf,"TRUE");
	}
}

// script command type: display "" 
void PUvm::display(char *at)
{
	char *pos = buf;
	char *fnd = CNIL;

	for (at++;*at && (!strncmp(at,"\"\"",2) || *at != '\"');*pos++ = *at++) if (!strncmp(at,"\"\"",2)) at++;
	*pos = 0;
	post(buf);
	if (*at == '\"') at++;
	while (*at && isspace(*at)) at++;
	while (*at && (*at == '$' || *at == '%')) {
		at = fndtag(at,&fnd);
		if (getslot(fnd,buf)) post(buf);
		delete fnd;
		while (*at && isspace(*at)) at++;
		if (*at == '\"') {
			pos = buf;
			for (at++;*at && (!strncmp(at,"\"\"",2) || *at != '\"');*pos++ = *at++) if (!strncmp(at,"\"\"",2)) at++;
			*pos = 0;
			post(buf);
			if (*at == '\"') at++;
			while (*at && isspace(*at)) at++;
		}
	}
}

// script command type: assignment $%
void PUvm::assign(char *at)
 {
	char *i, *j;
	char *fnd = CNIL;
	char *wrk = CNIL;

	i = fndtag(at,&fnd);

	if (*i != '=') {
		if (aflg) {
			strcpy(lastinput,"");
			if (getslot(fnd,buf)) {
				post(buf);
				delete lastinput;
				newstr(&lastinput,buf);
			} else {
				pppost(" Unknown ",fnd," ");
			}
		} else {
			blnInput = true;
			savslot("%~~inputslot~~",fnd);
			i = nxtcmd(at);
			stkpush(i);
		}
	} else {
		i++;
		while (*i && isspace(*i)) i++;

		for (j = buf;*i && *i != ',' && *i != ';' && *i != '?' && *i != '{';i++) {
			*j++ = *i;
			if (*i == '\"') {
				for (i++;*i && (!strncmp(i,"\"\"",2) || *i != '\"');*j++ = *i++) if (!strncmp(i,"\"\"",2)) *j++ = *i++;
				if (*i == '\"') *j++ = *i; else i--;
			} else if (*i == '(') for (i++;(*j++ = *i) != ')';i++);
		}
		*j = 0;

		newstr(&wrk,buf);
		calc(buf);
		if (!*buf) {
			if (*wrk == '$') fixslot(wrk + 1,buf); 
			else if (*wrk != '%') strcpy(buf,wrk);
		}
		delete wrk;

/* what to do here with fnd ending in a subscript, i.e. ,')'?
   Two possibilities; a situation tag or a multivalue list item --
   If the slot already exists and it does not end with '(' and the data
     between the ( ) formats to a list of expressions, then it is MV.
   Otherwise it should be treated as a situation tag. */

		if (!strcmp(fnd,"SUBJECT")) kb.Find(buf);
		else if (!strcmp(fnd,"CASE")) kb.curSub->m_cases.Find(buf);
		else if (!strcmp(fnd,"SCRIPT")) pgm.Find(buf);
		else if (!strcmp(fnd,"TOPIC"))  { delete bot->topic;  newstr(&(bot->topic),buf);  }
		else if (!strcmp(fnd,"CLIENT")) { delete bot->client; newstr(&(bot->client),buf); }
		else if (strcmp(fnd,"LAST")) savslot(fnd,buf);
		else {
			delete lastinput;
			newstr(&lastinput,buf);
		}
	}
	delete fnd;
}

// script command type: branch
void PUvm::branch(char *at)
{
	char *i, *j;
	char *fnd = CNIL;
	int k, l, n, flg;

	if (at == CNIL) return;

	i = at;
	if (*i == ':') i++;
	while (*i && isspace(*i)) i++;

	j = fndtag(i,&fnd);
	k = (int)strlen(fnd);
	while (k > 0 && fnd[k] <= ' ') fnd[k--] = 0;
	while (k > 0 && fnd[k] == ')') {
		while (k > 0 && fnd[k] != '(') k--;
		fnd[k--] = 0;
		while (k > 0 && fnd[k] <= ' ') fnd[k--] = 0;
	}

/* need to do something about the count - n */
	flg = true;
	n = 0;
	if (*j == '@') {
		for (l = 0,j++;isdigit(*j);buf[l++] = *j++);
		buf[l] = 0;
		if (l > 0) n = atoi(buf);
		flg = false;
	} else if (*j == '/') {
		for (l = 0,j++;isdigit(*j);buf[l++] = *j++);
		buf[l] = 0;
		if (l > 0) n = atoi(buf);
	}
	while (*j && isspace(*j)) j++;
	if (!n) n = 1;
                  
	i = getframe(fnd);                  
	if (i == CNIL) {
		post("\r\n* ");
		if (flg) post("Subroutine"); else post("Branch");
		pppost(" could not find: ",fnd,"\r\n");
	} else {
		if (flg) stkpush(nxtcmd(at));
		if (jfrm != CNIL) delete jfrm;
		newstr(&jfrm,fnd);
		if (cmdlist != CNIL) delete cmdlist;
		newstr(&cmdlist,i);
		delete i;
		jflg = idframe(cmdlist,buf);
	}
	delete fnd;
}

// script command type: condition []
int PUvm::condite(char *at)
{
	char *i, *j, *k, *r, *p;
	char *tmp = CNIL;
	char a;
	int m, n, x;
	int q, flg, neg;
	char retval[16];
	bool chk;
	int glb;
	int s1;
	int s2;
	int expflg;
	int cnt;

	i = at;
	q = 0;
	expflg = true;
	strcpy(retval,"UNK");

	flg = true;
	while (*i == '[' && flg) {
		k = i;

		if (strncmp(i,"[*]",3)) glb = true; else {
			glb = false;
			for (i = i + 3;*i && isspace(*i);i++);
			if (*i != '[') continue;
		}

		flg = false;
		s1 = (int)strlen(lastinput);

		while (*i++ != ']') {
			while (*i && isspace(*i)) i++;

			if (*i == '!') {
				for (i++;*i && *i <= ' ';i++);
				neg = true;
			} else neg = false;

			chk = false;
			cnt = 0;
			for (j = buf;*i && *i != ',' && *i != ']';i++) {
				if (*i == '_') cnt++;
				if (*i == '\"') {
					for (i++;strncmp(i,"\"\"",2) && *i != '\"';*j++ = *i++) if (!strncmp(i,"\"\"",2)) *j++ = *i++;
					chk = true;
				} else *j++ = *i;
			}
			*j = 0;
			while (*i && isspace(*i)) i++;

			newstr(&tmp,buf);

			if (*tmp == '$' || *tmp == '%') {
				flg = chkexp(tmp,chk);
				if (flg == 3) {
					delete tmp;
					return FAILURE;
				}
			} else if (chk) {
				expflg = false;
				if (*tmp) {
					flg = false;
					for (n = m = q;lastinput[m];m++) {
						for (x = 0,n = m;tmp[x] && tmp[x] == lastinput[n];x++,n++);
						if (!tmp[x]) {
							while (lastinput[n] && isspace(lastinput[n])) n++;
							if (m < s1 || (m == s1 && n > s2)) {
								s1 = m;
								s2 = n;
							}
							flg = true;
							break;
						}
						if (glb) break;
					}
				} else flg = true;
			} else if (cnt == 2) {
				for (x = 0;tmp[x] && tmp[x] != '_';x++);
				for (x++;tmp[x] && tmp[x] != '_';x++);
				x++;
				a = tmp[x];
				tmp[x] = 0;
				flg = false;
				if (getslot(tmp,buf)) {
					tmp[x] = a;
					if (!strcmp(tmp + x,buf)) flg = true;
				}
			} else {
				if (getslot(tmp,buf)) {
					if (strcmp(buf,"TRUE")) flg = false; else flg = true;
				} else {
					flg = true;
					if (getslot("%~~last conditional restarted at~~",buf)) {
						if (k - cmdlist == atoi(buf) || pflg) flg = false;
						pflg = false;
					}
					if (flg) {
						for (x = 0;tmp[x] && tmp[x] != '(';x++);
						a = tmp[x];
						tmp[x] = 0;
						r = getframe(tmp);
						tmp[x] = a;
						if (j == CNIL) flg = false;
						else {
							sprintf(buf,"%d",k - cmdlist);
							savslot("%~~last conditional restarted at~~",buf);
							stkpush(k);
							p = idframe(r,buf);
							delete jfrm;
							newstr(&jfrm,buf);
							if (cmdlist != CNIL) delete cmdlist;
							newstr(&cmdlist,p);
							jflg = cmdlist;
							delete r;
							delete tmp;
							return SUCCESS;
						}
					}
				}
			}

			delete tmp;
			if (neg) flg = !flg;
  
			if (flg) {
				q = s2;
				while (*i != ']') if (*i++ == '\"') while (*i++ != '\"');
			} else {
				while (*i && isspace(*i)) i++;
			}
		}

/* could do something with the confidence factors here (straight integer) */
		while (*i && isspace(*i)) i++;
		while (*i && isdigit(*i)) i++;
		while (*i && isspace(*i)) i++;
	}

/* conditional commands to do if true */
	if (flg && (expflg || !lastinput[q])) {
		kcnt++;
		strcpy(retval,"TRUE");
		jflg = i;
	} else {
		if (jflg == CNIL) jflg = nxtcmd(at);
		strcpy(retval,"FALSE");
	}

/* path mark stuff */
	if (*cursit && !xflg) {
		sprintf(buf,"(%s%s",curfrm,cursit);
		savslot(buf,retval);
	}           
  
	return SUCCESS;
}

int PUvm::chkexp(char *s,bool chk)
{
	char *i, *j;
	char *fnd = CNIL;
	char *wrk = CNIL;
	int op = 0;
	int flg = 3;
	double r;

	j = s;
	if (*j == '$') j++;
	i = fndtag(j,&fnd);
	i = j + strlen(fnd);
	while (*i && isspace(*i)) i++;

	if (!getslot(fnd,buf)) {
		if (*i == ',' || *i == ']') flg = false;
		delete fnd;
		return flg;
	}
	delete fnd;
	if (*i == ',' || *i == ']') return true;

	if (*i == '!') { i++; op = 1; }
	if (*i == '<') { i++; op += 8; }
	if (*i == '>') { i++; op += 4; }
	if (*i == '=') { i++; op += 2; }

	while (*i && isspace(*i)) i++;

	if (op > 1) {
		if (chk) {
			for (j = buf;*i && *i == *j;i++,j++);
			r = *j - *i;
		} else {
			newstr(&wrk,buf);
			strcpy(buf,i);
			calc(buf);
			for (i = wrk;*i && (isdigit(*i) || *i == '.' || (*i == '-' && i == wrk));i++);
			for (j = buf;*j && (isdigit(*j) || *j == '.' || (*j == '-' && j == buf));j++);
			if (*i || *j) {
				for (i = wrk,j = buf;*i && *i == *j;i++,j++);
				r = *j - *i;
			} else r = atof(wrk) - atof(buf);
			delete wrk;
		}
		flg = false;
		if (op == 2 && r == 0.0)               flg = true;
		if ((op == 3 || op == 12) && r != 0.0) flg = true;
		if ((op == 5 || op == 10) && r <= 0.0) flg = true;
		if (op == 8 && r < 0.0)                flg = true;               
		if ((op == 9 || op == 6) && r >= 0.0)  flg = true; 
		if (op == 4 && r > 0.0)                flg = true;               
	}

	return (flg);
}

// hash tree of commands

void PUvm::cmda(char *s)
{
	int dumm;
	if (!strncmp(s,"attributes",10)) { dumm = utattrs(s + 10); return; }
	dumm = unknown(s);
}

void PUvm::cmdb(char *s)
{
	int dumm;
	if (!strncmp(s,"bot ",4))    { dumm = utbot(s + 3);      return; }
	if (!strncmp(s,"break",5))   { dumm = utbrk(s + 5);      return; }
	dumm = unknown(s);
}

void PUvm::cmdc(char *s)
{
	int dumm;
	if (!strncmp(s,"calc ",5))     { dumm = utcalc(s + 4);    return; }
	if (!strncmp(s,"cases",5))     { dumm = utcases(s + 5);   return; }
	if (!strncmp(s,"clear",5))     { utclear();               return; }
	if (!strncmp(s,"copy",4))      { utcopy();                return; }
	if (!strncmp(s,"consider ",9)) { dumm = utconsdr(s + 8);  return; }
	if (!strncmp(s,"cut",3))       { utcut();                 return; }
	if (!strncmp(s,"category ",9)) { utcat(s + 8);            return; }
//	if (!strncmp(s,"cycclient ",10)) { dumm = utcycclient(s + 9); return; }
	dumm = unknown(s);
}

void PUvm::cmdd(char *s)
{
	int dumm;
	if (!strncmp(s,"db ",3))       { utdb(s + 2);            return; }
	if (!strncmp(s,"do ",3)) return;
	if (!strncmp(s,"drop",4))      { utdrop(s + 4);          return; }
	if (!strncmp(s,"declare ",8))  { dumm = utdeclar(s + 7); return; }
	if (!strncmp(s,"define ",7))   { dumm = utdefine(s + 6); return; }
	if (!strncmp(s,"determine ",10)) { utdetermine(s + 9);   return; }
	if (!strncmp(s,"discard",7))   { utdiscrd();             return; }
	dumm = unknown(s);
}

void PUvm::cmde(char *s)
{
	int dumm;
	if (!strncmp(s,"eop",3))       { uteop();                  return; }
	if (!strncmp(s,"erase",5))     { uterase();                return; }
	if (!strncmp(s,"exit",4))      { dumm = utexit(s + 4);     return; }
	if (!strncmp(s,"export ",7))   { dumm = utexport(s + 6);   return; }
	if (!strncmp(s,"extract ",8))  { dumm = utextract(s + 7);  return; }
	if (!strncmp(s,"evaluate ",9)) { dumm = uteval(s + 8);     return; }
	dumm = unknown(s);
}

void PUvm::cmdf(char *s)
{
	int dumm;
	if (!strncmp(s,"facts",5))  { dumm = utfacts(s + 5);       return; }
	if (!strncmp(s,"find ",5))  { dumm = utfind(s + 4);        return; }
	if (!strncmp(s,"frame ",6)) { dumm = utframe(s + 5);       return; }
	dumm = unknown(s);
}

void PUvm::cmdg(char *s)
{
	int dumm;
	if (!strncmp(s,"get ",4))    { utget(s + 3);               return; }
	if (!strncmp(s,"getbot ",6)) { utgetbot(s + 6);            return; }
	if (!strncmp(s,"graphmaster ",12)) { utgm(s + 11);          return; }
	dumm = unknown(s);
}

void PUvm::cmdh(char *s)
{
	int dumm;
	if (!strncmp(s,"home",4))         { uthome();               return; }
	if (!strncmp(s,"how ",4))         { uthow(s + 3);           return; }
	dumm = unknown(s);
}

void PUvm::cmdi(char *s)
{
	int dumm;
	if (!strncmp(s,"ignore ",7))   { dumm = utignore(s + 6);   return; }
	if (!strncmp(s,"import ",7))   { dumm = utimport(s + 6);   return; }
	if (!strncmp(s,"insert ",7))   { dumm = utinsert(s + 6);   return; }
	dumm = unknown(s);
}

void PUvm::cmdk(char *s)
{
	int dumm;
	if (!strncmp(s,"kill ",5))  { dumm = utkill(s + 4);        return; }
	dumm = unknown(s);
}

void PUvm::cmdl(char *s)
{
	int dumm;
	if (!strncmp(s,"load ",5))  { dumm = utload(s + 4);        return; }
	dumm = unknown(s);
}

void PUvm::cmdm(char *s)
{
	int dumm;
	if (!strncmp(s,"make ",5))  { dumm = utmake(s + 4);        return; }
	if (!strncmp(s,"match ",6)) { dumm = utmatch(s + 5);       return; }
	dumm = unknown(s);
}

void PUvm::cmdn(char *s)
{
	int dumm;
	if (!strncmp(s,"nl",2))          { post("\r\n");           return; }
	if (!strncmp(s,"normalize ",10)) { dumm = utnorm(s + 9);   return; }
	dumm = unknown(s);
}

void PUvm::cmdo(char *s)
{
	int dumm;
	if (!strncmp(s,"open ",5))     { dumm = utopen(s + 4);     return; }
	dumm = unknown(s);
}

void PUvm::cmdp(char *s)
{
	int dumm;
	if (!strncmp(s,"pin",3))       { utpin(s + 3);             return; }
	if (!strncmp(s,"pose ",5))     { xret = utpose(s + 4);     return; }
	//if (!strncmp(s,"post ",5))     { dumm = utpost(s + 4);     return; }
	if (!strncmp(s,"pos ",4))      { utpos(s + 3);             return; }
	if (!strncmp(s,"paste",5))     { utpaste();                return; }
	if (!strncmp(s,"pop ",4))      { dumm = utpop(s + 3);      return; }
	//if (!strncmp(s,"push ",5))     { dumm = utpush(s + 4);     return; }
	if (!strncmp(s,"put ",4))      { dumm = utput(s + 3);      return; }
	dumm = unknown(s);
}

void PUvm::cmdq(char *s)
{
	int dumm;
	if (!strncmp(s,"quit",4))   { dumm = utquit(s + 4);        return; }
	if (!strncmp(s,"queue ",6)) { dumm = utqueue(s + 5);       return; }
	dumm = unknown(s);
}

void PUvm::cmdr(char *s)
{
	int dumm;
	if (!strncmp(s,"read ",5))   { dumm = utread(s + 4);       return; }
	if (!strncmp(s,"recall ",7)) { dumm = utrecall(s + 6);     return; }
	if (!strncmp(s,"remove ",7)) { dumm = utremove(s + 6);     return; }
	if (!strncmp(s,"rescind",7)) { dumm = utrescnd(s + 7);     return; }
	if (!strncmp(s,"restart",7)) { dumm = utrstart(s + 7);     return; }
	if (!strncmp(s,"rewrite",7)) { dumm = utrwrite(s + 7);     return; }
	if (!strncmp(s,"rules",5))   { dumm = utrules(s + 5);      return; }
	if (!strncmp(s,"rule ",5))   { dumm = utrule(s + 4);       return; }
	dumm = unknown(s);
}

void PUvm::cmds(char *s)
{
	int dumm;
	if (!strncmp(s,"saveas",6))    { dumm = utsaveas(s + 6);   return; }
	if (!strncmp(s,"save ",5))     { dumm = utsave(s + 4);     return; }
	if (!strncmp(s,"say ",4))      { dumm = utsay(s + 3);      return; }
	if (!strncmp(s,"scripts",7))   { dumm = utscript(s + 7);   return; }
	if (!strncmp(s,"search ",7))   { dumm = utsearch(s + 6);   return; }
	if (!strncmp(s,"select",6))    { utsel(s + 6);             return; }
	if (!strncmp(s,"sentences ",10)) { utsentence(s + 9);      return; }
	if (!strncmp(s,"senses ",7))   { utsense(s + 6);           return; }
	if (!strncmp(s,"setbot ",7))   { utsetbot(s + 6);          return; }
	if (!strncmp(s,"share ",6))    { dumm = utshare(s + 5);    return; }
	if (!strncmp(s,"show ",5))     { dumm = utshow(s + 4);     return; }
	if (!strncmp(s,"subjects",8))  { dumm = utsubjects(s + 8); return; }
	if (!strncmp(s,"submit ",7))   { dumm = utsubmit(s + 6);   return; }
	if (!strncmp(s,"synonyms ",9)) { dumm = utsynonym(s + 8);  return; }
	dumm = unknown(s);
}

void PUvm::cmdu(char *s)
{
	int dumm;
	if (!strncmp(s,"use ",4))      { dumm = utuse(s + 3);      return; }
	//if (!strncmp(s,"urlclient ",10)) { dumm = uturlclient(s + 9); return; }
	dumm = unknown(s);
}

void PUvm::cmdw(char *s)
{
	int dumm;
	if (!strncmp(s,"what ",5))     { dumm = utwhat(s + 4);     return; }
	if (!strncmp(s,"which ",6))    { dumm = utwhich(s + 5);    return; }
	if (!strncmp(s,"words ",6))    { dumm = utword(s + 5);     return; }
	if (!strncmp(s,"write ",6))    { dumm = utwrite(s + 5);    return; }
	dumm = unknown(s);
}

int PUvm::unknown(char *at)
{
	char *i;
	char *j = CNIL;
	int k;

	for (i = at;*i && *i != '_';i++);	// underlined facts:  subject_verb_object
	if (*i) {
		for (i++;*i && *i != '_';i++);
		if (*i) {
			*i = 0;
			newstr(&j,at);
			strcat(j,"_");
			*i++ = '_';
			for (k = 0;*i && *i != ',' && *i != '{' && *i != '?';buf[k++] = *i++);
			buf[k] = 0;
			while (k > 0 && buf[k - 1] == ' ') { k--; buf[k] = 0; }
			for (k = (int)strlen(j) - 1;k > 0 && *(j + k) == ' ';*(j + k--) = 0);
			savslot(j,buf);				// save as: subject_verb_ = object
			delete j;
			return SUCCESS;
		}
	}
	strcpy(buf,at);
	if (i = strstr(buf,",")) *i = 0;
	if (*buf) pppost("\r\neh? - ",buf,"\r\n");
	return SUCCESS;
}

//
// predefined command statements
//

int PUvm::utattrs(char *at)
{
	bool blnFile = false;
	bool blnAll = false;
	bool blnXML = false;
	char *i;
	CString s;

	if (dflg) {
		i = syntag(at,buf);
		for (i = buf;*i;i++) {
			if (*i == 'f' || *i == 'F') blnFile = true;
			if (*i == 'v' || *i == 'V') blnAll = true;
			if (*i == 'x' || *i == 'X') blnXML = true;
		}
		if (blnFile) atr.FileAttributes(s);
		else if (blnAll) atr.FormatAttributes(s);
		else if (blnXML) atr.XMLAttributes(s);
		else atr.ListAttributes(s);
		sprintf(buf,"%s",s);
		post(buf);
	}
	return SUCCESS;
}

int PUvm::utbot(char *at)
{
	char *i;
	int iRet;

	i = syntag(at,buf);
	if (*buf) {
		newstr(&i,buf);
		*buf = 0;
		iRet = bot->getAnswer(i,buf);
		delete i;
		delete lastreply;
		newstr(&lastreply,buf);
	}
	return SUCCESS;
}

int PUvm::utbrk(char *at)
{
	char *i;

	for (i = at;*i && *i != '{';i++);
	jflg = i;
	return SUCCESS;
}

int PUvm::utcalc(char *at)
{
	char *i;
	int n;

	n = 0;
	for (i = at;*i && *i != ',' && *i != ';' && *i != '{' && *i != '?';i++) {
		buf[n++] = *i;
		if (*i == '\"') for (i++;*i && *i != '\"';buf[n++] = *i++);
		if (*i == '\"') buf[n++] = *i;
	}
	buf[n] = 0;
	if (*buf == '\"' && buf[n - 1] == '\"') {
		buf[n - 1] = 0;
		for (i = buf + 1,n = 0;*i;i++) if (strncmp(i,"\"\"",2)) buf[n++] = *i;
		buf[n] = 0;
	} else calc(buf);
	ppost("\r\n* Answer: ",buf);
	return SUCCESS;
}

int PUvm::utcases(char *at)
{
	bool blnFile = false;
	bool blnAll = false;
	bool blnXML = false;
	char *i;
	CString s;
	Subject *pSubj = kb.curSub;

	if (dflg) {
		i = syntag(at,buf);
		for (i = buf;*i;i++) {
			if (*i == 'f' || *i == 'F') blnFile = true;
			if (*i == 'v' || *i == 'V') blnAll = true;
			if (*i == 'x' || *i == 'X') blnXML = true;
			if (*i == 'l' || *i == 'L') pSubj = kb.curLoc;
			if (*i == 's' || *i == 'S') pSubj = kb.curTag;
		}
		if (blnFile) pSubj->m_cases.FileCases(s);
		else if (blnAll) pSubj->m_cases.FormatCases(s);
		else if (blnXML) pSubj->m_cases.XMLCases(s);
		else pSubj->m_cases.ListCases(s);
		sprintf(buf,"%s",s);
		post(buf);
	}
	return SUCCESS;
}

int PUvm::utcat(char *at)
{
	char *i;
	char *patrn;
	char *line;
	char chrlast, chrpat, chr;
	int iRet;
    fpos_t fpos;

	i = syntag(at,buf);
	if (!*buf) return FAILURE;
	strcat(buf,"@0000000000");
	while (replace(buf,"[","<"));
	while (replace(buf,"]",">"));
	while (replace(buf,"&lt;","<"));
	while (replace(buf,"&gt;",">"));
	newstr(&patrn,buf);
	i = syntag(i,buf);
	if (!*buf) { delete patrn; return FAILURE; }
	while (replace(buf,"&lt;","<"));
	while (replace(buf,"&gt;",">"));
	while (replace(buf,"&amp;","&"));
	newstr(&line,buf);

	SetCursor(AfxGetApp()->LoadCursor(IDC_WAIT));

	FILE *fpatrn = fopen(bot->fnpat,"r");
	FILE *fsortd = fopen(bot->fnsrt,"w");
	FILE *ftmplt = fopen(bot->fntmp,"a");
	FILE *findex = fopen(bot->fnidx,"w");

	fprintf(ftmplt,"==?==\n");
	fgetpos(ftmplt,&fpos);
	i = strstr(patrn,"@");
	sprintf(i,"@%li\n",fpos);

	chrpat = *patrn;
	if (islower(chrpat)) chrpat += 'A' - 'a';

	compress(line);
	fprintf(ftmplt,"%s\n\n",line);
	fclose(ftmplt);

	chrlast = '_';

	while (!feof(fpatrn)) {
		fgetpos(fsortd,&fpos);
		fgets(buf,MAX_LINE_SIZE,fpatrn);
		if (!strncmp(buf,"<eof>",5)) break;
		chr = *buf;
		if (islower(chr)) chr += 'A' - 'a';
		if (chrlast != chr && chrlast != '*') {
			if (chrlast == '_') chrlast = 'A'; else chrlast++;
			if (chr < 'A' || chr > 'Z') chr = 'Z' + 1;
			while (chrlast <= chr) {
				fprintf(findex,"%li\n",fpos);
				if (chrlast == chrpat || chrpat == '_') {
					fprintf(fsortd,patrn);
					chrpat = 1;
					fgetpos(fsortd,&fpos);
				}
				chrlast++; 
			}
			if (chr > 'Z') chr = chrlast = '*'; else chrlast--;
		}
		if (chr == chrpat) {
			if (strpcmp(buf,patrn) > 0) {
				fprintf(fsortd,patrn);
				fgetpos(fsortd,&fpos);
				chrpat = 1;
			} 
		}
		fprintf(fsortd,buf);
	}

	if (chrlast != '*') {
		if (chrlast == chrpat) {
			fprintf(fsortd,patrn);
			fgetpos(fsortd,&fpos);
			chrpat = 1;
		}
		if (chrlast == '_') chrlast = 'A'; else chrlast++;
		while (chrlast <= 'Z' + 1) {
			fprintf(findex,"%li\n",fpos);
			if (chrlast == chrpat || chrpat == '_') {
				fprintf(fsortd,patrn);
				fgetpos(fsortd,&fpos);
				chrpat = 1;
			}
			chrlast++;
		}
	}
	if (chrpat > 1) fprintf(fsortd,patrn);
	fprintf(fsortd,"<eof>\n\n");

	fclose(fpatrn);
	fclose(fsortd);
	fclose(findex);

	TCHAR szFileName[MAX_PATH_LEN];
	wsprintf(szFileName,TEXT("%hs"),bot->fnpat);
	iRet = DeleteFile(szFileName);

	TCHAR szOldFileName[MAX_PATH_LEN];
	wsprintf(szOldFileName,TEXT("%hs"),bot->fnsrt);
	iRet = MoveFile(szOldFileName,szFileName);

	delete patrn;
	delete line;

	bot->loadchunks();
	bot->reloadgm();

	SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));

	return SUCCESS;
}

int PUvm::utconsdr(char *at)
{
	char *i = syntag(at,buf);                         
	if (!*buf) return FAILURE;
	if (kb.Find(buf)) return SUCCESS;
	kb.Add(buf);
	Subject *pSubj = kb.curSub;
	pSubj->m_cases.AddGbl();
	pSubj->m_cases.curCase = pSubj->m_cases.curGbl;
	return (SUCCESS);
}


int PUvm::utdb(char *at)
{
	fpos_t fpos;
	char *i, *pos;
	char loc[10];
	long n, cnt = 0;
	bool saveonly = false;

	i = synlong(at,&n);
	if (!n) return FAILURE; 
	if (n < 0) { saveonly = true; n = -n; }
	sprintf(loc,"%ld",n);

	char *top = new char[MAX_LINE_SIZE];
	char *line = new char[MAX_LINE_SIZE];
	char *tmp = new char[MAX_BUF_LEN];
	char *fnpat = new char[MAX_PATH_LEN];
	char *fntmp = new char[MAX_PATH_LEN];

	strcpy(fnpat,bot->fnpat);
	strcpy(fntmp,bot->fntmp);
	i = syntag(i,buf);
	if (*buf) {
		strcpy(fnpat,buf);
		i = syntag(i,buf);
		if (*buf) strcpy(fntmp,buf);
	}	

	FILE *patn = fopen(fnpat,"r");
	FILE *tmpl = fopen(fntmp,"r");

	*top = 0;

	while (!feof(patn)) {
		fgets(line,MAX_LINE_SIZE,patn);
		if (!(i = strstr(line,"@"))) continue;
		cnt++;
		if (n == cnt) {
			*i = 0;
			strcpy(buf,i + 1);
			rtrim(buf);
			fpos = atol(buf);

			fsetpos(tmpl,&fpos);
			fgets(buf,MAX_BUF_LEN,tmpl);
			rtrim(buf);
			expand(buf);

			savslot("BotDB Index",loc);
			savslot("BotDB Pattern",line);
			savslot("BotDB Template",buf);
			if (saveonly || !dflg) break;

			if (i = strstr(line,"[topic]")) {
				strcpy(top,i + 7);
				*i = 0;
				replace(top,"</topic>","");
			} else *top = 0;
			if (i = strstr(line,"[that]")) {
				strcpy(tmp,i);
				replace(tmp,"[that]","<that>");
				strcpy(i,"</pattern>\n");
				strcat(line,tmp);
				if (!strstr(line,"</that>")) strcat(line,"</that>");
			} else strcat(line,"</pattern>");
			if (*top) pppost("<topic name=\"",top,"\">\r\n");
			pppost("<category>\r\n<pattern>",line,"\r\n");
			for (i = buf,pos = tmp;*i;i++) {
				if (!strncmp(i,"<random>",8)      || !strncmp(i,"<condition>",11) ||
					!strncmp(i,"</condition>",12) || !strncmp(i,"</random>",9)    ||
					!strncmp(i,"</li>",5)         || !strncmp(i,"</save>",7)      ||
					!strncmp(i,"</think>",8)      || !strncmp(i,"</system>",9)    ||
					!strncmp(i,"</gossip>",9)     || !strncmp(i,"<br/>",5)        ||
					!strncmp(i,"<BR/>",5)         || !strncmp(i,"<usesaved/>",11)) {
					while (*i != '>') *pos++ = *i++;
					*pos++ = *i;
					*pos++ = '\r';
					*pos++ = '\n';
				} else *pos++ = *i;
			}
			*pos = 0;
			if (strlen(buf) == strlen(tmp)) {
				pppost("<template>",tmp,"</template>\r\n</category>\r\n\r\n");
			} else {
				if (*(pos - 1) != '\n') strcat(tmp,"\n");
				pppost("<template>\r\n",tmp,"</template>\r\n</category>\r\n\r\n");
			}
			break;
		}
	}
	if (*top) post("</topic>\r\n");

	fclose(patn);
	fclose(tmpl);
	delete tmp;
	delete buf;
	delete line;
	delete top;
	delete fnpat;
	delete fntmp;
	return SUCCESS;
}

int PUvm::utdeclar(char *at)
{
	char *i;
	char *tag = CNIL;
      
	i = synvar(at,buf,&tag);      
	if (i != CNIL) {
		gblslot(tag,buf);
		delete tag;
		return SUCCESS;
	}
	delete tag;
	return FAILURE;
}

int PUvm::utdefine(char *at)
{
	char *i, *j;
	int pos = ALL_POS;
	int search = OVERVIEW;
	int whichsense = ALLSENSES;
	char *outbuf;
	char *morph;
	char *postbuf = new char[MAX_BUF_LEN];
	int n;
	ss *ans;
	ss *ptr;

	i = syntag(at,buf);
	if (*buf == '-') {
		if (j = strstr(buf," ")) *j = 0;
		if (!strcmp(buf,"-synsa")) { search = SIMPTR;         pos = ADJ; }
		if (!strcmp(buf,"-antsa")) { search = ANTPTR;         pos = ADJ; }
		if (!strcmp(buf,"-perta")) { search = PERTPTR;        pos = ADJ; }
		if (!strcmp(buf,"-attra")) { search = ATTRIBUTE;      pos = ADJ; }
		if (!strcmp(buf,"-domna")) { search = CLASSIFICATION; pos = ADJ; }
		if (!strcmp(buf,"-domta")) { search = CLASS;          pos = ADJ; }
		if (!strcmp(buf,"-famla")) { search = FREQ;           pos = ADJ; }
		if (!strcmp(buf,"-grepa")) { search = WNGREP;         pos = ADJ; }
		if (!strcmp(buf,"-synsn")) { search = HYPERPTR;       pos = NOUN; }
		if (!strcmp(buf,"-antsn")) { search = ANTPTR;         pos = NOUN; }
		if (!strcmp(buf,"-coorn")) { search = COORDS;         pos = NOUN; }
		if (!strcmp(buf,"-hypen")) { search = -HYPERPTR;      pos = NOUN; }
		if (!strcmp(buf,"-hypon")) { search = HYPOPTR;        pos = NOUN; }
		if (!strcmp(buf,"-treen")) { search = -HYPOPTR;       pos = NOUN; }
		if (!strcmp(buf,"-holon")) { search = HOLONYM;        pos = NOUN; }
		if (!strcmp(buf,"-sprtn")) { search = ISPARTPTR;      pos = NOUN; }
		if (!strcmp(buf,"-smemn")) { search = ISMEMBERPTR;    pos = NOUN; }
		if (!strcmp(buf,"-ssubn")) { search = ISSTUFFPTR;     pos = NOUN; }
		if (!strcmp(buf,"-hholn")) { search = -HHOLONYM;      pos = NOUN; }
		if (!strcmp(buf,"-meron")) { search = MERONYM;        pos = NOUN; }
		if (!strcmp(buf,"-subsn")) { search = HASSTUFFPTR;    pos = NOUN; }
		if (!strcmp(buf,"-partn")) { search = HASPARTPTR;     pos = NOUN; }
		if (!strcmp(buf,"-membn")) { search = HASMEMBERPTR;   pos = NOUN; }
		if (!strcmp(buf,"-hmern")) { search = -HMERONYM;      pos = NOUN; }
		if (!strcmp(buf,"-nomnn")) { search = DERIVATION;     pos = NOUN; }
		if (!strcmp(buf,"-derin")) { search = DERIVATION;     pos = NOUN; }
		if (!strcmp(buf,"-domnn")) { search = CLASSIFICATION; pos = NOUN; }
		if (!strcmp(buf,"-domtn")) { search = CLASS;          pos = NOUN; }
		if (!strcmp(buf,"-attrn")) { search = ATTRIBUTE;      pos = NOUN; }
		if (!strcmp(buf,"-famln")) { search = FREQ;           pos = NOUN; }
		if (!strcmp(buf,"-grepn")) { search = WNGREP;         pos = NOUN; }
		if (!strcmp(buf,"-synsv")) { search = HYPERPTR;       pos = VERB; }
		if (!strcmp(buf,"-simsv")) { search = RELATIVES;      pos = VERB; }
		if (!strcmp(buf,"-antsv")) { search = ANTPTR;         pos = VERB; }
		if (!strcmp(buf,"-coorv")) { search = COORDS;         pos = VERB; }
		if (!strcmp(buf,"-hypev")) { search = -HYPERPTR;      pos = VERB; }
		if (!strcmp(buf,"-hypov")) { search = HYPOPTR;        pos = VERB; }
		if (!strcmp(buf,"-treev")) { search = -HYPOPTR;       pos = VERB; }
		if (!strcmp(buf,"-tropv")) { search = -HYPOPTR;       pos = VERB; }
		if (!strcmp(buf,"-entav")) { search = ENTAILPTR;      pos = VERB; }
		if (!strcmp(buf,"-causv")) { search = CAUSETO;        pos = VERB; }
		if (!strcmp(buf,"-nomnv")) { search = DERIVATION;     pos = VERB; }
		if (!strcmp(buf,"-deriv")) { search = DERIVATION;     pos = VERB; }
		if (!strcmp(buf,"-domnv")) { search = CLASSIFICATION; pos = VERB; }
		if (!strcmp(buf,"-domtv")) { search = CLASS;          pos = VERB; }
		if (!strcmp(buf,"-framv")) { search = FRAMES;         pos = VERB; }
		if (!strcmp(buf,"-famlv")) { search = FREQ;           pos = VERB; }
		if (!strcmp(buf,"-grepv")) { search = WNGREP;         pos = VERB; }
		if (!strcmp(buf,"-synsr")) { search = SYNS;           pos = ADV; }
		if (!strcmp(buf,"-antsr")) { search = ANTPTR;         pos = ADV; }
		if (!strcmp(buf,"-pertr")) { search = PERTPTR;        pos = ADV; }
		if (!strcmp(buf,"-domnr")) { search = CLASSIFICATION; pos = ADV; }
		if (!strcmp(buf,"-domtr")) { search = CLASS;          pos = ADV; }
		if (!strcmp(buf,"-famlr")) { search = FREQ;           pos = ADV; }
		if (!strcmp(buf,"-grepr")) { search = WNGREP;         pos = ADV; }
		if (!strcmp(buf,"-over"))  { search = OVERVIEW;       pos = ALL_POS; }
		if (!j) i = syntag(i,buf);
		else { for(i = j + 1;*i == ' ';i++); strcpy(buf,i); }
	}

	if (*buf) {
		if (wninit()) { delete postbuf; return FAILURE; }	// initialize wordnet database

		*postbuf = 0;
		lowercase(buf);
		while (replace(buf," ","-"));
		if (strchr(buf,'%')) {
			ans = GetSynsetForSense(buf);
			for (ptr = ans;ptr != NULL;ptr = ptr->nextss) {
				setSearchBuffer("");
				printsynset("",ans,"",DEFON,ALLWORDS,SKIP_ANTS,SKIP_MARKER);
				if (getSearchBuffer()[0]) {
					strcat(postbuf,GetWORD(buf));
					strcat(postbuf," as ");
					strcat(postbuf,getpartsname(GetPOS(buf)));
					strcat(postbuf," --> ");
					for (i = j = getSearchBuffer();*i;i++) {
						if (*i == '\n') {
							*i = 0;
							strcat(postbuf,j);
							strcat(postbuf,"\r\n");
							j = i + 1;
						}
					}
					strcat(postbuf,j);
					strcat(postbuf,"\r\n");
				}
			}
			free_syns(ans);
		} else {
			for (n = 1; n <= NUMPARTS; n++) {		// search word for senses
				if (pos == n || pos == ALL_POS) {
					outbuf = findtheinfo(buf,n,search,whichsense);
					if (*outbuf) {
						for (i = j = outbuf + 1;*i;i++) {
							if (*i == '\n') {
								*i = 0;
								strcat(postbuf,j);
								strcat(postbuf,"\r\n");
								j = i + 1;
							}
						}
						strcat(postbuf,j);
						strcat(postbuf,"\r\n");
					}
					if ((morph = morphstr(buf,n)) != NULL) {
						do {
							outbuf = findtheinfo(morph,n,search,whichsense);
							if (*outbuf) {
								for (i = j = outbuf + 1;*i;i++) {
									if (*i == '\n') {
										*i = 0;
										strcat(postbuf,j);
										strcat(postbuf,"\r\n");
										j = i + 1;
									}
								}
								strcat(postbuf,j);
								strcat(postbuf,"\r\n");
							}
						} while ((morph = morphstr(NULL,n)) != NULL);
					}
					if (pos != ALL_POS) break;
				}
			}
		}
		if (dflg) post(postbuf);
		else savslot("Definition",postbuf);
	}
	delete postbuf;
	return SUCCESS;
}

int PUvm::utdetermine(char *at)
{
	int iRet = SUCCESS;
	char *i;
	char *tag = CNIL;
	CString str;
	CString attr;
      
	i = syntag(at,buf);
	newstr(&tag,buf);
	if (getslot(tag,buf)) {
		if (dflg) {
			str.Format(_T("%hs is %hs"),tag,buf);
			MessageBox(NULL,str,_T("Expert's Determination"),MB_OK);
			delete tag;
			return SUCCESS;
		}
	}
	attr.Format(_T("%hs"),tag);
	if (!xpt.SeekTarget(attr)) {
		if (dflg) {
			str.Format(_T("Expert's goal is %hs\r\nGoal not found."),tag);
			MessageBox(NULL,str,_T("Expert's Determination"),MB_OK);
			delete tag;
			return FAILURE;
		}
	}
	xpt.bflg = false;
	if (xpt.AssertTarget(attr)) {
		if (dflg) {
			if (getslot(tag,buf)) {
				str.Format(_T("%hs is %hs"),tag,buf);
				MessageBox(NULL,str,_T("Expert's Determination"),MB_OK);
			} else MessageBox(NULL,_T("Success"),_T("Expert's Determination"),MB_OK);
		}
	} else {
		if (dflg) {
			str.Format(_T("Expert's goal %hs can't be determined."),tag);
			MessageBox(NULL,str,_T("Expert's Determination"),MB_OK);
			iRet = FAILURE;
		}
	}	
	delete tag;
	return iRet;
}

int PUvm::utdiscrd()
{
	int m = SUCCESS;

	if (kb.curSub == NULL) return FAILURE;

	sprintf(buf,"%s",kb.curSub->m_name);
	if (strcmp(buf,"New Cases")) {
		if (kb.Remove(buf)) {
			if (kb.Find("New Cases")) return m;
			if (kb.curSub == NULL) m = FAILURE;
		}
	} else {
		kb.curSub->m_cases.RemoveAll();
		kb.curSub->m_cases.AddGbl();
		kb.curSub->m_cases.AddShare();
	}
	
	return m;
}

int PUvm::utdrop(char *at)
{
	int m = SUCCESS;

	sprintf(buf,"%s",kb.curSub->m_cases.curCase->m_name);

	if (strcmp(buf,"~~global~~")) {
		if (kb.curSub->m_cases.Remove(buf)) {
			kb.curSub->m_cases.curCase = kb.curSub->m_cases.curGbl;
			if (kb.curSub->m_cases.curCase == NULL) m = FAILURE;
		}
		if (kb.curTag->m_cases.Remove(buf)) {
			kb.curTag->m_cases.curCase = kb.curTag->m_cases.curGbl;
		}
	} else kb.curSub->m_cases.Add(buf);
	
	return m;
}

int PUvm::uteval(char *at)
{
	char *i;
	char *tag = CNIL;
                         
	i = synvar(at,buf,&tag);                         
	if (i != CNIL && *buf) {
		calc(buf);
		savslot(tag,buf);
		delete tag;
		return SUCCESS;
	}
	delete tag;
	return FAILURE;
}

int PUvm::utexit(char *at)
{
	char *i, *j;

	while (cmdlist != CNIL) {
		for (i = at;(j = nxtcmd(i)) != CNIL;i = j);
		while (*i == '?') {
			for (i++;*i && isspace(*i);i++);
			if (*i == ',') for (i++;*i && isspace(*i);i++);
			while ((j = nxtcmd(i)) != CNIL) i = j;
		}
		if (stkret()) {
			if (jflg == CNIL) i = cmdlist; else i = jflg;
		} else break;
	}
	xflg = true;
	jflg = i;
	return SUCCESS;
}

int PUvm::utexport(char *at)
{
	CString str;
	char *i;
	char *fn = CNIL;
	char *text;
	int m = FAILURE;
	bool blnXML = false;

	i = syntag(at,buf);
	if (*buf) {
		newstr(&fn,buf);
		sprintf(buf,"%s%s",fnrul,fn);
		if (strstr(fn,".xml")) blnXML = true; else if (!strstr(fn,".txt")) strcat(buf,".txt");
		delete fn;
		FILE *ofile = fopen(buf,"w");

		if (blnXML) {
			fprintf(ofile,"<rules>\n");
			atr.XMLAttributes(str); 
		} else {
			atr.FileAttributes(str);
		}
		
		text = new char[str.GetLength() + 10];
		sprintf(text,"%s",str);
		fprintf(ofile,"%s",text);
		delete text;

		if (blnXML) xpt.XMLRules(str); else xpt.FileRules(str);
		
		text = new char[str.GetLength() + 10];
		sprintf(text,"%s",str);
		fprintf(ofile,"%s",text);
		delete text;

		if (blnXML) fprintf(ofile,"<\rules>\n");
		
		fclose(ofile);
		m = SUCCESS;
	}
	return (m);
}

int PUvm::utextract(char *at)
{
	char *i;
	char *tag = CNIL;
	char *fnd = CNIL;
	int n;

	i = synnum(at,&n);
	if ((i = synvar(i,buf,&tag)) != CNIL) {
		i = fndtag(i,&fnd);
		if (kb.curSub->m_cases.curCase->m_facts.Find(tag)) {
			if (kb.curSub->m_cases.curCase->m_facts.curFact->Extract(n - 1,buf)) if (*fnd) savslot(fnd,buf);
		}
		delete fnd;
	}
	delete tag;
	return SUCCESS;
}

int PUvm::utfacts(char *at)
{
	bool blnXML = false;
	char *i;
	CString s;
	Case *pCase = kb.curSub->m_cases.curCase;

	if (dflg) {
		i = syntag(at,buf);
		for (i = buf;*i;i++) {
			if (*i == 'l' || *i == 'L') pCase = kb.curLoc->m_cases.curCase;
			if (*i == 's' || *i == 'S') pCase = kb.curTag->m_cases.curCase;
			if (*i == 'x' || *i == 'X') blnXML = true;
		}
		if (blnXML) pCase->m_facts.XMLFacts(s); else pCase->m_facts.FormatFacts(s);
		sprintf(buf,"%s",s);
		post(buf);
	}
	return SUCCESS;
}

int PUvm::utframe(char *at)
{
	char *i;
	CString key;
	Frame *pFrame;

	if (dflg) {
		i = syntag(at,buf);
		key.Format(_T("%hs"),buf);
		if (pgm.curScript->m_frames.m_mapStringToFrame.Lookup(key,pFrame)) {
			sprintf(buf,"\r\n{%s}\r\n%s\r\n",pFrame->m_name,pFrame->m_behavior);
			post(buf);
		} else pppost("\r\nframe ",buf," not found!\r\n");
	}
	return SUCCESS;
}

int PUvm::utget(char *at)
{
	Subject *pSubj = kb.curSub;
	Case *pCase;
	char *fn = CNIL;
	char *pos;
	char *tag = CNIL;
	char *i, *j, *k;
	int m = FAILURE;
	double cf = 1.0;

	if (pSubj == NULL) return m;

	i = syntag(at,buf);
	if (*buf) {
		newstr(&fn,buf);
		sprintf(buf,"%s%s",fnkb,fn);
		FILE *ifile = fopen(buf,"r");
		if (ifile == NULL) {
			sprintf(buf,"%s",fn);
			ifile = fopen(buf,"r");
			if (ifile == NULL) {
				delete fn;
				return m;
			}
		}

		while (fgets(buf,4096,ifile)) {
			if (*buf == '{') {
				if (j = strchr(buf,'}')) *j = 0;
				rtrim(buf);
				j = buf + 1;
				if (!strnicmp(j,(char*)"case:",5)) j += 5;
				while (*j && *j <= ' ') j++;
				if (*j) {
					pSubj->m_cases.Add(j);
					kb.curTag->m_cases.Add(j);
					pCase = pSubj->m_cases.curCase;
					if (!strcmp(j,"~~global~~")) pSubj->m_cases.curGbl = pCase;
					m = SUCCESS;
				}
			} else {
				for (pos = buf;*pos && *pos != '=' && *pos != ',' && *pos != '_';pos++);
				if (*pos) {
					if (m == SUCCESS) {
						k = pos;
						if (*pos == '_') for (pos++;*pos && *pos != '_';pos++);
						else *pos++ = 0;
						rtrim(pos);
						if (*k == '_') {
							if (*pos) pos++;
							for (j = pos;*j;j++);
							for (k = j++;k >= pos;k--) *(k + 1) = *k;
							*pos++ = 0;
							while (j > pos && *j <= ' ') *j-- = 0;
							if (*j == ']') {
								cf = 1.0;
								for (j--,*j-- = 0;j > pos && *j != '[';j--);
								if (*j == '[') { *j++ = 0; cf = atof(j); }
							}
						} else {
							for (j = k;j > buf && *j <= ' ';j--) *j = 0;
							if (*j == ']') {
								cf = 1.0;
								for (j--,*j-- = 0;j > buf && *j != '[';j--);
								if (*j == '[') { *j++ = 0; cf = atof(j); }
							}
						}
						if (cf != 1.0) {
							if (*buf) {
								if (tag != CNIL) delete tag;
								newstr(&tag,buf);
								pCase->m_facts.Add(tag,pos,"File",cf);
							} else {
								pCase->m_facts.Find(tag);
								pCase->m_facts.curFact->Push(pos);
							}
						} else {
							if (*buf) {
								if (tag != CNIL) delete tag;
								newstr(&tag,buf);
								pCase->m_facts.Add(tag,pos);
							} else {
								pCase->m_facts.Find(tag);
								pCase->m_facts.curFact->Push(pos);
							}
						}
					}
				}
			}
		}
		fclose(ifile);
	}
	if (tag != CNIL) delete tag;
	return (m);
}

int PUvm::utgetbot(char *at)
{
	char *i;
	char *tag = CNIL;
	int n;

	i = fndtag(at,&tag);
	if (*tag && *tag != '%') {
		remslot(tag);
		savslot(tag,bot->thats[0]);
		for (n = 1;n < 5;n++) kb.curSub->m_cases.curCase->m_facts.curFact->Push(bot->thats[n]);
		delete tag;
		i = fndtag(i,&tag);
		if (*tag && *tag != '%') {
			remslot(tag);
			savslot(tag,bot->inputs[0]);
			for (n = 1;n < 5;n++) kb.curSub->m_cases.curCase->m_facts.curFact->Push(bot->inputs[n]);
			delete tag;
			i = fndtag(i,&tag);
			if (*tag && *tag != '%') savslot(tag,bot->topic);
		}
	}
	delete tag;
	return SUCCESS;
}

int PUvm::utgm(char *at)
{
	char *i;

	i = syntag(at,buf);
	if (*buf) {
		if (!strcmp(buf,"on")) {
			bot->usegraphmaster = true;
		} else if (!strcmp(buf,"off")) {
			bot->usegraphmaster = false;
		} else if (!strcmp(buf,"load")) {
			bot->usegraphmaster = true;
			bot->loadgm();
		} else if (!strcmp(buf,"unload")) {
			bot->usegraphmaster = false;
			bot->unloadgm();
		} else if (!strcmp(buf,"reload")) {
			bot->reloadgm();
		}
	}

	return SUCCESS;
}

int PUvm::uthow(char *at)
{
	char *i;
	CString s;
	CString r;

	if (dflg) {
		i = syntag(at,buf);
		r.Format(_T("%hs"),buf);
		xpt.How(r,s);
		sprintf(buf,"%s",s);
		post(buf);
	}
	return SUCCESS;
}

int PUvm::utignore(char *at)
{
	char *i, *pos;
	char loc[20];
	fpos_t fpos;
	long cnt = 0;
	bool fnd = false;
	char *pat;
	int n;
	int m = FAILURE;

	i = syntag(at,buf);
	if (*buf) {
		strcat(buf,"@");
		newstr(&pat,buf);
		n = (int)strlen(pat);

		FILE *patn = fopen(bot->fnpat,"r+");
		while (!feof(patn)) {
			fgetpos(patn,&fpos);
			fgets(buf,MAX_SIZE,patn);
			if (!(pos = strstr(buf,"@"))) continue;
			cnt++;
			if (!strnicmp(buf,pat,n)) { fnd = true; break; }
		}

		if (fnd) {
			*pos = '!';
			fsetpos(patn,&fpos);
			fputs(buf,patn);
			fclose(patn);

			rtrim(pos);
			*pos++ = 0;
			fpos = atol(pos);
			sprintf(loc,"%ld",cnt);
			savslot("BotDB Index",loc);
			savslot("BotDB Pattern",buf);

			FILE *tmpl = fopen(bot->fntmp,"r");
			fsetpos(tmpl,&fpos);
			fgets(buf,MAX_BUF_LEN,tmpl);
			fclose(tmpl);

			rtrim(buf);
			expand(buf);
			savslot("BotDB Template",buf);

			bot->reloadgm();

			m = SUCCESS;
		} else fclose(patn);

		fclose(patn);
		delete pat;
	}
	return m;
}

int PUvm::utimport(char *at)
{
	char *i, *j, *k;
	char *fn;
	bool blnAttr = false;
	bool blnRule = false;
	char *aN = CNIL;
	char *aT = CNIL;
	char *aQ = CNIL;
	char *aD = CNIL;
	char *aP = CNIL;
	char *rN = CNIL;
	char *rS = CNIL;
	double rCF;

	i = syntag(at,buf);
	if (*buf) {
		newstr(&fn,buf);
		sprintf(buf,"%s%s.txt",fnrul,fn);
		delete fn;
		FILE *ifile = fopen(buf,"r");
		if (ifile == NULL) return FAILURE;

		while (fgets(buf,MAX_BUF_LEN,ifile)) {
			i = buf;
			rtrim(i);
			switch (*i++) {
				case '$':	// attribute
					switch (*i++) {
						case 'N':	// attribute name
							if (blnAttr) atr.Add(aN,aT,aQ,aD,aP);
							while (*i && *i <= ' ') i++;
							if (aN != CNIL) delete aN;
							if (aT != CNIL) delete aT;
							if (aQ != CNIL) delete aQ;
							if (aD != CNIL) delete aD;
							if (aP != CNIL) delete aP;
							newstr(&aN,i);
							newstr(&aT,"");
							newstr(&aQ,"");
							newstr(&aD,"");
							newstr(&aP,"");
							blnAttr = true;
							break;
						case 'T':	// attribute text
							while (*i && *i <= ' ') i++;
							if (aT != CNIL) delete aT;
							newstr(&aT,i);
							break;
						case 'Q':	// attribute question
							while (*i && *i <= ' ') i++;
							if (aQ != CNIL) delete aQ;
							newstr(&aQ,i);
							break;
						case 'D':	// attribute default
							while (*i && *i <= ' ') i++;
							if (aD != CNIL) delete aD;
							newstr(&aD,i);
							break;
						case 'P':	// attribute parms
							while (*i && *i <= ' ') i++;
							if (aP != CNIL) delete aP;
							newstr(&aP,i);
							break;
					}
					break;
				case ':':	// rule id
					if (blnAttr) { atr.Add(aN,aT,aQ,aD,aP); blnAttr = false; }
					if (blnRule) xpt.Add(rN,rS);
					while (*i && *i <= ' ') i++;
					if (rN == CNIL) delete rN;
					if (rS == CNIL) delete rS;
					newstr(&rN,i);
					newstr(&rS,"");
					blnRule = true;
					break;
				case ';':	// rule subject area
					while (*i && *i <= ' ') i++;
					if (rS == CNIL) delete rS;
					newstr(&rS,i);
					break;
				case '?':	// premise
					if (blnRule) { xpt.Add(rN,rS); blnRule = false; }
					while (*i && *i <= ' ') i++;
					for (j = i;*j && *j != ',';j++);
					if (!*j) break;
					*j++ = 0;
					for (k = j;*k && *k != ',';k++);
					if (!*k) break;
					*k++ = 0;
					xpt.curRule->AddPremise(i,j,k);
					break;
				case '!':	// result
					if (blnRule) { xpt.Add(rN,rS); blnRule = false; }
					while (*i && *i <= ' ') i++;
					for (j = i;*j && *j != ',';j++);
					if (!*j) break;
					k = j - 1;
					*j++ = 0;
					if (*k == '%') {
						*k-- = 0;
						for (;k >= i && isdigit(*k);k--);
						rCF = atof(k + 1) / 100;
						for (;k >= i && isspace(*k);k--);
						*(k + 1) = 0;
					} else rCF = 1.0;
					xpt.curRule->AddResult(i,j,rCF);
					break;
			}
		}
		fclose(ifile);
	}
	if (blnAttr) atr.Add(aN,aT,aQ,aD,aP);
	if (blnRule) xpt.Add(rN,rS);
	if (aN != CNIL) delete aN;
	if (aT != CNIL) delete aT;
	if (aQ != CNIL) delete aQ;
	if (aD != CNIL) delete aD;
	if (aP != CNIL) delete aP;
	if (rN == CNIL) delete rN;
	if (rS == CNIL) delete rS;
	return SUCCESS;
}

int PUvm::utinsert(char *at)
{
	char *i;
	char *tag = CNIL;
	int n;

	i = synnum(at,&n);
	if ((i = synvar(i,buf,&tag)) != CNIL) {
		if (*i && *i != ',' && *i != ';' && *i != '{' && *i != '?') {
			i = syntag(i,buf);
			if (kb.curSub->m_cases.curCase->m_facts.Find(tag)) {
//				if (
					kb.curSub->m_cases.curCase->m_facts.curFact->Insert(n - 1,buf);
//						);	// remslot(fnd);
			}
		}
	}
	delete tag;
	return SUCCESS;
}

int PUvm::utkill(char *at)
{
	char *i;
	int m = FAILURE;
	TCHAR szFileName[128];

	i = syntag(at,buf);
	if (*buf) {
		wsprintf(szFileName,TEXT("%hs"),buf);
		if (dflg) if (MessageBox(NULL,_T("Ok to delete file?"),szFileName,MB_YESNO) != IDYES) return m;
		m = !DeleteFile(szFileName);
	}
	return m;
}

int PUvm::utload(char *at)
{
	Subject *pSubj = kb.curSub;
	char *fn = CNIL;
	char *pos;
	char *tag = CNIL;
	char *i, *j, *k;
	int m = FAILURE;
	double cf = 1.0;

	if (pSubj == NULL) return m;

	i = syntag(at,buf);
	if (*buf) {
		newstr(&fn,buf);
		sprintf(buf,"%s%s.txt",fnkb,fn);
		FILE *ifile = fopen(buf,"r");
		if (ifile == NULL) {
			sprintf(buf,"%s",fn);
			ifile = fopen(buf,"r");
			if (ifile == NULL) {
				delete fn;
				return m;
			}
			if (j = strrchr(buf,'\\')) {
				strcpy(fn,j + 1);
				if (j = strstr(fn,".txt")) *j = 0;
			}
		}
		pSubj->m_cases.Add(fn);
		kb.curTag->m_cases.Add(fn);
		delete fn;
		Case *pCase = pSubj->m_cases.curCase;
		while (fgets(buf,4096,ifile)) {
			for (pos = buf;*pos && *pos != '=' && *pos != ',' && *pos != '_';pos++);
			if (*pos) {
				k = pos;
				if (*pos == '_') for (pos++;*pos && *pos != '_';pos++);
				else *pos++ = 0;
				rtrim(pos);
				if (*k == '_') {
					if (*pos) pos++;
					for (j = pos;*j;j++);
					for (k = j++;k >= pos;k--) *(k + 1) = *k;
					*pos++ = 0;
					while (j > pos && *j <= ' ') *j-- = 0;
					if (*j == ']') {
						cf = 1.0;
						for (j--,*j-- = 0;j > pos && *j != '[';j--);
						if (*j == '[') { *j++ = 0; cf = atof(j); }
					}
				} else {
					for (j = k;j > buf && *j <= ' ';j--) *j = 0;
					if (*j == ']') {
						cf = 1.0;
						for (j--,*j-- = 0;j > buf && *j != '[';j--);
						if (*j == '[') { *j++ = 0; cf = atof(j); }
					}
				}
				if (cf != 1.0) {
					if (*buf) {
						if (tag != CNIL) delete tag;
						newstr(&tag,buf);
						pCase->m_facts.Add(tag,pos,"File",cf);
					} else {
						pCase->m_facts.Find(tag);
						pCase->m_facts.curFact->Queue(pos);
					}
				} else {
					if (*buf) {
						if (tag != CNIL) delete tag;
						newstr(&tag,buf);
						pCase->m_facts.Add(tag,pos);
					} else {
						pCase->m_facts.Find(tag);
						pCase->m_facts.curFact->Queue(pos);
					}
				}
			}
		}
		fclose(ifile);
	}
	if (tag != CNIL) delete tag;
	return (m);
}

int PUvm::utmake(char *at)
{
	char *i;
	Subject *pSubj = kb.curSub;

	i = syntag(at,buf);                         
	if (!*buf) return FAILURE;    
	if (pSubj == NULL) return FAILURE;
	if (pSubj->m_cases.Find(buf)) {
		kb.curTag->m_cases.Find(buf);
		return SUCCESS;
	}
	pSubj->m_cases.Add(buf);
	kb.curTag->m_cases.Add(buf);
	return SUCCESS;
}

int PUvm::utmatch(char *at)
{
	char *i, *pos;
	char loc[20];
	fpos_t bestpos;
	long cnt = 0;
	bool fnd = false;
	int m = FAILURE;

	i = syntag(at,buf);
	if (*buf) {
		if (bot->usegraphmaster) bestpos = bot->gm_interpret(buf); else bestpos = bot->interpret(buf);
		if (bestpos) {
			sprintf(loc,"@%li",bestpos);

			FILE *patn = fopen(bot->fnpat,"r");
			while (!feof(patn)) {
				fgets(buf,MAX_SIZE,patn);
				if (!(pos = strstr(buf,"@"))) continue;
				cnt++;
				if (strstr(pos,loc)) { fnd = true; break; }
			}
			fclose(patn);

			if (fnd) {
				*pos = 0;
				sprintf(loc,"%ld",cnt);
				savslot("BotDB Pattern",buf);
				savslot("BotDB Index",loc);

				FILE *tmpl = fopen(bot->fntmp,"r");
				fsetpos(tmpl,&bestpos);
				fgets(buf,MAX_SIZE,tmpl);
				fclose(tmpl);

				rtrim(buf);
				expand(buf);
				savslot("BotDB Template",buf);

				m = SUCCESS;
			}
		}
	}
	return (m);
}

int PUvm::utnorm(char *at)
{
	char *i;
	char *tag = CNIL;
      
	i = synvar(at,buf,&tag);      
	if (i != CNIL) {
		bot->tidyup(buf);
		savslot(tag,buf);
	}
	delete tag;
	return (i == CNIL);
}

int PUvm::utpop(char *at)
{
	char *i;
	char *tag = CNIL;
	char *fnd = CNIL;

	if ((i = synvar(at,buf,&tag)) != CNIL) {
		i = fndtag(i,&fnd);
		while (*fnd) {
			if (kb.curSub->m_cases.curCase->m_facts.Find(tag)) {
				if (kb.curSub->m_cases.curCase->m_facts.curFact->Pop(buf)) savslot(fnd,buf);
			}
			delete fnd;
			i = fndtag(i,&fnd);
		}
		delete fnd;
	}
	delete tag;
	return SUCCESS;
}

char *PUvm::utpose(char *at)
{
	char *fn = CNIL;
	char *pos;
	char *i, *j, *k, *x;

	i = synutag(at,buf);
	if (i != CNIL) {
		if (*buf) {
			newstr(&fn,buf);

			sprintf(buf,"%s%s",fnpgm,fn);
			if (!strstr(fn,".txt") && !strstr(fn,".ini")) strcat(buf,".txt");

			FILE *ifile = fopen(buf,"r");
			if (ifile == NULL) {
				strcpy(buf,fn);
				for (j = buf + strlen(buf); j > buf && *j != '\\';j--);
				if (j <= buf || *j != '\\') { delete fn; return (CNIL); }
				strcpy(fn,j + 1);
				for (j = fn + strlen(fn); j > fn && *j != '.';j--);
				if (j > fn && *j == '.') *j = 0;
				ifile = fopen(buf,"r");
				if (ifile == NULL) { delete fn; return (CNIL); }
			}

			pgm.Add(fn);

			j = buf; *j++ = ' ';

			char *ln = new char[1024];
			while (fgets(ln,1024,ifile)) {
				for (pos = ln;*pos && *pos < ' ';pos++);
				if (*(j - 1) > ' ' && *(j - 1) != ',') *j++ = ' ';
				for (;*pos;pos++) if (*pos >= ' ' || *pos == 21 || *pos == '\n') {
					if (*pos == '\n') *j++ = '\r';
					*j++ = *pos;
				}
				*j = 0;

				x = CNIL;								// save already complete frames here...
				for (k = buf;*k && *k != '{';k++);
				while (*k == '{') {
					for (k++;*k && *k != '}';k++);
					if (*k != '}') break;
					for (k++;*k && *k != '{';k++);
					if (*k == '{') x = k;
				}
				if (x != CNIL) {
					newstr(&pos,x);
					*x = 0;
					pgm.curScript->LoadFrames(buf);
					strcpy(buf,pos);
					delete pos;
					for (j = buf;*j;j++);
				}
			}
			fclose(ifile);
			delete ln;

			pgm.curScript->LoadFrames(buf);

			delete fn;
			return i;
		}
	}
	return CNIL;
}

/*int PUvm::utpost(char *at)
{
	char *i;
	char *pos;
	char *tag;
	char ntag[32];
	int cnt = 1;
	CString srv;
	//CString url;
	CString hdr = _T("Content-Type: application/x-www-form-urlencoded");
	CString prm;
	CString prmlst;
	CInternetSession session;
	CHttpConnection* pConnection = NULL;
	CHttpFile* pFile = NULL;
	UINT result;

	i = syntag(at,buf);
	if (*buf) {
		if (!strncmp(buf,"http://",7)) {
			if (pos = strstr(buf + 7,"/")) {
				*pos++ = 0;
				srv = buf + 7;
				url = pos;
				pConnection = session.GetHttpConnection(srv);
				pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,url);
				i = synvar(i,buf,&tag);
				while (*tag) {
					while (replace(buf,"&","+"));
					while (replace(buf," ","+"));
					prm.Format(_T("%s=%s"),tag,buf);
					if (prmlst.GetLength() > 1) prmlst += "&";
					prmlst += prm;
					delete tag;
					i = synvar(i,buf,&tag);
				}
				delete tag;
				if (prmlst.GetLength() > 1) {
					result = pFile->SendRequest(hdr,(LPVOID)(LPCTSTR)prmlst,prmlst.GetLength());
					if (result) {
						while ((result = pFile->Read(buf,MAX_BUF_LEN)) > 0) {
							sprintf(ntag,"URLdata%d",cnt++);
							buf[result] = 0;
							savslot(ntag,buf);
						}
						pFile->Close();
					}
				}
				delete pFile;
				pConnection->Close();
				delete pConnection;
			}
		}
	}
	session.Close();
	return SUCCESS;
}

int PUvm::utpush(char *at)
{
	char *i;
	char *tag = CNIL;

	if ((i = synvar(at,buf,&tag)) == CNIL) {
		if (!*tag) { delete tag; return FAILURE; }
		savslot(tag,"");
	}		
	while (*i && *i != ',' && *i != ';' && *i != '{' && *i != '?') {
		i = syntag(i,buf);
		if (kb.curSub->m_cases.curCase->m_facts.Find(tag)) {
			kb.curSub->m_cases.curCase->m_facts.curFact->Push(buf);
		}
	}
	delete tag;
	return SUCCESS;
}*/

int PUvm::utput(char *at)
{
	Subject *pSubj = kb.curSub;
	CString str;
	char *i;
	char *fn = CNIL;
	char *text;
	int m = FAILURE;
	bool blnXML = false;

	if (pSubj == NULL) return m;

	i = syntag(at,buf);
	if (*buf) {
		newstr(&fn,buf);
		sprintf(buf,"%s%s",fnkb,fn);
		if (strstr(fn,".xml")) blnXML = true; else if (!strstr(fn,".txt")) strcat(buf,".txt");
		delete fn;
		FILE *ofile = fopen(buf,"w");
		if (blnXML) pSubj->m_cases.XMLCases(str); else pSubj->m_cases.FileCases(str);
		text = new char[str.GetLength() + 10];
		sprintf(text,"%s",str);
		fprintf(ofile,"%s",text);
		delete text;
		fclose(ofile);
		m = SUCCESS;
	}
	return (m);
}

int PUvm::utqueue(char *at)
{
	char *i;
	char *tag = CNIL;

	if ((i = synvar(at,buf,&tag)) == CNIL) {
		if (!*tag) { delete tag; return FAILURE; }
		savslot(tag,"");
	}
	while (*i && *i != ',' && *i != ';' && *i != '{' && *i != '?') {
		i = syntag(i,buf);
		if (kb.curSub->m_cases.curCase->m_facts.Find(tag)) {
			kb.curSub->m_cases.curCase->m_facts.curFact->Queue(buf);
		}
	}
	delete tag;
	return SUCCESS;
}

int PUvm::utquit(char *at)
{
	char *i;
	int k;

	i = synnum(at,&k);
	qflg = true;
	retnum = k;
	return SUCCESS;
}

int PUvm::utrecall(char *at)
{
	char *i, *pos;
	char loc[20];
	fpos_t fpos;
	long cnt = 0;
	bool fnd = false;
	char *pat;
	int n;
	int m = FAILURE;

	i = syntag(at,buf);
	if (*buf) {
		strcat(buf,"!");
		newstr(&pat,buf);
		n = (int)strlen(pat);

		FILE *patn = fopen(bot->fnpat,"r+");
		while (!feof(patn)) {
			fgetpos(patn,&fpos);
			fgets(buf,MAX_SIZE,patn);
			if (strstr(buf,"@")) cnt++;
			if (!(pos = strstr(buf,"!"))) continue;
			if (!strnicmp(buf,pat,n)) { fnd = true; break; }
		}

		if (fnd) {
			*pos = '@';
			fsetpos(patn,&fpos);
			fputs(buf,patn);
			fclose(patn);

			rtrim(pos);
			*pos++ = 0;
			fpos = atol(pos);
			sprintf(loc,"%ld",++cnt);
			savslot("BotDB Index",loc);
			savslot("BotDB Pattern",buf);

			FILE *tmpl = fopen(bot->fntmp,"r");
			fsetpos(tmpl,&fpos);
			fgets(buf,MAX_BUF_LEN,tmpl);
			fclose(tmpl);

			rtrim(buf);
			expand(buf);
			savslot("BotDB Template",buf);

			bot->reloadgm();

			m = SUCCESS;
		} else fclose(patn);

		delete pat;
	}
	return m;
}

int PUvm::utremove(char *at)
{
	char *i, *j, *k;
	int n;
	bool sflg = false;
	Case *pCase;

	for (j = at;*j && isspace(*j);j++);
	while (*j == '$' || *j == '%' || *j == '*') {
		if (*j == '$') j++;
		i = fndtag(j,&k);
		if (!*k && !strncmp(j,"*()",3)) { i = j + 3; sflg = true; }
		if (!*k && *j == '*') { i = j + 1; strcpy(k,"*"); }
		if (!*k) { delete k; break; }
		n = (int)strlen(k);
		if (*k == '*') {
			if (sflg) {
				sflg = false;
				if (kb.curTag != NULL) {
					pCase = kb.curTag->m_cases.curCase;
					if (pCase != NULL) pCase->m_facts.RemoveAll();
				}
			} else {
				if (kb.curSub != NULL) {
					pCase = kb.curSub->m_cases.curCase;
					if (pCase != NULL) pCase->m_facts.RemoveAll();
				}
			}
		} else remslot(k);
		for (j = i;*j && isspace(*j);j++);
		delete k;
	}
	return SUCCESS;
}

int PUvm::utrescnd(char *at)
{
	char *i;
	int m = FAILURE;

	i = syntag(at,buf);
	if (*buf) {
		if (pgm.Remove(buf)) m = SUCCESS;
	}
	return (m);
}

int PUvm::utrstart(char *at)
{
	char *i;

	for (i = cmdlist;*i && *i != '}';i++);
	if (*i == '}') jflg = ++i; else jflg = cmdlist;
	return SUCCESS;
}

int PUvm::utrules(char *at)
{
	bool blnFile = false;
	bool blnAll = false;
	bool blnXML = false;
	char *i;
	CString s;

	if (dflg) {
		i = syntag(at,buf);
		for (i = buf;*i;i++) {
			if (*i == 'f' || *i == 'F') blnFile = true;
			if (*i == 'v' || *i == 'V') blnAll = true;
			if (*i == 'x' || *i == 'X') blnXML = true;
		}
		if (blnFile) xpt.FileRules(s);
		else if (blnAll) xpt.FormatRules(s);
		else if (blnXML) xpt.XMLRules(s);
		else xpt.ListRules(s);
		sprintf(buf,"%s",s);
		post(buf);
	}
	return SUCCESS;
}

int PUvm::utrule(char *at)
{
	CString s;
	char *i;

	if (dflg) {
		i = syntag(at,buf);
		if (xpt.Find(buf)) {
			xpt.curRule->FormatRule(s);
			sprintf(buf,"%s",s);
			post(buf);
		}
	}
	return SUCCESS;
}

int PUvm::utrwrite(char *at)
{
	char *i;
	char *pos;
	char *opatrn;
	char *patrn;
	char *tmpl;
	char *fn;
	int iRet;
	bool blnTopic = true;
	bool blnThat = true;
	bool blnPatrn = false;

	i = syntag(at,buf);
	if (!*buf) return FAILURE;
	while (replace(buf,"]",">"));
	while (replace(buf,"[","<"));
	while (replace(buf,"&gt;",">"));
	while (replace(buf,"&lt;","<"));
	opatrn = new char[strlen(buf) + 30];
	sprintf(opatrn,"<pattern>%s",buf);
	i = syntag(i,buf);
	if (!*buf) { delete opatrn; return FAILURE; }
	while (replace(buf,"]",">"));
	while (replace(buf,"[","<"));
	while (replace(buf,"&gt;",">"));
	while (replace(buf,"&lt;","<"));
	patrn = new char[strlen(buf) + 20];
	strcpy(patrn,buf);
	i = syntag(i,buf);
	if (!*buf) { delete opatrn; delete patrn; return FAILURE; }
	while (replace(buf,"&gt;",">"));
	while (replace(buf,"&lt;","<"));
	while (replace(buf,"&amp;","&"));
	newstr(&tmpl,buf);
	i = syntag(i,buf);
	if (!*buf) { delete opatrn; delete patrn; delete tmpl; return FAILURE; }
	newstr(&fn,buf);

	char *otop = new char[MAX_LINE_SIZE];
	char *othat = new char[MAX_LINE_SIZE];

	if (i = strstr(opatrn,"<topic>")) {
		sprintf(otop,"<topic name=\"%s\">",i + 7);
		*i = 0;
		replace(otop,"</topic>","");
	} else *otop = 0;
	*othat = 0;
	if (i = strstr(opatrn,"<that>")) {
		strcpy(othat,i);
		strcpy(i,"</pattern>");
		if (!strstr(othat,"</that>")) strcat(othat,"</that>");
	} else strcat(opatrn,"</pattern>");

	char *top = new char[MAX_LINE_SIZE];

	if (i = strstr(patrn,"<topic>")) {
		strcpy(top,i + 7);
		*i = 0;
		replace(top,"</topic>","");
	} else *top = 0;
	if (i = strstr(patrn,"<that>")) {
		strcpy(buf,i);
		strcpy(i,"</pattern>\n");
		strcat(patrn,buf);
		if (!strstr(patrn,"</that>")) strcat(patrn,"</that>");
	} else strcat(patrn,"</pattern>");

	char *aiml = new char[MAX_BUF_LEN];

	if (*top) sprintf(aiml,"<topic name=\"%s\">\n",top); else *aiml = 0;

	strcat(aiml,"<category>\n<pattern>");
	strcat(aiml,patrn);
	strcat(aiml,"\n");

	for (i = tmpl,pos = buf;*i;i++) {
		if (!strncmp(i,"<random>",8)      || !strncmp(i,"<condition>",11) ||
			!strncmp(i,"</condition>",12) || !strncmp(i,"</random>",9)    ||
			!strncmp(i,"</li>",5)         || !strncmp(i,"</save>",7)      ||
			!strncmp(i,"</think>",8)      || !strncmp(i,"</system>",9)    ||
			!strncmp(i,"</gossip>",9)     || !strncmp(i,"<br/>",5)        ||
			!strncmp(i,"<BR/>",5)         || !strncmp(i,"<usesaved/>",11)) {
			while (*i != '>') *pos++ = *i++;
			*pos++ = *i;
			*pos++ = '\n';
		} else *pos++ = *i;
	}
	*pos = 0;
	if (strlen(buf) == strlen(tmpl)) {
		strcat(aiml,"<template>");
		strcat(aiml,buf);
	} else {
		if (*(pos - 1) != '\n') strcat(buf,"\n");
		strcat(aiml,"<template>\n");
		strcat(aiml,buf);
	}
	strcat(aiml,"</template>\n</category>\n");
	if (*top) strcat(aiml,"</topic>\n");
	delete tmpl;
	delete patrn;
	delete top;

	char *line = new char[MAX_BUF_LEN];
	char *next = new char[MAX_BUF_LEN];

	*buf = 0;
	FILE *faiml = fopen(fn,"r");
	if (faiml) {
		FILE *fwork = fopen(bot->fnwrk,"w");
		if (fgets(line,MAX_BUF_LEN,faiml)) {
			while (!feof(faiml)) {
				*next = 0;
				fgets(next,MAX_BUF_LEN,faiml);
				if (*otop) {
					if (strstr(line,otop)) blnTopic = true;
					if (strstr(line,"</topic>")) blnTopic = false;
				}
				if (strstr(line,"<category>")) {
					while (!strstr(line,"<template>")) {
						if (strlen(line) + strlen(next) > MAX_BUF_LEN) break;
						strcat(line,next);
						fgets(next,MAX_BUF_LEN,faiml);
					}
					strcpy(buf,pos = strstr(line,"<template>"));
					*pos = 0;
					if (strstr(line,opatrn)) blnPatrn = true; else blnPatrn = false;
					if (*othat) {
						if (strstr(line,othat)) blnThat = true; else blnThat = false;
					}
					if (!blnPatrn || !blnThat || !blnTopic) {
						fprintf(fwork,"%s",line);
						*line = 0;
					} else {
						strcpy(line,buf); *buf = 0;
						while (!strstr(line,"</category>")) {
							if (strlen(line) + strlen(next) > MAX_BUF_LEN) break;
							strcat(line,next);
							fgets(next,MAX_BUF_LEN,faiml);
						}
						pos = strstr(line,"</category>") + 11;
						while (*pos && *pos <= ' ') pos++;
						strcpy(buf,pos);
						if (!strcmp(next,"\n") || !strcmp(next,"\r\n")) *next = 0;
						*line = 0;
					}
				}
				if (*line) {
					if (pos = strstr(line,"</aiml>")) {
						strcpy(buf,pos);
						*pos = 0;
						fprintf(fwork,"%s",line);
						fprintf(fwork,"%s\n",aiml);
					} else if (pos = strstr(line,"</alice>")) {
						strcpy(buf,pos);
						*pos = 0;
						fprintf(fwork,"%s",line);
						fprintf(fwork,"%s\n",aiml);
					} else {
						fprintf(fwork,"%s",line);
					}
				}
				strcpy(line,buf); *buf = 0;
				strcat(line,next);
			}
		}
		if (*line) fprintf(fwork,"%s",line);
		fclose(faiml);
		fclose(fwork);

		TCHAR szFileName[MAX_PATH_LEN];
		wsprintf(szFileName,TEXT("%hs"),fn);
		iRet = DeleteFile(szFileName);

		TCHAR szOldFileName[MAX_PATH_LEN];
		wsprintf(szOldFileName,TEXT("%hs"),bot->fnwrk);
		iRet = MoveFile(szOldFileName,szFileName);
	} else {
		FILE *faiml = fopen(fn,"w");
		if (faiml) {
			fprintf(faiml,"<aiml>\n");
			fprintf(faiml,"%s",aiml);
			fprintf(faiml,"</aiml>");
			fclose(faiml);
		}
	}
	delete opatrn;
	delete othat;
	delete otop;
	delete aiml;
	delete line;
	delete next;
	delete fn;

	return SUCCESS;
}

int PUvm::utsave(char *at)
{
	Case *pCase;
	CString str;
	char *i;
	char *fn = CNIL;
	char *text;
	int m = FAILURE;
	bool blnXML = false;

	if (kb.curSub == NULL) return m;
	pCase = kb.curSub->m_cases.curCase;
	if (pCase == NULL) return m;

	i = syntag(at,buf);
	if (*buf) {
		newstr(&fn,buf);
		sprintf(buf,"%s%s",fnkb,fn);
		if (strstr(fn,".xml")) blnXML = true; else if (!strstr(fn,".txt")) strcat(buf,".txt");
		delete fn;
		FILE *ofile = fopen(buf,"w");
		if (blnXML) pCase->m_facts.XMLFacts(str); else pCase->m_facts.FormatFacts(str);
		text = new char[str.GetLength() + 10];
		sprintf(text,"%s",str);
		fprintf(ofile,"%s",text);
		delete text;
		fclose(ofile);
		m = SUCCESS;
	}
	return (m);
}

int PUvm::utsay(char *at)
{

	return SUCCESS;
}

int PUvm::utscript(char *at)
{
	CString s;
	char *i;
	bool blnAll = false;
	bool blnCur = false;

	if (dflg) {
		i = syntag(at,buf);
		for (i = buf;*i;i++) {
			if (*i == 'v' || *i == 'V') blnAll = true;
			if (*i == 'c' || *i == 'C') blnCur = true;
		}
		if (blnCur) {
			if (blnAll) pgm.curScript->FormatScript(s);
			else pgm.curScript->ListScript(s);
		} else {
			if (blnAll) pgm.FormatScripts(s);
			else pgm.ListScripts(s);
		}
		sprintf(buf,"%s",s);
		post(buf);
	}
	return SUCCESS;
}

int PUvm::utsearch(char *at)
{
	char *i, *pos;
	char loc[20];
	fpos_t fpos;
	long cnt = 0;
	bool fnd = false;
	char *pat;
	int n;
	int m = FAILURE;

	i = syntag(at,buf);
	if (*buf) {
		newstr(&pat,buf);
		n = (int)strlen(pat);

		FILE *aiml = fopen(bot->fnpat,"r");
		while (!feof(aiml)) {
			fgets(buf,MAX_SIZE,aiml);
			if (!(pos = strstr(buf,"@"))) continue;
			cnt++;
			if (!strnicmp(buf,pat,n)) { fnd = true; break; }
		}
		fclose(aiml);

		if (fnd) {
			rtrim(pos);
			*pos++ = 0;
			fpos = atol(pos);
			sprintf(loc,"%ld",cnt);
			savslot("BotDB Index",loc);
			savslot("BotDB Pattern",buf);

			FILE *tmpl = fopen(bot->fntmp,"r");
			fsetpos(tmpl,&fpos);
			fgets(buf,MAX_BUF_LEN,tmpl);
			fclose(tmpl);

			rtrim(buf);
			expand(buf);
			savslot("BotDB Template",buf);

			m = SUCCESS;
		}
		delete pat;
	}
	return m;
}

int PUvm::utsense(char *at)
{
	CString t;
	char *i, *j;
	char sensenum[MAX_VALUE_LEN];
	char *tag = CNIL;
	char *morph;
	Case *pCase;
	Fact *pFact;
	int pos = ALL_POS;
	int n;
	int search = OVERVIEW;
	int whichsense = ALLSENSES;
	ss *ans;
	ss *ptr;

	i = syntag(at,buf);
	if (*buf == '-') {
		if (j = strstr(buf," ")) *j = 0;
		if (!strcmp(buf,"-n")) pos = NOUN;
		if (!strcmp(buf,"-v")) pos = VERB;
		if (!strcmp(buf,"-a")) pos = ADJ;
		if (!strcmp(buf,"-r")) pos = ADV;
		if (j) for(i = j + 1;*i == ' ';i++);
	} else i = at;

	if ((i = synvar(i,buf,&tag)) != CNIL) {
		pCase = kb.curSub->m_cases.curCase;
		if (pCase->m_facts.Find(tag)) {
			delete tag;
			pFact = pCase->m_facts.curFact;
			while (!pFact->m_mvlist.IsEmpty()) t = pFact->m_mvlist.RemoveTail();

			if (wninit()) return FAILURE;
			lowercase(buf);
			while (replace(buf," ","-"));
			for (n = 1; n <= NUMPARTS; n++) {
				if (pos == ALL_POS || pos == n) {
					ans = findtheinfo_ds(buf, n, search, whichsense);
					for (ptr = ans;ptr != NULL;ptr = ptr->nextss) {
						if (!strncmp(ptr->pos,"s",1)) sprintf(sensenum,"%s%%5:00:%-2.2d:%s:%-2.2d",buf,ptr->lexid[ptr->whichword - 1],ptr->headword,ptr->headsense);
						else sprintf(sensenum,"%s%%%-1.1d:%-2.2d:%-2.2d::",buf,n,ptr->fnum,ptr->lexid[ptr->whichword - 1]);
						pFact->Push(sensenum);
					}
					free_syns(ans);
					if ((morph = morphstr(buf,n)) != NULL) {
						do {
							ans = findtheinfo_ds(morph, n, search, whichsense);
							for (ptr = ans;ptr != NULL;ptr = ptr->nextss) {
								if (!strncmp(ptr->pos,"s",1)) sprintf(sensenum,"%s%%5:00:%-2.2d:%s:%-2.2d",buf,ptr->lexid[ptr->whichword - 1],ptr->headword,ptr->headsense);
								else sprintf(sensenum,"%s%%%-1.1d:%-2.2d:%-2.2d::",buf,n,ptr->fnum,ptr->lexid[ptr->whichword - 1]);
								pFact->Push(sensenum);
							}
							free_syns(ans);
						} while ((morph = morphstr(NULL,n)) != NULL);
					}
					if (pos != ALL_POS) break;
				}
			}
		}
	} else delete tag;

	return SUCCESS;
}

int PUvm::utsentence(char *at)
{
	CString t;
	char c;
	char *i, *p, *q;
	char *tag = CNIL;
	char delim[8];
	Case *pCase;
	Fact *pFact;

	strcpy(delim,".?!;");  // ***  add your own sentence delimiters here  ***

	if ((i = synvar(at,buf,&tag)) != CNIL) {
		pCase = kb.curSub->m_cases.curCase;
		if (pCase->m_facts.Find(tag)) {
			pFact = pCase->m_facts.curFact;
			while (!pFact->m_mvlist.IsEmpty()) t = pFact->m_mvlist.RemoveTail();
			for (p = buf;*p;p++) {
				for (q = p;*q && !strchr(delim,*q);q++);
				if (*q) {
					*q++;
					c = *q;
					*q = 0;
					if (*p) rtrim(p);
					if (*p) pFact->Push(p);
					for (*q = c,p = q;*p && isspace(*p);p++); p--;
				} else {
					if (*p) rtrim(p);
					if (*p) pFact->Push(p);
					break;
				}
			}
		}
	}
	delete tag;
	return SUCCESS;
}

int PUvm::utsetbot(char *at)
{
	char *i;
	char *tag = CNIL;
	int n;

	if ((i = synvar(at,buf,&tag)) != CNIL) {
		if (kb.curSub->m_cases.curCase->m_facts.Find(tag)) {
			strcpy(bot->thats[0],buf);
			for (n = 1;n < 5;n++) {
				if (kb.curSub->m_cases.curCase->m_facts.curFact->Extract(n,buf)) strcpy(bot->thats[n],buf);
			}
		}
		delete tag;
		if ((i = synvar(i,buf,&tag)) != CNIL) {
			if (kb.curSub->m_cases.curCase->m_facts.Find(tag)) {
				strcpy(bot->inputs[0],buf);
				for (n = 1;n < 5;n++) {
					if (kb.curSub->m_cases.curCase->m_facts.curFact->Extract(n,buf)) strcpy(bot->inputs[n],buf);
				}
			}
			delete tag;
			i = fndtag(i,&tag);
			if (*tag) if (getslot(tag,buf)) strcpy(bot->topic,buf);
		}
	}
	delete tag;
	return SUCCESS;
}

int PUvm::utshare(char *at)
{
	char *i;
	char *tag = CNIL;
      
	i = synvar(at,buf,&tag);      
	if (i != CNIL) {
		shrslot(tag,buf);
		delete tag;
		return SUCCESS;
	}
	delete tag;
	return FAILURE;
}

int PUvm::utshow(char *at)
{
	char *i;

	i = syntag(at,buf);
	if (*buf) {
		dflg = !stricmp(buf,(char*)"on")  || !stricmp(buf,(char*)"deduce");
		aflg = !stricmp(buf,(char*)"off") || !stricmp(buf,(char*)"think");
	}
	return SUCCESS;
}

int PUvm::utsubjects(char *at)
{
	bool blnXML = false;
	bool blnAll = false;
	char *i;
	CString s;

	if (dflg) {
		i = syntag(at,buf);
		for (i = buf;*i;i++) {
			if (*i == 'v' || *i == 'V') blnAll = true;
			if (*i == 'x' || *i == 'X') blnXML = true;
		}
		if (blnAll) kb.FormatSubjects(s);
		else if (blnXML) kb.XMLSubjects(s);
		else kb.ListSubjects(s);

		sprintf(buf,"%s",s);
		post(buf);
	}
	return SUCCESS;
}

int PUvm::utsubmit(char *at)
{
	char *i;
	int m = SUCCESS;

	i = syntag(at,buf);
	if (*buf) {
		stkpush(i);
		if (cmdlist != CNIL) delete cmdlist;
		newstr(&cmdlist,buf);
		jflg = cmdlist;
	} else m = FAILURE;
	return (m);
}

int PUvm::utsynonym(char *at)
{
	CString s;
	CString t;
	char *i, *j;
	char *tag = CNIL;
	char *morph;
	char *w;
	Case *pCase;
	Fact *pFact;
	int npos;
	int pos = ALL_POS;
	int search = OVERVIEW;
	int whichsense = ALLSENSES;
	ss *ans;
	ss *ptr;
	int m, mvcnt;
	int n;
	POSITION mvpos;
	bool blnNew;

	i = syntag(at,buf);
	if (*buf == '-') {
		if (j = strstr(buf," ")) *j = 0;
		if (!strcmp(buf,"-n")) pos = NOUN;
		if (!strcmp(buf,"-v")) pos = VERB;
		if (!strcmp(buf,"-a")) pos = ADJ;
		if (!strcmp(buf,"-r")) pos = ADV;
		if (j) for(i = j + 1;*i == ' ';i++);
	} else i = at;

	if ((i = synvar(i,buf,&tag)) != CNIL) {
		pCase = kb.curSub->m_cases.curCase;
		if (pCase->m_facts.Find(tag)) {
			delete tag;
			pFact = pCase->m_facts.curFact;
			while (!pFact->m_mvlist.IsEmpty()) t = pFact->m_mvlist.RemoveTail();

			if (wninit()) return FAILURE;

			lowercase(buf);
			while(replace(buf," ","-"));
			if (strchr(buf,'%')) {
				ans = GetSynsetForSense(buf);
				for (ptr = ans;ptr != NULL;ptr = ptr->nextss) {
					for (n = 0; n < ptr->wcount;n++) {
						strcpy(buf,ptr->words[n]);
						if (i = strchr(buf,'(')) *i = 0;
						while (replace(buf,"_"," "));
						pFact->Push(buf);
					}
				}
				free_syns(ans);
			} else {
				newstr(&w,buf);
				for (npos = 1; npos <= NUMPARTS; npos++) {
					if (pos == ALL_POS || pos == npos) {
						ans = findtheinfo_ds(w, npos, search, whichsense);
						for (ptr = ans;ptr != NULL;ptr = ptr->nextss) {
							for (n = 0; n < ptr->wcount;n++) {
								strcpy(buf,ptr->words[n]);
								if (i = strchr(buf,'(')) *i = 0;
								while (replace(buf,"_"," "));
								t = buf;
								mvcnt = (int)pFact->m_mvlist.GetCount();
								blnNew = true;
								for (m = 0;m < mvcnt;m++) {
									mvpos = pFact->m_mvlist.FindIndex(m);
									if (t == pFact->m_mvlist.GetAt(mvpos)) {
										blnNew = false;
										break;
									}
								}
								if (blnNew) pFact->Push(buf);
							}
						}
						free_syns(ans);
						if ((morph = morphstr(w,npos)) != NULL) {
							do {
								ans = findtheinfo_ds(morph, npos, search, whichsense);
								for (ptr = ans;ptr != NULL;ptr = ptr->nextss) {
									for (n = 0; n < ptr->wcount;n++) {
										strcpy(buf,ptr->words[n]);
										if (i = strchr(buf,'(')) *i = 0;
										while (replace(buf,"_"," "));
										t = buf;
										blnNew = true;
										mvcnt = (int)pFact->m_mvlist.GetCount();
										for (m = 0;m < mvcnt;m++) {
											mvpos = pFact->m_mvlist.FindIndex(m);
											if (t == pFact->m_mvlist.GetAt(mvpos)) {
												blnNew = false;
												break;
											}
										}
										if (blnNew) pFact->Push(buf);
									}
								}
								free_syns(ans);
							} while ((morph = morphstr(NULL,npos)) != NULL);
						}
						if (pos == npos) break;
					}
				}
				delete w;
			}
		}
	} else delete tag;

	return SUCCESS;
}



int PUvm::utuse(char *at)
{
	char *i;
	char *fn;
	char *fnb;
	int n;
	bool flg = false;

	i = syntag(at,buf);
	if (*buf) {
		newstr(&fn,buf);
		i = syntag(i,buf);
		if (*buf) {
			newstr(&fnb,buf);
			i = syntag(i,buf);
			if (*buf) {
				strcpy(bot->fnidx,fn);
				strcpy(bot->fnpat,fnb);
				strcpy(bot->fntmp,buf);
				i = syntag(i,buf);
				if (*buf) strcpy(bot->fnbot,buf);
			} else {
				strcpy(bot->fnbot,fnb);
				flg = true;
			}
			delete fnb;
		} else flg = true;
		if (flg) {
			if (*fn != '\\') {
				strcpy(buf,fn);
				fullpath(buf);
				delete fn;
				newstr(&fn,buf);
			}
			n = (int)strlen(fn) - 1;
			if (fn[n] == '\\') fn[n] = 0;
			sprintf(bot->fnidx,"%s\\index.txt",fn);
			sprintf(bot->fnpat,"%s\\patterns.txt",fn);
			sprintf(bot->fntmp,"%s\\templates.txt",fn);
		}
		delete fn;
	} else {
		strcpy(bot->fnidx,fullpath("db\\index.txt"));
		strcpy(bot->fnpat,fullpath("db\\patterns.txt"));
		strcpy(bot->fntmp,fullpath("db\\templates.txt"));
		strcpy(bot->fnbot,fullpath("bot\\vars.txt"));

		FILE *fdirs = fopen(fullpath("options.ini"),"r");
		while (!feof(fdirs)) {
			fgets(buf,MAX_SIZE,fdirs);
			rtrim(buf);
			if (!strncmp(buf,"index=",6))       strcpy(bot->fnidx,buf + 6);
			if (!strncmp(buf,"patterns=",10))   strcpy(bot->fnpat,buf + 10);
			if (!strncmp(buf,"templates=",11))  strcpy(bot->fntmp,buf + 11);
			if (!strncmp(buf,"bot=",4))         strcpy(bot->fnbot,buf + 4);
		}
		fclose(fdirs);
	}
	bot->loadchunks();		
	bot->reloadgm();
	return SUCCESS;
}

int PUvm::utwhat(char *at)
{
	char *i;
	CString s;
	CString r;

	if (dflg) {
		i = syntag(at,buf);
		r.Format(_T("%hs"),buf);
		xpt.What(r,s);
		sprintf(buf,"%s",s);
		post(buf);
	}
	return SUCCESS;
}

int PUvm::utwhich(char *at)
{
	char *i;
	CString s;
	CString r;

	if (dflg) {
		i = syntag(at,buf);
		r.Format(_T("%hs"),buf);
		xpt.Which(r,s);
		sprintf(buf,"%s",s);
		post(buf);
	}
	return SUCCESS;
}

int PUvm::utword(char *at)
{
	CString t;
	char *i, *j, *p, *q;
	char *tag = CNIL;
	char *morph;
	char delim[64];
	Case *pCase;
	Fact *pFact;
	bool pos  = false;
	bool posn = false;
	bool posv = false;
	bool posa = false;
	bool posr = false;
	bool poss = false;
	bool posm = false;
	bool posf = false;
	bool posb = false;
	bool posx = false;
	bool posinc = false;
	bool posexc = false;
	int m, mvcnt;
	POSITION mvpos;
	bool blnNew;
	bool ok;
	bool done;

	strcpy(delim," .?!;;,_\\\t\r\n\"@#$%^&*+=`~(){}[]|/<>");  // ***  add your own word delimiters here  ***

	i = syntag(at,buf);
	if (*buf == '-') {
		if (j = strstr(buf," ")) *j = 0;
		if (strchr(buf,'n')) pos = posn = true;
		if (strchr(buf,'v')) pos = posv = true;
		if (strchr(buf,'a')) pos = posa = true;
		if (strchr(buf,'r')) pos = posr = true;
		if (strchr(buf,'s')) pos = poss = true;
		if (strchr(buf,'m')) pos = posm = true;
		if (strchr(buf,'f')) pos = posf = true;
		if (strchr(buf,'b')) pos = posb = true;
		if (strchr(buf,'x')) posx = true;
		if (strchr(buf,'^')) posinc = true;
		if (strchr(buf,'!')) posexc = true;
		if (j) for(i = j + 1;*i == ' ';i++);
	} else i = at;

	if (pos) if (wninit()) return FAILURE;

	if ((i = synvar(i,buf,&tag)) != CNIL) {
		pCase = kb.curSub->m_cases.curCase;
		if (pCase->m_facts.Find(tag)) {
			pFact = pCase->m_facts.curFact;
			if (!posinc && !posexc) while (!pFact->m_mvlist.IsEmpty()) t = pFact->m_mvlist.RemoveTail();
			lowercase(buf);
			while(replace(buf," ","-"));
			for (p = buf;*p;p++) {
				for (q = p;*q && !strchr(delim,*q);q++);
				if (*q) {
					*q++ = 0;
					done = false;
				} else done = true;
				if (*p) rtrim(p);
				if (*p) {
					if (pos) {
						ok = false;
						if (posn) {
							if (in_wn(p,NOUN)) ok = true;
							else if ((morph = morphstr(p,NOUN)) != NULL) {
								do {
									if (in_wn(morph,NOUN)) { ok = true; break; }
								} while ((morph = morphstr(NULL,NOUN)) != NULL);
							}
						}
						if (!ok && posv) {
							if (in_wn(p,VERB)) ok = true;
							else if ((morph = morphstr(p,VERB)) != NULL) {
								do {
									if (in_wn(morph,VERB)) { ok = true; break; }
								} while ((morph = morphstr(NULL,VERB)) != NULL);
							}
						}
						if (!ok && posa) {
							if (in_wn(p,ADJ)) ok = true;
							else if ((morph = morphstr(p,ADJ)) != NULL) {
								do {
									if (in_wn(morph,ADJ)) { ok = true; break; }
								} while ((morph = morphstr(NULL,ADJ)) != NULL);
							}
						}
						if (!ok && posr) {
							if (in_wn(p,ADV)) ok = true;
							else if ((morph = morphstr(p,ADV)) != NULL) {
								do {
									if (in_wn(morph,ADV)) { ok = true; break; }
								} while ((morph = morphstr(NULL,ADV)) != NULL);
							}
						}
						if (!ok && poss) if (infile(p,fnstop))  ok = true;
						if (!ok && posm) if (infile(p,fnmname)) ok = true;
						if (!ok && posf) if (infile(p,fnfname)) ok = true;
						if (!ok && posb) if (infile(p,fnbadwd)) ok = true;
						if (posx) ok = !ok;
					} else ok = true;
					if (ok) {
						if (posexc) {
							t = p;
							mvcnt = (int)pFact->m_mvlist.GetCount();
							for (m = 0;m < mvcnt;m++) {
								mvpos = pFact->m_mvlist.FindIndex(m);
								if (t == pFact->m_mvlist.GetAt(mvpos)) {
									pFact->m_mvlist.RemoveAt(mvpos);
									break;
								}
							}
						} else if (posinc) {
							t = p;
							mvcnt = (int)pFact->m_mvlist.GetCount();
							blnNew = true;
							for (m = 0;m < mvcnt;m++) {
								mvpos = pFact->m_mvlist.FindIndex(m);
								if (t == pFact->m_mvlist.GetAt(mvpos)) {
									blnNew = false;
									break;
								}
							}
							if (blnNew) pFact->Push(p);
						} else	pFact->Push(p);
					}
				}
				if (done) break;
				for (p = q;*p && isspace(*p);p++); p--;
			}
		}
	}
	delete tag;
	return SUCCESS;
}

int PUvm::utwrite(char *at)
{
	char *i;
	char *pos;
	char *patrn;
	char *line;
	char *fn;
	int iRet;

	i = syntag(at,buf);
	if (!*buf) return FAILURE;
	while (replace(buf,"]",">"));
	while (replace(buf,"[","<"));
	while (replace(buf,"&gt;",">"));
	while (replace(buf,"&lt;","<"));
	patrn = new char[strlen(buf) + 20];
	strcpy(patrn,buf);
	i = syntag(i,buf);
	if (!*buf) { delete patrn; return FAILURE; }
	while (replace(buf,"&gt;",">"));
	while (replace(buf,"&lt;","<"));
	while (replace(buf,"&amp;","&"));
	newstr(&line,buf);
	i = syntag(i,buf);
	if (!*buf) { delete patrn; delete line; return FAILURE; }
	newstr(&fn,buf);

	char *top = new char[MAX_LINE_SIZE];
	char *aiml = new char[MAX_BUF_LEN];
	*top = 0;
	*aiml = 0;

	if (i = strstr(patrn,"<topic>")) {
		strcpy(top,i + 7);
		*i = 0;
		replace(top,"</topic>","");
	} else *top = 0;
	if (i = strstr(patrn,"<that>")) {
		strcpy(aiml,i);
		strcpy(i,"</pattern>\n");
		strcat(patrn,aiml);
		if (!strstr(patrn,"</that>")) strcat(patrn,"</that>");
	} else strcat(patrn,"</pattern>");

	if (*top) sprintf(aiml,"<topic name=\"%s\">\n",top); else *aiml = 0;

	strcat(aiml,"<category>\n<pattern>");
	strcat(aiml,patrn);
	strcat(aiml,"\n");

	for (i = line,pos = buf;*i;i++) {
		if (!strncmp(i,"<random>",8)      || !strncmp(i,"<condition>",11) ||
			!strncmp(i,"</condition>",12) || !strncmp(i,"</random>",9)    ||
			!strncmp(i,"</li>",5)         || !strncmp(i,"</save>",7)      ||
			!strncmp(i,"</think>",8)      || !strncmp(i,"</system>",9)    ||
			!strncmp(i,"</gossip>",9)     || !strncmp(i,"<br/>",5)        ||
			!strncmp(i,"<BR/>",5)         || !strncmp(i,"<usesaved/>",11)) {
			while (*i != '>') *pos++ = *i++;
			*pos++ = *i;
			*pos++ = '\n';
		} else *pos++ = *i;
	}
	*pos = 0;
	if (strlen(buf) == strlen(line)) {
		strcat(aiml,"<template>");
		strcat(aiml,buf);
	} else {
		if (*(pos - 1) != '\n') strcat(buf,"\n");
		strcat(aiml,"<template>\n");
		strcat(aiml,buf);
	}
	strcat(aiml,"</template>\n</category>\n");
	if (*top) strcat(aiml,"</topic>\n");

	FILE *faiml = fopen(fn,"r");
	if (faiml) {
		FILE *fwork = fopen(bot->fnwrk,"w");
		while (fgets(buf,MAX_BUF_LEN,faiml)) {
			if (pos = strstr(buf,"</aiml>")) {
				strcpy(line,pos);
				*pos = 0;
				fprintf(fwork,"%s",buf);
				fprintf(fwork,"%s",aiml);
				fprintf(fwork,"%s",line);
			} else if (pos = strstr(buf,"</alice>")) {
				strcpy(line,pos);
				*pos = 0;
				fprintf(fwork,"%s",buf);
				fprintf(fwork,"%s",aiml);
				fprintf(fwork,"%s",line);
			} else fprintf(fwork,"%s",buf);
		}
		fclose(faiml);
		fclose(fwork);

		TCHAR szFileName[MAX_PATH_LEN];
		wsprintf(szFileName,TEXT("%hs"),fn);
		iRet = DeleteFile(szFileName);

		TCHAR szOldFileName[MAX_PATH_LEN];
		wsprintf(szOldFileName,TEXT("%hs"),bot->fnwrk);
		iRet = MoveFile(szOldFileName,szFileName);
	} else {
		FILE *faiml = fopen(fn,"w");
		if (faiml) {
			fprintf(faiml,"<aiml>\n");
			fprintf(faiml,"%s",aiml);
			fprintf(faiml,"</aiml>");
			fclose(faiml);
		}
	}
	delete patrn;
	delete line;
	delete top;
	delete aiml;
	delete fn;

	return SUCCESS;
}

//
// Socket support functions
//

// SZ: Let user know there's a problem with socket comms. in urlclient() and utcycclient()
void PUvm::sockError(char *str)
{
	char strMsg[32];			// A lousy 32 char. buffer!
	sprintf(strMsg,"%s failed!", str);	// Complete the message.

	// Tell the user :-)
	MessageBox(NULL, strMsg, "SOCKET ERROR", MB_OK);

	WSACleanup();				// Wassup???

}

//
// file support functions
//

bool PUvm::infile(char *s,char *fn)
{
	char wrk[MAX_PHRASE_LEN];
	bool fnd = false;

	FILE *f = fopen(fn,"r");
	if (f == NULL) return fnd;

	while (!feof(f)) {
		fgets(wrk,MAX_PHRASE_LEN,f);
		if (!strncmp(wrk,"<eof>",5)) break;
		rtrim(wrk);
		if (!stricmp(wrk,s)) { fnd = true; break; }
	}
	fclose(f);

	return fnd;
}

bool PUvm::getslot(char *tag,char *val)
{
	if (kb.getValue(tag,val)) return true;
	return fixslot(tag,val);
}

bool PUvm::savslot(char *t,char *v)
{
	return kb.putValue(t,v);
}

bool PUvm::gblslot(char *t,char *v)
{
	return kb.putGlobal(t,v);
}

bool PUvm::shrslot(char *t,char *v)
{
	return kb.putShare(t,v);
}

bool PUvm::remslot(char *t)
{
	return kb.delValue(t);
}

bool PUvm::fixslot(char *t,char *v)
{
	if (!strcmp(t,"SCRIPT"))     { sprintf(v,"%s",pgm.curScript->m_name); return true; }
	if (!strcmp(t,"FRAME"))      { strcpy(v,curfrm);             return true; }
	if (!strcmp(t,"SITUATION"))  { strcpy(v,cursit);             return true; }
	if (!strcmp(t,"LAST"))       { strcpy(v,lastinput);          return true; }
	if (!strcmp(t,"REPLY"))      { strcpy(v,lastreply);          return true; }
	if (!strcmp(t,"DBSIZE"))     { strcpy(v,bot->getsize());     return true; }
	if (!strcmp(t,"THAT"))       { strcpy(v,(bot->thats)[0]);      return true; }
	if (!strcmp(t,"THAT1"))      { strcpy(v,(bot->thats)[0]);      return true; }
	if (!strcmp(t,"THAT2"))      { strcpy(v,(bot->thats)[1]);      return true; }
	if (!strcmp(t,"THAT3"))      { strcpy(v,(bot->thats)[2]);      return true; }
	if (!strcmp(t,"THAT4"))      { strcpy(v,(bot->thats)[3]);      return true; }
	if (!strcmp(t,"THAT5"))      { strcpy(v,(bot->thats)[4]);      return true; }
	if (!strcmp(t,"INPUT"))      { strcpy(v,(bot->inputs)[0]);     return true; }
	if (!strcmp(t,"INPUT1"))     { strcpy(v,(bot->inputs)[0]);     return true; }
	if (!strcmp(t,"INPUT2"))     { strcpy(v,(bot->inputs)[1]);     return true; }
	if (!strcmp(t,"INPUT3"))     { strcpy(v,(bot->inputs)[2]);     return true; }
	if (!strcmp(t,"INPUT4"))     { strcpy(v,(bot->inputs)[3]);     return true; }
	if (!strcmp(t,"INPUT5"))     { strcpy(v,(bot->inputs)[4]);     return true; }
	if (!strcmp(t,"STAR"))       { bot->contexts.GetStar("input",0,v); return true; }
	if (!strcmp(t,"STAR1"))      { bot->contexts.GetStar("input",0,v); return true; }
	if (!strcmp(t,"STAR2"))      { bot->contexts.GetStar("input",1,v); return true; }
	if (!strcmp(t,"STAR3"))      { bot->contexts.GetStar("input",2,v); return true; }
	if (!strcmp(t,"STAR4"))      { bot->contexts.GetStar("input",3,v); return true; }
	if (!strcmp(t,"STAR5"))      { bot->contexts.GetStar("input",4,v); return true; }
	if (!strcmp(t,"STAR6"))      { bot->contexts.GetStar("input",5,v); return true; }
	if (!strcmp(t,"STAR7"))      { bot->contexts.GetStar("input",6,v); return true; }
	if (!strcmp(t,"STAR8"))      { bot->contexts.GetStar("input",7,v); return true; }
	if (!strcmp(t,"STAR9"))      { bot->contexts.GetStar("input",8,v); return true; }
	if (!strcmp(t,"TOPIC"))      { strcpy(v,bot->topic);         return true; }
	if (!strcmp(t,"THATSTAR"))   { bot->contexts.GetStar("that",0,v);  return true; }
	if (!strcmp(t,"THATSTAR1"))  { bot->contexts.GetStar("that",0,v);  return true; }
	if (!strcmp(t,"THATSTAR2"))  { bot->contexts.GetStar("that",1,v);  return true; }
	if (!strcmp(t,"THATSTAR3"))  { bot->contexts.GetStar("that",2,v);  return true; }
	if (!strcmp(t,"THATSTAR4"))  { bot->contexts.GetStar("that",3,v);  return true; }
	if (!strcmp(t,"THATSTAR5"))  { bot->contexts.GetStar("that",4,v);  return true; }
	if (!strcmp(t,"TOPICSTAR"))  { bot->contexts.GetStar("topic",0,v); return true; }
	if (!strcmp(t,"TOPICSTAR1")) { bot->contexts.GetStar("topic",0,v); return true; }
	if (!strcmp(t,"TOPICSTAR2")) { bot->contexts.GetStar("topic",1,v); return true; }
	if (!strcmp(t,"TOPICSTAR3")) { bot->contexts.GetStar("topic",2,v); return true; }
	if (!strcmp(t,"TOPICSTAR4")) { bot->contexts.GetStar("topic",3,v); return true; }
	if (!strcmp(t,"TOPICSTAR5")) { bot->contexts.GetStar("topic",4,v); return true; }
	if (!strcmp(t,"CLIENT"))     { strcpy(v,bot->client);          return true; }
	if (!strcmp(t,"CURSOR")) {
		CEdit& edit = host->GetEditCtrl();
		DWORD dwSel = edit.GetSel();
		int atpos = HIWORD(dwSel);
		sprintf(v,"%i",atpos);
		return true;
	}
	return false;
}

void PUvm::Cycsetvar(char *varname,char *value)
{
	char *buf = new char[MAX_BUF_LEN];
	char *retbuf = new char[MAX_BUF_LEN];


	// Retract its value if any
	// probably needs to be a loop to remove any excess
	// and ensure that N and Cyc are Nsync
	Cycloadvar(varname,"");
	getslot(varname,retbuf);
	if (strlen(retbuf) > 0) {
		sprintf(buf,"(fi-unassert '(#$aimlPred #$aimlVar-%s %s) #$AimlContextMt)",varname,retbuf);
		savslot("xcycput",buf);
		//utcycclient(urlcycput);
	}

	// Assert its value
	sprintf(buf,"(fi-assert '(#$aimlPred #$aimlVar-%s \"%s\") #$AimlContextMt)",varname,value);
	savslot("xcycput",buf);
	
	delete buf;
	delete retbuf;
}

void PUvm::Cycloadvar(char *varname,char *defval)
{
	char *buf = new char[MAX_BUF_LEN];
	char *retbuf = new char[MAX_BUF_LEN];
	char *p1;
	char *p2;

	if  (!strcmp(varname,"xcycset") || !strcmp(varname,"xcycget")) return;

	// Find or create the variable as a constant
	sprintf(buf,"(fi-find-or-create \"aimlVar-%s\")",varname);
	savslot("xcycget",buf);

	// Ask about its value
	sprintf(buf,"(fi-ask '(#$aimlPred #$aimlVar-%s ?VAL) #$AimlContextMt)",varname);
	savslot("xcycget",buf);
	// Now get it and install it
	// should be "(((?VAL."MyValue")))"
	if (getslot("CYCdata1",retbuf)) {
		if (p1 = strstr(retbuf,".")) {
			p1++;					// go past the "."
			p2 = strstr(p1,")");	// Find the first close
			*p2 = 0;				// change to end of string
			savslot(varname,p1);	// save to the local var
		}
	}

	delete retbuf;
	delete buf;
}

char *PUvm::idframe(char *f,char *id)
{
  char *i;

  for (i = f + 1;*i && *i != '}';*id++ = *i++);
  *id = 0;
  if (!*i) return (CNIL);
  i++;
  return (i);
}

char *PUvm::getframe(char *frm)
{            
	Script *pScript = pgm.curScript;
	Script *qScript;
	Frame *qFrame;
	Case *pCase = kb.curSub->m_cases.curCase;
	Case *qCase;
	char *i, *j, *k;
	bool flg = false;
	int x;
  
	if (pScript != NULL) {
		if (pScript->m_frames.Find(frm)) {
			qFrame = pScript->m_frames.curFrame;
			j = new char[strlen(frm) + qFrame->m_behavior.GetLength() + 4];
			sprintf(j,"{%s}%s",frm,qFrame->m_behavior);
			return j;
		}
	}

	if (kb.curSub != NULL) {
		qCase = pCase;
		if (qCase != NULL) {
			if (qCase->m_facts.Find("Method")) {
				j = new char[qCase->m_facts.curFact->m_value.GetLength() + 2];
				sprintf(j,"%s",qCase->m_facts.curFact->m_value);
				flg = true;
			}
		}
	}

	if (!flg) {               
		if (kb.curLoc != NULL) {
			qCase = kb.curLoc->m_cases.curCase;
			if (qCase != NULL) {
				if (qCase->m_facts.Find("Method")) {
					j = new char[qCase->m_facts.curFact->m_value.GetLength() + 2];
					sprintf(j,"%s",qCase->m_facts.curFact->m_value);
					flg = true;
				}
			}
		}
	}

	if (!flg) {
		qCase = pCase;
		while (qCase->m_facts.Find("General")) {
			k = new char[qCase->m_facts.curFact->m_value.GetLength() + 2];
			sprintf(k,"%s",qCase->m_facts.curFact->m_value);
			if (kb.curSub->m_cases.Find(k)) {
				delete k;
				qCase = kb.curSub->m_cases.curCase;
				if (qCase->m_facts.Find("Method")) {
					j = new char[qCase->m_facts.curFact->m_value.GetLength() + 2];
					sprintf(j,"%s",qCase->m_facts.curFact->m_value);
					flg = true;
					break;
				}	
			} else { delete k; break; }
		}
		kb.curSub->m_cases.curCase = pCase;
	}

	if (!flg) {
		flg = pgm.GetFirst();
		while (flg) {
			qScript = pgm.curScript;
			if (pScript != qScript) {
				if (qScript->m_frames.Find(frm)) {
					qFrame = qScript->m_frames.curFrame;
					j = new char[strlen(frm) + qFrame->m_behavior.GetLength() + 4];
					sprintf(j,"{%s}%s",frm,qFrame->m_behavior);
					pgm.curScript = pScript;
					return j;
				}
			}
			flg = pgm.GetNext();
		}
		pgm.curScript = pScript;
	}

	if (!flg) return CNIL;

	while (*j && *j != '{') j++;
	if (!*j) { delete j; return CNIL; }
   
	for (i = j;*i;i++) {
		if (*i == '{') {
			for (k = i + 1,x = 0;*k && *k != '}' && *k == frm[x];k++,x++);
			if (*k == '}' && !frm[x]) {
				newstr(&k,i);
				delete j;
				return k;
			}
		} else if (*i == '\"') for (i++;*i && *i != '\"';i++);
	}
	return CNIL;
}

char *PUvm::topframe(char *frm)
{
	Script *pScript = pgm.curScript;
	Frame *pFrame;
	char *i;

	POSITION pos = pScript->m_frames.m_mapStringToFrame.GetStartPosition();
	if (pos != NULL) {
		CString key;
		pScript->m_frames.m_mapStringToFrame.GetNextAssoc(pos,key,pFrame);
		sprintf(frm,"%s",pFrame->m_name);
		i = new char[strlen(frm) + pFrame->m_behavior.GetLength() + 4];
		sprintf(i,"{%s},%s",frm,pFrame->m_behavior);
		return i;
	}
	return CNIL;
}

char *PUvm::nxtframe(char *frm)
{
	Script *pScript = pgm.curScript;
	Frame *pFrame;
	int cnt = (int)pScript->m_frameslist.GetCount();
	int n;
	char *i;
	POSITION pos;

	newstr(&i,frm);
	for (n = 0; n < cnt;n++) {
		pos = pScript->m_frameslist.FindIndex(n);
		if (pos == NULL) continue;
		sprintf(frm,"%s",pScript->m_frameslist.GetAt(pos));
		if (!strcmp(i,frm)) {
			pos = pScript->m_frameslist.FindIndex(n + 1);
			if (pos == NULL) break;
			sprintf(frm,"%s",pScript->m_frameslist.GetAt(pos));
			if (pScript->m_frames.Find(frm)) {
				pFrame = pScript->m_frames.curFrame;
				delete i;
				i = new char[strlen(frm) + pFrame->m_behavior.GetLength() + 4];
				sprintf(i,"{%s}%s",frm,pFrame->m_behavior);
				return i;
			}
			break;
		}
	}
	strcpy(frm,i);
	delete i;
	return CNIL;
}

void PUvm::calc(char *s) {
	char *wrk = kb.calc(s);
	strcpy(buf,wrk);
	delete wrk;
}

// parse functions

char *PUvm::fndtag(char *a,char **t)
{ 
	char *i = buf;
	char *j = a;

	while (*j && isspace(*j)) j++;  
	if (*j == '$') j++;
	if (*j == '%') *i++ = *j++;
	while (*j && (isalnum(*j) || *j == '(' || *j == ' ' || *j == '_' || *j == '~')) {
		if (*j == '(') while (*j && *j != ')') *i++ = *j++;
		if (*j) *i++ = *j++;
	}
	*i = 0;
	for (i--;i > buf && (isspace(*i) || isdigit(*i));i--,j--);
	for (i++;*i && isdigit(*i);i++,j++);
	*i = 0;
	while (*j && isspace(*j)) j++;
	newstr(t,buf);
	return j;
}

char *PUvm::synint(char *at,int *x)
{
	char *i, tmp[32];
	int n;

	for (i = at;*i && isspace(*i);i++);
	for (n = 0;*i && isdigit(*i) && n < 30;tmp[n++] = *i++); tmp[n] = 0;
	if (n) *x = atoi(tmp); else *x = 0;
	return (i);
}

char *PUvm::synlong(char *at,long *x)
{
	char *i, *j;
	char *k = CNIL;
	char tmp[32];
	int n;
	long sgn = 1;

	for (i = at;*i && isspace(*i);i++);
	if (*i == '-') { i++; sgn = -1; }
	for (n = 0;*i && isdigit(*i) && n < 30;tmp[n++] = *i++); tmp[n] = 0;
	if (n) *x = sgn * atol(tmp);
	else {
		if (*i == '$') i++;
		j = fndtag(i,&k);
		if (j == CNIL) *x = 0;
		else {  
			if (getslot(k,buf)) {
				i = j;
				for (n = 0;buf[n];n++) if (buf[n] == '.') buf[n--] = 0;
				*x = atol(buf);
			} else *x = 0;
		}
		delete k;
	}
	return (i);
}

char *PUvm::synnum(char *at,int *x)
{
	char *i, *j;
	char *k = CNIL;
	char tmp[32];
	int n;

	for (i = at;*i && isspace(*i);i++);
	for (n = 0;isdigit(*i) && n < 30;tmp[n++] = *i++); tmp[n] = 0;

	if (n) *x = atoi(tmp);
	else {
		if (*i == '$') i++;
		j = fndtag(i,&k);
		if (j == CNIL) *x = 0;
		else {  
			if (getslot(k,buf)) {
				i = j;
				for (n = 0;buf[n];n++) if (buf[n] == '.') buf[n--] = 0;
				*x = atoi(buf);
			} else *x = 0;
		}
		delete k;
	}
	return (i);
}

char *PUvm::synvar(char *at,char *s,char **tag)
{
	char *i, *j;

	*tag = CNIL;
	for (i = at;*i && isspace(*i);i++);
	if (*i == '$' || *i == '%') {
		if (*i == '$') i++;
		j = fndtag(i,tag);
		if (getslot(*tag,s)) return (j);
	} else {
		newstr(tag,"");
		strcpy(s,"");
	}
	return CNIL;
}

char *PUvm::syntag(char *at,char *s)
{
	char *i, *j;
	char *k = CNIL;

	for (i = at;*i && isspace(*i);i++);
	if (*i == '$' || *i == '%') {
		if (*i == '$') i++;
		j = fndtag(i,&k);
		if (!getslot(k,s)) { delete k; return (i); } /* not found tag error to deal with here...*/
		delete k;
		return (j);
	} 
	if (*i == '\"') {
		j = s;
		for (i++;*i && *i != '\"';i++) *j++ = *i;
		*j = 0;
		if (*i == '\"') i++;
		return i;
	}
	for (j = s;*i && *i != ',' && *i != ';' && *i != '{' && *i != '?';i++)
	if (j != s || *i != '\"') *j++ = *i;
	*j-- = 0;
	while (j > s && isspace(*j)) *j-- = 0;
	if (j > s && *j == '\"') *j = 0;
	return (i);
}

char *PUvm::synutag(char *at,char *s)
{
	char *i, *j;
	char *k = CNIL;

	for (i = at;*i && isspace(*i);i++);
	if (*i == '$' || *i == '%') {
		if (*i == '$') i++;
		j = fndtag(i,&k);
		if (!getslot(k,s)) { delete k; return (i); } /* not found tag error to deal with here...*/
		uppercase(s);
		delete k;
		return (j);
	}
	for (k = s;*i && *i != ',' && *i != ';' && *i != '{' && *i != '?';i++)
	if (k != s || *i != '\"') *k++ = *i;
	*k-- = 0;
	while (k > s && isspace(*k)) *k-- = 0;
	if (k > s && *k == '\"') *k = 0;
	uppercase(s);
	return (i);
}

// formatting the output in host edit control

int PUvm::utclear()
{
	CEdit& edit = host->GetEditCtrl();
	edit.SetSel(0,-1);
	edit.ReplaceSel(_T("")); 
	host->UpdateWindow();
	Sleep(10);
	return SUCCESS;
}

int PUvm::utcopy()
{
	CEdit& edit = host->GetEditCtrl();
	DWORD dwSel = edit.GetSel();
	int atpos = HIWORD(dwSel);
	int topos = LOWORD(dwSel);
	if (atpos == topos && pinpos >= 0) { edit.SetSel(pinpos,atpos); pinpos = -1; }
	edit.Copy();
	host->UpdateWindow();
	Sleep(10);
	return SUCCESS;
}

int PUvm::utcut()
{
	CEdit& edit = host->GetEditCtrl();
	DWORD dwSel = edit.GetSel();
	int atpos = HIWORD(dwSel);
	int topos = LOWORD(dwSel);
	if (atpos == topos && pinpos >= 0) { edit.SetSel(pinpos,atpos); pinpos = -1; }
	edit.Cut();
	host->UpdateWindow();
	Sleep(10);
	return SUCCESS;
}

int PUvm::uteop()
{
	CEdit& edit = host->GetEditCtrl();
	int n = edit.GetWindowTextLength();
	edit.SetSel(n,n);
	host->UpdateWindow();
	Sleep(10);
	return SUCCESS;
}

int PUvm::uterase()
{
	CEdit& edit = host->GetEditCtrl();
	DWORD dwSel = edit.GetSel();
	int atpos = HIWORD(dwSel);
	int topos = LOWORD(dwSel);
	if (atpos == topos && pinpos >= 0) { edit.SetSel(pinpos,atpos); pinpos = -1; }
	edit.ReplaceSel(TEXT(""));
	host->UpdateWindow();
	Sleep(10);
	return SUCCESS;
}

int PUvm::utfind(char *at)
{
	CString str;
	char *i;
	int m = FAILURE;

	i = syntag(at,buf);
	if (*buf) {
		str = buf;
		m = host->FindText(str,true,true);
	}
	return (m);
}

int PUvm::uthome()
{
	CEdit& edit = host->GetEditCtrl();
	edit.SetSel(0,0);
	host->UpdateWindow();
	Sleep(10);
	return SUCCESS;
}

int PUvm::utopen(char *at)
{
	CString fn;
	char *i;
	int m = false;
	CDocument *doc = host->GetDocument();

	i = syntag(at,buf);
	if (*buf) {
		fn = buf;
		m = doc->OnOpenDocument(fn);
		if (m) doc->SetPathName(fn);
	}
	return (!m);
}

int PUvm::utpaste()
{
	CEdit& edit = host->GetEditCtrl();
	edit.Paste();
	host->UpdateWindow();
	Sleep(10);
	return SUCCESS;
}

int PUvm::utpin(char *at)
{
	CEdit& edit = host->GetEditCtrl();
	DWORD dwSel = edit.GetSel();
	char *i;

	for (i = at;*i && isspace(*i);i++);
	if (*i == 'n') pinpos = -1; else pinpos = HIWORD(dwSel);
	return SUCCESS;
}

int PUvm::utpos(char *at)
{
	int n;
	CEdit& edit = host->GetEditCtrl();
	char *i;

	for (i = at;*i && isspace(*i);i++);
	if (*i) {
		i = synnum(i,&n);
		edit.SetSel(n,n);
	} else if (pinpos >= 0) edit.SetSel(pinpos,pinpos);
	host->UpdateWindow();
	Sleep(10);
	return SUCCESS;
}

int PUvm::utread(char *at)
{
	CString str;
	char *i;
	char *fnd;

	i = syntag(at,buf);
	if (*buf) {
		newstr(&fnd,buf);
		host->GetSelectedText(str);
		if (str.GetLength() + 2 < MAX_SIZE) {
			sprintf(buf,"%s",str);
			savslot(fnd,buf);
		}
		delete fnd;
	}
	return SUCCESS;
}

int PUvm::utsaveas(char *at)
{
	CString str;
	char *i;
	int m = FAILURE;
	CDocument *doc = host->GetDocument();

	i = syntag(at,buf);
	if (*buf) {
		str = buf;
		m = doc->DoSave(str);
	} else if (dflg) m = doc->DoFileSave();
	return (m);
}

int PUvm::utsel(char *at)
{
	int n;
	CEdit& edit = host->GetEditCtrl();
	DWORD dwSel = edit.GetSel();
	int atpos = HIWORD(dwSel);
	char *i;

	for (i = at;*i && isspace(*i);i++);
	if (*i) {
		i = synnum(i,&n);
		while (*i && isspace(*i)) i++;
		if (*i) i = synnum(i,&atpos);
		edit.SetSel(n,atpos);
	} else if (pinpos >= 0) edit.SetSel(pinpos,atpos);
	host->UpdateWindow();
	Sleep(10);
	return SUCCESS;
}

//
// AIML creation stuff
//

int PUvm::classifyaiml(char *fn)
{
	char *line = new char[MAX_BUF_LEN];
	char *text = new char[MAX_BUF_LEN];
	char *best = new char[MAX_INSTRING];
	bool fnd;
	bool alpha;
	char c;
	char chr;
	char *pos;

	FILE *ifile = fopen(fn,"r");
	if (ifile == NULL) return FAILURE;

	FILE *aiml = fopen(bot->fnpat,"r");

	while (fgets(buf,MAX_BUF_LEN,ifile)) {
		sprintf(line," %s ",buf);
		bot->substitute(line,bot->fnsub,true);
		strcpy(text,line + 1);
		cleaner(text);

		c = *text;
		if (islower(c)) c += 'A' - 'a';
		alpha = ((c >= 'A') && (c <= 'Z'));
		*best = 0;

		if (alpha) fsetpos(aiml,&(bot->chunk[c - 'A'])); 
		else fsetpos(aiml,&(bot->chunk[26]));
		fnd = false;
		while (!feof(aiml)) {
			fgets(line,MAX_LINE_SIZE,aiml);
			if (!(pos = strstr(line,"@"))) continue;
			if (alpha) {
				chr = *line;
				if (islower(chr)) chr += 'A' - 'a';
				if (chr != c) {
					if (fnd) break;
					alpha = false;
					fsetpos(aiml,&(bot->chunk[26]));
					continue;
				}
			}
			*pos = 0;
			if (*line == '*') continue;
			strcpy(buf,line);

			while (strstr(line,"<name/>")) replace(line,"<name/>",bot->getbot("name"));
			while (strstr(line,"<bot/>")) replace(line,"<bot/>",bot->getbot("name"));

// replace other bot variables in pattern?

			if (!bot->match(text,line)) continue;

			if (!stricmp(text,line) || strstr(line,"_")) {
				strcpy(best,buf);
				if (strstr(line,"_")) fnd = true;
				break;
			}

			if (fnd) if (patrns(line,best)) continue;
			strcpy(best,buf);
			fnd = true;
		}

		if (fnd) {
			pppost("Pattern: ",best,"\r\n");
			pppost("Input:   ",text,"\r\n\r\n");
		}

		while (fgets(buf,MAX_BUF_LEN,ifile)) {
			rtrim(buf);
			if (!*buf) break;
		}
		if (feof(ifile)) break;
	}
	fclose(aiml);
	fclose(ifile);
	delete line;
	delete text;
	delete best;
	return SUCCESS;
}

int PUvm::convertaiml(char *fn)
{
	char *line = new char[MAX_BUF_LEN];

	FILE *ifile = fopen(fn,"r");
	if (ifile == NULL) return FAILURE;

	while (fgets(buf,MAX_BUF_LEN,ifile)) {
		sprintf(line," %s ",buf);
		bot->substitute(line,bot->fnsub,true);
		strcpy(buf,line + 1);
		cleaner(buf);
		uppercase(buf);
		pppost("<category>\r\n<pattern>",buf,"</pattern>\r\n<template>\r\n");
		while (fgets(buf,MAX_BUF_LEN,ifile)) {
			rtrim(buf);
			if (*buf) ppost(buf,"\r\n"); else break;
		}
		post("</template>\r\n</category>\r\n\r\n");
		if (feof(ifile)) break;
	}
	fclose(ifile);
	delete line;
	return SUCCESS;
}

int PUvm::converttargets(char *fn)
{
	char *line = new char[MAX_BUF_LEN];
	int iRet;
	bool blnTarget = bot->logtarget;

	FILE *ifile = fopen(fn,"r");
	if (ifile == NULL) return FAILURE;

// save bot history, i.e., inputs and thats and topic?

	bot->logtarget = true;

	while (fgets(line,MAX_BUF_LEN,ifile)) {
		rtrim(line);
		*buf = 0;
		iRet = bot->getAnswer(line,buf);
		while (fgets(buf,MAX_BUF_LEN,ifile)) {
			rtrim(buf);
			if (!*buf) break;
		}
		if (feof(ifile)) break;
	}
	fclose(ifile);

	bot->logtarget = blnTarget;

	delete line;
	return SUCCESS;
}

// the host provides the input and output...

void PUvm::post(char *s)
{
	CString display;
	display.Format(TEXT("%hs"),s);
	CEdit& edit = host->GetEditCtrl();
	edit.ReplaceSel(display); 
	host->UpdateWindow();
	Sleep(10);	
}

void PUvm::ppost(char *s,char *t)
{
	CString display;
	display.Format(TEXT("%hs%hs"),s,t);
	CEdit& edit = host->GetEditCtrl();
	edit.ReplaceSel(display); 
	host->UpdateWindow();
	Sleep(10);	
}

void PUvm::pppost(char *r,char *s,char *t)
{
	CString display;
	display.Format(TEXT("%hs%hs%hs"),r,s,t);
	CEdit& edit = host->GetEditCtrl();
	edit.ReplaceSel(display); 
	host->UpdateWindow();
	Sleep(10);	
}

bool PUvm::onCmd(CString str)
{
	cmdCase = kb.curLoc->m_cases.curCase;

	if (!blnInput) {
		sprintf(tmpid,"~~cmd%d~~",tmpcnt++);
		kb.curLoc->m_cases.Add(tmpid);
	}

	if (jfrm != CNIL) delete jfrm;
	newstr(&jfrm,"Command Line");

	if (cmdlist != CNIL) delete cmdlist;
	cmdlist = new char[str.GetLength() + 2];
	sprintf(cmdlist,"%s",str);

	int xret = docmds(CNIL);
	
	if (!blnInput) {
		kb.curLoc->m_cases.Remove(tmpid);
		if (cmdCase != NULL) kb.curLoc->m_cases.curCase = cmdCase;
		cmdCase = NULL;
	}
	return qflg;
}

bool PUvm::resume(char *str)
{
	if (stkret()) {
		delete lastinput;
		newstr(&lastinput,str);
		if (getslot("%~~inputslot~~",buf)) savslot(buf,lastinput);
		blnInput = false;
		docmds(jflg);
		if (!blnInput) {
			kb.curLoc->m_cases.Remove(tmpid);
			if (cmdCase != NULL) kb.curLoc->m_cases.curCase = cmdCase;
			cmdCase = NULL;
		}
		return qflg;
	}
	post("\r\n* Error: last set of commands after input lost on stack");
	if (blnInput) {
		SetCaretBlinkTime(200);
		CEdit& edit = host->GetEditCtrl();
		dwAtSel = edit.GetSel();
	}
	blnInput = false;
	return false;
}


/* Core.cpp -  Implimentation of the core bot component.
   Programming/Implimentation: Arron Boucher.
   Debugging: Sean Barnes.
   08/07/2004
*/

#include "stdafx.h"
#include "core.h"
#include <process.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Core::Core(CString currentPath)
{
	SetPath(currentPath.GetBuffer());
	int i;
	m_initProp = FALSE;

	gm = NULL;

	best = new char[MAX_INSTRING];
	bestthat = new char[MAX_VALUE_LEN];
	besttopic = new char[MAX_VALUE_LEN];

	wrk = new char[MAX_BUF_LEN];
	varvalue = new char[MAX_VALUE_LEN];
	topic = new char[MAX_VALUE_LEN];
	client = new char[MAX_PHRASE_LEN];

	for (i = 0;i < 10;i++) {
		saves[i] = new char[MAX_INSTRING];
		sage[i] = 1000;
	}

	for (i = 0;i < 5;i++) {
		thats[i] = new char[MAX_LOADSTRING]; *thats[i] = 0;
		inputs[i] = new char[MAX_INSTRING]; *inputs[i] = 0;
	}

	for (i = 0;i < MAX_CACHE;i++) {
		termcache_key[i] = new char[MAX_STAR_LEN]; *termcache_key[i]=0;
		termcache_value[i] = new char[MAX_STAR_LEN]; *termcache_value[i]=0;
	}

	exitplease = false;

	newsec = false;
	second = new char[MAX_LOADSTRING];  *second = 0;
	clrscrn = false;
	logtarget = false;
	usegraphmaster = false;
	useconnect = false;
	stopbot = false;
	autoname = false;
	nosave = false;

	autocyc = false;
	usetermcache = false;
	intAgent = 1;

	happyness = 0;
	helpfullness = 0;
	humor = 0;
	affection = 0;
	trust = 0;
	interest = 0;

	setcompress(true);

	strcpy(fnini,fullpath("bot\\init.txt"));
	strcpy(fnchr,fullpath("bot\\character.txt"));
	strcpy(fnmds,fullpath("bot\\moods.txt"));
	strcpy(fnbot,fullpath("bot\\vars.txt"));
	strcpy(fnctx,fullpath("bot\\contexts.txt"));
	strcpy(fndat,fullpath("data\\"));
	strcpy(fnwrk,fullpath("data\\temp.txt"));
	strcpy(fnsrt,fullpath("db\\unsorted.txt"));
	strcpy(fnidx,fullpath("db\\index.txt"));
	strcpy(fnpat,fullpath("db\\patterns.txt"));
	strcpy(fntmp,fullpath("db\\templates.txt"));
	strcpy(fninf,fullpath("logs\\information.txt"));
	strcpy(fnerr,fullpath("logs\\errors.txt"));
	strcpy(fntar,fullpath("logs\\targets.txt"));
	strcpy(fngos,fullpath("logs\\gossip.txt"));
	strcpy(fnsub,fullpath("sub\\substitute.txt"));
	strcpy(fnper,fullpath("sub\\person.txt"));
	strcpy(fnper2,fullpath("sub\\person2.txt"));
	strcpy(fnperf,fullpath("sub\\personf.txt"));
	strcpy(fngndr,fullpath("sub\\gender.txt"));
	strcpy(fnaiml,fullpath("logs\\aiml.txt"));
	strcpy(fnmore,fullpath("logs\\moreaiml.txt"));
	strcpy(fnquik,fullpath("logs\\quickaiml.txt"));


	FILE *fdirs = fopen(fullpath("options.ini"),"r");
	if (fdirs) {
		while (!feof(fdirs)) {
			fgets(wrk,MAX_BUF_LEN,fdirs);
			rtrim(wrk);
			if (!strncmp(wrk,(char*)"character=",10))   strcpy(fnchr,wrk + 10);
			if (!strncmp(wrk,(char*)"moods=",6))        strcpy(fnmds,wrk + 6);
			if (!strncmp(wrk,(char*)"bot=",4))          strcpy(fnbot,wrk + 4);
			if (!strncmp(wrk,(char*)"contexts=",9))     strcpy(fnctx,wrk + 9);
			if (!strncmp(wrk,(char*)"data=",5))         strcpy(fndat,wrk + 5);
			if (!strncmp(wrk,(char*)"temp=",5))         strcpy(fnwrk,wrk + 5);
			if (!strncmp(wrk,(char*)"index=",6))        strcpy(fnidx,wrk + 6);
			if (!strncmp(wrk,(char*)"patterns=",10))    strcpy(fnpat,wrk + 10);
			if (!strncmp(wrk,(char*)"unsorted=",9))     strcpy(fnsrt,wrk + 9);
			if (!strncmp(wrk,(char*)"templates=",11))   strcpy(fntmp,wrk + 11);
			if (!strncmp(wrk,(char*)"info=",5))         strcpy(fninf,wrk + 5);
			if (!strncmp(wrk,(char*)"errors=",7))       strcpy(fnwrk,wrk + 7);
			if (!strncmp(wrk,(char*)"targets=",8))      strcpy(fntar,wrk + 8);
			if (!strncmp(wrk,(char*)"gossip=",7))       strcpy(fngos,wrk + 7);
			if (!strncmp(wrk,(char*)"subs=",5))         strcpy(fnsub,wrk + 5);
			if (!strncmp(wrk,(char*)"person=",7))       strcpy(fnper,wrk + 7);
			if (!strncmp(wrk,(char*)"person2=",8))      strcpy(fnper2,wrk + 8);
			if (!strncmp(wrk,(char*)"personf=",8))      strcpy(fnperf,wrk + 8);
			if (!strncmp(wrk,(char*)"gender=",7))       strcpy(fngndr,wrk + 7);
			if (!strncmp(wrk,(char*)"dumpaiml=",9))     strcpy(fnaiml,wrk + 9);
			if (!strncmp(wrk,(char*)"moreaiml=",9))     strcpy(fnmore,wrk + 9);
			if (!strncmp(wrk,(char*)"quickaiml=",10))   strcpy(fnquik,wrk + 10);
			if (!strncmp(wrk,(char*)"useconnect=",11))  useconnect = !strnicmp(wrk + 11,(char*)"true",4);
			if (!strncmp(wrk,(char*)"usegraphmaster=",15))  usegraphmaster = !strnicmp(wrk + 15,(char*)"true",4);
			if (!strncmp(wrk,(char*)"autoname=",9))     autoname = !strnicmp(wrk + 9,(char*)"true",4);
			if (!strncmp(wrk,(char*)"compress=",9))     setcompress(!strnicmp(wrk + 9,(char*)"true",4));

			if (!strncmp(wrk,(char*)"autocyc=",9))     autocyc = !strnicmp(wrk + 8,(char*)"true",4);
			if (!strncmp(wrk,(char*)"usetermcache=",13))  usetermcache = !strnicmp(wrk + 13,(char*)"true",4);
			
		}
		fclose(fdirs);
	}
	else
	{
	  m_lastError = fullpath("options.ini");
	  m_lastError = "Directory not found" + m_lastError;
	  //AfxMessageBox(m_lastError);
      return;
	}


	loadchunks();
	loadcontexts();
	if(usegraphmaster)
	   loadgm();

	m_initProp = TRUE;
}

Core::~Core()
{
	int i;

	delete [] best;
	delete [] bestthat;
	delete [] besttopic;
	delete [] wrk;
	delete [] varvalue;
	delete [] topic;
	delete [] client;
	for (i = 0;i < 5;i++) {
		delete [] thats[i];
		delete [] inputs[i];
	}
	for (i = 0;i < 10;i++) delete [] saves[i];
	delete [] second;

	for (i = 0;i < MAX_CACHE;i++) {
		delete [] termcache_key[i];
		delete [] termcache_value[i];
	}

	for (i = 0 ; i < MAX_CONTEXTS;i++) if (m_contexts[i] != NULL) delete [] m_contexts[i];


	if(gm)
	  delete gm;
	  gm = 0;
}

void Core::loadchunks()
{
	int i;
	char *buf = new char[MAX_PHRASE_LEN];

	FILE *indx = fopen(fnidx,"r");
	if (indx) {
		for (i = 0;i < 27;i++) {
			fgets(buf,MAX_PHRASE_LEN,indx);
			chunk[i] = (fpos_t)atoi(buf);
		}
		fclose(indx);
		dbfnd = true;
	} else  dbfnd = false;

	delete [] buf;
}

void Core::loadcontexts()
{
	int i;
	char *buf = new char[MAX_PHRASE_LEN];

	for (i = 0;i < MAX_CONTEXTS;i++) m_contexts[i] = NULL;
	FILE *ctx = fopen(fnctx,"r");
	if (ctx) {
		i = 0;
		while (!feof(ctx)) {
			fgets(buf,MAX_PHRASE_LEN,ctx);
			rtrim(buf);
			m_contexts[i] = new char[strlen(buf) + 2];
			strcpy(m_contexts[i],buf);
			if (++i >= MAX_CONTEXTS) break;
		}
		fclose(ctx);
	} else  {
		m_contexts[0] = new char[6];
		strcpy(m_contexts[0],"that");
		m_contexts[1] = new char[7];
		strcpy(m_contexts[1],"topic");
	}

	delete [] buf;
}

void Core::initialize()
{

	if (fexists(fnini)) {
		FILE *init = fopen(fnini,"r");
		fgets(client,MAX_PATH_LEN,init);
		fclose(init);
	} else strcpy(client,"user");

 	rtrim(client);
	pu->utmake(client);
	pu->utload(client);
	setname(client);

	strcpy(thats[0],getvar("that",""));
	strcpy(topic,getvar("topic",""));
	randomize();
}

// bot variables

char *Core::getsize()
{
	char *buf = new char[MAX_LINE_SIZE];
	int count = 0;
	FILE *fpatt = fopen(fnpat,"r");
	while (!feof(fpatt)) {
		fgets(buf,MAX_LINE_SIZE,fpatt);
		if (strchr(buf,'@')) count++;
	}
	fclose(fpatt);
	delete buf;
	sprintf(varvalue,"%d",count);
	return varvalue;
}

char *Core::getbot(char *varname)
{
	char *at;

	if (strlen(varname) > MAX_TAG_LEN) return varname; 

	*varvalue = 0;
	char *tag = new char[MAX_VALUE_LEN];
	FILE *facts = fopen(fnbot,"r");
	while (!feof(facts)) {
		fgets(tag,MAX_VALUE_LEN,facts);
		if (!strncmp(tag,"<eof>",5)) break;
		if (at = strstr(tag,"=")) {
			*at++ = 0;
			if (!strcmp(tag,varname)) {
				strcpy(varvalue,at);
				if (at = strstr(varvalue,";")) *at = 0;
				break; 
			}
		}
	}
	fclose(facts);
	delete tag;
	return varvalue;
}

// simple persistent storage

char *Core::getvar(char *varname,char *defval)
{
	*varvalue = 0;

    if (autocyc && strcmp(varname,"xcycset") && strcmp(varname,"xcycget")) pu->Cycloadvar(varname,defval);

	if (!pu->getslot(varname,varvalue)) {
		if (defval != NULL) if (strlen(defval) <= MAX_VALUE_LEN) {
			setvar(varname,defval);
			strcpy(varvalue,defval);
		}
	}
	return varvalue;
}

void Core::setvar(char *varname,char *value)
{
	char *i, *j, *k;

	newstr(&k,value);
	for (i = j = k;*i;i++) if (*i >= ' ') *j++ = *i; *j = 0;
	pu->savslot(varname,k);
	delete k;
	if (!strcmp(varname,"topic")) strcpy(topic,value);
	if (autoname) if (!strcmp(varname,"name")) setname(value);

	// optionally if autocyc is set, tell opencyc here
    if (autocyc && strcmp(varname,"xcycset") && strcmp(varname,"xcycget")) pu->Cycsetvar(varname,value);
} 

void Core::setname(char *name)
{
	cleanspace(name);
	if (!*name) return;

	if (strcmp(client,name)) {
		setvar("topic",topic);
		setvar("that",thats[0]);
	}

	strcpy(client,name);
	pu->utmake(client);
	if (pu->utload(client)) pu->savslot("name",client);
}

// internal system stuff

void Core::execute(char *command, char *reply)
{
	bool found;

	*reply = 0;
	if (!strcmp(command,"exit no save"))    { nosave = exitplease = true; return; }
	if (!strcmp(command,"exit"))            { exitplease = true;      return; }
	if (!strcmp(command,"stop"))            { stopbot = true;         return; }
 	if (!strcmp(command,"clear screen"))    { clrscrn = true;         return; }
	if (!strcmp(command,"targetting off"))  { logtarget = false;      return; }
	if (!strcmp(command,"targetting on"))   { logtarget = true;       return; }
	if (!strcmp(command,"naming on"))       { autoname = true;        return; }
	if (!strcmp(command,"naming off"))      { autoname = false;       return; }
	if (!strncmp(command,"shell ",6))       { system(command + 6);    return; }
	if (!strncmp(command,"script ",7))      { pu->onCmd(command + 7); return; }
	if (!strncmp(command,"store ",6)) {
		FILE *info = fopen(fninf,"a");
		char *subj = new char[MAX_BUF_LEN];
		strcpy(subj,command + 6);
		fprintf(info,subj);
		fprintf(info,"\n");
		delete subj;
		fclose(info);
		return;
	}
	if (!strncmp(command,"get ",4))	{
		FILE *info = fopen(fninf,"r");
		if (info) {
			char *subj = new char[MAX_BUF_LEN];
			char *temp = new char[MAX_BUF_LEN];
			strcpy(subj,command + 4);
			found = false;
			while (!feof(info)) {
				if (fgets(temp,MAX_BUF_LEN,info) > 0) {
					if (strstr(temp,subj)) {
						if (!found) {
							strcat(reply,"You said about ");
							strcat(reply,command + 4);
							strcat(reply,":\r\n");
						}
						found = true;
						strcat(reply,temp);
					}
				}
			}
 			if (!found) strcat(reply,"I can't find any info about that.");
			delete subj;
			delete temp;
			fclose(info);
		} else strcat(reply,"I can't find any info about that.");
		return;
	}
}

// internal mood stuff

void Core::updatemood(char *text)
{
	int i;
	char *tag = new char[MAX_TAG_LEN];
	char *buf = new char[MAX_PHRASE_LEN];
	float plus[6];
	float mult[6];

	FILE *mood = fopen(fnchr,"r");
	if (mood) {
		while (!feof(mood)) {
			fgets(tag,MAX_TAG_LEN,mood);
			*(strstr(tag,">") + 1) = 0;
			if (!strcmp(tag,"<eof>")) break;
			for (i = 0;i < 6;i++) {
				fgets(buf,MAX_PHRASE_LEN,mood);
				plus[i] = (float)atof(strstr(buf,"+") + 2);
				*strstr(buf,"+") = 0;
				mult[i] = (float)atof(strstr(buf,"*") + 1);
			}
			while (strstr(text,tag)) {
				happyness	 = mult[0] * happyness	  + plus[0];
				helpfullness = mult[1] * helpfullness + plus[1];
				humor		 = mult[2] * humor		  + plus[2];
				affection	 = mult[3] * affection	  + plus[3];
				trust		 = mult[4] * trust		  + plus[4];
				interest	 = mult[5] * interest	  + plus[5];
				replace(text,tag,"");
			}
		}
		fclose(mood);

		happyness    = (happyness > 0.5f)?0.5f:((happyness < -0.5f)?-0.5f:happyness);
		helpfullness = (helpfullness > 0.5f)?0.5f:((helpfullness < -0.5f)?-0.5f:helpfullness);
		humor        = (humor > 0.5f)?0.5f:((humor < -0.5f)?-0.5f:humor);
		affection    = (affection > 0.5f)?0.5f:((affection < -0.5f)?-0.5f:affection);
		trust        = (trust > 0.5f)?0.5f:((trust < -0.5f)?-0.5f:trust);
		interest     = (interest > 0.5f)?0.5f:((interest < -0.5f)?-0.5f:interest);
	}
	delete tag;
	delete buf;
}

int Core::calcmood(char *text)
{
	char *tag = new char[MAX_TAG_LEN];
	char *pos;
	char *nextli = strstr(text,"</li>");
	int tagnr = 0;
	int retval = 0;

	if (pos = strstr(text,"<sec>")) if (pos < nextli) nextli = pos;

	FILE *fmoods = fopen(fnmds,"r");
	if (fmoods) {
		while (!feof(fmoods)) {
			fgets(tag,MAX_TAG_LEN,fmoods);
			*(strstr(tag,">") + 1) = 0;
			if (!strcmp(tag,"<eof>")) break;
			if (pos = strstr(text,tag)) if (pos < nextli) { retval = tagnr; break; }
			if (!strcmp(tag,"<serious/>")) retval = tagnr;
			tagnr++;
		}
		fclose(fmoods);
	}

    delete tag;
	return retval;
}

// logging

void Core::logerror(char *tag,char *text)
{
	char *buf = new char[MAX_TAG_LEN];

	FILE *temp = fopen(fnerr,"a");
	fprintf(temp,"%s %s=%s\r\n",getdts(buf),tag,text);
	fclose(temp);
	delete buf;
}

void Core::savetarget(char *ti_input,char *ti_that,char *ti_topic,char *tm_pattern,char *tm_that,char *tm_topic,char *tm_template,char *reply)
{
	if (!*tm_that)  strcpy(tm_that, "*");
	if (!*tm_topic) strcpy(tm_topic,"*");

	FILE *temp = fopen(fntar,"a");
	fprintf(temp,"<target>\n");
	fprintf(temp,"  <input>\n");
	fprintf(temp,"    <text>%s</text>\n",ti_input);
	fprintf(temp,"    <that>%s</that>\n",ti_that);
	fprintf(temp,"    <topic>%s</topic>\n",ti_topic);
	fprintf(temp,"  </input>\n");
	fprintf(temp,"  <match>\n");
	fprintf(temp,"    <pattern>%s</pattern>\n",tm_pattern);
	fprintf(temp,"    <that>%s</that>\n",tm_that);
	fprintf(temp,"    <topic>%s</topic>\n",tm_topic);
	fprintf(temp,"    <template>%s</template>\n",tm_template);
	fprintf(temp,"  </match>\n");
	fprintf(temp,"  <reply>%s</reply>\n",reply);
	fprintf(temp,"</target>\n");
	fclose(temp);
}

void Core::savegossip(char *text)
{
	char *buf = new char[MAX_TAG_LEN];

	FILE *temp = fopen(fngos,"a");
	fprintf(temp,"%s(%s):%s\n",getdts(buf),client,text);
	fclose(temp);
	delete buf;
}

// formatting internals

void Core::substitute(char *text,char *fn,bool both)
{
	fpos_t bof;

	char *tmp = new char[MAX_BUF_LEN];
	char *buf = new char[MAX_BUF_LEN];
	char *line = new char[MAX_PHRASE_LEN];
	char *string1 = new char[MAX_PHRASE_LEN];
	char *string2 = new char[MAX_PHRASE_LEN];
	char *beststring1 = new char[MAX_PHRASE_LEN];
	char *beststring2 = new char[MAX_PHRASE_LEN];
	char *bestpos;
	char *pos;
	char *at;
	bool notfnd;

	sprintf(buf," %s ",text);
	FILE *sfile = fopen(fn,"r");
	if (sfile) {
		for (at = buf;*at;at++) {
			*beststring1 = 0;
			bestpos = at + strlen(at);
			notfnd = true;
			bof = 0;
			fsetpos(sfile,&bof);
			while (!feof(sfile)) {	
				fgets(line,MAX_PHRASE_LEN,sfile);
				if (!(pos = strstr(line,"="))) continue;
				*pos = 0;
				strcpy(string1,line);
				strcpy(string2,pos + 1);
				if (!(pos = strstr(string2,";"))) continue;
				*pos = 0;
				doDefault(string1);
				doDefault(string2);
				if (pos = strstr(at,string1)) {
					if (pos <= bestpos) {
						if (strlen(beststring1) < strlen(string1) || pos < bestpos) {
							bestpos = pos;
							strcpy(beststring1,string1);
							strcpy(beststring2,string2);
							notfnd = false;
						}
					}
				}

				if (both) {
					lowercase(string1);
					lowercase(string2);
					if (pos = strstr(at,string1)) {
						if (pos <= bestpos) {
							if (strlen(beststring1) < strlen(string1) || pos < bestpos) {
								bestpos = pos;
								strcpy(beststring1,string1);
								strcpy(beststring2,string2);
								notfnd = false;
							}
						}
					}
				}

			}
			if (notfnd) break;
			strcpy(tmp,bestpos + strlen(beststring1));
			strcpy(bestpos,beststring2);
			strcat(bestpos,tmp);
			at = bestpos + strlen(beststring2) - 1;
		}
		fclose(sfile);
	}

	pos = buf + strlen(buf) - 1;
	if (*pos == ' ' && strcmp(buf," ")) *pos = 0;
	strcpy(text,buf + 1);

	delete beststring2;
	delete beststring1;
	delete string2;
	delete string1;
	delete line;
	delete buf;
	delete tmp;
}

void Core::tidyup(char *text)
{
	char *buf = new char[MAX_INSTRING];
	char *at, *pos;

	for (at = text + strlen(text);at >= text;at--) {
		if (strchr(".;?!",*at) || *at <= ' ') *at = 0; else break;
	}
	sprintf(buf," %s ",text);
	substitute(buf,fnsub,true);
	for (at = pos = buf;*at;at++) if (strchr(".;?!",*at)) pos = at;
	for (pos++;*pos && *pos <= ' ';pos++);
	strcpy(text,pos);
	delete buf;
	cleaner(text);
}

void Core::tidyup_ps(char *text)
{
	char *buf = new char[MAX_INSTRING];
	char *pos = text;
	char *c;

	sprintf(buf," %s ",text);
	substitute(buf,fnsub,false);

	for (c = buf + 1;*c;c++) {
		if (isalnum(*c) || strchr(" @*_-:'",*c) || (*c == '.' && isalnum(*(c + 1))) )
			*pos++ = *c;
	}
	delete buf;
	*pos-- = 0;
	while (pos >= text && *pos <= ' ') *pos-- = 0;
}

// pattern side processing

bool Core::match(char *text,char *pattern)
{
	int w;
	int x;
	int y;
	int z;
	char *p;
	char *t;
	char c;
	char *at;

	if (!stricmp(pattern,text)) return true;

	if (!strstr(pattern,"*") && !strstr(pattern,"_")) return false;

	w = (int)(strstr(pattern,"_") - pattern);
	x = (int)(strstr(pattern,"*") - pattern);
	if ((x >= 0 && w > x) || w < 0) w = x;

	if (w > 0) if (strnicmp(text,pattern,w)) return false;

	p = pattern;
	t = text;

	while (true) {

		t += w;
		p += w;

		for (y = 1,z = 0;p[y] == ' ' || p[y] == '*' || p[y] == '_';y++) if (p[y] != ' ') z++;
		while (p[--y] == ' ');
		if (w < y) {
			while (z--) {
				if (!(at = strchr(t,' '))) break;
				t = at + 1;
			}
			if (z >= 0) return false;
			p += y;
		}

		p++;
		if (!*p) break;

		if (!strstr(p,"*") && !strstr(p,"_")) {
			if (!(at = stristr(t,p))) return false;
			if (stricmp(at,p)) return false;
			y = (int)strlen(p);
			z = (int)strlen(t);
			if (stricmp(t + z - y,p)) return false;
			break;
		}

		w = (int)(strstr(p,"_") - p);
		x  = (int)(strstr(p,"*") - p);
		if ((x >= 0 && w > x) || w < 0) w = x;

		if (!w) return false;

		c = p[w]; p[w] = 0;
		if (!(at = stristr(t,p))) { p[w] = c; return false; }
		p[w] = c;

		t = at;
	}
	if (strstr(p,"*") || strstr(p,"_")) return false;

	return true;
}

bool Core::matchsave(char *text,char *pattern, char *context)
{
	int i = 0;
	int w;
	int x;
	int y;
	int z;
	char *p;
	char *t;
	char c;
	char *at;
	char buf[MAX_STAR_LEN];

	contexts.Add(context);
	contexts.curStar->m_path = text;
	contexts.curStar->m_pattern = pattern;

	if (!stricmp(pattern,text)) return true;

	w = (int)(strstr(pattern,"_") - pattern);
	x = (int)(strstr(pattern,"*") - pattern);
	if ((x >= 0 && w > x) || w < 0) w = x;

	p = pattern;
	t = text;
	while (true) {
		t += w;
		p += w;

		for (y = 1,z = 0;p[y] == ' ' || p[y] == '*' || p[y] == '_';y++) if (p[y] != ' ') z++;
		while (p[--y] == ' ');
		if (w < y) {
			while (z--) {
				if (!(at = strchr(t,' '))) break;
				strcpy(buf,t); buf[at - t] = 0;
				contexts.curStar->Push(buf);
				i++;
				t = at + 1;
			}
			if (z >= 0) return false;
			p += y;
		}

		strcpy(buf,t);

		p++;
		if (!*p) {
			contexts.curStar->Push(buf);
			i++;
			break;
		}

		if (!strstr(p,"*") && !strstr(p,"_")) {
			buf[strlen(t) - strlen(p)] = 0;
			contexts.curStar->Push(buf);
			i++;
			break;
		}

		w = (int)(strstr(p,"_") - p);
		x  = (int)(strstr(p,"*") - p);
		if ((x >= 0 && w > x) || w < 0) w = x;

		c = p[w]; p[w] = 0;
		at = stristr(t,p);
		p[w] = c;

		buf[at - t] = 0;
		contexts.curStar->Push(buf);
		i++;
		t = at;
	}

	return true;
}

bool Core::testguard(fpos_t templpos)
{
	char *p1;
	char *p2;
	bool ans;

	if (!templpos) return TRUE;

	char *line = new char[MAX_BUF_LEN];

	FILE *tmpl = fopen(fntmp,"r");
	fsetpos(tmpl,&templpos);

	fgets(line,MAX_BUF_LEN,tmpl);
	rtrim(line);
	expand(line);
	fclose(tmpl);
	
 //extractguard(line,guard);   only one allowed per template

	p1 = strstr(line,"<guard>");
	p2 = strstr(line,"</guard>");
	if (p1 == NULL || p2 == NULL || p2 < p1) {
		delete line;
		return TRUE;
	}

 // real simple extraction from line for <guard> to </guard>
	*p2 = 0; 
	char *guard = new char[MAX_BUF_LEN];
	strcpy(guard,p1 + 7);

// now go find a logical result in a word: "TRUE" or "FALSE"
	
	if (!strncmp(guard,"<script>",8)) {													// evaluate Script for $GUARD
		replace(guard,"<script>","");
		if (p2 = strstr(guard,"</script>")) {
			*p2 = 0;
			doScript(guard);
			ans = (strstr(getvar("GUARD","TRUE"),"TRUE")) ? TRUE : FALSE;
		}
	} else if (!strncmp(guard,"<condition>",11) || !strncmp(guard,"<condition ",11)) {	// evaluate AIML condition
		if (XMLtag(guard,line)) {
			p1 = guard + strlen(line);
			if (p2 = strstr(p1,"</condition>")) {
				*p2 = 0;
				ans = (strstr(doCondition(line,p1),"TRUE")) ? TRUE : FALSE;
			}
		}
	} else {																// evaluate guard as originally done for CYC
		ans = (strstr(doCycSystem(guard),"TRUE")) ? TRUE : FALSE;
	}

	delete line;
	delete guard;
 
	return ans;
}

fpos_t Core::interpret(char *text)
{
	fpos_t tpos;
	fpos_t bestpos = 0;
	char c = *text;	if (islower(c)) c += 'A' - 'a';
	char chr;
	char *pos;
	char *line = new char[MAX_LINE_SIZE];
	char *nowthat = new char[MAX_BUF_LEN];
	char *trythat = new char[MAX_LINE_SIZE];
	char *trytopic = new char[MAX_LINE_SIZE];
	char *buf = new char[MAX_LOADSTRING];
	bool alpha = ((c >= 'A') && (c <= 'Z'));
	bool topicused;
	bool perfecttopic;
	bool thatused;
	bool perfectthat;
	bool fnd = false;
	bool sup = true;

	unloadgm();

	*best = 0;
	*bestthat = 0;
	*besttopic = 0;

	strcpy(nowthat,thats[0]);
	uppercase(nowthat);
	tidyup(nowthat);

	FILE *aiml = fopen(fnpat,"r");

	while (!feof(aiml)) {
		fgets(line,MAX_LINE_SIZE,aiml);
		if (!(pos = strstr(line,"@"))) continue;
		if (sup) {
			if (*line != '_') {
				if (fnd) break;
				sup = false;
				if (alpha) fsetpos(aiml,&chunk[c - 'A']); 
				else fsetpos(aiml,&chunk[26]);
				continue;
			}
		} else if (alpha) {
			chr = *line;
			if (islower(chr)) chr += 'A' - 'a';
			if (chr != c) {
				if (fnd) break;
				alpha = false;
				fsetpos(aiml,&chunk[26]);
				continue;
			}
		}
		strcpy(buf,pos + 1);
		rtrim(buf);
		*pos = 0;
		tpos = atol(buf);

		while (strstr(line,"<name/>")) replace(line,"<name/>",getbot("name"));
		while (strstr(line,"<bot/>")) replace(line,"<bot/>",getbot("name"));

// replace other bot variables in pattern?


// for each [context] do the match, but first try the pattern?
// unfortunately this sequential processing doesn't accomodate anything but "topic" and "that" contexts

		topicused = false;
		perfecttopic = false;
		if (pos = strstr(line,"[topic]")) {
			strcpy(trytopic,pos + 7);
			*pos = 0;
			tidyup_ps(trytopic);
			if (stricmp(topic,trytopic)) {
				if (!match(topic,trytopic)) continue;
				topicused = true;
			} else { topicused = perfecttopic = true; }
		}

		thatused = false;
		perfectthat = false;
		if (pos = strstr(line,"[that]")) {
			strcpy(trythat,pos + 6);
			*pos = 0;
			tidyup_ps(trythat);
			if (stricmp(nowthat,trythat)) {
				if (!match(nowthat,trythat)) continue;
				thatused = true;
			} else { thatused = perfectthat = true; }
		}

		if (!match(text,line)) continue;

		// build stars first and then test <guard> in the template
		contexts.RemoveAll();

		if (topicused && !perfecttopic) matchsave(topic,trytopic,"topic"); 
		if (thatused && !perfectthat) 	matchsave(nowthat,trythat,"that");
		if (strcmp(text,line))			matchsave(text,line,"input");
		if (!testguard(tpos)) continue;

		if (!stricmp(text,line) || strstr(line,"_")) {  // strstr to make _ match better than exact despite best judgement
			if (topicused && perfecttopic) {
				if (thatused && perfectthat) {
					strcpy(best,line);
					strcpy(bestthat,trythat);
					strcpy(besttopic,trytopic);
					bestpos = tpos;
					break;
				} else if (!thatused) {
					strcpy(best,line);
					strcpy(bestthat,"");
					strcpy(besttopic,trytopic);
					bestpos = tpos;
					break;
				}
			}
			if (!topicused) {
				if (thatused && perfectthat) {
					strcpy(best,line);
					strcpy(bestthat,trythat);
					strcpy(besttopic,"");
					bestpos = tpos;
					break;
				} else if (!thatused) {
					strcpy(best,line);
					strcpy(bestthat,"");
					strcpy(besttopic,"");
					bestpos = tpos;
					break;
				}
			}
		}

		if (fnd) {
			if (topicused) { if (patrns(trytopic,besttopic)) continue; } else if (*besttopic) continue;
			if (thatused)  { if (patrns(trythat,bestthat))   continue; } else if (*bestthat) continue;
			if (patrns(line,best)) continue;
		}
		if (topicused) strcpy(besttopic,trytopic); else *besttopic = 0;
		if (thatused) strcpy(bestthat,trythat); else *bestthat = 0;
		strcpy(best,line);
		bestpos = tpos;
		fnd = true;
	}
	fclose(aiml);

	contexts.RemoveAll();

	if (*besttopic) matchsave(topic,besttopic,"topic"); 
	if (*bestthat) 	matchsave(nowthat,bestthat,"that");
	if (*best)		matchsave(text,best,"input");

	delete line;
	delete nowthat;
	delete trythat;
	delete trytopic;
	delete buf;

	return bestpos;
}

fpos_t Core::gm_interpret(char *text)
{
	fpos_t bestpos = 0;
	CString path = text;

	//loadgm();

	char *wrk = new char[MAX_BUF_LEN];

	strcpy(wrk,thats[0]);
	lowercase(wrk);
	tidyup(wrk);
	strcpy(m_that,wrk);
	strcpy(m_input,text);

	strcpy(best,"*");
	strcpy(bestthat,"*");
	strcpy(besttopic,"*");

	Node *ans = NULL;

	ans = gm->Select(path,this);
	if (ans != NULL) {
		bestpos = ans->m_template;

		// the famous "fill up the match structure for target logging" hack
		if (contexts.Find("input")) sprintf(best,"%s",contexts.curStar->m_pattern);
		if (contexts.Find("that"))  sprintf(bestthat,"%s",contexts.curStar->m_pattern);
		if (contexts.Find("topic")) sprintf(besttopic,"%s",contexts.curStar->m_pattern);
	}

	delete [] wrk;

	return bestpos;
}

void Core::loadgm()
{
	if (gm == NULL) 
	{ 
		gm = new idx(); 
		gm->bot = this; 
	}
	if (!gm->m_size) 
		gm->Load(fnpat);
}

void Core::unloadgm()
{
	if (gm != NULL) { delete gm; gm = NULL; }
}

void Core::reloadgm()
{
	unloadgm();
	if (usegraphmaster) loadgm();
}

// main stuff to do the work

void Core::respond(char *text, char *reply)
{
	
	recursion++;
	if (recursion > MAX_RECURSIONS) {
		strcpy(reply,"Internal error: Dangerous recursion."); 
		return;
	}

	char *line = new char[MAX_BUF_LEN];
	char *txt = new char[MAX_BUF_LEN];
	fpos_t bestpos;

	
	sprintf(line," %s ",text);
	substitute(line,fnsub,true);
	strcpy(txt,line + 1);
	cleaner(txt);

	if (usegraphmaster)
	{
		bestpos = gm_interpret(txt); 
	}
	else 
	{
		bestpos = interpret(txt);
	}

	if (bestpos) {
		FILE *tmpl = fopen(fntmp,"r");
		fsetpos(tmpl,&bestpos);

		fgets(line,MAX_BUF_LEN,tmpl);
		rtrim(line);
		expand(line);
		fclose(tmpl);
	} else *line = 0;

	newsec = false;
	delete txt;

	if (logtarget) {
		char *ti_input = new char[MAX_INSTRING];
		char *ti_that = new char[MAX_LOADSTRING];
		char *ti_topic = new char[MAX_INSTRING];
		char *tm_pattern = new char[MAX_INSTRING];
		char *tm_that = new char[MAX_INSTRING];
		char *tm_topic = new char[MAX_INSTRING];
		char *tm_template = new char[MAX_BUF_LEN];
		strcpy(ti_input,text);
		strcpy(ti_that,thats[0]);
		strcpy(ti_topic,topic);
		strcpy(tm_pattern,best);
		strcpy(tm_that,bestthat);
		strcpy(tm_topic,besttopic);
		strcpy(tm_template,line);

		while (replace(line,"<boring/>",""));
		fixnewlines(line);
		reevaluate(line,reply);//TEST
		
		savetarget(ti_input,ti_that,ti_topic,tm_pattern,tm_that,tm_topic,tm_template,reply);
		delete ti_input;
		delete ti_that;
		delete ti_topic;
		delete tm_pattern;
		delete tm_that;
		delete tm_topic;
		delete tm_template;
	} else {
		while (replace(line,"<boring/>",""));
		fixnewlines(line);
		reevaluate(line,reply);//TEST
    }
	if (!newsec) *second = 0;

	delete line;
}

// actually do the templates now

void Core::reevaluate(char *line, char *reply)
{
	if (!*line) return;

	while (replace(line,"<sr/>","<srai><star/></srai>"));
	while (replace(line,"<gender/>","<gender><star/></gender>"));
	while (replace(line,"<setname/>","<set name=\"name\"><star/></set>"));
	while (replace(line,"<friends/>","<bot name=\"friends\"/>"));
	while (replace(line,"<sign/>","<bot name=\"sign\"/>"));
	while (replace(line,"<wear/>","<bot name=\"wear\"/>"));
	while (replace(line,"<location/>","<bot name=\"location\"/>"));
	while (replace(line,"<birthday/>","<bot name=\"birthday\"/>"));
	while (replace(line,"<birthplace/>","<bot name=\"birthplace\"/>"));
	while (replace(line,"<question/>","<bot name=\"question\"/>"));
	while (replace(line,"<for_fun/>","<bot name=\"for_fun\"/>"));
	while (replace(line,"<forfun/>","<bot name=\"forfun\"/>"));
	while (replace(line,"<friends/>","<bot name=\"friends\"/>"));
	while (replace(line,"<boyfriend/>","<bot name=\"boyfriend\"/>"));
	while (replace(line,"<girlfriend/>","<bot name=\"girlfriend\"/>"));
	while (replace(line,"<look_like/>","<bot name=\"look_like\"/>"));
	while (replace(line,"<looklike/>","<bot name=\"looklike\"/>"));
	while (replace(line,"<talk_about/>","<bot name=\"talk_about\"/>"));
	while (replace(line,"<talkabout/>","<bot name=\"talkabout\"/>"));
	while (replace(line,"<kind_music/>","<bot name=\"kind_music\"/>"));
	while (replace(line,"<kindmusic/>","<bot name=\"kindmusic\"/>"));
	while (replace(line,"<favoriteband/>","<bot name=\"favoriteband\"/>"));
	while (replace(line,"<favoritebook/>","<bot name=\"favoritebook\"/>"));
	while (replace(line,"<favoritecolor/>","<bot name=\"favoritecolor\"/>"));
	while (replace(line,"<favoritefood/>","<bot name=\"favoritefood\"/>"));
	while (replace(line,"<favoritesong/>","<bot name=\"favoritesong\"/>"));
	while (replace(line,"<favoritemovie/>","<bot name=\"favoritemovie\"/>"));
	while (replace(line,"<favorite_book/>","<bot name=\"favoritebook\"/>"));
	while (replace(line,"<favorite_color/>","<bot name=\"favoritecolor\"/>"));
	while (replace(line,"<favorite_food/>","<bot name=\"favoritefood\"/>"));
	while (replace(line,"<favorite_movie/>","<bot name=\"favoritemovie\"/>"));
	while (replace(line,"<favorite_song/>","<bot name=\"favoritesong\"/>"));
	while (replace(line,"<favorite_band/>","<bot name=\"favoriteband\"/>"));
	while (replace(line,"<botmaster/>","<bot name=\"master\"/>"));

	char *buf = new char[MAX_BUF_LEN];

	strcat(reply,docmds(line,buf));

	delete buf;
}

char *Core::docmds(char *text,char *buf)
{
	char *at;
	char *pos = text;
	char *otag = new char[MAX_TAG_LEN];
	char *data = new char[MAX_BUF_LEN];
	char *ctag = new char[MAX_TAG_LEN];

	*buf = 0;
	while (at = strstr(pos,"<")) {
		if (at != pos) {
			*at = 0;
			strcat(buf,pos);
			*at = '<';
			pos = at;
		}
		if (XMLnode(at,otag,data,ctag)) {
			if (!*ctag) {
				pos += strlen(otag);
				strcat(buf,doatomic(otag));
			} else {
				pos += strlen(otag) + strlen(data) + strlen(ctag);
				strcat(buf,donode(otag,data));
			}
		} else {
			strcat(buf,"<");
			pos++;
		}
	}
	strcat(buf,pos);

	delete otag;
	delete data;
	delete ctag;

	return buf;
}

char *Core::doatomic(char *tag)
{
	int i, j;
	char *at;
	char *temp = new char[MAX_BUF_LEN];
	char *dval = new char[MAX_TAG_LEN];

	*wrk = 0;
	contexts.GetStar("input",0,temp);

	if (!strcmp(tag,"<name/>"))                strcpy(wrk,getbot("name"));
	else if (!strcmp(tag,"<bot/>"))            strcpy(wrk,getbot("name"));
	else if (!strcmp(tag,"<date/>"))           strcpy(wrk,getdts(temp));
	else if (!strcmp(tag,"<size/>"))           strcpy(wrk,getsize());
	else if (!strcmp(tag,"<id/>"))             strcpy(wrk,getbot("ip"));
	else if (!strcmp(tag,"<version/>"))        strcpy(wrk,getbot("version"));
	else if (!strcmp(tag,"<star/>"))           contexts.GetStar("input",0,wrk); //strcpy(wrk,stars[0]);
	else if (!strcmp(tag,"<star2/>"))          contexts.GetStar("input",1,wrk); //strcpy(wrk,stars[1]);
	else if (!strcmp(tag,"<thatstar/>"))       contexts.GetStar("that",0,wrk); //strcpy(wrk,thatstars[0]);
	else if (!strcmp(tag,"<thatstar2/>"))      contexts.GetStar("that",1,wrk); //strcpy(wrk,thatstars[1]);
	else if (!strcmp(tag,"<topicstar/>"))      contexts.GetStar("topic",0,wrk); //strcpy(wrk,topicstars[0]);
	else if (!strcmp(tag,"<topicstar2/>"))     contexts.GetStar("topic",1,wrk); //strcpy(wrk,topicstars[1]);
	else if (!strcmp(tag,"<that/>"))           split(wrk,thats[0],1);
	else if (!strcmp(tag,"<justbeforethat/>")) split(wrk,thats[1],1);
	else if (!strcmp(tag,"<input/>"))          split(wrk,inputs[0],1);
	else if (!strcmp(tag,"<justthat/>"))       split(wrk,inputs[1],1);
	else if (!strcmp(tag,"<beforethat/>"))     split(wrk,inputs[2],1);
	else if (!strcmp(tag,"<usesaved/>"))       strcpy(wrk,doUseSaved(temp));
	else if (!strcmp(tag,"<person/>"))         strcpy(wrk,doPerson(temp,fnper));
	else if (!strcmp(tag,"<person2/>"))        strcpy(wrk,doPerson(temp,fnper2));
	else if (!strcmp(tag,"<personf/>"))        strcpy(wrk,doPerson(temp,fnperf));
	else if (!strncmp(tag,"<get ",5)) {
		if (XMLvalue(tag,"name",temp)) {
			if (XMLvalue(tag,"default",dval)) {
				strcpy(wrk,getvar(temp,dval));
			} else {
				strcpy(wrk,getvar(temp,NULL));
			}
		} else *wrk = 0;
	}
	else if (!strncmp(tag,"<bot ",5)) {
		if (XMLvalue(tag,"name",temp)) {
			strcpy(wrk,getbot(temp));
		} else *wrk = 0;
	}
	else if (!strncmp(tag,"<that ",6)) {
		if (XMLvalue(tag,"index",dval)) {
			if (at = strstr(dval,",")) {
				*at = 0;
				i = atoi(dval) - 1;
				j = atoi(at + 1);
				if (i >= 0 && i < 5) split(wrk,thats[i],j); else *wrk = 0;
			} else {
				i = atoi(dval) - 1;
				if (i >= 0 && i < 5) split(wrk,thats[i],1); else *wrk = 0;
			}
		} else *wrk = 0;
	}
	else if (!strncmp(tag,"<input ",7)) {
		if (XMLvalue(tag,"index",dval)) {
			if (at = strstr(dval,",")) {
				*at = 0;
				i = atoi(dval) - 1;
				j = atoi(at + 1);
				if (i >= 0 && i < 5) split(wrk,inputs[i],j); else *wrk = 0;
			} else {
				i = atoi(dval) - 1;
				if (i >= 0 && i < 5) split(wrk,inputs[i],1); else *wrk = 0;
			}
		} else *wrk = 0;
	}
	else if (!strncmp(tag,"<star ",6)) {
		if (XMLvalue(tag,"name",temp)) {
			lowercase(temp);
			if (XMLvalue(tag,"index",dval)) {
				i = atoi(dval) - 1;
				contexts.GetStar(temp,i,wrk);
			} else contexts.GetStar(temp,0,wrk);
		} else {
			if (XMLvalue(tag,"index",dval)) {
				i = atoi(dval) - 1;
				contexts.GetStar("input",i,wrk);
			} else *wrk = 0;
		}
	}
	else if (!strncmp(tag,"<thatstar ",10)) {
		if (XMLvalue(tag,"index",dval)) {
			i = atoi(dval) - 1;
			contexts.GetStar("that",i,wrk);
		} else *wrk = 0;
	}
	else if (!strncmp(tag,"<topicstar ",11)) {
		if (XMLvalue(tag,"index",dval)) {
			i = atoi(dval) - 1;
			contexts.GetStar("topic",i,wrk);
		} else *wrk = 0;
	} else {
		updatemood(tag);
		if (*tag) logerror("unknown atomic tag",tag);
	}

	delete dval;
	delete temp;
	return wrk;
}

char *Core::donode(char *tag,char *text)
{
	strcpy(wrk,text);
	if (!*tag) return wrk;

	if (!strcmp(tag,"<think>"))              strcpy(wrk,doThink(text));
	else if (!strcmp(tag,"<srai>"))          strcpy(wrk,doSRAI(text));
	else if (!strcmp(tag,"<guard>"))         strcpy(wrk,"");
	else if (!strcmp(tag,"<lowercase>"))     strcpy(wrk,doLowercase(text));
	else if (!strcmp(tag,"<uppercase>"))     strcpy(wrk,doUppercase(text));
	else if (!strcmp(tag,"<formal>"))        strcpy(wrk,doFormalcase(text));
	else if (!strcmp(tag,"<sentence>"))      strcpy(wrk,doSentencecase(text));
	else if (!strcmp(tag,"<person>"))        strcpy(wrk,doPerson(text,fnper));
	else if (!strcmp(tag,"<person2>"))       strcpy(wrk,doPerson(text,fnper2));
	else if (!strcmp(tag,"<personf>"))       strcpy(wrk,doPerson(text,fnperf));
	else if (!strcmp(tag,"<gender>"))        strcpy(wrk,doPerson(text,fngndr));
	else if (!strcmp(tag,"<gossip>"))        strcpy(wrk,doGossip(text));
	else if (!strcmp(tag,"<system>"))        strcpy(wrk,doSystem(text));
	else if (!strcmp(tag,"<script>"))        strcpy(wrk,doScript(text));
	else if (!strcmp(tag,"<condition>"))     strcpy(wrk,doCondition(tag,text));
	else if (!strcmp(tag,"<random>"))        strcpy(wrk,doRandom(text));
	else if (!strcmp(tag,"<li>"))            strcpy(wrk,doDefault(text));
	else if (!strcmp(tag,"<cycterm>"))       strcpy(wrk,doCycTerm(text));
	else if (!strcmp(tag,"<cycrandom>"))     strcpy(wrk,doCycRandom(text));
	else if (!strcmp(tag,"<cycretract>"))    strcpy(wrk,doCycRetract(text));
	else if (!strcmp(tag,"<cycassert>"))     strcpy(wrk,doCycAssert(text));
	else if (!strcmp(tag,"<cycsystem>"))     strcpy(wrk,doCycSystem(text));
	else if (!strcmp(tag,"<cycexp>"))        strcpy(wrk,doCycExp(text));
	else if (!strcmp(tag,"<cyccondition>"))  strcpy(wrk,doCycCondition(tag,text));
	else if (!strncmp(tag,"<cyccondition ",14)) strcpy(wrk,doCycCondition(tag,text));
	else if (!strncmp(tag,"<li ",4))         strcpy(wrk,doLi(tag,text));
	else if (!strncmp(tag,"<condition ",11)) strcpy(wrk,doCondition(tag,text));
	else if (!strncmp(tag,"<set ",5))        strcpy(wrk,doSet(tag,text));
	//else if (!strncmp(tag,"<APPLET ",8))     { strcpy(wrk,""); }
	else if (!strcmp(tag,"<sec>"))           { strcpy(second,text); *wrk = 0; newsec = true;}
	else if (!strcmp(tag,"<save>"))          {
		int best = 0;
		int bestage = 0;
		for (int i = 0;i < 10;i++) if (sage[i] > bestage) {	bestage = sage[i]; best = i; }
		sage[best] = 0;
		strcpy(saves[best],text);
		*wrk = 0;
	} else logerror(tag,text);
	return wrk;
}

char *Core::donested(char *text)
{
	int len;
	char *at = text;
	char *otag = new char[MAX_BUF_LEN];
	char *data = new char[MAX_BUF_LEN];
	char *ctag = new char[MAX_BUF_LEN];
	if (XMLnode(text,otag,data,ctag)) {
		len = (int)(strlen(otag) + strlen(data) + strlen(ctag));
		strcpy(otag,text); otag[len] = 0;
		strcpy(ctag,text + len);
		docmds(otag,data);
		strcpy(text,data);
		strcat(text,ctag);
		at = text + strlen(data);
	}
	delete otag;
	delete data;
	delete ctag;
	return at;
}

// the aiml template commands

char *Core::doSRAI(char *text)
{
	char *buf = new char[MAX_BUF_LEN];

	Stars savContexts;
	if (contexts.GetFirst()) {
		savContexts.Copy(contexts.curStar);
		while (contexts.GetNext()) savContexts.Copy(contexts.curStar);
	}

	docmds(text,buf);
	strcpy(text,buf);
	*buf = 0;

	respond(text,buf);
	strcpy(text,buf);

	contexts.RemoveAll();
	if (savContexts.GetFirst()) {
		contexts.Copy(savContexts.curStar);
		while (savContexts.GetNext()) contexts.Copy(savContexts.curStar);
	}

	delete buf;
	return text;
}

char *Core::doSet(char *tag,char *text)
{
	char *var = new char[MAX_TAG_LEN];
	if (XMLvalue(tag,"name",var)) {
		char *buf = new char[MAX_BUF_LEN];
		docmds(text,buf);
		setvar(var,buf);
		strcpy(text,buf);
		delete buf;
	} else *text = 0;
	delete var;
	return text;
}

char *Core::doCondition(char *tag,char *text)
{
	int fnd = false;
	int len;
	char *at;
	char *pos;
	char *var = new char[MAX_TAG_LEN];
	char *val = new char[MAX_VALUE_LEN];
	char *buf = new char[MAX_BUF_LEN];
	char *otag = new char[MAX_TAG_LEN];
	char *data = new char[MAX_BUF_LEN];
	char *ctag = new char[MAX_TAG_LEN];

	if (XMLvalue(tag,"name",var)) {
		strcpy(buf,getvar(var,""));
		if (XMLvalue(tag,"value",val)) {
			if (match(buf,val)) {
				docmds(text,buf);
				fnd = true;
			}
		} else {	// condition list values
			at = text;
			while (pos = strstr(at,"<li")) {
				if (XMLnode(pos,otag,data,ctag)) {
					len = (int)(strlen(otag) + strlen(data) + strlen(ctag));
					if (XMLvalue(otag,"value",val)) {
						if (match(buf,val)) {
							at = docmds(data,buf);
							fnd = true;
							break;
						}
					} else if (!strcmp(otag,"<li>")) {
						at = docmds(data,buf);
						fnd = true;
						break;
					}
					at = pos + len - 1;
				} else at++;
			}
		}
	} else {
		at = text;
		while (pos = strstr(at,"<li")) {
			if (XMLnode(pos,otag,data,ctag)) {
				len = (int)(strlen(otag) + strlen(data) + strlen(ctag));
				if (XMLvalue(otag,"name",var)) {
					strcpy(buf,getvar(var,""));
					if (XMLvalue(otag,"value",val)) {
						if (match(buf,val)) {
							at = docmds(data,buf);
							fnd = true;
							break;
						}
					}
				} else if (!strcmp(otag,"<li>")) {
					at = docmds(data,buf);
					fnd = true;
					break;
				}
				at = pos + len - 1;
			} else at++;
		}
	}
	if (fnd) strcpy(text,buf); else *text = 0;
	delete var;
	delete val;
	delete buf;
	delete otag;
	delete data;
	delete ctag;
	return text;
}

char *Core::doRandom(char *text)
{
	int i, j, h;
	int pos;
	int last;
	int item;
	int items;
	float cmood;
	double tdist;
	double rnd;

	char *at;
	char *otag = new char[MAX_BUF_LEN];
	char *data = new char[MAX_BUF_LEN];
	char *ctag = new char[MAX_TAG_LEN];

	int *list = new int[MAX_RANDOM_LI];
	int *mood = new int[MAX_RANDOM_LI];
	double *sqdist = new double[MAX_RANDOM_LI];
	
	items = 0;
	last = -1;
	while ((pos = (int)(strstr(text + last + 1,"<li>") - text)) > -1) {
		list[items] = pos;
		if (XMLnode(text + pos,otag,data,ctag)) {
			last = pos + (int)(strlen(otag) + strlen(data) + strlen(ctag) - 1);
		} else last = pos;
		mood[items++] = calcmood(pos + 4 + text);
	}
	for (i = 0;i < items;i++) {
		for (j = 1;j < items;j++) {
			if (mood[j - 1] < mood[j]) {
				h = mood[j]; mood[j] = mood[j - 1]; mood[j - 1] = h;
				h = list[j]; list[j] = list[j - 1]; list[j - 1] = h;
			}
		}
	}
	cmood = (happyness + 0.5f * helpfullness + humor + affection + 0.5f * trust) / 3 + 0.5f;
	if (cmood < 0) cmood = 0;
	if (cmood > 1) cmood = 1;
	tdist = 0;
	for (i = 0;i < items;i++) tdist += (sqdist[i] = 1.0 / ((cmood - mood[i]) * (cmood - mood[i])));
	for (i = 0;i < items;i++) {
		sqdist[i] *= 1 / tdist;
		if (i > 0) sqdist[i] += sqdist[i - 1];
	}
	rnd = (double)rand() / (double)RAND_MAX;

	item = 0;
	for (i = 0;i < items;i++) {
		if (rnd < sqdist[i]) break;
		item++;
	}
	 if (item == items) item = items - 1;

	if (XMLnode(text + list[item],otag,data,ctag)) {
		at = docmds(data,text);
	} else *text = 0;

	delete otag;
	delete data;
	delete ctag;
	delete list;
	delete mood;
	delete sqdist;
	return text;
}

char *Core::doUppercase(char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	docmds(text,buf);
	uppercase(buf);
	strcpy(text,buf);
	delete buf;
	return text;
}

char *Core::doLowercase(char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	docmds(text,buf);
	lowercase(buf);
	strcpy(text,buf);
	delete buf;
	return text;
}

char *Core::doFormalcase(char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	int upshift = true;
	docmds(text,buf);
	char *c = buf - 1;
	while (*(++c)) {
		if (upshift) if (islower(*c)) *c += 'A' - 'a';
		upshift = isspace(*c);
	}
	strcpy(text,buf);
	delete buf;
	return text;
}

char *Core::doSentencecase(char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	int upshift = true;
	char delim[8];

	strcpy(delim,".?!");  // ***  add your own sentence delimiters here  ***

	docmds(text,buf);
	char *c = buf - 1;
	while (*(++c)) {
		if (upshift) if (islower(*c)) *c += 'A' - 'a';
		upshift = strchr(delim,*c) || (upshift && isspace(*c));
	}
	strcpy(text,buf);
	delete buf;
	return text;
}

char *Core::doThink(char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	docmds(text,buf);
	delete buf;
	*text = 0;
	return text;
}

char *Core::doPerson(char *text,char *fname)
{
	if (!*text) return text;

	char *buf = new char[MAX_BUF_LEN];
	strcpy(text,docmds(text,buf));
	delete buf;
	substitute(text,fname, true);
	return text;
}

char *Core::doSystem(char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	execute(docmds(text,buf),text);
	delete buf;
	return text;
}

char *Core::doScript(char *text)
{
	pu->onCmd(text);
	*text = 0;
	return text;
}

char *Core::doGossip(char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	strcpy(text,docmds(text,buf));
	delete buf;
	savegossip(text);
	return text;
}


char *Core::doUseSaved(char *text)
{
	bool answered = false;
	char *line = new char[MAX_LINE_SIZE];
	char *reply = new char[MAX_BUF_LEN];
	int i;

	if (*second && !newsec) {
		strcpy(line,second);
		reevaluate(line,reply);
		*second = 0;
	} else {
		for (i = 0;i < 10;i++) {
			if (sage[i] < 15) {
				strcpy(line,saves[i]);
				reevaluate(line,reply);
				sage[i] = 1000;
				answered = true;
				break;
			}
		}
		if (!answered) strcpy(reply,"");
	}
	strcpy(text,reply);
	if (*text) strcat(text," ");
	delete line;
	delete reply;
	return text;
}

char *Core::doLi(char *tag,char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	docmds(text,buf);
	delete buf;
	return text;
}

char *Core::doDefault(char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	docmds(text,buf);
	strcpy(text,buf);
	delete buf;
	return text;
}

// Common Sense Extensions to AIML...

void Core::infix_phrase(char *buf,char *symbol,char *phrase)
{
	char *p1;
	char *p2;
	long l = (int)strlen(symbol);

	while (p1 = strstr(buf,symbol)) {
		// find next #
		p1 = p1 + l + 2;
		if (p2 = strstr(p1,"#$")) {
			*(p2 - 1) = '^';			
			p1 = strstr(p2 + 1,symbol);

		}
	}

	while (strstr(buf,symbol)) replace(buf,symbol," ");
	while (strstr(buf,"^")) replace(buf,"^",phrase);
}

bool Core::lookupCycTerm(char *cyctemplate, char *text)
{
	char *askbuf = new char[MAX_BUF_LEN];
    char *p1;
	char *p2;
	bool retVal = FALSE;

    sprintf(askbuf,cyctemplate,text);
	pu->savslot("xcycget",askbuf);
	if (pu->getslot("CYCdata1",askbuf)) {
		if (p1 = strstr(askbuf,"?CYCOBJECT .")) {
			p1 = p1 + 13; // go past the "?CYCOBJECT."
			p2 = strstr(p1,")"); //Find the first close
			*p2 = 0; // change to end of string
			// copy to the return text
			strcpy(text,p1);
			retVal = TRUE;
		}
	}
	delete askbuf;
	return retVal;
}

bool Core::lookupCycConstant(bool TheWordFlag, char *text)
{
	char *askbuf = new char[MAX_BUF_LEN];
    char *p1;
	char *p2;
	bool retVal = FALSE;

	if (TheWordFlag) sprintf(askbuf,"(constant-name-case-collision \"%s-TheWord\" )",text);
	else             sprintf(askbuf,"(constant-name-case-collision \"%s\")",text);
	
	pu->savslot("xcycget",askbuf);

	if (pu->getslot("CYCdata1",askbuf)) {
		if (p1 = strstr(askbuf,"200 #$")) {
			p1 = p1 + 4; // go past the "?CYCOBJECT."
			if (p2 = strchr(p1,13)) *p2 = 0; // Find the first close // change to end of string

			if (TheWordFlag) {	// copy to the return text
				char *wtext = new char[MAX_BUF_LEN];
				strcpy(wtext,p1);
				if (!retVal) retVal=lookupCycTerm("(fi-ask '(#$denotation %s ?TEXT ?TYPE ?CYCOBJECT) #$EverythingPSC)",wtext);
				if (!retVal) retVal=lookupCycTerm("(fi-ask '(#$denotationRelatedTo %s ?TEXT ?TYPE ?CYCOBJECT) #$EverythingPSC)",wtext);
				if (retVal) strcpy(text,wtext);
				delete [] wtext;
			} else {
				strcpy(text,p1);
				retVal = TRUE;
			}
		}
	}
	delete askbuf;
	return retVal;
}

char *Core::doCycTerm(char *text)
{
	//Try to find a matching cyc term
	//
	char *buf = new char[MAX_BUF_LEN];
	char *ltext = new char[MAX_BUF_LEN];
	char *askbuf = new char[MAX_BUF_LEN];
    char *p1;
	char *p2;
	bool fnd;
	double timediff;
	time_t now_time;
	int cacheptr;

	int upshift = true;
	docmds(text,buf);

	char *c = buf - 1;
	while (*(++c)) {
		if (upshift) if (islower(*c)) *c += 'A' - 'a';
		upshift = isspace(*c);
	}

	// some simple article trimming
	if (!strncmp(buf,"a ",2)) replace(buf,"a ","");
	if (!strncmp(buf,"an ",3)) replace(buf,"and ","");
	if (!strncmp(buf,"the ",4)) replace(buf,"the ","");
	if (!strncmp(buf,"this ",5)) replace(buf,"this ","");
	if (!strncmp(buf,"that ",5)) replace(buf,"that ","");

	strcpy(text,buf);
	strcpy(ltext,buf);
	*ltext = tolower (*ltext);

    if (usetermcache) {
		cacheptr = 0;
		c = ltext;
		while (*c) cacheptr = (cacheptr+((int)*c++)) % MAX_CACHE;
		// cache assumes a 2 second life span
		// good enough for most conversation chains
		// but responsive enough to catch new data from the system
		time(&now_time);
		timediff = difftime(now_time,termcache_timestamp[cacheptr]);
		if (timediff < 0 || timediff > 8.0) *termcache_value[cacheptr] = 0;
		
		if (*termcache_value[cacheptr] && !strcmp(termcache_key[cacheptr],ltext)) {
			strcpy(ltext,termcache_value[cacheptr]);
			strcpy(text,ltext);
			fnd = TRUE;
		} else {
			strcpy(termcache_key[cacheptr],ltext);
			*termcache_value[cacheptr] = 0;
			fnd = FALSE;
		}
	} else {
		cacheptr = 0;
		fnd = FALSE;
	}

	// Multi-word possibilities
	if (strchr(text,' ')) {
		char *multiword[3];
		int i;

		for(i = 0;i < 3;i++) {
			multiword[i] = new char[10 + strlen(text) * 4];
			strcpy(multiword[i],text);
		}

		while (strchr(multiword[0],' ')) replace(multiword[0]," ","");
		while (strchr(multiword[1],' ')) replace(multiword[1]," ","_");
		while (strchr(multiword[2],' ')) replace(multiword[2]," ","-");
		
		for(i = 0;!fnd && i < 3;i++) {
			if (!fnd) fnd = lookupCycConstant(false, multiword[i]);
			if (!fnd) fnd = lookupCycConstant(true, multiword[i]);
			if (fnd) strcpy(text,multiword[i]);
		}

		for (i = 0;i < 3;i++) delete multiword[i];
	}

	// Try direct constant lookup ( X and X-TheWord )

	if (!fnd) fnd = lookupCycConstant(false,text);
	if (!fnd) fnd = lookupCycConstant(true,text);

	// Try CycTerms

	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$denotation #$%s-TheWord ?TEXT ?TYPE ?CYCOBJECT) #$EverythingPSC)",text);

	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$denotationRelatedTo #$%s-TheWord ?TEXT ?TYPE ?CYCOBJECT) #$EverythingPSC)",text);

	// Name lookup
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$nameString ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);


	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$initialismString ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$abbreviationString-PN ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$preferredNameString ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$countryName-LongForm ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$countryName-ShortForm ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$acronymString ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$scientificName ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$termStrings ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$termStrings-GuessedFromName ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$prettyName ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$nicknames ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$preferredTermStrings ?CYCOBJECT  \"%s\") #$EverythingPSC)",text);
   	
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$preferredGenUnit ?CYCOBJECT ?POS #$%s-TheWord ) #$EverythingPSC)",text);


	// Generic word lookup
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$and (#$massNumber ?WORD \"%s\") (#$or (#$denotation ?WORD ?TEXT ?TYPE ?CYCOBJECT) (#$denotationRelatedTo ?WORD ?TEXT ?TYPE ?CYCOBJECT) )) #$EverythingPSC)",ltext);
	if (!fnd) fnd = lookupCycTerm("(fi-ask '(#$and (#$wordStrings ?WORD \"%s\") (#$or (#$denotation ?WORD ?TEXT ?TYPE ?CYCOBJECT) (#$denotationRelatedTo ?WORD ?TEXT ?TYPE ?CYCOBJECT) )) #$EverythingPSC)",ltext);

	// Last chance just complete its guess
	if (!fnd) {
		// try to find a completion
		sprintf(askbuf,"(fi-complete \"%s\")",text);
		pu->savslot("xcycget",askbuf);

		if (pu->getslot("CYCdata1",askbuf)) {
			p1 = strstr(askbuf,"200 (#$");
			if (p1 != NULL) {
				p1 = p1 + 5;			// go past the "?CYCOBJECT."
				p2 = strstr(p1," ");	//Find the first close
				if (p2 == NULL) p2 = strstr(p1,")");
				*p2 = 0;				// change to end of string
				strcpy(text,p1);		// copy to the return text
			}
		}

	}

	// was it found in a ltext query ?
    if (*ltext == '#' && *text != '#') strcpy(text,ltext);

    if (*text != '#') {
		// try to make a cyc format term anyway
		sprintf(askbuf,"#$%s",text);
		strcpy(text,askbuf);
	}

	if (usetermcache) {
		termcache_timestamp[cacheptr] = now_time;
		strcpy(termcache_value[cacheptr],text);
	}

	delete buf;
	delete askbuf;
	delete ltext;
	return text;
}

char *Core::doCycSystem(char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	docmds(text,buf);
	strcpy(text,buf);
	*buf = 0;
	sprintf(buf,"%s",text);
	pu->savslot("xcycget",buf);
	pu->getslot("CYCdata1",buf);
	strcpy(text,buf);
	if (!strncmp(buf,"200 (NIL)",9)) strcpy(text,"TRUE");
	if (!strncmp(buf,"200 NIL",7)) strcpy(text,"FALSE");
	delete buf;
	return text;
}

char *Core::doCycExp(char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	char *p1;
	char *p2;

	docmds(text,buf);
	strcpy(text,buf);

	sprintf(buf,"%s",text);

	// transform (#$isa #$x #$y) -> x is a y
	infix_phrase(buf,"#$isa"," is a ");

    // transform (#$genls #$x #$y) ->  x is a kind of y
	infix_phrase(buf,"#$genls"," is a kind of ");

	// transform (#$disjointWith #$x #$y) -> x is disjoint with y
	infix_phrase(buf,"#$disjointWith"," is disjoint with ");

    // DeLISP
	while (strstr(buf,"(")) replace(buf,"(","");
	while (strstr(buf,")")) replace(buf,")","");
	while (strstr(buf,":TRUE")) replace(buf,":TRUE"," .");
	while (strstr(buf,"200")) replace(buf,"200","");

	// Now translate each remaining constant
	char *wordbuf1 = new char[128];
	char *wordbuf2 = new char[128];
	
	p1 = strstr(buf,"#$");
	while (p1 != NULL) {
		if (p2 = strstr(p1 + 1," ")) {
			memset(wordbuf1,0,128);
			memset(wordbuf2,0,128);
			strncpy(wordbuf1,p1,(p2 + 1) - p1);
			sprintf(wordbuf2,"(generate-phrase '%s )",wordbuf1);
			pu->savslot("xcycget",wordbuf2);
			pu->getslot("CYCdata1",wordbuf2);
			if (!strstr(wordbuf2,"500")) { 	 // only do successful terms
				replace(wordbuf2,"200 ","");
				replace(wordbuf2,"\r"," ");
				replace(wordbuf2,"\n"," ");
				while(strstr(p1,wordbuf1)) replace(p1,wordbuf1,wordbuf2);
			}
			p1 = strstr(p2 + 1,"#$");
		} else p1 = NULL;
	}
	delete wordbuf1;
	delete wordbuf2;
	strcpy(text,buf);
	delete buf;
	return text;
}

char *Core::doCycAssert(char *text)
{
	char *buf = new char[MAX_BUF_LEN];
	docmds(text,buf);
	strcpy(text,buf);
	*buf = 0;
	sprintf(buf,"(fi-assert '%s)",text);
	pu->savslot("xcycput",buf);
	pu->getslot("CYCdata1",buf);
	strcpy(text,buf);
	if (!strncmp(buf,"200 (NIL)",9)) strcpy(text,"TRUE");
	if (!strncmp(buf,"200 NIL",7)) strcpy(text,"FALSE");
	delete buf;
	return text;
}

char *Core::doCycRetract(char *text)
{
	char *cycAns;
	char *cycDel;
	char *otag = new char[MAX_BUF_LEN];
	char *data = new char[MAX_BUF_LEN];
	char *cycLI = new char[MAX_BUF_LEN];
	char *ctag = new char[MAX_TAG_LEN];
	char *qbuffer = new char[MAX_BUF_LEN];
	char *rbuffer = new char[MAX_BUF_LEN];
	char *p1;
	char *pstringStart;
	char *patomStart;
	char *pstringEnd;
	char *patomEnd;

	//run query and get the answer
	sprintf(qbuffer,"(fi-ask '%s)",text);
	cycAns = doCycSystem(qbuffer);
	*cycLI = 0;

	//translate ?REPLY bindings into <li> tags
	p1 = strstr(cycAns,"?REPLY ");
	if (!p1) {
		// it has no variables so its fully specified
		sprintf(rbuffer,"(fi-unassert '%s )",text);
		cycDel = doCycSystem(rbuffer);
	}

	while (p1) {
	    // is it a string or cycatom ?
		pstringStart = strstr(p1 + 6,"\"");
		patomStart = strstr(p1 + 6,"#$");
		if (pstringStart && (pstringStart < patomStart || !patomStart) ) {
			// its a string
			pstringEnd = strstr(pstringStart + 1,"\"");
			if (pstringEnd) *++pstringEnd = 0;
			sprintf(rbuffer,"(fi-unassert '%s )",text);
			replace(rbuffer,"?REPLY",pstringStart);
			cycDel = doCycSystem(rbuffer);

			strcat(cycLI,rbuffer);
			strcat(cycLI,"\n");
			if (pstringEnd) p1 = pstringEnd + 1; else p1++;
		} else if (patomStart && (patomStart < pstringStart || !pstringStart) ) {
			// its an atom
			patomEnd = strstr(patomStart + 2,")");
			if (patomEnd) *patomEnd = 0;
			sprintf(rbuffer,"(fi-unassert '%s )",text);
			replace(rbuffer,"?REPLY",patomStart);
			cycDel = doCycSystem(rbuffer);
			strcat(cycLI,rbuffer);
			strcat(cycLI,"\n");
			if (patomEnd) p1 = patomEnd + 1; else p1++;
		}

		p1 = strstr(p1,"?REPLY ");
	}

	strcpy(text,cycLI);
	if (!*cycLI) strcpy(text,"()\n"); 
	
	delete otag;
	delete cycLI;
	delete data;
	delete ctag;
	delete qbuffer;
	delete rbuffer;
	return text;
}

char *Core::doCycRandom(char *text)
{
	int i, j, h;
	int pos;
	int last;
	int item;
	int items;
	float cmood;
	double tdist;
	double rnd;

	char *at;
	char *cycAns;
	char *otag = new char[MAX_BUF_LEN];
	char *data = new char[MAX_BUF_LEN];
	char *cycLI = new char[MAX_BUF_LEN];
	char *ctag = new char[MAX_TAG_LEN];
	char *p1;
	char *pstringStart;
	char *patomStart;
	char *pstringEnd;
	char *patomEnd;

	int *list = new int[MAX_RANDOM_LI];
	int *mood = new int[MAX_RANDOM_LI];
	double *sqdist = new double[MAX_RANDOM_LI];
	
	items = 0;
	last = -1;

	//run query and get the answer
	cycAns = doCycSystem(text);
	*cycLI = 0;

	//translate ?REPLY bindings into <li> tags
	p1 = strstr(cycAns,"?REPLY ");
	while (p1 != NULL) {
	    // is it a string or cycatom ?
		pstringStart = strstr(p1 + 6,"\"");
		patomStart = strstr(p1 + 6,"#$");
		if (pstringStart && (pstringStart < patomStart || !patomStart) ) {
			// its a string
			pstringEnd = strstr(pstringStart + 1,"\"");
			if (pstringEnd) *pstringEnd = 0;
			strcat(cycLI,"<li>");
			strcat(cycLI,pstringStart + 1);
			strcat(cycLI,"</li>");
			if (pstringEnd != NULL) p1 = pstringEnd + 1; else p1++;
		}
		else if (patomStart && (patomStart < pstringStart || !pstringStart) ) {
			// its an atom
			patomEnd = strstr(patomStart + 2,")");
			if (patomEnd) *patomEnd = 0;
			strcat(cycLI,"<li>");
			strcat(cycLI,patomStart + 2);
			strcat(cycLI,"</li>");
			if (patomEnd) p1 = patomEnd + 1; else p1++;
		}

		p1 = strstr(p1,"?REPLY ");
	}
	strcpy(text,cycLI);
	if (!*cycLI) strcpy(text,"<li>Unknown</li>"); 
	// just reply
	while ((pos = (int)(strstr(text + last + 1,"<li>") - text)) > -1) {
		list[items] = pos;
		if (XMLnode(text + pos,otag,data,ctag)) {
			last = pos + (int)(strlen(otag) + strlen(data) + strlen(ctag) - 1);
		} else last = pos;
		mood[items++] = calcmood(pos + 4 + text);
	}
	for (i = 0;i < items;i++) {
		for (j = 1;j < items;j++) {
			if (mood[j - 1] < mood[j]) {
				h = mood[j]; mood[j] = mood[j - 1]; mood[j - 1] = h;
				h = list[j]; list[j] = list[j - 1]; list[j - 1] = h;
			}
		}
	}
	cmood = (happyness + 0.5f * helpfullness + humor + affection + 0.5f * trust) / 3 + 0.5f;
	if (cmood < 0) cmood = 0;
	if (cmood > 1) cmood = 1;
	tdist = 0;
	for (i = 0;i < items;i++) tdist += (sqdist[i] = 1.0 / ((cmood - mood[i]) * (cmood - mood[i])));
	for (i = 0;i < items;i++) {
		sqdist[i] *= 1 / tdist;
		if (i > 0) sqdist[i] += sqdist[i - 1];
	}
	rnd = (double)rand() / (double)RAND_MAX;

	item = 0;
	for (i = 0;i < items;i++) {
		if (rnd < sqdist[i]) break;
		item++;
	}
	if (item == items) item = items - 1;

	if (XMLnode(text + list[item],otag,data,ctag)) {
		at = docmds(data,text);
	} else *text = 0;

	delete otag;
	delete cycLI;
	delete data;
	delete ctag;
	delete list;
	delete mood;
	delete sqdist;
	return text;
}

char *Core::doCycCondition(char *tag,char *text)
{
	// basically the same as <condition> but with
	// match replaced by cycquery
	// 
	// <cyccondition query="(#$isa <cycterm><star/></cycterm> #$Mammal>">Its a Mammal</cyccondition>
	//
	// <cyccondition>
	//   <li query="(#$isa <cycterm><star/></cycterm> #$Mammal>">Its a Mammal.</li>
	//   <li query="(#$isa <cycterm><get name="topic"/></cycterm> #$Mammal>">We're talking about mammals.</li>
	//   <li> Its a whatever </li>
	// </cyccondition>
	//

	int fnd = false;
	int cycfnd = false;
	int len;
	char *at;
	char *pos;
	char *ans;
	char *var = new char[MAX_BUF_LEN];
	char *val = new char[MAX_BUF_LEN];
	char *buf = new char[MAX_BUF_LEN];
	char *query = new char [MAX_BUF_LEN];
	char *otag = new char[MAX_BUF_LEN];
	char *data = new char[MAX_BUF_LEN];
	char *ctag = new char[MAX_BUF_LEN];

	if (XMLvalue(tag,"query",query)) {
		ans = doCycSystem(query);
		if (strstr(ans,"TRUE")) {
			docmds(text,buf);
			fnd = true;
		}
	} else {
		at = text;
		while (pos = strstr(at,"<li")) {
			if (XMLnode(pos,otag,data,ctag)) {
				len = (int)(strlen(otag) + strlen(data) + strlen(ctag));
				if (XMLvalue(otag,"query",query)) {
					ans = doCycSystem(query);
					if (strstr(ans,"TRUE")) {
						docmds(data,buf);
						fnd = true;
						break;
					}
				} else if (!strcmp(otag,"<li>")) {
					at = docmds(data,buf);
					fnd = true;
					break;
				}
				at = pos + len - 1;
			} else at++;
		}
	}
	if (fnd) strcpy(text,buf); else *text = 0;
	delete var;
	delete val;
	delete buf;
	delete otag;
	delete data;
	delete ctag;
	delete query;
	return text;
}

// the big loop

bool Core::getAnswer(char *request, char *reply)
{
	int i;
	char *p, *q, *s;
	char delim[8];

	strcpy(delim,".?!;");  // ***  add your own sentence delimiters here  ***
	
	exitplease = false;
	contexts.RemoveAll();
	loadchunks();

	recursion = 0;
	clrscrn = false;

	*reply = 0;
	strcpy(inputs[0],request);
	
	for (p = request;*p;p++) {
		for (q = p;*q && !strchr(delim,*q);q++);
		if (*q) {
			*q++ = 0;
			newstr(&s,p);
			if (*s) rtrim(s);
			if (*s) { respond(s,reply); strcat(reply," "); }
			delete s;
			for (p = q;*p && isspace(*p);p++); p--;
		} else {
			if (*p) rtrim(p);
			if (*p) respond(p,reply);
			break;
		}
	}

	for (i = 4;i > 0;i--) strcpy(inputs[i],inputs[i - 1]);
	strcpy(inputs[0],"");

	for (i = 4;i > 0;i--) strcpy(thats[i],thats[i - 1]);

	char *buf = new char[MAX_BUF_LEN];
	strcpy(buf,reply);
	if (strlen(buf) > MAX_LOADSTRING) buf[MAX_LOADSTRING - 1] = 0;
	strcpy(thats[0],buf);
	delete [] buf;

	for (i = 0;i < 10;i++) sage[i]++;
	
	/*if (exitplease) {
		setvar("topic",topic);
		setvar("that",thats[0]);
		FILE *init = fopen(fnini,"w");
		fprintf(init,client);
		fprintf(init,"\n");
		fclose(init);
		pu->utsave(client);
	}*/

	return exitplease;
}

/* Programming/Implimentation: Arron Boucher.
   Debugging: Sean Barnes.
   08/07/2004
*/

// AIML.cpp: implementation of the AIML Graphmaster classes.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "nodes.h"
#include <stdlib.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Template Link

TemplateLink::TemplateLink()
{
	Next = NULL;
	templatepos = 0;
}

TemplateLink::~TemplateLink()
{
	//RemoveAll();
}

void TemplateLink::Add(fpos_t new_template)
{
	 if (templatepos == 0) {
		 templatepos = new_template;
	 } else {
		if (Next == NULL) Next = new TemplateLink;
		Next->Add(new_template);
	 }
}

void TemplateLink::RemoveAll()
{
	TemplateLink *pLink;
	TemplateLink *pOld;

	 pLink = this->Next;
     while (pLink != NULL) {
		 pOld = pLink;
		 pLink = pLink->Next;
		 delete pOld;
	 }
}

/////////////////////////////////////////////////////////////////////////////
// Node

Node::Node()
{
	TemplateChain = NULL;
	m_template = 0;	
}

Node::~Node()
{
	if (TemplateChain != NULL) {
		TemplateChain->RemoveAll();
		delete TemplateChain;
	}
	RemoveAll();
}

void Node::AddWord(CString key)
{
	Key = key;
	curWord = new Node();

	Node *pTemp = m_WordNodes[key];
	delete pTemp;

	m_WordNodes[key] = curWord;
}

void Node::AddContext(CString key)
{
	Key = key;
	curContext = new Node();

	Node *pTemp = m_ContextNodes[key];
	delete pTemp;

	m_ContextNodes[key] = curContext;
}

void Node::RemoveAll()
{
	CString str;
	Node *pNode;
	POSITION pos = m_WordNodes.GetStartPosition();
	while (pos != NULL) {
		m_WordNodes.GetNextAssoc(pos,str,pNode);
		delete pNode;
	}
	m_WordNodes.RemoveAll();
	pos = m_ContextNodes.GetStartPosition();
	while (pos != NULL) {
		m_ContextNodes.GetNextAssoc(pos,str,pNode);
		delete pNode;
	}
	m_ContextNodes.RemoveAll();
}

bool Node::RemoveWord(CString key)
{
	Node *pWord;
	if (!m_WordNodes.Lookup(key,pWord)) return false;
	m_WordNodes.RemoveKey(key);
	delete pWord;
	return true;
}

bool Node::RemoveContext(CString key)
{
	Node *pContext;
	if (!m_ContextNodes.Lookup(key,pContext)) return false;
	m_ContextNodes.RemoveKey(key);
	delete pContext;
	return true;
}

bool Node::FindWord(CString key)
{
	CString myKey = key;
	myKey.MakeUpper();

	if (!m_WordNodes.Lookup(myKey,curWord)) return false;
	Key = key;
	return true;
}

bool Node::FindContext(CString key)
{
	if (!m_ContextNodes.Lookup(key,curContext)) return false;
	return true;
}

bool Node::GetFirst()
{
	At = m_ContextNodes.GetStartPosition();
	if (At == NULL) return false;
	m_ContextNodes.GetNextAssoc(At,Key,curContext);
	return true;
}

bool Node::GetNext()
{
	if (At == NULL) return false;
	m_ContextNodes.GetNextAssoc(At,Key,curContext);
	if (At == NULL) return false;
	return true;
}

//////////////////////////////////////////////////////////////////////
// Graphmaster: the whole binary tree
//////////////////////////////////////////////////////////////////////

idx::idx()
{
	root = new Node();
	m_size = 0;
}

idx::~idx()
{
	delete root;
}

Node *idx::Select(CString path,Core *World)
{
	Star *s = new Star("input");
	s->m_path = path;
	Node *m = this->select(root,true,"input",path,"",s,World);
	delete s;
	return m;
}

Node *idx::select(Node *node,bool blnContext,CString part,CString str,CString path,Star *star,Core *World)
{
	CString tail;
	CString word;
	CString xtail;
	CString xword;
	CString temp;
	CString cntxt;
	int i;
	int len = str.GetLength();
	int at = 0;
	int thru;
	int more;
	bool lpflg;
	char *buf;
	Star *s;
	TemplateLink *pLink;
	
	Node *m = NULL;

	for (at = 0;at < len;at++) if (str[at] != ' ') break;

	if (at >= len) {

		if (blnContext) return NULL;

		if (node->m_ContextNodes.GetCount() < 1) {
		} else {
			InternContext(part,path,star,World);
			
			// do priority contexts first
			for (i = 0;i < MAX_CONTEXTS;i++) {
				if (World->m_contexts[i] == NULL) break;
				cntxt = World->m_contexts[i];
				if (node->FindContext(cntxt)) {
					s = new Star(World->m_contexts[i]);
					if (!cntxt.Compare("input")) tail = World->m_input;
					else if (!cntxt.Compare("that")) tail = World->m_that;
					else tail = World->getvar(World->m_contexts[i],"");
					s->m_path = tail;
					m = select(node->curContext,true,cntxt,tail,"",s,World);
					delete s;
					if (m != NULL) return m;
				}
			}

			// then do rest of contexts
			lpflg = node->GetFirst();
			while (lpflg) {
				word = node->Key;
				word.MakeLower();
				for (i = 0;i < MAX_CONTEXTS;i++) {
					if (World->m_contexts[i] == NULL) break;
					cntxt = World->m_contexts[i];
					if (cntxt == word) i = MAX_CONTEXTS + 10;
				}
				if (i > MAX_CONTEXTS) {
					buf = new char[word.GetLength() + 2];
					sprintf(buf,"%s",word);
					if (!strcmp(buf,"input")) tail = World->m_input;
					else if (!strcmp(buf,"that")) tail = World->m_that;
					else tail = World->getvar(buf,"");
					s = new Star(buf);
					s->m_path = tail;
					delete buf;
					m = select(node->curContext,true,word,tail,"",s,World);
					delete s;
					if (m != NULL) return m;
				}
				lpflg = node->GetNext();
			}
		}
		// a template is only valid if the guard matches, otherwise its back to searching
		if (node->m_template) {
			InternContext(part,path,star,World);
			if (node->TemplateChain) {
				for (pLink = node->TemplateChain; pLink; pLink = pLink->Next)
					if (World->testguard(node->m_template = pLink->templatepos)) return node;
			} else  if (World->testguard(node->m_template)) return node;
		}
		return m;
	}

	for (thru = at;thru < len;thru++) if (str[thru] == ' ') break;

	word = str.Mid(at,thru - at);
	tail = str.Mid(thru);
	
	if (node->FindWord("_")) {
		star->Push(word);
		m = select(node->curWord,false,part,tail,path + " _",star,World);
		if (m != NULL) return m;
		star->Pop();
		for (more = thru;more < len;more++) if (str[more] != ' ') break;
		while (more < len) {	// try more words until none left
			while (more < len && str[more] != ' ') more++;
			xword = str.Mid(at,more - at);
			xtail = str.Mid(more);
			star->Push(xword);
			m = select(node->curWord,false,part,xtail,path + " _",star,World);
			if (m != NULL) return m;
			star->Pop();
			while (more < len && str[more] == ' ') more++;
		}
	}

	if (node->FindWord(word)) {
		m = select(node->curWord,false,part,tail,path + " " + word,star,World);
		if (m != NULL) return m;
	}

	if (node->FindWord("?")) {
		star->Push(word);
		m = select(node->curWord,false,part,tail,path + " ?",star,World);
		if (m != NULL) return m;
		star->Pop();
	}

	if (node->FindWord("*")) {
		star->Push(word);
		m = select(node->curWord,false,part,tail,path + " *",star,World);
		if (m != NULL) return m;
		star->Pop();
		for (more = thru;more < len;more++) if (str[more] != ' ') break;
		while (more < len) {	// try more words until none left
			while (more < len && str[more] != ' ') more++;
			xword = str.Mid(at,more - at);
			xtail = str.Mid(more);
			star->Push(xword);
			m = select(node->curWord,false,part,xtail,path + " *",star,World);
			if (m != NULL) return m;
			star->Pop();
			while (more < len && str[more] == ' ') more++;
		}
	}

	if (node->FindWord("@")) {
		star->Push(str);
		m = select(node->curWord,false,part,"",path + " *",star,World);
		if (m != NULL) return m;
		star->Pop();
	}

	return NULL;
}

void idx::InternContext(CString t, CString p, Star *s, Core *World)
{
	if (t.GetLength() == 0) return;

	char *buf = new char[MAX_BUF_LEN];
	int i;
	int j = (int)s->m_mvlist.GetCount();
	char *at;

	sprintf(buf,"%s",t);
	lowercase(buf);
	World->contexts.Add(buf);

	p.TrimLeft();
	p.TrimRight();
	World->contexts.curStar->m_path = s->m_path;
	World->contexts.curStar->m_pattern = p;

	for (i = 0;i < j;i++) {
		s->Find(i,buf);
		for (at = buf;*at && *at <= ' ';at++);
		World->contexts.curStar->Push(at);
	}
	delete buf;
}

bool idx::Load(char *fname)
{
	char *line = new char[MAX_BUF_LEN];
	bool rtn = false;
	TCHAR szStatus[1024];
	char wrk[20];
	char *pos;
	fpos_t tpos;

	FILE *fpat = fopen(fname,"r");
	if (fpat) {
		while (!feof(fpat)) {
			fgets(line,MAX_BUF_LEN,fpat);
			if (!strncmp(line,"<eof>",5)) break;

			if (!(pos = strstr(line,"@"))) continue;
			strcpy(wrk,pos + 1);
			rtrim(wrk);
			*pos = 0;
			tpos = atol(wrk);

			Add(line,tpos);
		}
		fclose(fpat);

		wsprintf(szStatus,_T("Finished Learning AIML. (%d categories)"),m_size);

		delete line;
		return true;
	} 
	delete line;
	return false;
}

bool idx::Add(char *str,fpos_t value)
{

	CString path;
	Node *leaf;
	char *cntxt = new char[MAX_BUF_LEN];
	char *line = new char[MAX_BUF_LEN];
	char *pos;
	char *pos2;

	strcpy(line,str);
	strcat(line,"[");
	pos = strstr(line,"[");
	strcpy(cntxt,pos);
	*pos = 0;

	while (strstr(line,"<name/>")) replace(line,"<name/>",bot->getbot("name"));
	while (strstr(line,"<bot/>"))  replace(line,"<bot/>", bot->getbot("name"));

	rtrim(line);
	strcat(line,":");
	replace(line,"*:","@");
	replace(line,":","");

	path = line;
	leaf = add(root,path);

	// now pick the priority contexts
	// todo: fix this to read the list from a configuration file

	if (pos = strstr(cntxt,"[that]")) {
		strcpy(line,pos);
		pos2 = strstr(line + 1,"[");
		*pos2 = 0;
		replace(cntxt,line,"");
		leaf = addCntxt(leaf,line);
	}

	if (pos = strstr(cntxt,"[topic]")) {
		strcpy(line,pos);
		pos2 = strstr(line + 1,"[");
		*pos2 = 0;
		replace(cntxt,line,"");
		leaf = addCntxt(leaf,line);
	}

	// then go for what contexts are left
	while (strcmp(cntxt,"[")) {
		strcpy(line,pos);
		pos2 = strstr(line + 1,"[");
		*pos2 = 0;
		replace(cntxt,line,"");
		leaf = addCntxt(leaf,line);
	}

	// finally link in the template
	leaf->m_template = value; // Original: overwrites duplicate templates

	// New: merge everything into a linked list
	if (leaf->TemplateChain == NULL) leaf->TemplateChain = new TemplateLink;
	leaf->TemplateChain->Add(value);

	m_size++;

	delete line;
	delete cntxt;
	return true;
}

Node *idx::addCntxt(Node *parent, char *str) {
	char *name = new char[MAX_STAR_LEN];
	char *line = new char[MAX_BUF_LEN];
	char *pos;

	strcpy(name,str + 1);
	pos = strstr(name,"]");
	*pos = 0;

	strcpy(line,pos + 1);
	rtrim(line);
	strcat(line,":");
	replace(line,"*:","@");
	replace(line,":","");

	CString cntxt = name;
	CString patrn = line;

	delete name;
	delete line;

	cntxt.MakeLower();

	if (!parent->FindContext(cntxt)) parent->AddContext(cntxt);
	return add(parent->curContext,patrn);
}

Node *idx::add(Node *parent,CString str) {
	CString tail;
	CString word;
	int len = str.GetLength();
	int at = 0;
	int thru;

	for (at = 0;at < len;at++) if (str[at] != ' ') break;

	if (at >= len) return parent;

	for (thru = at;thru < len;thru++) if (str[thru] == ' ') break;

	word = str.Mid(at,thru - at);
	tail = str.Mid(thru);

	word.MakeUpper();

	if (!parent->FindWord(word)) parent->AddWord(word);
	return add(parent->curWord,tail);
}

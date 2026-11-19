/* Programming/Implimentation: Arron Boucher.
   Debugging: Sean Barnes.
   08/07/2004
*/

// facts.cpp: implementation of the Facts class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "facts.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Fact

Fact::Fact()
{
	timestamp(m_timestamp);
}

Fact::Fact(char *attr,char *value,char *source,double cf)
{
	m_attribute = attr;
	m_value = value;
	m_source = source;
	m_cf = cf;
	timestamp(m_timestamp);
}

Fact::~Fact()
{
}

void Fact::Push(char *value)
{
	CString t;

	t.Format(_T("%hs"),value);
	m_mvlist.AddTail(t);
}

void Fact::Queue(char *value)
{
	CString t;

	t.Format(_T("%hs"),value);
	m_mvlist.AddHead(t);
}

bool Fact::Pop(char *value)
{
	CString t;

	if (m_mvlist.IsEmpty()) return false;
	t = m_mvlist.RemoveTail();
	sprintf(value,"%s",t);
	return true;
}

bool Fact::Find(int n,char *value)
{
	CString t;

	POSITION pos = m_mvlist.FindIndex(n);
	if (pos == NULL) return false;
	t = m_mvlist.GetAt(pos);
	sprintf(value,"%s",t);
	return true;
}

bool Fact::Insert(int n,char *value)
{
	CString t;

	POSITION pos = m_mvlist.FindIndex(n);
	if (pos == NULL) return false;
	t.Format(_T("%hs"),value);
	m_mvlist.InsertBefore(pos,t);
	return true;
}

bool Fact::Extract(int n,char *value)
{
	CString t;

	POSITION pos = m_mvlist.FindIndex(n);
	if (pos == NULL) return false;
	t = m_mvlist.GetAt(pos);
	m_mvlist.RemoveAt(pos);
	sprintf(value,"%s",t);
	return true;
}

void Fact::FormatFact(CString& str)
{
	str = _T("");
	if (m_attribute.IsEmpty()) return;

	if (m_cf == 1.00) {
		if (m_attribute.Right(1) != "_") str.Format(_T("%s=%s"),m_attribute,m_value);
		else str.Format(_T("%s%s"),m_attribute,m_value);
	} else {
		if (m_attribute.Right(1) != "_") str.Format(_T("%s[%f]=%s"),m_attribute,m_cf,m_value);
		else str.Format(_T("%s%s [%f]"),m_attribute,m_value,m_cf);
	}

	if (m_mvlist.IsEmpty()) return;

	POSITION pos = m_mvlist.GetHeadPosition();
	while (pos != NULL) {
		str += _T("\r\n=");
		str += m_mvlist.GetNext(pos);
	}
}

void Fact::XMLFact(CString& str)
{
	if (m_cf == 1.00) str.Format(_T("<fact name=\"%s\">%s</fact>"),m_attribute,m_value);
	else str.Format(_T("<fact name=\"%s\" cf=\"%f\">%s</fact>"),m_attribute,m_cf,m_value);
}

//////////////////////////////////////////////////////////////////////
// Facts Construction/Destruction
//////////////////////////////////////////////////////////////////////

Facts::Facts()
{
	curFact = NULL;
	At = NULL;
}

Facts::~Facts()
{
	RemoveAll();
}

void Facts::Add(char *attr,char *value,char *source,double cf)
{
	Key = attr;
	curFact = new Fact(attr,value,source,cf);

	Fact *pTemp = m_mapStringToFact[Key];
	delete pTemp;

	m_mapStringToFact[Key] = curFact;
	At = NULL;
}

void Facts::Put(CString attr,CString value,CString source,double cf)
{
	Key = attr;
	curFact = new Fact();
	curFact->m_attribute = attr;
	curFact->m_value = value;
	curFact->m_source = source;
	curFact->m_cf = cf;

	Fact *pTemp = m_mapStringToFact[Key];
	delete pTemp;

	m_mapStringToFact[Key] = curFact;
	At = NULL;
}

void Facts::Add(char *attr,char *value)
{
	Key = attr;
	curFact = new Fact(attr,value,"calc",1.0);

	Fact *pTemp = m_mapStringToFact[Key];
	delete pTemp;

	m_mapStringToFact[Key] = curFact;
	At = NULL;
}

void Facts::RemoveAll()
{
	POSITION pos = m_mapStringToFact.GetStartPosition();
	while (pos != NULL) {
		CString str;
		Fact *pFact;
		m_mapStringToFact.GetNextAssoc(pos, str, pFact);
		delete pFact;
	}
	m_mapStringToFact.RemoveAll();
	At = NULL;
	Key = "";
	curFact = NULL;
}

bool Facts::Remove(char *attr)
{
	CString key(attr);
	Fact *pFact;
	if (!m_mapStringToFact.Lookup(key,pFact)) return false;
	m_mapStringToFact.RemoveKey(key);
	delete pFact;
	At = NULL;
	Key = "";
	curFact = NULL;
	return true;
}

bool Facts::Seek(CString attr)
{
	Key = attr;
	if (!m_mapStringToFact.Lookup(Key,curFact)) return false;
	At = NULL;
	return true;
}

bool Facts::Find(char *attr)
{
	Key = attr;
	if (!m_mapStringToFact.Lookup(Key,curFact)) return false;
	At = NULL;
	return true;
}

bool Facts::GetFirst()
{
	At = m_mapStringToFact.GetStartPosition();
	if (At == NULL) return false;
	m_mapStringToFact.GetNextAssoc(At,Key,curFact);
	return true;
}

bool Facts::GetNext()
{
	if (At == NULL) return false;
	m_mapStringToFact.GetNextAssoc(At,Key,curFact);
	if (At == NULL) return false;
	return true;
}

void Facts::FormatFacts(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToFact.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Fact *pFact;
		m_mapStringToFact.GetNextAssoc(pos, key, pFact);
		pFact->FormatFact(s);
		str += s;
		str += TEXT("\r\n");
	}
}

void Facts::XMLFacts(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToFact.GetStartPosition();

	str = "<facts>";
	while (pos != NULL) {
		CString key;
		Fact *pFact;
		m_mapStringToFact.GetNextAssoc(pos, key, pFact);
		pFact->XMLFact(s);
		str += s;
	}
	str += "</facts>";
}

/////////////////////////////////////////////////////////////////////////////
// Case

Case::Case()
{
}

Case::Case(char *name)
{
	m_name = name;
	timestamp(m_timestamp);
}

Case::~Case()
{
	m_facts.RemoveAll();
}

void Case::FormatCase(CString& str)
{
	CString s;

	m_facts.FormatFacts(s);
	str.Format(_T("Case(%s):\r\n%s\r\n"),m_name,s);
}

void Case::XMLCase(CString& str)
{
	CString s;

	m_facts.XMLFacts(s);
	str.Format(_T("<case name=\"%s\">%s</case>"),m_name,s);
}

//////////////////////////////////////////////////////////////////////
// Cases Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cases::Cases()
{
	curCase = NULL;
	curGbl = NULL;
	At = NULL;
}

Cases::~Cases()
{
	RemoveAll();
}

void Cases::Add(char *name)
{
	Key = name;
	curCase = new Case(name);

	Case *pTemp = m_mapStringToCase[Key];
	delete pTemp;

	m_mapStringToCase[Key] = curCase;
}

void Cases::AddGbl()
{
	Key = "~~global~~";
	curGbl = new Case("~~global~~");

	Case *pTemp = m_mapStringToCase[Key];
	delete pTemp;

	m_mapStringToCase[Key] = curGbl;
}

void Cases::AddShare()
{
	CString key = "~~common share~~";
	Case *pCase = new Case("~~common share~~");

	Case *pTemp = m_mapStringToCase[key];
	delete pTemp;

	m_mapStringToCase[key] = pCase;
}

void Cases::RemoveAll()
{
	POSITION pos = m_mapStringToCase.GetStartPosition();
	while (pos != NULL) {
		CString str;
		Case *pCase;
		m_mapStringToCase.GetNextAssoc(pos, str, pCase);
		pCase->m_facts.RemoveAll();
		delete pCase;
	}
	m_mapStringToCase.RemoveAll();
	At = NULL;
	Key = "";
	curCase = NULL;
	curGbl = NULL;
}

bool Cases::Remove(char *name)
{
	CString key(name);
	Case *pCase;
	if (!m_mapStringToCase.Lookup(key,pCase)) return false;
	m_mapStringToCase.RemoveKey(key);
	delete pCase;
	At = NULL;
	Key = "";
	curCase = NULL;
	return true;
}

bool Cases::Find(char *name)
{
	Case *fndCase;
	Key = name;
	if (!m_mapStringToCase.Lookup(Key,fndCase)) return false;
	curCase = fndCase;
	return true;
}

bool Cases::Seek(CString name)
{
	Key = name;
	if (!m_mapStringToCase.Lookup(Key,curCase)) return false;
	return true;
}

bool Cases::GetFirst()
{
	At = m_mapStringToCase.GetStartPosition();
	if (At == NULL) return false;
	m_mapStringToCase.GetNextAssoc(At,Key,curCase);
	return true;
}

bool Cases::GetNext()
{
	if (At == NULL) return false;
	m_mapStringToCase.GetNextAssoc(At,Key,curCase);
	if (At == NULL) return false;
	return true;
}

void Cases::ListCases(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToCase.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Case *pCase;
		m_mapStringToCase.GetNextAssoc(pos, key, pCase);
		str += pCase->m_name;
		if (pos != NULL) str += TEXT(", ");
	}
}

void Cases::FormatCases(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToCase.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Case *pCase;
		m_mapStringToCase.GetNextAssoc(pos, key, pCase);
		pCase->FormatCase(s);
		str += s;
	}
}

void Cases::FileCases(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToCase.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Case *pCase;
		m_mapStringToCase.GetNextAssoc(pos, key, pCase);
		pCase->FormatCase(s);
		str += s;
	}
}

void Cases::XMLCases(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToCase.GetStartPosition();

	str = "<cases>";
	while (pos != NULL) {
		CString key;
		Case *pCase;
		m_mapStringToCase.GetNextAssoc(pos, key, pCase);
		pCase->XMLCase(s);
		str += s;
	}
	str += "</cases>";
}

/////////////////////////////////////////////////////////////////////////////
// Subject

Subject::Subject(char *name)
{
	m_name = name;
	timestamp(m_timestamp);
}

Subject::~Subject()
{
	m_cases.RemoveAll();
}

void Subject::FormatSubject(CString& str)
{
	CString s;

	m_cases.FormatCases(s);
	str.Format(_T("Subject(%s):\r\n%s"),m_name,s);
}

void Subject::XMLSubject(CString& str)
{
	CString s;

	m_cases.XMLCases(s);
	str.Format(_T("<subject name=\"%s\">%s</subject>"),m_name,s);
}

//////////////////////////////////////////////////////////////////////
// Subjects Construction/Destruction
//////////////////////////////////////////////////////////////////////

Subjects::Subjects()
{
	curSub = NULL;
	curLoc = NULL;
	curTag = NULL;
	At = NULL;
	srand( (unsigned)time( NULL ) );
}

Subjects::~Subjects()
{
	RemoveAll();
}

void Subjects::Add(char *name)
{
	Key = name;
	curSub = new Subject(name);

	Subject *pTemp = m_mapStringToSubject[Key];
	delete pTemp;

	m_mapStringToSubject[Key] = curSub;
}

void Subjects::AddLoc(char *name)
{
	Key = name;
	curLoc = new Subject(name);

	Subject *pTemp = m_mapStringToSubject[Key];
	delete pTemp;

	m_mapStringToSubject[Key] = curLoc;
}

void Subjects::AddTag(char *name)
{
	Key = name;
	curTag = new Subject(name);

	Subject *pTemp = m_mapStringToSubject[Key];
	delete pTemp;

	m_mapStringToSubject[Key] = curTag;
}

void Subjects::RemoveAll()
{
	POSITION pos = m_mapStringToSubject.GetStartPosition();
	while (pos != NULL) {
		CString str;
		Subject *pSubject;
		m_mapStringToSubject.GetNextAssoc(pos, str, pSubject);
		delete pSubject;
	}
	m_mapStringToSubject.RemoveAll();
	At = NULL;
	Key = "";
	curSub = NULL;
	curLoc = NULL;
	curTag = NULL;
}

bool Subjects::Remove(char *name)
{
	CString key(name);
	Subject *pSubject;
	if (!m_mapStringToSubject.Lookup(Key,pSubject)) return false;
	m_mapStringToSubject.RemoveKey(key);
	pSubject->m_cases.RemoveAll();
	delete pSubject;
	At = NULL;
	Key = "";
	curSub = NULL;
	return true;
}

bool Subjects::Find(char *name)
{
	Subject *fndSub;
	Key = name;
	if (!m_mapStringToSubject.Lookup(Key,fndSub)) return false;
	curSub = fndSub;
	return true;
}

bool Subjects::GetFirst()
{
	At = m_mapStringToSubject.GetStartPosition();
	if (At == NULL) return false;
	m_mapStringToSubject.GetNextAssoc(At,Key,curSub);
	return true;
}

bool Subjects::GetNext()
{
	if (At == NULL) return false;
	m_mapStringToSubject.GetNextAssoc(At,Key,curSub);
	if (At == NULL) return false;
	return true;
}

void Subjects::ListSubjects(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToSubject.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Subject *pSubject;
		m_mapStringToSubject.GetNextAssoc(pos, key, pSubject);
		pSubject->m_cases.ListCases(s);
		str += _T("Subject:");
		str += pSubject->m_name;
		str += TEXT("\r\n  Cases:");
		str += s;
		str += TEXT("\r\n");
	}
}

void Subjects::FormatSubjects(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToSubject.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Subject *pSubject;
		m_mapStringToSubject.GetNextAssoc(pos, key, pSubject);
		pSubject->FormatSubject(s);
		str += s;
		str += TEXT("\r\n");
	}
}

void Subjects::XMLSubjects(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToSubject.GetStartPosition();

	str = "<subjects>";
	while (pos != NULL) {
		CString key;
		Subject *pSubject;
		m_mapStringToSubject.GetNextAssoc(pos, key, pSubject);
		pSubject->XMLSubject(s);
		str += s;
	}
	str += "</subjects>";
}


//  Access Utilities

bool Subjects::getValue(char *name,char *val)
{
	Subject *fndSub;
	CString key = "New Cases";
	Case *pCase = curSub->m_cases.curCase;
	Case *qCase;
	char *k;

	if (*name == '%') {
		if (curLoc == NULL) return false;
		qCase = curLoc->m_cases.curCase;
		if (qCase == NULL) return false;
		if (qCase->m_facts.Find(name + 1)) {
			sprintf(val,"%s",qCase->m_facts.curFact->m_value);
			return true;
		}
		return false;
	}

	if (curSub != NULL) {
		qCase = pCase;
		if (qCase != NULL) {
			if (qCase->m_facts.Find(name)) {
				sprintf(val,"%s",qCase->m_facts.curFact->m_value);
				return true;
			}
		}
		if (curTag != NULL) {
			qCase = curTag->m_cases.curCase;
			if (qCase != NULL) {
				if (qCase->m_facts.Find(name)) {
					sprintf(val,"%s",qCase->m_facts.curFact->m_value);
					return true;
				}
			}
		}
		qCase = pCase;
		while (qCase->m_facts.Find("General")) {
			k = new char[qCase->m_facts.curFact->m_value.GetLength() + 2];
			sprintf(k,"%s",qCase->m_facts.curFact->m_value);
			if (curSub->m_cases.Find(k)) {
				delete k;
				qCase = curSub->m_cases.curCase;
				if (qCase->m_facts.Find(name)) {
					sprintf(val,"%s",qCase->m_facts.curFact->m_value);
					curSub->m_cases.curCase = pCase;
					return true;
				}	
			} else { delete k; break; }
		}
		curSub->m_cases.curCase = pCase;

		qCase = curSub->m_cases.curGbl;
		if (qCase != NULL) {
			if (qCase != pCase) {
				if (qCase->m_facts.Find(name)) {
					sprintf(val,"%s",qCase->m_facts.curFact->m_value);
					return true;
				}
			}
		}

		if (m_mapStringToSubject.Lookup(key,fndSub)) {
			pCase = fndSub->m_cases.curCase;
			if (fndSub->m_cases.Find("~~common share~~")) {
				qCase = fndSub->m_cases.curCase;
				if (qCase->m_facts.Find(name)) {
					sprintf(val,"%s",qCase->m_facts.curFact->m_value);
					fndSub->m_cases.curCase = pCase;
					return true;
				}
				fndSub->m_cases.curCase = pCase;
			}
		}
	}

	k = val;
	if (!strcmp(name,"KM")) { *k++ = -1; *k = 0; return true; }
	if (!strcmp(name,"RM")) { *k++ = -2; *k = 0; return true; }
	if (!strcmp(name,"FM")) { *k++ = -3; *k = 0; return true; }
	if (!strcmp(name,"VM")) { *k++ = -4; *k = 0; return true; }
	if (!strcmp(name,"SM")) { *k++ = -5; *k = 0; return true; }
	if (!strcmp(name,"TM")) { *k++ = -6; *k = 0; return true; }
	if (!strcmp(name,"CR")) { sprintf(val,"\r"); return true; }
	if (!strcmp(name,"CRLF")) { sprintf(val,"\r\n"); return true; }
	if (!strcmp(name,"LF")) { sprintf(val,"\n"); return true; }
	if (!strcmp(name,"CM")) { strcpy(val,",");   return true; }
	if (!strcmp(name,"SP")) { strcpy(val," ");   return true; }
	if (!strcmp(name,"DM")) { strcpy(val,"|");   return true; }
	if (!strcmp(name,"QM")) { sprintf(val,"\""); return true; }

	if (!strcmp(name,"SUBJECT")) { sprintf(val,"%s",curSub->m_name);                   return true; }
	if (!strcmp(name,"CASE"))    { sprintf(val,"%s",curSub->m_cases.curCase->m_name);  return true; }
	if (!strcmp(name,"STACK"))   { sprintf(val,"%s",curLoc->m_cases.curCase->m_name);  return true; }
	if (!strcmp(name,"NOW"))     { getdts(val);                                        return true; }
	if (!strcmp(name,"DATE"))    { getdate(val);                                       return true; }
	if (!strcmp(name,"TIME"))    { gettime(val);                                       return true; }
	if (!strcmp(name,"RANDOM"))  { sprintf(val,"%Lf",(double)rand() / (double)RAND_MAX); return true; }
	if (!strcmp(name,"PATH"))    { sprintf(val,fullpath(""));                          return true; }

	return false;
}

bool Subjects::putValue(char *name,char *val)
{
	Case *qCase;

	if (*name == '%' && curLoc != NULL) {
		qCase = curLoc->m_cases.curCase;
		if (qCase != NULL) {
			qCase->m_facts.Add(name + 1,val);
			return true;
		}
		return false;
	}
	if (*name == '(' && curTag != NULL) {
		qCase = curTag->m_cases.curCase;
		if (qCase != NULL) {
			qCase->m_facts.Add(name + 1,val);
			return true;
		}
		return false;
	}
	if (curSub != NULL) {
		qCase = curSub->m_cases.curCase;
		if (qCase != NULL) {
			qCase->m_facts.Add(name,val);
			return true;
		}
	}
	return false;
}

bool Subjects::putGlobal(char *name,char *val)
{
	Case *qCase;

	if (curSub != NULL) {
		qCase = curSub->m_cases.curGbl;
		if (qCase != NULL) {
			qCase->m_facts.Add(name,val);
			return true;
		}
	}
	return false;
}

bool Subjects::putShare(char *name,char *val)
{
	Subject *fndSub;
	CString key = "New Cases";
	Case *pCase;
	Case *qCase;

	if (m_mapStringToSubject.Lookup(key,fndSub)) {
		pCase = fndSub->m_cases.curCase;
		if (fndSub->m_cases.Find("~~common share~~")) {
			qCase = fndSub->m_cases.curCase;
			qCase->m_facts.Add(name,val);
			fndSub->m_cases.curCase = pCase;
			return true;
		}
		fndSub->m_cases.curCase = pCase;
	}
	return false;
}

bool Subjects::delValue(char *name)
{
	Case *pCase;

	if (curSub != NULL) {
		pCase = curSub->m_cases.curCase;
		if (pCase != NULL) {
			if (pCase->m_facts.Remove(name)) return true;
		}
	}
	if (curLoc != NULL) {
		pCase = curLoc->m_cases.curCase;
		if (pCase != NULL) {
			if (pCase->m_facts.Remove(name)) return true;
		}
	}
	if (curTag != NULL) {
		pCase = curTag->m_cases.curCase;
		if (pCase != NULL) {
			if (pCase->m_facts.Remove(name)) return true;
		}
	}
	return false;
}

Fact *Subjects::getFact(CString name)
{
	Case *pCase = curSub->m_cases.curCase;
	Case *qCase;

	if (name.Left(1) == '%') {
		if (curLoc == NULL) return NULL;
		qCase = curLoc->m_cases.curCase;
		if (qCase != NULL) if (qCase->m_facts.Seek(name.Mid(1))) return qCase->m_facts.curFact;
		return NULL;
	}
	if (curSub != NULL) {
		qCase = pCase;
		if (qCase != NULL) if (qCase->m_facts.Seek(name)) return qCase->m_facts.curFact;
		if (curTag != NULL) {
			qCase = curTag->m_cases.curCase;
			if (qCase != NULL) if (qCase->m_facts.Seek(name)) return qCase->m_facts.curFact;
		}
		qCase = pCase;
		while (qCase->m_facts.Find("General")) {
			if (curSub->m_cases.Seek(qCase->m_facts.curFact->m_value)) {
				qCase = curSub->m_cases.curCase;
				if (qCase->m_facts.Seek(name)) {
					curSub->m_cases.curCase = pCase;
					return qCase->m_facts.curFact;
				}	
			} else break;
		}
		curSub->m_cases.curCase = pCase;

		qCase = curSub->m_cases.curGbl;
		if (qCase == pCase) return NULL;
		if (qCase != NULL) if (qCase->m_facts.Seek(name)) return qCase->m_facts.curFact;
	}
	return NULL;
}

//
// calculator		/* a simple reverse polish stacking calculator? (echoes of fortran) */
//

char *Subjects::varname(char *a,char **t)
{ 
	char *buf = new char[strlen(a) + 2];
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
	newstr(t,buf);
	delete buf;
	while (*j && isspace(*j)) j++;
	return j;
}

int Subjects::stkint(char *p)
{
	int x;

	if (!*p) x = 0; else x = atoi(p);
	delete p;
	return (x);
}

double Subjects::stknum(char *p)
{
	double x;

	if (!*p) x = 0.0; else x = atof(p);
	delete p;
	return (x);
}

char *Subjects::stksav(double z)
{
	int i;
	char *p = CNIL;
	char buf[20];

	sprintf(buf,"%#.15f",z);
	i = (int)strlen(buf) - 1;
	if (i > 0) {
		while (i && *(buf + i) == '0') buf[i--] = 0;
		if (*(buf + i) == '.') *(buf + i) = 0;
	}
	newstr(&p,buf);
	return (p);
}

int Subjects::fourbang(int h,int op)
{
	double x, y, z;         

	x = stknum(hold[h--]);
	y = stknum(hold[h]);
	switch (op) {
		case  4: z = y - x;      break;
		case  5: z = y + x;      break;
		case  6: z = y / x;      break;
		case  7: z = y * x;      break;
		case  8: z = pow(x,y);   break; 
		case 22: z = fmod(y,x);  break;
	}
	hold[h] = stksav(z);
	return h;
}

int Subjects::funcbang(int h,int op)
{
	double y, z;         

	y = stknum(hold[h]);
	switch (op) {
		case 10: z = fabs(y);     break;
		case 11: z = sin(y);      break;
		case 12: z = cos(y);      break;
		case 13: z = tan(y);      break;
		case 14: z = atan(y);     break;
		case 15: z = floor(y);    break; // floor and int
		case 16: z = ceil(y);     break;
		case 17: z = log10(y);    break;
		case 18: z = pow(10.0,y); break;
		case 19:
			if (y < 0.0) y = -y;
			z = sqrt(y);
			break;
		case 20: z = exp(y);      break;
		case 21: z = y * rand() / RAND_MAX; break;
	}
	hold[h] = stksav(z);
	return h;
}

int Subjects::confunc(int h)
{          
	char *p = new char[strlen(hold[h - 1]) + strlen(hold[h]) + 2];

	strcpy(p,hold[h - 1]);
	strcat(p,hold[h]);
	delete hold[h--];
	delete hold[h];
	hold[h] = p;          
	return h;
}
 
int Subjects::lenfunc(int h)
{
	double x, y; 

	x = strlen(hold[h]);
	y = stknum(hold[h]);
	hold[h] = stksav(x);
	return h;
}
 
int Subjects::cntfunc(int h)
{
	double x = 0; 
	CString name(hold[h]);
	double y; 
	Case *pCase = curSub->m_cases.curCase;
	Case *qCase;
	bool notfnd = true;

	if (name.Left(1) == '%') {
		if (curLoc != NULL) {
			qCase = curLoc->m_cases.curCase;
			if (qCase != NULL) {
				if (qCase->m_facts.Seek(name.Mid(1)))
					x = qCase->m_facts.curFact->m_mvlist.GetCount() + 1;
			}
		}
	} else 	if (curSub != NULL) {
		qCase = pCase;
		if (name.Left(1) == '$') name = name.Mid(1);
		if (qCase != NULL) {
			if (qCase->m_facts.Seek(name)) {
				x = qCase->m_facts.curFact->m_mvlist.GetCount() + 1;
				notfnd = false;
			}
		}
		if (notfnd) {
			while (qCase->m_facts.Find("General")) {
				if (curSub->m_cases.Seek(qCase->m_facts.curFact->m_value)) {
					qCase = curSub->m_cases.curCase;
					if (qCase->m_facts.Seek(name)) {
						x = qCase->m_facts.curFact->m_mvlist.GetCount() + 1;
						notfnd = false;
						break;
					}
				} else break;
			}	
			curSub->m_cases.curCase = pCase;
		}

		if (notfnd) {
			qCase = curSub->m_cases.curGbl;
			if (qCase != pCase) {
				if (qCase != NULL) if (qCase->m_facts.Seek(name)) 
					x = qCase->m_facts.curFact->m_mvlist.GetCount() + 1;
			}
		}
	}

	y = stknum(hold[h]);
	hold[h] = stksav(x);
	return h;
}

int Subjects::trimfunc(int h)
{
	char *a, *b;
	int w;

	for (w = 0,a = hold[h],b = a;*a;a++)
		if (w > 0 || *a > ' ') { *b++ = *a; w++; }
	for (a = b,a--;w > 0 && isspace(*a--);w--,b--); *b = 0;
	return h;
}

int Subjects::numfunc(int h)
{
	char *p = CNIL;
	char *a;
	int k, w, m, y;
	char buf[128];

	y = stkint(hold[h--]);       
	for (w = 0,a = hold[h];*a && *a != '.';w++,a++); m = w;
	if (*a == '.') {
		for (w++,a++;isdigit(*a) && y > 0;a++,w++,y--); *a = 0;
		strcpy(buf,hold[h]);
	} else {
		strcpy(buf,hold[h]);
		strcat(buf,".");
	}                 
	while (y-- > 0) strcat(buf,"0");
	newstr(&p,buf);
	for (a = p,w = 0;*a;a++) {
		k = m / 3;
		if (m > 0 && m-- == k + k + k && w > 0) buf[w++] = ',';
		buf[w++] = *a;
	}
	buf[w] = 0;
	delete p;
	newstr(&p,buf);
	delete hold[h];
	hold[h] = p;          
	return h;
}

int Subjects::decfunc(int h)
{
	char *p = CNIL;
	char *a;
	int w, y;
	char buf[128];

	y = stkint(hold[h--]);       
	for (w = 0,a = hold[h];*a && *a != '.';w++,a++);
	if (*a == '.') {
		for (w++,a++;isdigit(*a) && y > 0;a++,w++,y--); *a = 0;
		strcpy(buf,hold[h]);
	} else {
		strcpy(buf,hold[h]);
		strcat(buf,".");
	}                 
	while (y-- > 0) strcat(buf,"0");
	newstr(&p,buf);
	delete hold[h];
	hold[h] = p;          
	return h;
}

int Subjects::padfunc(int h)
{
	char *p = CNIL;
	int k, w;
	char buf[128];

	k = stkint(hold[h--]);
	strcpy(buf,hold[h]);
	w = (int)strlen(buf);          
	if (k > w) for (k -= w;k-- > 0;strcat(buf," ")); else buf[k] = 0;
	newstr(&p,buf);
	delete hold[h];
	hold[h] = p;          
	return h;
}

int Subjects::justfunc(int h)
{
	char *p = CNIL;
	int k, w, y;         
	char buf[128];
 
	y = stkint(hold[h--]);       
	strcpy(buf,hold[h]);
	w = (int)strlen(buf);                                               
	while (w++ < y) { for (k = w;k > 0;k--) buf[k] = buf[k - 1]; *buf = ' '; }
	if (w > y) w = y; buf[w] = 0;
	newstr(&p,buf);
	delete hold[h];
	hold[h] = p;          
	return h;
}

int Subjects::zerofunc(int h)
{
	char *p = CNIL;
	int k, w, y;
	char buf[128];

	y = stkint(hold[h--]);       
	strcpy(buf,hold[h]);
	w = (int)strlen(buf);                                               
	while (w++ < y) { for (k = w;k > 0;k--) buf[k] = buf[k - 1]; *buf = '0'; }
	newstr(&p,buf);
	delete hold[h];
	hold[h] = p;          
	return h;
}

int Subjects::midfunc(int h)
{
	char *p, *a, *b;
	int k, w, x;         

	x = stkint(hold[h--]);       
	k = stkint(hold[h--]);
	p = hold[h];
	w = (int)strlen(p);
	char *buf = new char[w + 2];
	if (w > k--) {
		for (a = p + k,b = buf;*a && x > 0;x--,*b++ = *a++); *b = 0;
	} else strcpy(buf,"");
	newstr(&p,buf);
	delete buf;
	delete hold[h];
	hold[h] = p;
	return h;
}

int Subjects::fldfunc(int h)
{
	char *p, *a, *b;
	int k, y;

	b = hold[h];
	if (isdigit(*b)) k = stkint(hold[h--]); else { h--; k = *b; delete b; }
	y = stkint(hold[h--]);
	p = hold[h];
	char *buf = new char[strlen(p) + 2];
	for (a = p;*a && y > 1;a++) if (*a == k) y--;
	for (b = buf;*a && *a != k;*b++ = *a++); *b = 0;
	delete p;
	newstr(&p,buf);
	delete buf;
	hold[h] = p;          
	return h;
}
 
int Subjects::chgfunc(int h)
{
	char *a;
	char *b;
	int k, w;

	b = hold[h];
	if (isdigit(*b)) k = stkint(hold[h--]); else { h--; k = *b; delete b; }
	b = hold[h];
	if (isdigit(*b)) w = stkint(hold[h--]); else { h--; w = *b; delete b; }
	for (a = hold[h];*a;a++) if (*a == w) *a = k;
	return h;
}
 
int Subjects::replacefunc(int h)
{
	char *a;
	char *b;
	char *c;
	char *buf = new char[MAX_SIZE];

	b = hold[h--];
	a = hold[h--];
	strcpy(buf,hold[h]);
	replace(buf,a,b);
	delete a;
	delete b;
	newstr(&c,buf);
	delete buf;
	delete hold[h];
	hold[h] = c;
	return h;
}
 
int Subjects::findfunc(int h)
{
	char *a;
	char *b;
	char *c;
	char *buf = new char[MAX_SIZE];
	int i;

	b = hold[h--];
	a = hold[h];
	if (c = strstr(a,b)) 
		i = (int)(c - a + 1); 
	else 
		i = 0;
	delete a;
	delete b;
	sprintf(buf,"%d",i);
	newstr(&c,buf);
	delete buf;
	hold[h] = c;
	return h;
}

int Subjects::leftfunc(int h)
{
	char *a;
	char *b;
	char c;
	UINT i;

	b = hold[h];
	if (isdigit(*b)) {
		i = stkint(hold[h--]);
		a = hold[h];
	} else {
		h--;
		c = *b;
		delete b;
		a = hold[h];
		for (i = 0;a[i] && a[i] != c;i++);
	}
	if (strlen(a) > i) {
		c = a[i];
		a[i] = 0;
		newstr(&b,a);
		a[i] = c;
		delete a;
		hold[h] = b;
	}
	return h;
}

int Subjects::rightfunc(int h)
{
	char *a;
	char *b;
	char *c;
	char chr;
	UINT i;

	b = hold[h];
	if (isdigit(*b)) {
		i = stkint(hold[h--]);
		a = hold[h];
	} else {
		h--;
		chr = *b;
		delete b;
		a = hold[h];
		for (i = (int)strlen(a);i >= 0 && a[i] != chr;i--);
		i = (int)strlen(a) - i;
	}
	if (strlen(a) > i) {
		i = (int)strlen(a) - i;
		c = a + i;
		newstr(&b,c);
		delete a;
		hold[h] = b;
	}
	return h;
}
 
int Subjects::tagfunc(int h)
{
	char *x;
	char *txt = hold[h];
	char tag[MAX_TAG_LEN];

	if (!XMLtag(txt,tag)) *tag = 0;
	newstr(&x,tag);
	delete txt;
	hold[h] = x;
	return h;
}
 
int Subjects::attrfunc(int h)
{
	char *attr = hold[h--];
	char *txt = hold[h];
	char tag[MAX_TAG_LEN];
	char val[MAX_TAG_LEN];
	char *buf;

	*val = 0;
	if (XMLtag(txt,tag)) if (!XMLvalue(tag,attr,val)) *val = 0;
	newstr(&buf,val);
	hold[h] = buf;

	delete attr;
	delete txt;
	return h;
}
 
int Subjects::datafunc(int h)
{
	char *txt;
	char otag[MAX_TAG_LEN];
	char ctag[MAX_TAG_LEN];
	char *buf;

	txt = hold[h];
	buf = new char[strlen(txt) + 2];
	*buf = 0;
	if (!XMLnode(txt,otag,buf,ctag)) *buf = 0;
	strcpy(txt,buf);
	delete buf;
	newstr(&buf,txt);
	hold[h] = buf;
	delete txt;
	return h;
}
 
int Subjects::datefunc(int h)
{
	SYSTEMTIME temp;
	FILETIME atime;
	FILETIME btime;
	ULARGE_INTEGER a_dts;
	ULARGE_INTEGER b_dts;
	char *a;
	char *b;
	char *at;
	char *buf = new char[MAX_TAG_LEN];

	b = hold[h--];
	a = hold[h];

	temp.wMonth = atoi(a);
	at = strstr(a,"/");
	temp.wDay = atoi(++at);
	at = strstr(at,"/");
	temp.wYear = atoi(++at);
	at = strstr(at," ");
	if (at) {
		temp.wHour = atoi(++at);
		at = strstr(at,":");
		temp.wMinute = atoi(++at);
		at = strstr(at,":");
		temp.wSecond = atoi(++at);
	} else temp.wHour = temp.wMinute = temp.wSecond = 0;
	temp.wMilliseconds = 0;
	if (temp.wYear < 80) temp.wYear += 2000;
	else if (temp.wYear < 100) temp.wYear += 1900;
	SystemTimeToFileTime(&temp,&atime);
	a_dts.HighPart = atime.dwHighDateTime;
	a_dts.LowPart  = atime.dwLowDateTime;

	temp.wMonth = atoi(b);
	at = strstr(b,"/");
	temp.wDay = atoi(++at);
	at = strstr(at,"/");
	temp.wYear = atoi(++at);
	at = strstr(at," ");
	if (at) {
		temp.wHour = atoi(++at);
		at = strstr(at,":");
		temp.wMinute = atoi(++at);
		at = strstr(at,":");
		temp.wSecond = atoi(++at);
	} else temp.wHour = temp.wMinute = temp.wSecond = 0;
	temp.wMilliseconds = 0;
	if (temp.wYear < 80) temp.wYear += 2000;
	else if (temp.wYear < 100) temp.wYear += 1900;
	SystemTimeToFileTime(&temp,&btime);
	b_dts.HighPart = btime.dwHighDateTime;
	b_dts.LowPart  = btime.dwLowDateTime;

	if (CompareFileTime(&atime,&btime) > 0) {
		a_dts.QuadPart -= b_dts.QuadPart;
		sprintf(buf,"%I64d",a_dts.QuadPart);
	} else {
		b_dts.QuadPart -= a_dts.QuadPart;
		sprintf(buf,"-%I64d",b_dts.QuadPart);
	}
	at = strstr(buf,"0000000");
	*at = 0;

	delete a;
	delete b;
	newstr(&a,buf);
	delete buf;
	hold[h] = a;
	return h;
}

int Subjects::subscript(int h)
{
	char *buf = new char[MAX_SIZE];
	char *p;
	int x;

	if (!strcmp(hold[h],"max")) { x = -1; delete hold[h--]; }
	else x = stkint(hold[h--]);

	if (curSub->m_cases.curCase->m_facts.Find(hold[h])) {
		if (x < 0) x = (int)curSub->m_cases.curCase->m_facts.curFact->m_mvlist.GetCount();
		if (!x) {
			sprintf(buf,"%s",curSub->m_cases.curCase->m_facts.curFact->m_value);
		} else if (!curSub->m_cases.curCase->m_facts.curFact->Find(x - 1,buf)) strcpy(buf,"");
	}

	newstr(&p,buf);
	delete hold[h];
	delete buf;
	hold[h] = p;
	return h;
}

int Subjects::getfunc(char *fn)
{   
	if (!strcmp(fn,"RIGHT"))    return 44;
	if (!strcmp(fn,"LEFT"))     return 43;
	if (!strcmp(fn,"FIND"))     return 42;
	if (!strcmp(fn,"DATEDIFF")) return 41;
	if (!strcmp(fn,"XMLDATA"))  return 40;
	if (!strcmp(fn,"XMLATTR"))  return 39;
	if (!strcmp(fn,"XMLTAG"))   return 38;
	if (!strcmp(fn,"CHANGE"))   return 37;
	if (!strcmp(fn,"FIELD"))    return 36;
	if (!strcmp(fn,"SUBSTR"))   return 35;
	if (!strcmp(fn,"ZEROFILL")) return 34;
	if (!strcmp(fn,"JUSTIFY"))  return 33;
	if (!strcmp(fn,"PAD"))      return 32;
	if (!strcmp(fn,"DEC"))      return 31;
	if (!strcmp(fn,"NUM"))      return 30;
	if (!strcmp(fn,"PROPER"))   return 29;
	if (!strcmp(fn,"LOWER"))    return 28;
	if (!strcmp(fn,"UPPER"))    return 27;
	if (!strcmp(fn,"TRIM"))     return 26;
	if (!strcmp(fn,"LEN"))      return 25;
	if (!strcmp(fn,"MVCNT"))    return 24;
	if (!strcmp(fn,"REPLACE"))  return 23;
	if (!strcmp(fn,"MOD"))      return 22;
	if (!strcmp(fn,"RND"))      return 21;
	if (!strcmp(fn,"EXP"))      return 20;
	if (!strcmp(fn,"SQRT"))     return 19;
	if (!strcmp(fn,"POW"))      return 18;
	if (!strcmp(fn,"LOG"))      return 17;
	if (!strcmp(fn,"CEIL"))     return 16;
	if (!strcmp(fn,"FLOOR"))    return 15;
	if (!strcmp(fn,"INT"))      return 15;
	if (!strcmp(fn,"ATAN"))     return 14;
	if (!strcmp(fn,"TAN"))      return 13;
	if (!strcmp(fn,"COS"))      return 12;
	if (!strcmp(fn,"SIN"))      return 11;
	if (!strcmp(fn,"ABS"))      return 10;  
	return (-1);
} 

int Subjects::rollup(char *s)  /* roll into the reverse polish stack op1[64], op2[64] */
{
	char *i, *l;
	char *m = CNIL;
	char *tmp = CNIL;
	int j, h, b, tos, k1, k2, flg, subflg, newflg, litflg;
	char *buf = new char[MAX_SIZE];

	h = 0;
	tos = 0;
	push[h] = 2;                         
	subflg = 0;                                  
	newflg = 1;

	for (i = s;*i && isspace(*i);i++);
	for (l = i;*i;i++) {
		if (*i == '\"') {
			for (i++;*i && (!strncmp(i,"\"\"",2) || *i != '\"');i++) if (!strncmp(i,"\"\"",2)) i++;
			if (*i == '\"') i++;
			while (*i && isspace(*i)) i++;
		}
		for (j = 0;dlm[j] && dlm[j] != *i;j++);  /* scan operator as delimiter */
		if (dlm[j]) {
			if (j == 11) j = 3;   /* turn a } into a )  which pops rest of RPN */
 
/* save the value to the left of the function operator */

			while (*l && isspace(*l)) l++;
			if (l < i) {
        
				flg = 0;                             
				litflg = 0;
				if (isdigit(*l) || *l == '.') {
					for (b = 0;l < i;l++) if (*l != ' ') buf[b++] = *l; buf[b] = 0;
				} else if (*l == '$' || *l =='%') {
					l = varname(l,&m);
					if (!getValue(m,buf)) {
						b = (int)strlen(m) - 1;
						if (b > 1 && *(m + b) == ')') {
							for (b = 0;*(m + b) && *(m + b) != '(';b++); *(m + b) = 0;
							if (!getValue(m,buf)) *buf = 0;
							else { strcpy(buf,m); flg = 1; }
						} else *buf = 0;
					}
					delete m; 
				} else if (*l == '\'' && *(l + 1) && *(l + 2) == '\'') {             
					l++, sprintf(buf,"%d",(int)(*l)); l++; l++;
				} else if (*l == '\"') {
					for (b = 0,l++;*l && (!strncmp(l,"\"\"",2) || *l != '\"');l++) {
						buf[b++] = *l;                
						if (!strncmp(l,"\"\"",2)) l++;
					} 
					if (*l == '\"') l++;
					buf[b] = 0;
					litflg = 1;
					if (l > i) i = l;
				} else *buf = 0;
				newstr(&tmp,buf);
				op1[tos] = tmp;
				op2[tos++] = flg;     /* value operator */  
				if (flg) subflg = 1;
				newflg = 0;
			}
			l = i + 1;

/* save the function on the left of the value - i.e., *i or j */

			if (j != 1) {       /* not a predefined function, i.e., '@'  */
				if (j == 3 || !j) {     /* either a close parenthesis or a comma */
					while (push[h] != push[0]) {
						op1[tos] = CNIL;
						op2[tos++] = push[h--];             /* operator type */
					}
					if (j) h--;              
					if (subflg) { op1[tos] = CNIL; op2[tos++] = 3; subflg = 0; }
					newflg = 0;
				} else {          /* the rest of the operators + - * / ! ~ and ( */
					if (j == 4 && newflg) {
						newstr(&tmp,"0");
						op1[tos] = tmp;
						op2[tos++] = 0;      /* function type operator */
						h++;
						push[h] = j;
						newflg = 0;
					} else {
						if (push[h] != push[0] && j != 2) {  /* j not = ( */
							k1 = push[h] / 2;
							k2 = j / 2;
							while (k1 >= k2 && h > 0) {
								op1[tos] = CNIL;
								op2[tos++] = push[h--];      /* function type operator */
								k1 = push[h] / 2;
							}
						}
						h++;
						push[h] = j;
						if (j == 2) newflg = 1; else newflg = 0;
					}   
				}
			} else {
/* the functions are here  */
				i++;
				for (j = 0;*i && *i != '(' && j < 4096;i++) if (*i != ' ') buf[j++] = *i;
				buf[j] = 0;
				if (*i) l = i + 1; else l = i--;
				j = getfunc(buf);
				h++;
				push[h++] = j;
				push[h] = 2;
				newflg = 1;
			}
		} /* end of the check for delimiter loop */
		if (isspace(*i)) { while (*i && isspace(*i)) i++; i--; }
	}
  
	while (h > 0) {
		op1[tos] = CNIL;
		op2[tos++] = push[h--];             /* operator type */
	}

	delete buf;
	return (tos);
}

void Subjects::rollout(int tos,char **s)  /* roll out the reverse polish stack into hold[64] */
{
	char *p = CNIL;
	int h, i, j;

	h = 0;
	newstr(&p,"");
	hold[h] = p;
	for (i = 0;i < tos; i++) {
		if (op2[i] == 0 || op2[i] == 1) {
			h++;
			hold[h] = op1[i];
		} else {
			j = op2[i];          
			switch (j) {
/* needs these operators: and, or, nand, nor, xor, imp */
/* needs these operators: <, =, >, <=, !=, >=, !<, !> */
				case 3:   h = subscript(h);    break;
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:   h = fourbang(h,j);   break;
				case 9:   h = confunc(h);      break;
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
				case 20:
				case 21:  h = funcbang(h,j);   break;
				case 22:  h = fourbang(h,j);   break;
				case 23:  h = replacefunc(h);  break;
				case 24:  h = cntfunc(h);      break;
				case 25:  h = lenfunc(h);      break;
				case 26:  h = trimfunc(h);     break;
				case 27:  uppercase(hold[h]);  break;
				case 28:  lowercase(hold[h]);  break;
				case 29:  propercase(hold[h]); break;
				case 30:  h = numfunc(h);      break;
				case 31:  h = decfunc(h);      break;
				case 32:  h = padfunc(h);      break;
				case 33:  h = justfunc(h);     break;
				case 34:  h = zerofunc(h);     break;
				case 35:  h = midfunc(h);      break;
				case 36:  h = fldfunc(h);      break;
				case 37:  h = chgfunc(h);      break;
				case 38:  h = tagfunc(h);      break;
				case 39:  h = attrfunc(h);     break;
				case 40:  h = datafunc(h);     break;
				case 41:  h = datefunc(h);     break;
				case 42:  h = findfunc(h);     break;
				case 43:  h = leftfunc(h);     break;
				case 44:  h = rightfunc(h);    break;
			}
		}
	}
	*s = new char[strlen(hold[h]) + 2];
	strcpy(*s,hold[h]);
	while (h >= 0) delete hold[h--];
}

char *Subjects::calc(char *s)
{
	if (!*s) return CNIL;
	
	strcpy(dlm,",@()-+/*^&~}");

	char *wrk = new char[strlen(s) + 3];
	strcpy(wrk,s);
	strcat(wrk,"}");

	int tos = rollup(wrk);

	delete wrk;

	rollout(tos,&wrk);

	return wrk;
}

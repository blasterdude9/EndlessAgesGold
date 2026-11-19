/* Programming/Implimentation: Arron Boucher.
   Debugging: Sean Barnes.
   08/07/2004
*/

// star.s.cpp: implementation of the Stars class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stars.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Star

Star::Star()
{
	m_pattern = "";
	m_path = "";
}

Star::Star(char *value)
{
	m_value = value;
	m_pattern = "";
	m_path = "";
}

Star::~Star()
{
}

void Star::Push(char *value)
{
	CString t;

	t.Format(_T("%hs"),value);
	m_mvlist.AddTail(t);
}

void Star::Push(CString t)
{
	m_mvlist.AddTail(t);
}

bool Star::Pop(char *value)
{
	CString t;

	*value = 0;
	if (m_mvlist.IsEmpty()) return false;
	t = m_mvlist.RemoveTail();
	sprintf(value,"%s",t);
	return true;
}

bool Star::Pop()
{
	CString t;

	if (m_mvlist.IsEmpty()) return false;
	t = m_mvlist.RemoveTail();
	return true;
}

bool Star::Find(int n,char *value)
{
	CString t;

	POSITION pos = m_mvlist.FindIndex(n);
	if (pos == NULL) return false;
	t = m_mvlist.GetAt(pos);
	sprintf(value,"%s",t);
	return true;
}

//////////////////////////////////////////////////////////////////////
// Stars Construction/Destruction
//////////////////////////////////////////////////////////////////////

Stars::Stars()
{
	curStar = NULL;
	At = NULL;
}

Stars::~Stars()
{
	RemoveAll();
}

void Stars::Add(char *attr)
{
	Key = attr;
	curStar = new Star(attr);

	Star *pTemp = m_mapStringToStar[Key];
	delete pTemp;

	m_mapStringToStar[Key] = curStar;
	At = NULL;
}

void Stars::Put(CString attr)
{
	Key = attr;
	curStar = new Star();
	curStar->m_value = attr;

	Star *pTemp = m_mapStringToStar[Key];
	delete pTemp;

	m_mapStringToStar[Key] = curStar;
	At = NULL;
}

void Stars::RemoveAll()
{
	POSITION pos = m_mapStringToStar.GetStartPosition();
	while (pos != NULL) {
		CString str;
		Star *pStar;
		m_mapStringToStar.GetNextAssoc(pos, str, pStar);
		delete pStar;
	}
	m_mapStringToStar.RemoveAll();
	At = NULL;
	Key = "";
	curStar = NULL;
}

bool Stars::Remove(char *attr)
{
	CString key(attr);
	Star *pStar;
	if (!m_mapStringToStar.Lookup(key,pStar)) return false;
	m_mapStringToStar.RemoveKey(key);
	delete pStar;
	At = NULL;
	Key = "";
	curStar = NULL;
	return true;
}

bool Stars::Seek(CString attr)
{
	Key = attr;
	if (!m_mapStringToStar.Lookup(Key,curStar)) return false;
	At = NULL;
	return true;
}

bool Stars::Find(char *attr)
{
	Key = attr;
	if (!m_mapStringToStar.Lookup(Key,curStar)) return false;
	At = NULL;
	return true;
}

bool Stars::GetFirst()
{
	At = m_mapStringToStar.GetStartPosition();
	if (At == NULL) return false;
	m_mapStringToStar.GetNextAssoc(At,Key,curStar);
	return true;
}

bool Stars::GetNext()
{
	if (At == NULL) return false;
	m_mapStringToStar.GetNextAssoc(At,Key,curStar);
	if (At == NULL) return false;
	return true;
}

bool Stars::GetStar(char *attr, int at, char *buf)
{
	CString key(attr);
	Star *pStar;

	strcpy(buf,"");

	if (!m_mapStringToStar.Lookup(key,pStar)) return false;

	pStar->Find(at,buf);
	return true;
}

void Stars::Copy(Star *s)
{
	CString t;
	POSITION pos;

	Key = s->m_value;
	curStar = new Star();
	curStar->m_value = Key;

	Star *pTemp = m_mapStringToStar[Key];
	delete pTemp;

	m_mapStringToStar[Key] = curStar;
	At = NULL;

	for (int i = 0;i < s->m_mvlist.GetCount();i++) {
		pos = s->m_mvlist.FindIndex(i);
		t = s->m_mvlist.GetAt(pos);
		curStar->m_mvlist.AddTail(t);
	}
}

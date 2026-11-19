/* Programming/Implimentation: Arron Boucher.
   Debugging: Sean Barnes.
   08/07/2004
*/

// scripts.cpp: implementation of the Scripts class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scripts.h"
#include "stdio.h"
#include "support.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Frame

Frame::Frame()
{
}

Frame::Frame(char *name,char *behavior)
{
	m_name = name;
	m_behavior = behavior;
}

Frame::~Frame()
{
}

void Frame::FormatFrame(CString& str)
{
	str.Format(_T("{%s}\r\n%s\r\n"),m_name,m_behavior);
}

//////////////////////////////////////////////////////////////////////
// Frames Construction/Destruction
//////////////////////////////////////////////////////////////////////

Frames::Frames()
{
	At = NULL;
}

Frames::~Frames()
{
	RemoveAll();
}

void Frames::Add(char *name,char *behavior)
{
	Key = name;
	curFrame = new Frame(name,behavior);

	Frame *pTemp = m_mapStringToFrame[Key];
	delete pTemp;

	m_mapStringToFrame[Key] = curFrame;
	At = NULL;
}

void Frames::RemoveAll()
{
	POSITION pos = m_mapStringToFrame.GetStartPosition();
	while (pos != NULL) {
		CString str;
		Frame *pFrame;
		m_mapStringToFrame.GetNextAssoc(pos,str,pFrame);
		delete pFrame;
	}
	m_mapStringToFrame.RemoveAll();
	At = NULL;
	Key = "";
	curFrame = NULL;
}

bool Frames::Remove(char *name)
{
	CString key(name);
	Frame *pFrame;
	if (!m_mapStringToFrame.Lookup(key,pFrame)) return false;
	m_mapStringToFrame.RemoveKey(key);
	delete pFrame;
	At = NULL;
	Key = "";
	curFrame = NULL;
	return true;
}

bool Frames::Seek(CString name)
{
	Key = name;
	if (!m_mapStringToFrame.Lookup(Key,curFrame)) return false;
	At = NULL;
	return true;
}

bool Frames::Find(char *name)
{
	Key = name;
	if (!m_mapStringToFrame.Lookup(Key,curFrame)) return false;
	At = NULL;
	return true;
}

bool Frames::GetFirst()
{
	At = m_mapStringToFrame.GetStartPosition();
	if (At == NULL) return false;
	m_mapStringToFrame.GetNextAssoc(At,Key,curFrame);
	return true;
}

bool Frames::GetNext()
{
	if (At == NULL) return false;
	m_mapStringToFrame.GetNextAssoc(At,Key,curFrame);
	if (At == NULL) return false;
	return true;
}

void Frames::FormatFrames(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToFrame.GetStartPosition();
	str = "";
	while (pos != NULL) {
		CString key;
		Frame *pFrame;
		m_mapStringToFrame.GetNextAssoc(pos,key,pFrame);
		pFrame->FormatFrame(s);
		str += s;
	}
}

void Frames::ListFrames(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToFrame.GetStartPosition();
	str = "";
	while (pos != NULL) {
		CString key;
		Frame *pFrame;
		m_mapStringToFrame.GetNextAssoc(pos,key,pFrame);
		str += "{";
		str += pFrame->m_name;
		str += "}";
		if (pos != NULL) str += ", ";
	}
}

/////////////////////////////////////////////////////////////////////////////
// Script

Script::Script()
{
}

Script::Script(char *name)
{
	m_name = name;
}

Script::~Script()
{
	m_frames.RemoveAll();
}

void Script::LoadFrames(char *text)
{
	char *at = text;
	char *name = CNIL;
	char *behavior = CNIL;
	char *pos, c;

	while (*at) {
		for (;*at != '{';at++) if (!*at) return;
		for (at++,pos = at;*at && *at != '}';at++);
		if (*at != '}') return;
		*at = 0;
		newstr(&name,pos);
		*at = '}';
		for (at++,pos = at;*at && *at != '{';at++);
		c = *at; *at = 0;
		while (*pos && *pos <= ' ') pos++;
		newstr(&behavior,pos);
		*at = c;
		rtrim(behavior);
		m_frames.Add(name,behavior);
		m_frameslist.AddTail(name);
		delete name;
		delete behavior;
	}
}

void Script::FormatScript(CString& str)
{
	CString s;
	m_frames.FormatFrames(s);
	str.Format(_T("Script:%s\r\n%s"),m_name,s);
}

void Script::ListScript(CString& str)
{
	int i;
	int cnt = (int)m_frameslist.GetCount();
	POSITION pos;

	str.Format(_T("Script:%s\r\n"),m_name);
	for (i = 0;i < cnt;i++) {
		pos = m_frameslist.FindIndex(i);
		if (pos == NULL) continue;
		str += _T("{");
		str += m_frameslist.GetAt(pos);
		str += _T("}");
		if (i + 1 < cnt) str += _T(", ");
	}
}

//////////////////////////////////////////////////////////////////////
// Scripts Construction/Destruction
//////////////////////////////////////////////////////////////////////

Scripts::Scripts()
{
	At = NULL;
}

Scripts::~Scripts()
{
	RemoveAll();
}

void Scripts::Add(char *name)
{
	Key = name;
	curScript = new Script(name);

	Script *pTemp = m_mapStringToScript[Key];
	delete pTemp;

	m_mapStringToScript[Key] = curScript;
	At = NULL;
}

void Scripts::RemoveAll()
{
	POSITION pos = m_mapStringToScript.GetStartPosition();
	while (pos != NULL) {
		CString str;
		Script *pScript;
		m_mapStringToScript.GetNextAssoc(pos,str,pScript);
		pScript->m_frames.RemoveAll();
		delete pScript;
	}
	m_mapStringToScript.RemoveAll();
	At = NULL;
	Key = "";
	curScript = NULL;
}

bool Scripts::Remove(char *name)
{
	CString key(name);
	Script *pScript;
	if (!m_mapStringToScript.Lookup(key,pScript)) return false;
	m_mapStringToScript.RemoveKey(key);
	pScript->m_frames.RemoveAll();
	delete pScript;
	At = NULL;
	Key = "";
	curScript = NULL;
	return true;
}

bool Scripts::Seek(CString name)
{
	Key = name;
	if (!m_mapStringToScript.Lookup(Key,curScript)) return false;
	At = NULL;
	return true;
}

bool Scripts::Find(char *name)
{
	Script *fndScript;
	Key = name;
	if (!m_mapStringToScript.Lookup(Key,fndScript)) return false;
	curScript = fndScript;
	At = NULL;
	return true;
}

bool Scripts::GetFirst()
{
	At = m_mapStringToScript.GetStartPosition();
	if (At == NULL) return false;
	m_mapStringToScript.GetNextAssoc(At,Key,curScript);
	return true;
}

bool Scripts::GetNext()
{
	if (At == NULL) return false;
	m_mapStringToScript.GetNextAssoc(At,Key,curScript);
	if (At == NULL) return false;
	return true;
}

void Scripts::FormatScripts(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToScript.GetStartPosition();
	str = "";
	while (pos != NULL) {
		CString key;
		Script *pScript;
		m_mapStringToScript.GetNextAssoc(pos,key,pScript);
		pScript->FormatScript(s);
		str += s;
	}
}

void Scripts::ListScripts(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToScript.GetStartPosition();
	str = "";
	while (pos != NULL) {
		CString key;
		Script *pScript;
		m_mapStringToScript.GetNextAssoc(pos,key,pScript);
		pScript->ListScript(s);
		str += s;
	}
}

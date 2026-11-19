/* Programming/Implimentation: Arron Boucher.
   Debugging: Sean Barnes.
   08/07/2004
*/
// rules.cpp: implementation of the Rules class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "rules.h"
#include "math.h"
#include "facts.h"
#include "string.h"
#include "support.h"

 #ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
 // ATTRIBUTE

Attribute::Attribute()
{
}

Attribute::Attribute(char *name,char *title,char *prompt,char *def,char *parms)
{
	m_name = name;
	m_title = title;
	m_prompt = prompt;
	m_default = def;
	m_parms = parms;
}

Attribute::~Attribute()
{
}

void Attribute::FormatAttribute(CString& str)
{
	str.Format(_T("Attribute:%s\r\nTitle:%s\r\n"),m_name,m_title);
	if (*m_prompt) {
		str += TEXT("Prompt:");
		str += m_prompt;
		str += TEXT("\r\n");
	}
	if (*m_default) {
		str += TEXT("Default:");
		str += m_default;
		str += TEXT("\r\n");
	}
	if (*m_parms) {
		str += TEXT("Parms:");
		str += m_parms;
		str += TEXT("\r\n");
	}
}

void Attribute::FileAttribute(CString& str)
{
	str.Format(_T("$N %s\r\n$T %s\r\n$Q %s\r\n$D %s\r\n$P %s\r\n"),m_name,m_title,m_prompt,m_default,m_parms);
}

void Attribute::XMLAttribute(CString& str)
{
	str.Format(_T("<ATTRIBUTE NAME=\"%s\">"),m_name);
	if (*m_title) {
		str += TEXT("<TITLE>");
		str += m_title;
		str += TEXT("</TITLE>");
	}
	if (*m_prompt) {
		str += TEXT("<PROMPT>");
		str += m_prompt;
		str += TEXT("</PROMPT>");
	}
	if (*m_default) {
		str += TEXT("<DEFAULT>");
		str += m_default;
		str += TEXT("</DEFAULT>");
	}
	if (*m_parms) {
		str += TEXT("<PARMS>");
		str += m_parms;
		str += TEXT("</PARMS>");
	}
	str += TEXT("</ATTRIBUTE>");
}

//////////////////////////////////////////////////////////////////////
// ATTRIBUTES Construction/Destruction
//////////////////////////////////////////////////////////////////////

Attributes::Attributes()
{

}

Attributes::~Attributes()
{
	RemoveAll();
}

void Attributes::Add(char *name,char *title,char *prompt,char *def,char *parms)
{
	CString key(name);
	Attribute *pAttribute = new Attribute(name,title,prompt,def,parms);

	Attribute *pTemp = m_mapStringToAttribute[key];
	delete pTemp;

	m_mapStringToAttribute[key] = pAttribute;
}

void Attributes::RemoveAll()
{
	POSITION pos = m_mapStringToAttribute.GetStartPosition();
	while (pos != NULL) {
		CString str;
		Attribute *pAttribute;
		m_mapStringToAttribute.GetNextAssoc(pos,str,pAttribute);
		delete pAttribute;
	}
	m_mapStringToAttribute.RemoveAll();

	Key = "";
	Name = "";
	Title = "";
	Prompt = "";
	DefaultValue = "";
	Parms = "";
}

bool Attributes::Remove(char *name)
{
	CString key(name);
	Attribute *pAttribute;
	if (!m_mapStringToAttribute.Lookup(key,pAttribute)) return false;
	m_mapStringToAttribute.RemoveKey(key);
	delete pAttribute;
	return true;
}

bool Attributes::Find(char *name)
{
	CString key(name);
	Attribute *pAttribute;
	if (!m_mapStringToAttribute.Lookup(key,pAttribute)) return false;
	Name = pAttribute->m_name;
	Title = pAttribute->m_title;
	Prompt = pAttribute->m_prompt;
	DefaultValue = pAttribute->m_default;
	Parms = pAttribute->m_parms;
	return true;
}

bool Attributes::Seek(CString name)
{
	CString key(name);
	Attribute *pAttribute;
	if (!m_mapStringToAttribute.Lookup(key,pAttribute)) return false;
	Name = pAttribute->m_name;
	Title = pAttribute->m_title;
	Prompt = pAttribute->m_prompt;
	DefaultValue = pAttribute->m_default;
	Parms = pAttribute->m_parms;
	return true;
}

bool Attributes::GetFirst()
{
	At = m_mapStringToAttribute.GetStartPosition();
	if (At == NULL) return false;

	Attribute* pAttribute;
	m_mapStringToAttribute.GetNextAssoc(At,Key,pAttribute);

	Name = pAttribute->m_name;
	Title = pAttribute->m_title;
	Prompt = pAttribute->m_prompt;
	DefaultValue = pAttribute->m_default;
	Parms = pAttribute->m_parms;
	return true;
}

bool Attributes::GetNext()
{
	if (At == NULL) return false;
	Attribute* pAttribute;

	m_mapStringToAttribute.GetNextAssoc(At,Key,pAttribute);
	if (At == NULL) return false;

	Name = pAttribute->m_name;
	Title = pAttribute->m_title;
	Prompt = pAttribute->m_prompt;
	DefaultValue = pAttribute->m_default;
	Parms = pAttribute->m_parms;
	return true;
}

void Attributes::ListAttributes(CString& str)
{
	POSITION pos = m_mapStringToAttribute.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Attribute *pAttr;
		m_mapStringToAttribute.GetNextAssoc(pos,key,pAttr);
		str += pAttr->m_name;
		if (pos != NULL) str += TEXT(", ");
	}
}

void Attributes::FormatAttributes(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToAttribute.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Attribute *pAttr;
		m_mapStringToAttribute.GetNextAssoc(pos,key,pAttr);
		pAttr->FormatAttribute(s);
		str += s;
		str += TEXT("\r\n");
	}
}

void Attributes::FileAttributes(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToAttribute.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Attribute *pAttr;
		m_mapStringToAttribute.GetNextAssoc(pos,key,pAttr);
		pAttr->FileAttribute(s);
		str += s;
	}
}

void Attributes::XMLAttributes(CString& str)
{
	CString s;
	POSITION pos = m_mapStringToAttribute.GetStartPosition();

	str = TEXT("<ATTRIBUTES>");
	while (pos != NULL) {
		CString key;
		Attribute *pAttr;
		m_mapStringToAttribute.GetNextAssoc(pos,key,pAttr);
		pAttr->XMLAttribute(s);
		str += s;
	}
	str += TEXT("</ATTRIBUTES>");
}

/////////////////////////////////////////////////////////////////////////////
// PREMISE

Premise::Premise()
{
}

Premise::Premise(char *attr,char *rel,char *value)
{
	m_attribute = attr;
	m_relation = rel;
	m_value = value;
}

Premise::~Premise()
{
}

void Premise::FormatPremise(CString& str)
{
	str.Format(_T("  %s %s %s\r\n"),m_attribute,m_relation,m_value);
}

void Premise::FilePremise(CString& str)
{
	str.Format(_T("?%s,%s,%s\r\n"),m_attribute,m_relation,m_value);
}

void Premise::XMLPremise(CString& str)
{
	str.Format(_T("<PREMISE ATTRIBUTE=\"%s\" REL=\"%s\"><VALUE>%s</VALUE></PREMISE>"),m_attribute,m_relation,m_value);
}

/////////////////////////////////////////////////////////////////////////////
// RESULT

Result::Result()
{
}

Result::Result(char *attr,char *value,double cf)
{
	m_attribute = attr;
	m_value = value;
	m_cf = cf;
}

Result::~Result()
{
}

void Result::FormatResult(CString& str)
{
	if (m_cf >= .995) str.Format(_T("  %s = %s\r\n"),m_attribute,m_value); 
	else str.Format(_T("  %s %.0lf%% = %s\r\n"),m_attribute,floor(100 * m_cf + 0.5),m_value);
}

void Result::FileResult(CString& str)
{
	str.Format(_T("!%s %.0lf%%,%s\r\n"),m_attribute,floor(100 * m_cf + 0.5),m_value);
}

void Result::XMLResult(CString& str)
{
	str.Format(_T("<RESULT ATTRIBUTE=\"%s\" CF=\"%.2lf\"><VALUE>%s</VALUE></RESULT>"),m_attribute,m_cf,m_value);
}

/////////////////////////////////////////////////////////////////////////////
// RULE

Rule::Rule()
{
}

Rule::Rule(char *ruleid,char *subject)
{
	m_ruleid = ruleid;
	m_subject = subject;
}

Rule::~Rule()
{
	int i;

	for (i = 0;i < m_premises.GetSize(); i++) delete m_premises[i];
	m_premises.RemoveAll();

	for (i = 0;i < m_results.GetSize(); i++) delete m_results[i];
	m_results.RemoveAll();
}

void Rule::AddPremise(char *attr,char *relation,char *value)
{
	Premise *pPremise = new Premise(attr,relation,value);
	m_premises.Add(pPremise);
}

void Rule::AddResult(char *attr,char *value,double cf)
{
	Result *pResult = new Result(attr,value,cf);
	m_results.Add(pResult);
}

void Rule::SpecRule(CString& str)
{
	int i;
	CString s;

	str = TEXT("If\r\n");
	for (i = 0;i < m_premises.GetSize(); i++) {
		m_premises[i]->FormatPremise(s);
		str += s;
	}
	 str += TEXT("Then\r\n");
	for (i = 0;i < m_results.GetSize(); i++) {
		m_results[i]->FormatResult(s);
		str += s;
	}
}

void Rule::FormatRule(CString& str)
{
	CString s;

	str.Format(_T("Rule ID: %s\r\n"),m_ruleid);
	if (*m_subject) {
		str += TEXT("Ref: ");
		str += m_subject;
		str += TEXT("\r\n");
	}
	SpecRule(s);
	str += s;
}

void Rule::FileRule(CString& str)
{
	int i;
	CString s;

	str.Format(_T(":%s\r\n;%s\r\n"),m_ruleid,m_subject);
	for (i = 0;i < m_premises.GetSize(); i++) {
		m_premises[i]->FilePremise(s);
		str += s;
	}
	for (i = 0;i < m_results.GetSize(); i++) {
		m_results[i]->FileResult(s);
		str += s;
	}
	str += "\r\n";
}

void Rule::XMLRule(CString& str)
{
	int i;
	CString s;

	str.Format(_T("<RULE ID=\"%s\">\r\n<REF>%s</REF>\r\n"),m_ruleid,m_subject);
	for (i = 0;i < m_premises.GetSize(); i++) {
		m_premises[i]->XMLPremise(s);
		str += s;
	}
	for (i = 0;i < m_results.GetSize(); i++) {
		m_results[i]->XMLResult(s);
		str += s;
	}
	str += TEXT("</RULE>");
}

//////////////////////////////////////////////////////////////////////
// RULES Construction/Destruction
//////////////////////////////////////////////////////////////////////

Rules::Rules()
{
}

Rules::~Rules()
{
	RemoveAll();
}

void Rules::Add(char *ruleid,char *subject)
{
	CString key(ruleid);
	curRule = new Rule(ruleid,subject);

	Rule *pTemp = m_mapStringToRule[key];
	delete pTemp;

	m_mapStringToRule[key] = curRule;
}

void Rules::RemoveAll()
{
	POSITION pos = m_mapStringToRule.GetStartPosition();
	while (pos != NULL) {
		CString str;
		Rule *pRule;
		m_mapStringToRule.GetNextAssoc(pos, str, pRule);
		delete pRule;
	}
	m_mapStringToRule.RemoveAll();

	Key = "";
	Ruleid = "";
	Subject = "";
	curRule = NULL;
}

bool Rules::Remove(char *ruleid)
{
	CString key(ruleid);
	Rule *pRule;
	if (!m_mapStringToRule.Lookup(key,pRule)) return false;
	m_mapStringToRule.RemoveKey(key);
	if (curRule == pRule) curRule = NULL;
	delete pRule;
	return true;
}

bool Rules::Seek(CString ruleid)
{
	if (!m_mapStringToRule.Lookup(ruleid,curRule)) return false;
	Ruleid = curRule->m_ruleid;
	Subject = curRule->m_subject;
	return true;
}

bool Rules::Find(char *ruleid)
{
	CString key(ruleid);
	if (!m_mapStringToRule.Lookup(key,curRule)) return false;
	Ruleid = curRule->m_ruleid;
	Subject = curRule->m_subject;
	return true;
}

bool Rules::GetFirst()
{
	At = m_mapStringToRule.GetStartPosition();
	if (At == NULL) return false;

	m_mapStringToRule.GetNextAssoc(At,Key,curRule);

	Ruleid = curRule->m_ruleid;
	Subject = curRule->m_subject;
	return true;
}

bool Rules::GetNext()
{
	if (At == NULL) return false;

	m_mapStringToRule.GetNextAssoc(At,Key,curRule);
	if (At == NULL) return false;

	Ruleid = curRule->m_ruleid;
	Subject = curRule->m_subject;
	return true;
}

void Rules::ListRules(CString &str)
{
	POSITION pos = m_mapStringToRule.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Rule *pRule;
		m_mapStringToRule.GetNextAssoc(pos,key,pRule);
		str += pRule->m_ruleid;
		if (pos != NULL) str += TEXT(", ");
	}
}

void Rules::FormatRules(CString &str)
{
	CString s;
	POSITION pos = m_mapStringToRule.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Rule *pRule;
		m_mapStringToRule.GetNextAssoc(pos,key,pRule);
		pRule->FormatRule(s);
		str += s;
		str += TEXT("\r\n");
	}
}

void Rules::FileRules(CString &str)
{
	CString s;
	POSITION pos = m_mapStringToRule.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Rule *pRule;
		m_mapStringToRule.GetNextAssoc(pos,key,pRule);
		pRule->FileRule(s);
		str += s;
	}
}

void Rules::XMLRules(CString &str)
{
	CString s;
	POSITION pos = m_mapStringToRule.GetStartPosition();

	str = "";
	while (pos != NULL) {
		CString key;
		Rule *pRule;
		m_mapStringToRule.GetNextAssoc(pos,key,pRule);
		pRule->XMLRule(s);
		str += s;
	}
}

int Rules::SeekTarget(CString s)
{
	POSITION pos = m_mapStringToRule.GetStartPosition();
	return SeekNextTarget(pos, s);
}

int Rules::SeekNextTarget(POSITION pos,CString s)
{
	int i;
	CString key;
	CString attr;
	Rule *pRule;

	while (pos != NULL) {
		m_mapStringToRule.GetNextAssoc(pos,key,pRule);
		for (i = 0;i < pRule->m_results.GetSize();i++) {
			attr = pRule->m_results[i]->m_attribute;
			if (!attr.Compare(s)) return true;
		}
	}
	return false;
}

int Rules::AssertTarget(CString s)
{
	CString key = _T("");

	goals.AddHead(s);
	POSITION at = atrule.AddHead(key);
	POSITION pos = m_mapStringToRule.GetStartPosition();

	int iRet = AssertNextTarget(at, pos, s);

	goals.RemoveHead();
	atrule.RemoveHead();
	return iRet;
}

int Rules::AssertNextTarget(POSITION at, POSITION pos, CString s)
{
	return 0;
}


void Rules::CompareOp(CString a, CString op, CString v,double cf)
{
	double da, dv;

	if (isNumeric(a) && isNumeric(v)) {
		da = atof(a);
		dv = atof(v);
		if (!op.Compare(_T("="))) {
			if (da == dv) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T("<"))) {
			if (da < dv) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T(">"))) {
			if (da > dv) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T("<="))) {
			if (da <= dv) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T(">="))) {
			if (da >= dv) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T("!="))) {
			if (da != dv) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T("<>"))) {
			if (da != dv) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		}
	} else {
		if (!op.Compare(_T("="))) {
			if (!a.Compare(v)) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T("<"))) {
			if (a.Compare(v) < 0) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T(">"))) {
			if (a.Compare(v) > 0) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T("<="))) {
			if (a.Compare(v) <= 0) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T(">="))) {
			if (a.Compare(v) >= 0) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T("!="))) {
			if (a.Compare(v)) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		} else if (!op.Compare(_T("<>"))) {
			if (a.Compare(v)) tCF += cf - (tCF * cf); else fCF += cf - (fCF * cf);
		}
	}
}


void Rules::Why(CString &s)
{
	int i;
	CString g;
	CString r;
	CString t;
	POSITION pG;
	POSITION pR;

	pG = goals.GetHeadPosition();
	pR = atrule.GetHeadPosition();
	g = goals.GetHead();
	r = atrule.GetHead();
	for (i = 0;i <= wcnt;i++) {
		if (pG == NULL) break;
		if (pR == NULL) break;
		g = goals.GetNext(pG);
		r = atrule.GetNext(pR);
	}
	if (pG == NULL) wcnt = 0; else wcnt++;

	s.Format(_T("Why determine %s?\r\nRule: %s\r\n"),g,r);
	RuleStatus(r,t);
	s += t;
}

void Rules::What(CString r,CString &s)
{
	CString t;

	s.Format(_T("Rule Status for Rule: %s\r\n\r\n"),r);
	RuleStatus(r,t);
	s += t;
}

void Rules::RuleStatus(CString r,CString &s)
{
	Rule *pRule;
	Premise *pPremise;
	Result *pResult;
	Fact *pFact;
	CString t;
	CString x;
	int i;
	int unk = 0;
	double cf, tcf, fcf, xcf;

	s = _T("");
	if (!m_mapStringToRule.Lookup(r,pRule)) return;
	x = _T("=");

	cf = 1.00;
	for (i = 0;i < pRule->m_premises.GetSize();i++) {
		pPremise = pRule->m_premises[i];
		pFact = kb->getFact(pPremise->m_attribute);
		if (pFact != NULL) {
			tcf = tCF; fcf = fCF;
			tCF = 0.0; fCF = 0.0;
			CompareOp(pFact->m_value,pPremise->m_relation,pPremise->m_value,pFact->m_cf);
			if (tCF < fCF) xcf = (tCF - fCF) / (1 - tCF); else xcf = (tCF - fCF) / (1 - fCF);
			tCF = tcf; fCF = fcf;
			if (xcf < cf) cf = xcf;
			ExplainPremise(pFact->m_attribute,pPremise->m_relation,pFact->m_value,xcf,pFact->m_source,i,t);
			s += t;
			if (xcf < 0.20) {
				s += _T("   the rule needed: ");
				FormatParameter(pPremise->m_attribute,pPremise->m_relation,pPremise->m_value,t);
				s += t;
			}
		}
	}
	for (i = 0;i < pRule->m_premises.GetSize();i++) {
		pPremise = pRule->m_premises[i];
		pFact = kb->getFact(pPremise->m_attribute);
		if (pFact == NULL) {
			if (unk == 0) {
				if (cf > 0.02) s += _T("If"); else s += _T("Even if");
				s += _T(" we can show that ");
			} else {
				s += _T("   and that ");
			}
			FormatParameter(pPremise->m_attribute,pPremise->m_relation,pPremise->m_value,t);
			s += t;
			unk++;
		}
	}
	for (i = 0;i < pRule->m_results.GetSize();i++) {
		pResult = pRule->m_results[i];
		if (i) s += _T("   and ");
		else {
			if (cf < 0.20) s += _T("\r\nWe will need some other rule to show ");
			else if (unk > 0) {
				s += _T("\r\nThe conclusion could");
				if (cf < 0.20) s += _T(" not");
				s += _T(" be made ");
			} else {
				pFact = kb->getFact(pResult->m_attribute);
				if (pFact == NULL) s += _T("\r\nThen we will know "); else s += _T("\r\nTherefore we know ");
			}
		}
		if (pResult->m_cf > 0.995) t = _T(""); else t.Format(_T(" %.0f%% "),floor(pResult->m_cf * 100 + 0.5));
		s += t;
		s += _T("that ");
		FormatParameter(pResult->m_attribute,x,pResult->m_value,t);
		s += t;
	}
	if (cf >= 0) t.Format(_T("\r\nCertainty Factor %.0f%% true...   unknowns: %d\r\n"),floor(cf * 100 + 0.5),unk);
	else t.Format(_T("\r\nCertainty Factor %.0f%% false...  unknowns: %d\r\n"),floor(cf * -100 + 0.5),unk);
	s += t;
}

void Rules::FormatParameter(CString a,CString r,CString v,CString &s)
{
	CString t;
	int i;

	if (atr->Seek(a)) {
		t = atr->Title;
		i = t.Find(_T("/not/"));
		if (i > 0) {
			s = t.Left(i);
			if (v.CompareNoCase(_T("yes")) && v.CompareNoCase(_T("true"))) s += _T("not ");
			s += t.Mid(i + 6);
		} else {
			s = t;
			if (!r.Compare(_T("!="))) s += _T(" is not ");
			else if (!r.Compare(_T("<>"))) s += _T(" is not ");
			else if (!r.Compare(_T("<"))) s += _T(" is less than ");
			else if (!r.Compare(_T(">"))) s += _T(" is greater than ");
			else if (!r.Compare(_T(">="))) s += _T(" is not less than ");
			else if (!r.Compare(_T("<="))) s += _T(" is not greater than ");
			else s += _T(" is ");
			s += v;
		}
	} else s.Format(_T("%s %s %s"),a,r,v);
	s += _T("\r\n");
}

void Rules::ExplainPremise(CString a,CString r,CString v,double cf, CString x,int i,CString &s)
{
	CString t;

	if (cf <= 0.995 && cf >= 0) t.Format(_T(" %.0f%%"),floor(cf * 100 + 0.5));
	else if (cf >= -0.995 && cf < 0) t.Format(_T(" %.0f%%"),-1 * floor(cf * -100 + 0.5));
	else t = _T("");

	if (!x.Compare(_T("User"))) {
		if (i) s = _T("   and you said"); else s = _T("You said");
		s += t;
	} else if (!x.Compare(_T("Calc"))) {
		if (i) s = _T("   and it was calculated"); else s = _T("A script frame calculated");
		s += t;
	} else if (!x.Compare(_T("File"))) {
		if (i) s = _T("   and it was assigned"); else s = _T("An assignment was made");
		s += t;
	} else {
		if (i) s = _T("    and"); else s = _T("We know");
		s += t;
		s += _T(" by rule, ");
		s += x;
		s += _T(",");
	}
	s += _T(" that ");
	FormatParameter(a, r, v, t);
	s += t;
}

void Rules::Which(CString a,CString &s)
{
	int i;
	CString attr;
	CString key;
	CString t;
	Rule *pRule;
	Premise *pPremise;
	bool none = true;

	if (atr->Seek(a)) s.Format(_T("Which rules consider %s?\r\n\r\n"),atr->Title);
	else s.Format(_T("Find rules which premise require %s\r\n"),a);

	POSITION pos = m_mapStringToRule.GetStartPosition();
	while (pos != NULL) {
		m_mapStringToRule.GetNextAssoc(pos,key,pRule);
		for (i = 0;i < pRule->m_premises.GetSize();i++) {
			pPremise = pRule->m_premises[i];
			attr = pPremise->m_attribute;
			if (!attr.Compare(a)) {
				t.Format(_T("Rule: %s asks if %s %s %s\r\n"),pRule->m_ruleid,pPremise->m_attribute,pPremise->m_relation,pPremise->m_value);
				s += t;
				none = false;
			}
		}
	}
	if (none) s += _T("No rules found that require this attribute.\r\n");
}

void Rules::How(CString a,CString &s)
{
	int i;
	CString attr;
	CString key;
	CString t;
	Rule *pRule;
	Result *pResult;
	bool none = true;

	if (atr->Seek(a)) {
		s.Format(_T("How can we establish %s?\r\n\r\n"),atr->Title);
		Evaluate(atr->Prompt,t);
		if (t.Compare(_T(""))) {
			s += _T("By asking the following question:\r\n");
			s += t;
			if (t.Right(1) != "?") s += _T("?");
			s += _T("\r\n");
			return;
		}
		Evaluate(atr->DefaultValue,t);
		if (t.Compare(_T(""))) {
			s += _T("From attribute's default value of ");
			s += t;
			s += _T("\r\n");
			return;
		}
	} else s.Format(_T("Find rules which product results of %s\r\n\r\n"),a);

	POSITION pos = m_mapStringToRule.GetStartPosition();
	while (pos != NULL) {
		m_mapStringToRule.GetNextAssoc(pos,key,pRule);
		for (i = 0;i < pRule->m_results.GetSize();i++) {
			pResult = pRule->m_results[i];
			attr = pResult->m_attribute;
			if (!attr.Compare(a)) {
				t.Format(_T("Rule: %s says that %s is %s\r\n"),pRule->m_ruleid,pResult->m_attribute,pResult->m_value);
				s += t;
				none = false;
			}
		}
	}
	if (none) s += _T("No rules found that produce this result.\r\n");
}

void Rules::Evaluate(CString a,CString &s)
{
	char *wrk;
	char *buf;
	char *i;
	char *j = CNIL;

	wrk = new char[a.GetLength() + 2];
	sprintf(wrk,"%s",a);
	buf = wrk;

	s = "";
	for (i = wrk;*i;i++) {
		if (*i == '[') j = i;
		if (*i == ']' &&  j != CNIL) {
			*i++ = 0;
			*j++ = 0;
			s += buf;
			buf = kb->calc(j);
			s += buf;
			delete buf;
			buf = i;
			j = CNIL;			
		}
	}
	s += buf;
	delete wrk;
}
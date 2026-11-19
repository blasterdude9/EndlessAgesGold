#pragma once
#include "axtextformatter.h"


struct SbadWordSTV
{
    CString badWord;
    CString replacementWord;
	char * badWordBuf;
    char * replacementWordBuf;
	int badWordLength;
    int replacementWordLength;
};

class  CAXLanguageFilter
{
public:
	CAXLanguageFilter(void);
	CAXLanguageFilter(CString path);
	~CAXLanguageFilter(void);


    SbadWordSTV          *m_badWords;
    int					  m_objectCount;
	CString               m_fileName;
	CAXTextFormatter      m_formatter;
	char                  m_preformat[3000];
	CString               m_lastError;
	

    int LoadFromFile(CString path);
	int GetSectionCount(CString path);
	void FilterX(CString & media);
	void FilterX2(CString & media);
	void DeleteBuffers();

};

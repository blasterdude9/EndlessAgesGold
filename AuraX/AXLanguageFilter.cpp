#include "StdAfx.h"
#include "axlanguagefilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAXLanguageFilter::CAXLanguageFilter(void)
{
    m_fileName = "filter.wwx";
	m_objectCount = 0;
	m_badWords = 0;
	ZeroMemory(m_preformat,3000);
	LoadFromFile(m_fileName);
}

CAXLanguageFilter::CAXLanguageFilter(CString path)
{
    m_fileName = path;
	m_objectCount = 0;
	m_badWords = 0;
	ZeroMemory(m_preformat,3000);
	LoadFromFile(m_fileName);
}

CAXLanguageFilter::~CAXLanguageFilter(void)
{
    DeleteBuffers();
}



void CAXLanguageFilter::DeleteBuffers()
{
     for(int loop=0;loop<m_objectCount;loop++)
	{
        if(m_badWords[loop].badWordBuf)
		{
           delete [] m_badWords[loop].badWordBuf;
           m_badWords[loop].badWordBuf = 0;
		}
        if(m_badWords[loop].replacementWordBuf)
		{
           delete [] m_badWords[loop].replacementWordBuf;
           m_badWords[loop].replacementWordBuf = 0;
		}
	}
  


	if(m_badWords)
	   delete [] m_badWords;
	 m_badWords = 0;
}

void CAXLanguageFilter::FilterX2(CString & media)
{
   /*for(int loop=0;loop<m_objectCount;loop++)
   {
       m_formatter.ReplaceNoCaseWordsALL(media,m_BannedWord[loop],m_ReplacementWord[loop]);
   }*/
}

void CAXLanguageFilter::FilterX(CString & media)
{
   if(m_objectCount < 1)
   {
      m_lastError = "Object count is less than 1 FilterX() error:3451";
      return;
   }
   
   if(media.GetLength() < 1)
   {
	  m_lastError = "Media length is less than 1 FilterX() error:3452";
      return;
   }


   CString lower = media;
   char * mediaBuffer = media.GetBuffer();
   lower.MakeLower();
   int bufferLength = lower.GetLength();
   char * buffer = lower.GetBuffer();
   
   int trace = 0;
   int traceLegacy = 0;

   while(traceLegacy < bufferLength)
   {
    if(trace > 2998)
	{
	   media = "";
	   return;
	}

	for(int loop=0;loop<m_objectCount;loop++)
	{
		if((traceLegacy+m_badWords[loop].badWordLength) < bufferLength+1)//overflow protection
		if(memcmp(&buffer[traceLegacy],m_badWords[loop].badWordBuf,m_badWords[loop].badWordLength)==0)
		{//format and adjust
           if(traceLegacy > 0)
		   {
              if(!m_formatter.ValidateBreakBasis(buffer[traceLegacy-1]))
                  continue;
		   }
		   int tempIndex = traceLegacy + m_badWords[loop].badWordLength;
		   if(tempIndex < bufferLength)
               if(!m_formatter.ValidateBreakBasis(buffer[tempIndex]))
                   continue;


		   if(m_badWords[loop].replacementWordLength > 0)
		      CopyMemory(&m_preformat[trace],m_badWords[loop].replacementWord,m_badWords[loop].replacementWordLength);


		   trace += m_badWords[loop].replacementWordLength;
		   traceLegacy += m_badWords[loop].badWordLength;

		   m_preformat[trace+m_badWords[loop].replacementWordLength] = 0;
			

		   continue;
		}//end format and adjust   
	}

    m_preformat[trace] = mediaBuffer[traceLegacy];
	m_preformat[trace+1] = 0;

	trace++;
	traceLegacy++;
   }
   
 
   media = m_preformat;
}

int CAXLanguageFilter::LoadFromFile(CString path)
{
	 m_objectCount = GetSectionCount(path);
	 if(m_objectCount == 0)
	 {
	    return m_objectCount;
	 }
	 m_objectCount++;
     CFileException exc;
     CFile fl;
	 CArchive the_Archive(&fl,CArchive::load);
     BOOL res = fl.Open(m_fileName,CFile::modeRead, &exc);
     if(res == FALSE)
	 {
	    m_lastError = "Cannot fine fild name LoadFromFile()";
        return 0;
	 }
	 if(m_badWords)
	    DeleteBuffers();

	 m_badWords = new SbadWordSTV[m_objectCount+2];

	 int trace = 0;
	 while(1)
	 {
		 CString badWord, replacement;
         if(!the_Archive.ReadString(badWord))
		    break;
         if(!the_Archive.ReadString(replacement))
		    break;

		 if(badWord.GetLength() == 0)
		    continue;

         m_badWords[trace].badWord = badWord;
	     m_badWords[trace].replacementWord = replacement;
		 
		 m_badWords[trace].badWordLength = badWord.GetLength();
		 m_badWords[trace].replacementWordLength = replacement.GetLength();

		 m_badWords[trace].badWordBuf = new char[m_badWords[trace].badWordLength+1];
		 CopyMemory(m_badWords[trace].badWordBuf,badWord.GetBuffer(),m_badWords[trace].badWordLength);
		 m_badWords[trace].badWordBuf[m_badWords[trace].badWordLength] = 0;
			
		 m_badWords[trace].replacementWordBuf = NULL;
		 if(m_badWords[trace].replacementWordLength>0)
		 {
		 m_badWords[trace].replacementWordBuf = new char[m_badWords[trace].replacementWordLength+1];
		 CopyMemory(m_badWords[trace].replacementWordBuf,replacement.GetBuffer(),m_badWords[trace].replacementWordLength);
		 m_badWords[trace].replacementWordBuf[m_badWords[trace].replacementWordLength] = 0;
		 }	 

		 trace++;
	 }

	 the_Archive.Close();
	 fl.Close();
	 return trace;
}


int CAXLanguageFilter::GetSectionCount(CString path)
{
     CFileException exc;
     CFile fl;
	 CArchive the_Archive(&fl,CArchive::load);
     BOOL res = fl.Open(m_fileName,CFile::modeRead, &exc);
     if(res == FALSE)
	 {
	    m_lastError = "Cannot find file name GetSectionCount()";
        return 0;
	 }
     int trace = 0;
	 while(1)
	 {
		 CString badWord, replacement;
         if(!the_Archive.ReadString(badWord))
		    break;
         if(!the_Archive.ReadString(replacement))
		    break;

         if(badWord.GetLength() == 0)
		    continue;

		 trace++;
	 }
	 trace--;

	 the_Archive.Close();
	 fl.Close();
	 return trace; 
}
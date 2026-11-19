#include "StdAfx.h"
#include "axtextformatter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAXTextFormatter::CAXTextFormatter(void)
{
}

CAXTextFormatter::~CAXTextFormatter(void)
{
}

BOOL CAXTextFormatter::ValidateBreakBasis(char & chr)
{
    switch(chr)
	{
	case ' ':
		    return TRUE;
	case '\r\n':
		    return TRUE;
	case '\n':
		    return TRUE;
	case '\r':
		    return TRUE;
	case '*':
		    return TRUE;
	case '&':
		    return TRUE;
	case '@':
		    return TRUE;
	case '!':
		    return TRUE;
	case '(':
		    return TRUE;
	case ')':
		    return TRUE;
	case '-':
		    return TRUE;
	case '_':
		    return TRUE;
	case '[':
		    return TRUE;
	case ']':
		    return TRUE;
	case '{':
		    return TRUE;
	case '}':
		    return TRUE;
	case '"':
		    return TRUE;
	case ',':
		    return TRUE;
	case '.':
		    return TRUE;
	case '<':
		    return TRUE;
	case '>':
		    return TRUE;
	}
	return FALSE;
}

void CAXTextFormatter::ReplaceNoCaseWordsALL(CString & current,CString find, CString replace)
{
    
     while(ReplaceNoCaseWordsE(current,find,replace))
	 {

	 }
}

BOOL CAXTextFormatter::ReplaceNoCaseWordsE(CString & current,CString find, CString replace)
{
     int mainCount				= current.GetLength();
	 int findCount				= find.GetLength();
	 find = find.MakeLower();
	 int replaceCount			= replace.GetLength();

	 if(findCount == 0 || mainCount == 0 || mainCount < findCount)
	    return FALSE;//not possible replace
	 
	 char * bufferMain			= current.GetBuffer();
     CString mainLower          = current;
	 mainLower.MakeLower();
	 char * bufferCMP			= mainLower.GetBuffer();
	 char * bufferFind			= find.GetBuffer();
	 char * bufferReplace		= replace.GetBuffer();
	 int trace					= 0;
	 BOOL proceed = FALSE;
	 BOOL newBufAvail = FALSE;
	 char * newBuf = NULL;
	 while(trace < ((mainCount-findCount)+1))
	 {
          if(memcmp(&bufferCMP[trace],bufferFind,findCount)==0)
		  {//found match
			    proceed = FALSE;
                if(trace+findCount < mainCount && trace > 0)
				{
				   if(ValidateBreakBasis(bufferMain[trace+findCount]) == TRUE && ValidateBreakBasis(bufferMain[trace-1]) == TRUE)
				      proceed = TRUE;
				}
				else
				   proceed = TRUE;

				if(proceed)
				{
				   int newCount = (mainCount - findCount)+replaceCount+1;
				   newBuf = new char[newCount];
				   newBuf[newCount-1] = NULL;
                   CopyMemory(newBuf,bufferMain,trace);//replace existing prev
				   CopyMemory(&newBuf[trace],bufferReplace,replaceCount);//add new stuffs
				   if(((mainCount-findCount)-trace) > 0)
				        CopyMemory(&newBuf[trace+replaceCount],&bufferMain[trace+findCount],(mainCount-findCount)-trace);//add new stuffs
				   break;
				}
		  }//end found match
          trace++;
	 }
	 if(newBuf)
	 {
        current = newBuf;
        delete [] newBuf;
		newBuf = 0;
		return TRUE;
	 }

return FALSE;
}
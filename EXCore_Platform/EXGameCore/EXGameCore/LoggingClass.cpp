#include "StdAfx.h"
#include "loggingclass.h"
#include <fstream>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AXLogSinkFn g_axLogSink = NULL;
static void *      g_axLogSinkCtx = NULL;

void AXSetLogSink(AXLogSinkFn fn, void * context)
{
	g_axLogSink = fn;
	g_axLogSinkCtx = context;
}

void AXLogSinkWrite(const char * line)
{
	AXLogSinkFn fn = g_axLogSink;
	if(fn && line && line[0])
		fn(line, g_axLogSinkCtx);
}

CLoggingClass::CLoggingClass(void)
{
	m_loggingType = 1;
	m_logFileName = "default.txt";
}

CLoggingClass::~CLoggingClass(void)
{
}

void CLoggingClass::ConvertToString(long stringTemp, char * buff)
{
        if(stringTemp==0)
		{
		   buff[0] = '0';
		   buff[1] = 0;
		}
		else
		{
        _ltoa(stringTemp,buff,10);
		}
}


void CLoggingClass::WriteDebugFile(long debugString,BOOL clearIt,BOOL returnCase)
{
	if(!m_loggingType)
		return;

	    char buffer[67];
        if(debugString==0)
		{
		   buffer[0] = '0';
		   buffer[1] = 0;
		}
		else
		{
        _ltoa(debugString,buffer,10);
		}

		WriteDebugFile(buffer,clearIt,returnCase);
}
//-----------------------------------------------------------------//
void CLoggingClass::WriteDebugFile(LPSTR debugString,BOOL clearIt,BOOL returnCase)
{
	if(!m_loggingType)
		return;

	if(returnCase)
		AXLogSinkWrite(debugString);

    int oldFile = 0;
	if(clearIt)	
	   oldFile = _open( m_logFileName.GetBuffer(), _O_WRONLY | _O_TRUNC  | _O_CREAT  |_O_BINARY , _S_IWRITE);  
	else
	{
	   oldFile = _open( m_logFileName.GetBuffer(), _O_WRONLY  | _O_CREAT  |_O_BINARY , _S_IWRITE);  
	   if(oldFile == -1)
	      return;
	   
	   _lseek( oldFile, 0L, SEEK_END );


	}

	if(oldFile == -1)
	      return;
	
	int trace = 0;
    while(debugString[trace++]!= 0)
	{
        //trace++;
	}
	long bytesRead = _write( oldFile, debugString, trace-1 );
	if(returnCase)
	{
	 LPSTR returnCase = "\r\n";
     _write( oldFile, returnCase, 2 );
	}
 
	_close( oldFile );

    
}
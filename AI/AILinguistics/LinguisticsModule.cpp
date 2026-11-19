/* Programming/Implimentation: Arron Boucher.
   Debugging: Sean Barnes.
   08/07/2004
*/

#include "StdAfx.h"
#include "linguisticsmodule.h"
#include "string.h"

char * WINAPI ExternalInit(char * datapath);
char * WINAPI TalkToBot(char * speech);
char * WINAPI SimpleTest();
int WINAPI ReleaseChatter();
static CLinguisticsModule * g_global = NULL;

CLinguisticsModule::CLinguisticsModule(CString dataPath)
{
   m_botBrainCore = new Core(dataPath);
   if(m_botBrainCore->m_initProp)
   {
	pu.bot = m_botBrainCore;
	m_botBrainCore->pu = &pu;
	m_botBrainCore->initialize();
	ZeroMemory(m_fromUserText,2048);
	ZeroMemory(m_botResponse,2048); 
   }
}

CLinguisticsModule::~CLinguisticsModule(void)
{
   if(m_botBrainCore)
   {
      delete m_botBrainCore;
	  m_botBrainCore = 0;
   }
}

CString CLinguisticsModule::SpeechHandler(CString fromUserText)
{
    if(!m_botBrainCore->m_initProp)
        return m_botBrainCore->m_lastError;

    ZeroMemory(m_fromUserText,2048);
    ZeroMemory(m_botResponse,2048); 

    for(int i=0;i<fromUserText.GetLength();i++)
        m_fromUserText[i] = fromUserText.GetAt(i);

	m_botBrainCore->getAnswer(m_fromUserText,m_botResponse);
    return m_botResponse;
}

char * WINAPI ExternalInit(char * datapath)
{
     if(!g_global)
         g_global = new CLinguisticsModule(datapath);
	 
     return "Completed";
}


char * WINAPI TalkToBot(char * speech)
{
     if(!g_global)
         return "Bot must be initialized";
	 
	 CString response = g_global->SpeechHandler(speech);
	
     return response.GetBuffer();
}

int WINAPI ReleaseChatter()
{
     if(g_global)
	 {
        delete g_global;
	    g_global = 0;
	 }
	
     return 0;
}

char * WINAPI SimpleTest()
{
	//_bstr_t test("hello there");
	
	/*ZeroMemory(test,256);
	test[0] = 'H';
	test[1] = 'i';
	string testSTR = "hello sean";*/
	return "hello";
}

 
#include "dplay8.h"
#include "dplobby.h"
#include "ABCore.h"

class __declspec(dllexport) ADplayModule
{

   public:

   ADplayModule(int mode,
	            int version,
				CRITICAL_SECTION & criticalSection);


   static IDirectPlay8Client *   m_clientInterface;
   static IDirectPlay8Server *   m_serverInterface;
   static CRITICAL_SECTION       m_recieveCriticalSection;
   DPNHANDLE                     m_dpAsncEnumHostHandle;
   int m_version;
   //status
   long m_totalBandwidthOutGoing;
   long m_packetsOutPerPass;


   //functions
   static HRESULT WINAPI DirectPlayMessageHandler( PVOID pvUserContext, 
                                                    DWORD dwMessageId, 
                                                    PVOID pMsgBuffer );
   void            SendData(BYTE * data,
						    DWORD size,
						    DWORD flags,
						    DPNID sendTo,
						    DWORD timeout);
   BOOL StopEnumerating();
   BOOL EnumHosts(LPSTR TCPIP);


};


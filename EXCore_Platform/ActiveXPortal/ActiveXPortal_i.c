

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Fri Jun 22 03:30:05 2007
 */
/* Compiler settings for .\ActiveXPortal.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_ActiveXPortalLib,0x7FB4ED24,0x9423,0x44BA,0xA7,0x03,0x15,0x9A,0x0F,0x71,0x21,0xF2);


MIDL_DEFINE_GUID(IID, DIID__DActiveXPortal,0xDE6E43D5,0x7B1B,0x404F,0x95,0x2A,0x1B,0xB7,0xCD,0x86,0xC4,0x5D);


MIDL_DEFINE_GUID(IID, DIID__DActiveXPortalEvents,0x4E6AF5AC,0xFB8E,0x47AD,0xB6,0x4A,0x86,0xF8,0x91,0x9D,0x99,0xFF);


MIDL_DEFINE_GUID(CLSID, CLSID_ActiveXPortal,0x3C403675,0xB43C,0x410B,0xBF,0x56,0xD4,0xD1,0xFB,0x68,0x35,0x6C);


MIDL_DEFINE_GUID(IID, DIID_IHMusic,0xFAFD559E,0xEECF,0x45C3,0xA2,0x30,0x0B,0x2F,0xE8,0x1B,0x62,0x46);


MIDL_DEFINE_GUID(CLSID, CLSID_HMusic,0x4F1C2C7F,0x0E52,0x4386,0x95,0xDF,0xF7,0xD1,0x8B,0xFA,0x67,0x00);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif






/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Mon Nov 07 14:14:50 2005
 */
/* Compiler settings for .\AX_EngineControl.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_AX_EngineControlLib,0x1CFD2496,0x48DE,0x462F,0xAD,0x3D,0x75,0x58,0xB8,0xEB,0x29,0xFC);


MIDL_DEFINE_GUID(IID, DIID__DAX_EngineControl,0xCFADE9AA,0x15CB,0x4F5C,0x8D,0x61,0xB5,0xD9,0x35,0xA3,0xDD,0xBF);


MIDL_DEFINE_GUID(IID, DIID__DAX_EngineControlEvents,0x05C5A3A1,0xB800,0x4747,0x9B,0x65,0x80,0xD0,0x51,0x68,0x05,0xF6);


MIDL_DEFINE_GUID(CLSID, CLSID_AX_EngineControl,0x14A6E84C,0x7F66,0x4BA6,0x80,0xCD,0x75,0x9C,0xE1,0x25,0xF2,0xD1);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif




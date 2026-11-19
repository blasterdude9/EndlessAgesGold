

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __ActiveXPortalidl_h__
#define __ActiveXPortalidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DActiveXPortal_FWD_DEFINED__
#define ___DActiveXPortal_FWD_DEFINED__
typedef interface _DActiveXPortal _DActiveXPortal;
#endif 	/* ___DActiveXPortal_FWD_DEFINED__ */


#ifndef ___DActiveXPortalEvents_FWD_DEFINED__
#define ___DActiveXPortalEvents_FWD_DEFINED__
typedef interface _DActiveXPortalEvents _DActiveXPortalEvents;
#endif 	/* ___DActiveXPortalEvents_FWD_DEFINED__ */


#ifndef __ActiveXPortal_FWD_DEFINED__
#define __ActiveXPortal_FWD_DEFINED__

#ifdef __cplusplus
typedef class ActiveXPortal ActiveXPortal;
#else
typedef struct ActiveXPortal ActiveXPortal;
#endif /* __cplusplus */

#endif 	/* __ActiveXPortal_FWD_DEFINED__ */


#ifndef __IHMusic_FWD_DEFINED__
#define __IHMusic_FWD_DEFINED__
typedef interface IHMusic IHMusic;
#endif 	/* __IHMusic_FWD_DEFINED__ */


#ifndef __HMusic_FWD_DEFINED__
#define __HMusic_FWD_DEFINED__

#ifdef __cplusplus
typedef class HMusic HMusic;
#else
typedef struct HMusic HMusic;
#endif /* __cplusplus */

#endif 	/* __HMusic_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __ActiveXPortalLib_LIBRARY_DEFINED__
#define __ActiveXPortalLib_LIBRARY_DEFINED__

/* library ActiveXPortalLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_ActiveXPortalLib;

#ifndef ___DActiveXPortal_DISPINTERFACE_DEFINED__
#define ___DActiveXPortal_DISPINTERFACE_DEFINED__

/* dispinterface _DActiveXPortal */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DActiveXPortal;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DE6E43D5-7B1B-404F-952A-1BB7CD86C45D")
    _DActiveXPortal : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DActiveXPortalVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DActiveXPortal * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DActiveXPortal * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DActiveXPortal * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DActiveXPortal * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DActiveXPortal * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DActiveXPortal * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DActiveXPortal * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DActiveXPortalVtbl;

    interface _DActiveXPortal
    {
        CONST_VTBL struct _DActiveXPortalVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DActiveXPortal_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DActiveXPortal_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DActiveXPortal_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DActiveXPortal_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DActiveXPortal_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DActiveXPortal_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DActiveXPortal_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DActiveXPortal_DISPINTERFACE_DEFINED__ */


#ifndef ___DActiveXPortalEvents_DISPINTERFACE_DEFINED__
#define ___DActiveXPortalEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DActiveXPortalEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DActiveXPortalEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("4E6AF5AC-FB8E-47AD-B64A-86F8919D99FF")
    _DActiveXPortalEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DActiveXPortalEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DActiveXPortalEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DActiveXPortalEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DActiveXPortalEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DActiveXPortalEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DActiveXPortalEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DActiveXPortalEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DActiveXPortalEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DActiveXPortalEventsVtbl;

    interface _DActiveXPortalEvents
    {
        CONST_VTBL struct _DActiveXPortalEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DActiveXPortalEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DActiveXPortalEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DActiveXPortalEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DActiveXPortalEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DActiveXPortalEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DActiveXPortalEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DActiveXPortalEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DActiveXPortalEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ActiveXPortal;

#ifdef __cplusplus

class DECLSPEC_UUID("3C403675-B43C-410B-BF56-D4D1FB68356C")
ActiveXPortal;
#endif

#ifndef __IHMusic_DISPINTERFACE_DEFINED__
#define __IHMusic_DISPINTERFACE_DEFINED__

/* dispinterface IHMusic */
/* [uuid] */ 


EXTERN_C const IID DIID_IHMusic;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("FAFD559E-EECF-45C3-A230-0B2FE81B6246")
    IHMusic : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IHMusicVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHMusic * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHMusic * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHMusic * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHMusic * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHMusic * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHMusic * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHMusic * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IHMusicVtbl;

    interface IHMusic
    {
        CONST_VTBL struct IHMusicVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHMusic_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHMusic_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHMusic_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHMusic_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHMusic_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHMusic_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHMusic_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IHMusic_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_HMusic;

#ifdef __cplusplus

class DECLSPEC_UUID("4F1C2C7F-0E52-4386-95DF-F7D18BFA6700")
HMusic;
#endif
#endif /* __ActiveXPortalLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



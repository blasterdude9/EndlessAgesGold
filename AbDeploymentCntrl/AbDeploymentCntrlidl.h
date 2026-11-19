

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Jul 22 17:08:50 2004
 */
/* Compiler settings for .\AbDeploymentCntrl.idl:
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


#ifndef __AbDeploymentCntrlidl_h__
#define __AbDeploymentCntrlidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DAbDeploymentCntrl_FWD_DEFINED__
#define ___DAbDeploymentCntrl_FWD_DEFINED__
typedef interface _DAbDeploymentCntrl _DAbDeploymentCntrl;
#endif 	/* ___DAbDeploymentCntrl_FWD_DEFINED__ */


#ifndef ___DAbDeploymentCntrlEvents_FWD_DEFINED__
#define ___DAbDeploymentCntrlEvents_FWD_DEFINED__
typedef interface _DAbDeploymentCntrlEvents _DAbDeploymentCntrlEvents;
#endif 	/* ___DAbDeploymentCntrlEvents_FWD_DEFINED__ */


#ifndef __AbDeploymentCntrl_FWD_DEFINED__
#define __AbDeploymentCntrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class AbDeploymentCntrl AbDeploymentCntrl;
#else
typedef struct AbDeploymentCntrl AbDeploymentCntrl;
#endif /* __cplusplus */

#endif 	/* __AbDeploymentCntrl_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __AbDeploymentCntrlLib_LIBRARY_DEFINED__
#define __AbDeploymentCntrlLib_LIBRARY_DEFINED__

/* library AbDeploymentCntrlLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_AbDeploymentCntrlLib;

#ifndef ___DAbDeploymentCntrl_DISPINTERFACE_DEFINED__
#define ___DAbDeploymentCntrl_DISPINTERFACE_DEFINED__

/* dispinterface _DAbDeploymentCntrl */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DAbDeploymentCntrl;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1C678D41-D494-487C-B202-DEFECC2535D2")
    _DAbDeploymentCntrl : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DAbDeploymentCntrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DAbDeploymentCntrl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DAbDeploymentCntrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DAbDeploymentCntrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DAbDeploymentCntrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DAbDeploymentCntrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DAbDeploymentCntrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DAbDeploymentCntrl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DAbDeploymentCntrlVtbl;

    interface _DAbDeploymentCntrl
    {
        CONST_VTBL struct _DAbDeploymentCntrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DAbDeploymentCntrl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DAbDeploymentCntrl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DAbDeploymentCntrl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DAbDeploymentCntrl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DAbDeploymentCntrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DAbDeploymentCntrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DAbDeploymentCntrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DAbDeploymentCntrl_DISPINTERFACE_DEFINED__ */


#ifndef ___DAbDeploymentCntrlEvents_DISPINTERFACE_DEFINED__
#define ___DAbDeploymentCntrlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DAbDeploymentCntrlEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DAbDeploymentCntrlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("67C753D9-6EB2-4F15-8A39-F75E801C24FE")
    _DAbDeploymentCntrlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DAbDeploymentCntrlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DAbDeploymentCntrlEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DAbDeploymentCntrlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DAbDeploymentCntrlEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DAbDeploymentCntrlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DAbDeploymentCntrlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DAbDeploymentCntrlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DAbDeploymentCntrlEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DAbDeploymentCntrlEventsVtbl;

    interface _DAbDeploymentCntrlEvents
    {
        CONST_VTBL struct _DAbDeploymentCntrlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DAbDeploymentCntrlEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DAbDeploymentCntrlEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DAbDeploymentCntrlEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DAbDeploymentCntrlEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DAbDeploymentCntrlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DAbDeploymentCntrlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DAbDeploymentCntrlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DAbDeploymentCntrlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_AbDeploymentCntrl;

#ifdef __cplusplus

class DECLSPEC_UUID("31CFD255-1408-4782-834E-C9F9C9D077C6")
AbDeploymentCntrl;
#endif
#endif /* __AbDeploymentCntrlLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



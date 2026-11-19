

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __AX_EngineControlidl_h__
#define __AX_EngineControlidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DAX_EngineControl_FWD_DEFINED__
#define ___DAX_EngineControl_FWD_DEFINED__
typedef interface _DAX_EngineControl _DAX_EngineControl;
#endif 	/* ___DAX_EngineControl_FWD_DEFINED__ */


#ifndef ___DAX_EngineControlEvents_FWD_DEFINED__
#define ___DAX_EngineControlEvents_FWD_DEFINED__
typedef interface _DAX_EngineControlEvents _DAX_EngineControlEvents;
#endif 	/* ___DAX_EngineControlEvents_FWD_DEFINED__ */


#ifndef __AX_EngineControl_FWD_DEFINED__
#define __AX_EngineControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class AX_EngineControl AX_EngineControl;
#else
typedef struct AX_EngineControl AX_EngineControl;
#endif /* __cplusplus */

#endif 	/* __AX_EngineControl_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __AX_EngineControlLib_LIBRARY_DEFINED__
#define __AX_EngineControlLib_LIBRARY_DEFINED__

/* library AX_EngineControlLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_AX_EngineControlLib;

#ifndef ___DAX_EngineControl_DISPINTERFACE_DEFINED__
#define ___DAX_EngineControl_DISPINTERFACE_DEFINED__

/* dispinterface _DAX_EngineControl */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DAX_EngineControl;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CFADE9AA-15CB-4F5C-8D61-B5D935A3DDBF")
    _DAX_EngineControl : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DAX_EngineControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DAX_EngineControl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DAX_EngineControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DAX_EngineControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DAX_EngineControl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DAX_EngineControl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DAX_EngineControl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DAX_EngineControl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DAX_EngineControlVtbl;

    interface _DAX_EngineControl
    {
        CONST_VTBL struct _DAX_EngineControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DAX_EngineControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DAX_EngineControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DAX_EngineControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DAX_EngineControl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DAX_EngineControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DAX_EngineControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DAX_EngineControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DAX_EngineControl_DISPINTERFACE_DEFINED__ */


#ifndef ___DAX_EngineControlEvents_DISPINTERFACE_DEFINED__
#define ___DAX_EngineControlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DAX_EngineControlEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DAX_EngineControlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("05C5A3A1-B800-4747-9B65-80D0516805F6")
    _DAX_EngineControlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DAX_EngineControlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DAX_EngineControlEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DAX_EngineControlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DAX_EngineControlEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DAX_EngineControlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DAX_EngineControlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DAX_EngineControlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DAX_EngineControlEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DAX_EngineControlEventsVtbl;

    interface _DAX_EngineControlEvents
    {
        CONST_VTBL struct _DAX_EngineControlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DAX_EngineControlEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DAX_EngineControlEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DAX_EngineControlEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DAX_EngineControlEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DAX_EngineControlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DAX_EngineControlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DAX_EngineControlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DAX_EngineControlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_AX_EngineControl;

#ifdef __cplusplus

class DECLSPEC_UUID("14A6E84C-7F66-4BA6-80CD-759CE125F2D1")
AX_EngineControl;
#endif
#endif /* __AX_EngineControlLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



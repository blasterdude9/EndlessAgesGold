//
//  Microsoft Windows Media Technologies
//  Copyright (C) Microsoft Corporation, 1999 - 1999  All rights reserved.
//
// You have a royalty-free right to use, modify, reproduce and distribute
// the Sample Application Files (including modified versions) in any way 
// you determine to be useful, provided that you agree that Microsoft 
// Corporation provides no warrant or support, and has no obligation or
// liability resulting from the use of any Sample Application Files. 
//
#ifndef SIMPLEPLAYER_H
#define SIMPLEPLAYER_H


#include "wmsdk.h"
#ifdef SUPPORT_DRM
#include "wmsdkdrm.h"
#endif

#include <mmsystem.h>
#include <vfw.h>


///////////////////////////////////////////////////////////////////////////////
class  AFX_EXT_CLASS CSimplePlayer : public IWMReaderCallback
{
public:
    CSimplePlayer();
    ~CSimplePlayer();
    
	BOOL initOnce;
	BOOL looping;
	BOOL m_IsPaused;

	HRESULT Pause();
	HRESULT Resume();
	HRESULT SeekStart();
	HRESULT Close();

	HRESULT Stop();
    virtual HRESULT Play( LPCWSTR pszUrl, HANDLE hCompletionEvent, HRESULT *phrCompletion );

//
// IUnknown Implemenation
//
public:
    LONG    m_cBuffersOutstanding;

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        REFIID riid,
        void **ppvObject );

    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

//
// IUnknown Implemenation
//
public:
    virtual HRESULT STDMETHODCALLTYPE OnSample( 
        /* [in] */ DWORD dwOutputNum,
        /* [in] */ QWORD cnsSampleTime,
        /* [in] */ QWORD cnsSampleDuration,
        /* [in] */ DWORD dwFlags,
        /* [in] */ INSSBuffer __RPC_FAR *pSample,
        /* [in] */ void __RPC_FAR *pvContext);
    
    virtual HRESULT STDMETHODCALLTYPE OnStatus( 
        /* [in] */ WMT_STATUS Status,
        /* [in] */ HRESULT hr,
        /* [in] */ WMT_ATTR_DATATYPE dwType,
        /* [in] */ BYTE __RPC_FAR *pValue,
        /* [in] */ void __RPC_FAR *pvContext);

//
// Helper Methods
//
protected:

    void OnWaveOutMsg( UINT uMsg, DWORD dwParam1, DWORD dwParam2 );

    static void CALLBACK WaveProc(
                          HWAVEOUT hwo,      
                          UINT uMsg,         
                          DWORD dwInstance,  
                          DWORD dwParam1,    
                          DWORD dwParam2 );
 
    HRESULT DoCodecDownload( GUID*  pCodecID );

    LONG    m_cRef;
    
    BOOL    m_fEof;
    HANDLE  m_hCompletionEvent;

    IWMReader *m_pReader;
    IWMHeaderInfo *m_pHeader;
    HWAVEOUT m_hwo;

    HRESULT *m_phrCompletion;

    HRESULT m_hrOpen;
    HANDLE m_hOpenEvent;

    union
    {
        WAVEFORMATEX m_wfx;
        BYTE m_WfxBuf[1024];
    };

    LPWSTR  m_pszUrl;

};

#endif // SIMPLEPLAYER_H

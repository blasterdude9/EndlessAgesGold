#include "StdAfx.h"
#include ".\axmedialib.h"

AXMediaLib::AXMediaLib(void * axHandleX)
{
    ///////////////////////////////////////////////////
    // Initialize WMP control
    //////////////////////////////////////////////////

	CAxWindow * axHandle = (CAxWindow *)axHandleX;
	m_playerInitializedOk = FALSE;
	CComPtr<IAxWinHostWindow>	spHost;
    CComPtr<IObjectWithSite>    spHostObject;
    CComObject<CRemoteHost>     *pRemoteHost = NULL;
    RECT                        rectWMP={0,0,0,0};
    HRESULT                     hr;
    
	/*AtlAxWinInit();
    m_pView = new CAxWindow();
 	hr = m_pView? S_OK : E_OUTOFMEMORY;
   
    if(hr != S_OK)
	   return;

    m_pView->Create(parentHandle, rectWMP, NULL, WS_CHILD);*/
	//hr = ::IsWindow(m_pView->m_hWnd)? S_OK : E_FAIL;
	hr = ::IsWindow(axHandle->m_hWnd)? S_OK : E_FAIL;
    if(hr != S_OK)
	   return;

	BOOL m_bIsRemote = TRUE;

	/*if(m_bIsRemote)
	{
        hr = axHandle->QueryHost(IID_IObjectWithSite, (void **)&spHostObject);
		hr = spHostObject.p? hr : E_FAIL;

		if(SUCCEEDED(hr))
		{
			hr = CComObject<CRemoteHost>::CreateInstance(&pRemoteHost);
			if(pRemoteHost)
			{
				pRemoteHost->AddRef();
			}
			else
			{
				hr = E_POINTER;
			}
		}

		if(SUCCEEDED(hr))
		{
			hr = spHostObject->SetSite((IWMPRemoteMediaServices *)pRemoteHost);
		}
	}*/

	if(hr != S_OK)
	   return;

		hr = axHandle->QueryHost(&spHost);
		hr = spHost.p? hr : E_FAIL;

	// Create WMP control using its CLSID
	if(hr != S_OK)
	   return;

		//hr = spHost->CreateControl(CComBSTR(L"{6BF52A52-394A-11d3-B153-00C04F79FAA6}"), m_pView->m_hWnd, 0);
		hr = spHost->CreateControl(CComBSTR(L"{6BF52A52-394A-11d3-B153-00C04F79FAA6}"), axHandle->m_hWnd, 0);

	if(hr != S_OK)
	   return;

		hr = axHandle->QueryControl(&m_spPlayer);
		hr = m_spPlayer.p? hr : E_FAIL;
	

    // Release remote host object
    if(pRemoteHost)
    {
        pRemoteHost->Release();
    }

	CComBSTR mode("invisible");
    m_spPlayer->put_uiMode(mode);
	m_playerInitializedOk = TRUE;
}

AXMediaLib::~AXMediaLib(void)
{
	m_spPlayer = NULL;
    /*if(m_pView != NULL)
    {
        delete m_pView;
    }*/
}

BOOL AXMediaLib::PlayURL(char * url)
{
    //CComBSTR pl_value("http://www.digitallyimported.com/wma/chillout.asx");
	CComBSTR pl_value(url);
	IWMPMedia * mediaTemp = NULL;
    HRESULT hr = m_spPlayer->newMedia(pl_value,&mediaTemp);
    if(hr == S_OK)
	{
		hr = m_spPlayer->put_currentMedia(mediaTemp);
		IWMPControls * controller = NULL;
		if(hr != S_OK)
		   return FALSE;
		hr = m_spPlayer->get_controls(&controller);
		if(hr != S_OK)
           return FALSE;
		controller->play();
		return TRUE;
	}
    return FALSE;
}

BOOL AXMediaLib::Stop()
{

   IWMPControls * controller = NULL;
   HRESULT hr = m_spPlayer->get_controls(&controller);
   if(hr != S_OK)
	    return FALSE;
   controller->stop();
   
   return TRUE;
}



#include "stdafx.h"
#include "CMusicClass.h"
#include "CABFunctionLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMusicAX::CMusicAX()
{
	InitializeCriticalSection(&m_cSection);
	/*m_pGraph = NULL;
    m_pControl = NULL;
    m_pEvent = NULL;
	m_audioControl = NULL;
	m_mediaSeekingControl = NULL;*/
	m_looping = true;
	m_maxLoopCount = 0;
	m_lastUpdateTime = GetTickCount();
	m_updateInterval = 50;
	m_targetStream = "";
    m_currentStream = "";
	m_transitionMode = 0;//0=none 1=fade out 2=fade in
	m_currentVolume = 0;//start silent
	m_validForCallback = 0;
	m_quePlay = 0;
	m_currentLoopCount = 0;
	m_currentTargetVolume = 0;
	m_lastSetVolume = 0;
	m_volumeSetting = 50;
	m_overrideStation = "";//blank means no override
	m_disabled = FALSE;//this will disable music
	m_disabledCurState = FALSE;
	m_musicStreamerEnabled = FALSE;
	m_urlMode = FALSE;
	ReadCfgFile();
}

CMusicAX::~CMusicAX()
{
	WriteCfgFile();
	/*if(m_pControl)
       m_pControl->Stop();*/

    SafeDelete();
	DeleteCriticalSection(&m_cSection);
}

void CMusicAX::WriteCfgFile()
{
	 CABFunctionLib cabLib;
     CFileException exc;
     CFile fl;
	 CArchive the_out_Archive(&fl,CArchive::store);
     BOOL res = fl.Open("musicsetting.cfg",CFile::modeCreate | CFile::modeWrite, &exc);
	 if(res == FALSE){
		return;
	 }

	 the_out_Archive.WriteString("Volume:");
	 the_out_Archive.WriteString(cabLib.ConvToString(m_volumeSetting));
	 the_out_Archive.WriteString("\r\n");
	 the_out_Archive.WriteString("Override:");
	 the_out_Archive.WriteString(m_overrideStation);
	 the_out_Archive.WriteString("\r\n");
	 the_out_Archive.WriteString("Disabled:");
	 the_out_Archive.WriteString(cabLib.ConvToString(m_disabled));
	 the_out_Archive.WriteString("\r\n");
	 the_out_Archive.Close();
	 fl.Close();
	 
}

void CMusicAX::ReadCfgFile()
{
	CFileException exc;
    CFile fl;
    BOOL res = fl.Open("musicsetting.cfg",CFile::modeRead, &exc);
	if(res == FALSE){
		return;
	}
	CArchive the_in_Archive(&fl,CArchive::load);
    CString temp = "";
    if(the_in_Archive.ReadString(temp))
	{
		temp.Replace("Volume:","");
		m_volumeSetting = atoi(temp);
	}
	if(the_in_Archive.ReadString(temp))
	{
	   temp.Replace("Override:","");
	   m_overrideStation = temp;
	}
	if(the_in_Archive.ReadString(temp))
	{
	   temp.Replace("Disabled:","");
	   m_disabled = atoi(temp);
	}


	the_in_Archive.Close();
	fl.Close();
}

void CMusicAX::InitializeInterfaces()
{
	/*SafeDelete();
    HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                          IID_IGraphBuilder, (void **)&m_pGraph);
    if (FAILED(hr))
    {
        return;
    }

    hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pControl);
    hr = m_pGraph->QueryInterface(IID_IMediaEvent, (void **)&m_pEvent);
    hr = m_pGraph->QueryInterface(IID_IBasicAudio, (void **)&m_audioControl);  
	hr = m_pGraph->QueryInterface(IID_IMediaSeeking, (void**)&m_mediaSeekingControl);*/
}

void CMusicAX::SafeDelete()
{
    /*if(m_pControl)
	   m_pControl->Release();
	   m_pControl = 0;
	   
    if(m_pEvent)
	   m_pEvent->Release();
	   m_pEvent = 0;

    if(m_audioControl)
	   m_audioControl->Release();
	   m_audioControl = 0;

    if(m_mediaSeekingControl)
	   m_mediaSeekingControl->Release();
	   m_mediaSeekingControl = 0;

    if(m_pGraph)
	   m_pGraph->Release();
	   m_pGraph = 0;*/
}

HRESULT CMusicAX::PlayFile(CString filePath,int loopCount)
{
     if(m_disabled)
	    return 0;

    if(m_transitionMode != 0)
	   return 0;

	m_currentLoopCount = loopCount;
	m_maxLoopCount = loopCount;

	if(m_currentStream.GetLength() == 0)
	{
	   m_currentStream = filePath;
	   m_transitionMode = 0;
	   m_quePlay = 1;
	}
	else
	{
	   m_targetStream = filePath;
	   m_transitionMode = 1;
	}
	
	/*if(m_validForCallback == 1)
	{//already playing set into pending mode


	}//end already playing set into pending mode*/
     //HRESULT hr = m_pGraph->RenderFile(L"C:\\Projects\\EngineMainApp\\Sounds\\Test.mp3", NULL);
    //CHString newString = filePath;
	//InitializeInterfaces();
	//m_currentVolume = 0;
	//HRESULT hr = m_pGraph->RenderFile((CHString)filePath, NULL);
	
    //LPCWSTR charArray = NULL;
	//charArray

	//HRESULT hr = m_pGraph->RenderFile(path, NULL);
    /*if (SUCCEEDED(hr))
    {
        //Run the graph.
        hr = m_pControl->Run();
        if (SUCCEEDED(hr))
        {
            //Wait for completion.
            //long evCode;
            //m_pEvent->WaitForCompletion(INFINITE, &evCode);

            //Note: Do not use INFINITE in a real application, because it
            //can block indefinitely.
        }
    }*/
	m_validForCallback = 1;
	return 0;
}

void CMusicAX::SetVolume(long volume)
{
    EnterCriticalSection(&m_cSection);
	/*
     if(m_audioControl)
        m_audioControl->put_Volume(InterpolateLong(-10000,0,100,volume));
*/

	 this->m_currentTargetVolume = volume;
	 LeaveCriticalSection(&m_cSection);
}

long CMusicAX::GetVolume()
{
     EnterCriticalSection(&m_cSection);
	 long volTemp = m_currentTargetVolume;
	 LeaveCriticalSection(&m_cSection);

	 return volTemp;
}

void CMusicAX::UpdateLastSetVolume(long vol)
{
	EnterCriticalSection(&m_cSection);
    m_lastSetVolume = vol;
    LeaveCriticalSection(&m_cSection);
}

void CMusicAX::SetMusicStreamerEnable(BOOL enabled)
{
	EnterCriticalSection(&m_cSection);
    m_musicStreamerEnabled = enabled;
    LeaveCriticalSection(&m_cSection);
}

BOOL CMusicAX::GetMusicStreamerStatus()
{
	BOOL result = FALSE;
	EnterCriticalSection(&m_cSection);
    result = m_musicStreamerEnabled;
    LeaveCriticalSection(&m_cSection);
	return result;
}


BOOL CMusicAX::GetDisabledStatus()
{
	BOOL result = FALSE;
	EnterCriticalSection(&m_cSection);
    result = m_disabled;
    LeaveCriticalSection(&m_cSection);
	return result;
}

BOOL CMusicAX::GetCurDisabledStateStatus()
{
	BOOL result = FALSE;
	EnterCriticalSection(&m_cSection);
    result = m_disabledCurState;
    LeaveCriticalSection(&m_cSection);
	return result;
}

void CMusicAX::SetDisabledCurState(BOOL enabled)
{
	EnterCriticalSection(&m_cSection);
    m_disabledCurState = enabled;
    LeaveCriticalSection(&m_cSection);
}


BOOL CMusicAX::VolumeUpdateNeeded()
{
	BOOL result = FALSE;
	EnterCriticalSection(&m_cSection);
    if(m_currentTargetVolume != m_lastSetVolume)
	   result = TRUE;
    LeaveCriticalSection(&m_cSection);
	return result;
}

void CMusicAX::SeekPosition(LONGLONG pos)
{

     /*if(m_mediaSeekingControl)
        m_mediaSeekingControl->SetPositions(&pos, AM_SEEKING_AbsolutePositioning,NULL, AM_SEEKING_NoPositioning);*/


}

void CMusicAX::Callback(long currenttime)
{
     /*if(m_disabled)
	    return;

	 if(m_urlMode == FALSE)
	 if(!m_validForCallback)
	    return;

     if((currenttime - m_lastUpdateTime) < m_updateInterval)
	     return;

	 if(m_urlMode == FALSE)
	 if(m_quePlay == 1)
	 {
	    InitializeInterfaces();
		
		LPCWSTR tempName = A2BSTR( m_currentStream.GetBuffer() );

	    if(m_pGraph->RenderFile(tempName, NULL)==0)
           m_pControl->Run();
        m_quePlay = 0;
	 }

     m_lastUpdateTime = currenttime;

	 if(m_urlMode == FALSE)
	 {
		if(m_pEvent->GetEvent(&m_evCode, &m_evParam1, &m_evParam2, 0)==0)
		if(EC_COMPLETE == m_evCode)
		{
			if(m_looping == true)
			{
				if(m_maxLoopCount > 0)
				{
					if(m_currentLoopCount == 0)
					{
						m_pControl->Stop();
						m_validForCallback = 0;
						return;
					}
					else
						m_currentLoopCount--;
					
				}
				SeekPosition(0);
			}
			else
				m_pControl->Stop();
		}
	 }

	 //handle Fades/transitions

	 if(m_transitionMode == 0)
	 {
	  if(m_currentVolume < m_volumeSetting)
	  {
        SetVolume(m_currentVolume++);
	  }
	  else if(m_currentVolume > m_volumeSetting)
	    SetVolume(m_currentVolume--);
	 }
	 else if(m_transitionMode == 1)
	 {
      if(m_currentVolume > 0)
	  {
        SetVolume(m_currentVolume--);
	  }
	  else
	  {
	    if(m_urlMode == FALSE)
		{
			m_currentStream = m_targetStream;
			m_targetStream = "";
			m_transitionMode = 0;
			m_quePlay = 1;
		}
	  }
	 }*/
}

void CMusicAX::Disable()
{
    m_disabled = TRUE;
    Stop();
   
}


void CMusicAX::Enable()
{

     m_disabled = FALSE;

}


void CMusicAX::Stop()
{
	 /*if(m_pControl)
	 {
     m_pControl->Stop();
     m_validForCallback = 0;
	 m_transitionMode = 0;
	 }*/
}

long CMusicAX::InterpolateLong(
						 long StartVec, 
						 long EndVec,
					     int NumOfStages, 
					     int ThisStage)
{
 float propfactor;
 if (NumOfStages <= 0)
  NumOfStages = 1;
  propfactor = (float)ThisStage/NumOfStages;
  long StageVec = (long)(StartVec + propfactor * (EndVec - StartVec));

 return StageVec;
}

#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "direct.h"

#include "CSoundManagerClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
CSoundSampleObj::CSoundSampleObj()
{
	m_soundBuffer         = NULL;
	m_3dBuffer            = NULL;
	m_fileName            = "NONE";
	m_loopable            = FALSE;
	m_soundFactor         = .01f;
	m_selfDeleteing       = TRUE;
	m_playing             = FALSE;
	
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
 BOOL CSoundSampleObj::UpdatePosition(D3DVECTOR & positionalUpdate)
{
    if(!m_3dBuffer)
		return FALSE;
   


	m_3dBuffer->SetPosition(        positionalUpdate.x,
								    positionalUpdate.y,
								    positionalUpdate.z,
								    DS3D_DEFERRED);
return TRUE;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
BOOL CSoundSampleObj::UpdateFreqBasedOnSpeed(D3DVECTOR velocityVector)
{
    if(!m_3dBuffer)
		return FALSE;

	
	
	m_3dBuffer->SetVelocity(velocityVector.x,velocityVector.y,velocityVector.z,DS3D_DEFERRED);
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Interpolation between animations BABY!! Imediate mode style-----------//
float CSoundSampleObj::InterpolateFloatsHiRes(
						            float StartVec, 
						            float EndVec,
					                float NumOfStages, 
					                float ThisStage)
{
 if(ThisStage > NumOfStages)
    ThisStage = NumOfStages;

 float propfactor;
 if (NumOfStages <= 0)
  NumOfStages = 1;
  propfactor = ThisStage/NumOfStages;
  float StageVec = StartVec + propfactor * (EndVec - StartVec);

 return StageVec;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
BOOL CSoundSampleObj::UpdateFreqBasedOnSpeed(float maxSpeed, float curSpeed, float startFreq)
{
    if(!m_3dBuffer)
		return FALSE;

	float freq = InterpolateFloatsHiRes(startFreq,100000.0f,maxSpeed,curSpeed);
	
	m_soundBuffer->SetFrequency((DWORD)freq);
return TRUE;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
 BOOL CSoundSampleObj::SetToDelete()
{
	if(!m_soundBuffer)
		return FALSE;

	if(m_loopable)
    m_soundBuffer->Stop();

	m_loopable = FALSE;
	m_selfDeleteing = TRUE;
     
return TRUE;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CSoundSampleObj::Clone(CSoundSampleObj ** clone,IDirectSound8 * directSoundPtr)
{
     CSoundSampleObj * clonePtr = new CSoundSampleObj();
	 //copyLocal->
	 //base data
	 clonePtr->m_fileName     = m_fileName;
	 clonePtr->m_soundFactor  = m_soundFactor;
	 clonePtr->m_loopable     = m_loopable;
	 //base sound buffer
     if(directSoundPtr){//valid direct sound ptr
	 CloneSoundObjects(m_soundBuffer,
				       m_3dBuffer,
			           &clonePtr->m_soundBuffer,
				       &clonePtr->m_3dBuffer,
					   directSoundPtr);
	 }//end valid direct sound ptr
	 else{
       clonePtr->m_soundBuffer = NULL;
       clonePtr->m_3dBuffer = NULL;
	 }

	 *clone = clonePtr;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------clone sound objects-------------------------------------------//
 BOOL CSoundSampleObj::CloneSoundObjects(IDirectSoundBuffer *baseSoundBuffer,
				                        IDirectSound3DBuffer *buffer3DControl,
								        IDirectSoundBuffer **newSndBuf,
				                        IDirectSound3DBuffer **New3DSndBuf,
									    IDirectSound8 * DirSnd)
{
if(!baseSoundBuffer)
return FALSE;
if(!buffer3DControl)
return FALSE;

IDirectSoundBuffer *newSoundBuffer = NULL;
IDirectSound3DBuffer *new3DSound = NULL;

DirSnd->DuplicateSoundBuffer(baseSoundBuffer,&newSoundBuffer);
if(!newSoundBuffer)
    return FALSE;
HRESULT er = newSoundBuffer->QueryInterface(IID_IDirectSound3DBuffer,(LPVOID *)&new3DSound); 
      if SUCCEEDED(er){     //Set 3-D parameters of this sound
         new3DSound->SetMode(DS3DMODE_NORMAL,DS3D_DEFERRED);
		 *newSndBuf = newSoundBuffer;
         *New3DSndBuf = new3DSound;
	     return TRUE;
	  }
return FALSE;///:)
//return TRUE;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
 void CSoundSampleObj::Play(BOOL looping)
{
	m_loopable = looping;
	if(!looping)
     m_soundBuffer->Play(0,0,0);
    else
	{
	 if(!m_playing)
	 if(m_soundBuffer->Play(0,0,DSBPLAY_LOOPING)==S_OK)
	    m_playing = TRUE; 
	}
	  
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
BOOL CSoundSampleObj::Initialize(IDirectSound8 * directSoundPtr)
{
   if(InitFromFile(directSoundPtr,m_fileName)==FALSE){
      //AfxMessageBox(m_fileName);
	   //->Stop();
      return FALSE;
   }
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
BOOL CSoundSampleObj::InitFromFile(IDirectSound8 * directSoundPtr,CString fileName)
{
if(directSoundPtr){//valid sound system
      //destroy existing pointers
      if(m_soundBuffer)
		 m_soundBuffer->Release();
	     m_soundBuffer=0;
	  if(m_3dBuffer)
		 m_3dBuffer->Release();
	     m_3dBuffer=0;
      //end destroy existing pointers
      m_soundBuffer = DSLoad3DSoundBuffer(directSoundPtr,fileName);
	  if(!m_soundBuffer){
		  return FALSE;
	  }
	  m_fileName = fileName;
      if(!m_soundBuffer){
		 m_soundBuffer = NULL;
         m_3dBuffer = NULL;
		 return FALSE;
	  }
	  
	  HRESULT er = m_soundBuffer->QueryInterface(IID_IDirectSound3DBuffer,(LPVOID *)&m_3dBuffer); 
      if SUCCEEDED(er){     //Set 3-D parameters of this sound
         m_3dBuffer->SetMode(DS3DMODE_NORMAL,DS3D_DEFERRED);
	     return TRUE;
	  }
  }//end valid sound system
  else
  return FALSE;
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
IDirectSoundBuffer *CSoundSampleObj::DSLoad3DSoundBuffer(IDirectSound8 *pDS, LPCTSTR lpName)
{
    IDirectSoundBuffer *pDSB = NULL;
    DSBUFFERDESC dsBD = {0};
    BYTE *pbWaveData;
    void *pvBase;

    dsBD.dwSize = sizeof(dsBD);
    dsBD.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME
			       | DSBCAPS_CTRLFREQUENCY | DSBCAPS_MUTE3DATMAXDISTANCE;
	
	//dsBD.guid3DAlgorithm = DS3DALG_HRTF_FULL;
	dsBD.guid3DAlgorithm = DS3DALG_HRTF_LIGHT;
	//dsBD.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;

    if (DSGetWaveResource(NULL, lpName, &dsBD.lpwfxFormat, &pbWaveData, &dsBD.dwBufferBytes))
    {
	if (SUCCEEDED(IDirectSound_CreateSoundBuffer(pDS, &dsBD, &pDSB, NULL)))
	{
	    if (!DSFillSoundBuffer(pDSB, pbWaveData, dsBD.dwBufferBytes))
	    {
		IDirectSoundBuffer_Release(pDSB);
		pDSB = NULL;
	    }
	}
	else
	{
	    pDSB = NULL;
	}
    } else if (DSGetWaveFile(NULL, lpName, &dsBD.lpwfxFormat, &pbWaveData,
			&dsBD.dwBufferBytes, &pvBase))
    {
	if (SUCCEEDED(IDirectSound_CreateSoundBuffer(pDS, &dsBD, &pDSB, NULL)))
	{
	    if (!DSFillSoundBuffer(pDSB, pbWaveData, dsBD.dwBufferBytes))
	    {
		IDirectSoundBuffer_Release(pDSB);
		pDSB = NULL;
	    }
	}
	else
	{
	    pDSB = NULL;
	}
	UnmapViewOfFile (pvBase);
    }

    return pDSB;
}
///////////////////////////////////////////////////////////////////////////////
//
//DSGetWaveResource
//
///////////////////////////////////////////////////////////////////////////////
BOOL CSoundSampleObj::DSGetWaveResource(HMODULE hModule, LPCTSTR lpName,
    WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize)
{
    HRSRC hResInfo;
    HGLOBAL hResData;
    void * pvRes;

    if (((hResInfo = FindResource(hModule, lpName, "WAV")) != NULL) &&
	((hResData = LoadResource(hModule, hResInfo)) != NULL) &&
	((pvRes = LockResource(hResData)) != NULL) &&
	DSParseWaveResource(pvRes, ppWaveHeader, ppbWaveData, pcbWaveSize))
    {
	return TRUE;
    }

    return FALSE;

}
///////////////////////////////////////////////////////////////////////////////
//
//DSParseWaveResource
//
///////////////////////////////////////////////////////////////////////////////
BOOL CSoundSampleObj::DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData,DWORD *pcbWaveSize)
{
    DWORD *pdw;
    DWORD *pdwEnd;
    DWORD dwRiff;
    DWORD dwType;
    DWORD dwLength;

    if (ppWaveHeader)
	*ppWaveHeader = NULL;

    if (ppbWaveData)
	*ppbWaveData = NULL;

    if (pcbWaveSize)
	*pcbWaveSize = 0;

    pdw = (DWORD *)pvRes;
    dwRiff = *pdw++;
    dwLength = *pdw++;
    dwType = *pdw++;

    if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
	goto exit;      //not even RIFF

    if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
	goto exit;      //not a WAV

    pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);

    while (pdw < pdwEnd)
    {
	dwType = *pdw++;
	dwLength = *pdw++;

	switch (dwType)
	{
	case mmioFOURCC('f', 'm', 't', ' '):
	    if (ppWaveHeader && !*ppWaveHeader)
	    {
		if (dwLength < sizeof(WAVEFORMAT))
		    goto exit;      //not a WAV

		*ppWaveHeader = (WAVEFORMATEX *)pdw;

		if ((!ppbWaveData || *ppbWaveData) &&
		    (!pcbWaveSize || *pcbWaveSize))
		{
		    return TRUE;
		}
	    }
	    break;

	case mmioFOURCC('d', 'a', 't', 'a'):
	    if ((ppbWaveData && !*ppbWaveData) ||
		(pcbWaveSize && !*pcbWaveSize))
	    {
		if (ppbWaveData)
		    *ppbWaveData = (LPBYTE)pdw;

		if (pcbWaveSize)
		    *pcbWaveSize = dwLength;

		if (!ppWaveHeader || *ppWaveHeader)
		    return TRUE;
	    }
	    break;
	}

	pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
    }

exit:
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
//
//DSFillSoundBuffer
//
///////////////////////////////////////////////////////////////////////////////
BOOL CSoundSampleObj::DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD cbWaveSize)
{
    if (pDSB && pbWaveData && cbWaveSize)
    {
	LPVOID pMem1, pMem2;
	DWORD dwSize1, dwSize2;

	if (SUCCEEDED(IDirectSoundBuffer_Lock(pDSB, 0, cbWaveSize,
	    &pMem1, &dwSize1, &pMem2, &dwSize2, 0)))
	{
	    CopyMemory(pMem1, pbWaveData, dwSize1);

	    if ( 0 != dwSize2 )
		CopyMemory(pMem2, pbWaveData+dwSize1, dwSize2);

	    IDirectSoundBuffer_Unlock(pDSB, pMem1, dwSize1, pMem2, dwSize2);
	    return TRUE;
	}
    }

    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
//
//DSGetWaveFile
//
///////////////////////////////////////////////////////////////////////////////
BOOL CSoundSampleObj::DSGetWaveFile(HMODULE hModule, LPCTSTR lpName,
    WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize,
	void **ppvBase)
{
    void *pvRes;
    HANDLE hFile, hMapping;

    *ppvBase = NULL;

    hFile = CreateFile (lpName, GENERIC_READ, FILE_SHARE_READ, 
			    NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return FALSE;

    hMapping = CreateFileMapping (hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hMapping == INVALID_HANDLE_VALUE)
    {
	CloseHandle (hFile); 
	return FALSE;
    }

    CloseHandle (hFile);

    pvRes = MapViewOfFile (hMapping, FILE_MAP_READ, 0, 0, 0);
    if (pvRes == NULL)
    {
	CloseHandle (hMapping);
	return FALSE;
    }

	CloseHandle (hMapping);

    if (DSParseWaveResource(pvRes, ppWaveHeader, ppbWaveData, pcbWaveSize) == FALSE)
    {
	UnmapViewOfFile (pvRes);
	return FALSE;
    }

    *ppvBase = pvRes;
    return TRUE;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CSoundSampleObj::SafeDelete()
{
	if(m_soundBuffer){
	   m_soundBuffer->Stop();
	   m_soundBuffer->Release();
       m_soundBuffer=0;
	}
    if(m_3dBuffer)
	   m_3dBuffer->Release();
       m_3dBuffer=0;

    
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CSoundSampleObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_fileName
		  << m_soundFactor;
	}
	else
	{
       ar >> m_fileName
		  >> m_soundFactor;
	
	   m_3dBuffer    = 0;
	   m_soundBuffer = 0;
	   m_loopable            = FALSE;
	   m_selfDeleteing       = TRUE;
	}
}     
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CSoundSampleObjList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CSoundSampleObj * sndPtr = (CSoundSampleObj *) GetNext(posLoc);
		sndPtr->SafeDelete();
		delete sndPtr;
		sndPtr = 0;
	}
	RemoveAll();
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
 void CSoundSampleObjList::CallBack()
{
	/*POSITION posLast;
	int trace = 0;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
		CSoundSampleObj * sndPtr = (CSoundSampleObj *) GetNext(posLoc);
		if(!sndPtr->m_selfDeleteing)
		{
	       continue;
		}
		DWORD result;
		sndPtr->m_soundBuffer->GetStatus(&result);

		if(result != DSBSTATUS_PLAYING && sndPtr->m_loopable == FALSE)
		{//done so remove from list
           sndPtr->SafeDelete();
		   delete sndPtr;
		   sndPtr = 0;
           RemoveAt(posLast);
		}//end done so remove from list
	}*/
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
CSoundBankObj::CSoundBankObj()
{
    m_primary       = NULL;
 	m_soundChain    = NULL;
	m_id            = 0;
}
    
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
 BOOL CSoundBankObj::InitPrimary(CString fileName,IDirectSound8 * directSoundPtr)
{
   if(m_primary)
   {
      m_primary->SafeDelete();
	  delete m_primary;
	  m_primary = 0;
   }
   if(!m_soundChain)
	   m_soundChain = new CSoundSampleObjList;
   m_primary = new CSoundSampleObj();
   m_primary->m_fileName = fileName;
	 
   return m_primary->Initialize(directSoundPtr);
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
CSoundSampleObj * CSoundBankObj::Play(BOOL looping,
								      IDirectSound8 * directSoundPtr,
								      D3DVECTOR & initialPosition,
									  float & soundFactor,
									  float clipRadius,
									  D3DVECTOR & earCenter)
{
   CSoundSampleObj * sample = NULL;
   if(!m_soundChain)
	   return NULL;

   if(!m_primary)
	   return NULL;

   if(!m_primary->m_3dBuffer)
	   return NULL;


   //sound clipping
   float dist = GL_matrixFunctions.MagnitudeSquared(earCenter.x,earCenter.y,earCenter.z,
							                        initialPosition.x,initialPosition.y,initialPosition.z);
   if(dist > clipRadius)
	  return NULL;




   DWORD result;
   m_primary->m_soundBuffer->GetStatus(&result);
   if(result != DSBSTATUS_PLAYING && looping == FALSE)
   {
      m_primary->m_soundFactor = soundFactor;
	  m_primary->m_soundFactor = .09f;
      m_primary->m_3dBuffer->SetPosition(initialPosition.x,
								         initialPosition.y,
								         initialPosition.z,
								         DS3D_DEFERRED);
      m_primary->Play(looping);
      return m_primary;
   }

   //try to resuse if possible
   for(POSITION rSndLoop = m_soundChain->GetHeadPosition();rSndLoop != NULL;)
   {
	  CSoundSampleObj * sndPtr = (CSoundSampleObj *)m_soundChain->GetNext(rSndLoop);
	  sndPtr->m_soundBuffer->GetStatus(&result);
	  if(result != DSBSTATUS_PLAYING)
	  {
         sndPtr->m_soundFactor = soundFactor;
		 sndPtr->m_soundFactor = .09f;
         sndPtr->m_3dBuffer->SetPosition(initialPosition.x,
								         initialPosition.y,
								         initialPosition.z,
								         DS3D_DEFERRED);
         sndPtr->Play(looping);
		 return sndPtr;
	  }
   }
   //if not see if this position is closer

   if(m_soundChain->GetCount() > 3)
	  return NULL;

   m_primary->Clone(&sample,directSoundPtr);
   if(sample)
   if(sample->m_3dBuffer)
   {
     sample->m_soundFactor = soundFactor;
	 sample->m_soundFactor = .09f;
     sample->m_3dBuffer->SetPosition(initialPosition.x,
								   initialPosition.y,
								   initialPosition.z,
								    DS3D_DEFERRED);
     sample->Play(looping);
	 m_soundChain->AddTail(sample);
	 return sample;
   }

   if(sample)
   {
      sample->SafeDelete();
      delete sample;
      sample = 0;
   }
   
   return NULL;
}

//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CSoundBankObj::SafeDelete()
{
    if(m_primary)
	{
       m_primary->SafeDelete();
	   delete m_primary;
	   m_primary = 0;
	}
    if(m_soundChain)
	{
	   m_soundChain->SafeDelete();
       delete m_soundChain;
	   m_soundChain = 0;
	}
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CSoundBankObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){

	   ar << m_soundChain
		  << m_primary
		  << m_id;
	}
	else
	{
       ar >> m_soundChain
		  >> m_primary
		  >> m_id;
	
	   m_soundChain->SafeDelete();
	}
}     
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CSoundBankList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CSoundBankObj * sndPtr = (CSoundBankObj *) GetNext(posLoc);
		sndPtr->SafeDelete();
		delete sndPtr;
		sndPtr = 0;
	}
	RemoveAll();

	if(m_soundListener)
	{
		m_soundListener->Release();
		m_soundListener = 0;
	}
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
 int CSoundBankList::GetUniqueID()
{
	int id = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CSoundBankObj * sndPtr = (CSoundBankObj *) GetNext(posLoc);
		if(sndPtr->m_id == id)
		   id = sndPtr->m_id + 1;
	}
	return id;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
 BOOL CSoundBankList::CreateSoundBank(CString fileName,
											int id
											)
{
    CSoundBankObj * newSBBank = new CSoundBankObj();
	newSBBank->m_id = id;
    BOOL result = newSBBank->InitPrimary(fileName,m_directSoundPtr);
	if(result == TRUE)
	{
       AddTail(newSBBank);
	   return TRUE;
	}
	else
	   return FALSE;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
CSoundSampleObj * CSoundBankList::PlayByID(int id,
									       BOOL looping,
									       D3DVECTOR & initialPosition,
									       float & soundFactor,
									       float clipRadius)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CSoundBankObj * sndPtr = (CSoundBankObj *) GetNext(posLoc);
		if(sndPtr->m_id == id)
		{
		   if(m_enableLogging)
		      AppendInfoToFile(sndPtr->m_primary->m_fileName);

           return sndPtr->Play(looping,
			                   m_directSoundPtr,
			                   initialPosition,
							   soundFactor,
							   clipRadius,
							   m_currentEarCenter);
		}
	}
	return NULL;
}
//-----------------------------------------------------------------------------
BOOL CSoundBankList::Get3DListenerInterface( )
{

    if(m_soundListener)
	   m_soundListener->Release();
	m_soundListener = 0;

    HRESULT             hr;
    DSBUFFERDESC        dsbdesc;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;


    // Obtain primary buffer, asking it for 3D control
    ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
    if( FAILED( hr = m_directSoundPtr->CreateSoundBuffer( &dsbdesc, &pDSBPrimary, NULL ) ) )
        return FALSE;

    if( FAILED( hr = pDSBPrimary->QueryInterface( IID_IDirectSound3DListener,
                                                  (VOID**)&m_soundListener ) ) )
    {
        pDSBPrimary->Release();
        return FALSE;
    }

    // Release the primary buffer, since it is not need anymore
    pDSBPrimary->Release();

    return TRUE;
}
//-----------------------------------------------------------------------------
BOOL CSoundBankList::UpdateListenerPosition(D3DMATRIX & ori)
{
	if(!m_soundListener)
	    return FALSE;

	m_pos.x = ori._41;
	m_pos.y = ori._42;
	m_pos.z = ori._43;

    m_matLib.GetOrientation(ori,&m_dir,&m_up);

	return UpdateListenerPosition(m_pos,m_dir,m_up);
}



//-----------------------------------------------------------------------------
BOOL CSoundBankList::UpdateListenerPosition(D3DVECTOR & soundCenter,D3DVECTOR & dirVect, D3DVECTOR & upVect)
{
	if(!m_soundListener)
	    return FALSE;

	m_currentEarCenter = soundCenter;

	CopyMemory( &m_listenerStruct.vPosition, &soundCenter, sizeof(D3DVECTOR) );
	CopyMemory( &m_listenerStruct.vOrientFront, &dirVect, sizeof(D3DVECTOR) );
	CopyMemory( &m_listenerStruct.vOrientTop, &upVect, sizeof(D3DVECTOR) );

	if(m_soundListener->SetAllParameters(&m_listenerStruct,DS3D_DEFERRED) != S_OK)
	   return FALSE;

	m_soundListener->CommitDeferredSettings();


    //HRESULT err = m_soundListener->SetPosition(soundCenter.x,soundCenter.y,soundCenter.z,DS3D_IMMEDIATE);
	//err = m_soundListener->SetOrientation(dirVect.x,dirVect.y,dirVect.z,upVect.x,upVect.y,upVect.z,DS3D_IMMEDIATE);

	//if( m_soundListener )
    //    m_soundListener->CommitDeferredSettings();

	return TRUE;
}
//------------------------------------------------------------------------------------//
void CSoundBankList::AppendInfoToFile(CString data) 
{
    CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open(m_logFileName,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite, &exc);
	fl.SeekToEnd( );
	the_out_Archive.WriteString( data );  
    the_out_Archive.WriteString( "\n");   
    the_out_Archive.Close();
    fl.Close(); 
}
//------------------------------------------------------------------------------------//
void CSoundBankList::EnableLogger(CString filename)
{
        m_enableLogging = TRUE;
		m_logFileName = filename;
		CFileException exc;
		CFile fl;
		CArchive the_out_Archive(&fl,CArchive::store);
		BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
		the_out_Archive.WriteString( "Cleared...");  
		the_out_Archive.WriteString( "\r\n");   
		the_out_Archive.Close();
		fl.Close(); 
}
//------------------------------------------------------------------------------------//
void CSoundBankList::DisableLogger()
{
    m_enableLogging = FALSE;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
 void CSoundBankList::Initialize(IDirectSound8 * directSoundPtr,LPDIRECTSOUND3DLISTENER listener)
{
	m_directSoundPtr	= directSoundPtr;
	m_soundListener		= listener;

	if(	m_soundListener)
	{
		m_soundListener->GetAllParameters(&m_listenerStruct);
		m_listenerStruct.flRolloffFactor = 0.25f;//attenuation 0-10.0
	}

	POSITION posLast;
	CSoundBankObj * intPlay = NULL;//this needs to be played to make vista happy
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
		CSoundBankObj * sndPtr = (CSoundBankObj *) GetNext(posLoc);
		if(sndPtr->m_primary->Initialize(directSoundPtr))
		{
			//sndPtr->m_primary->m_soundBuffer->Stop();
			intPlay = sndPtr;
		}
	}
	if(intPlay)
	{
		intPlay->m_primary->Play(FALSE);
		intPlay->m_primary->m_soundBuffer->Stop();
	}
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
int CSoundBankList::GetCurrentRuntimeSoundCount()
{
   int count = 0;
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
		CSoundBankObj * sndPtr = (CSoundBankObj *) GetNext(posLoc);
		count += (int)sndPtr->m_soundChain->GetCount();
   }
   return count;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
 void CSoundBankList::CallBack()
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
		CSoundBankObj * sndPtr = (CSoundBankObj *) GetNext(posLoc);
		sndPtr->m_soundChain->CallBack();	
	}
	//commit changes
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
 void CSoundBankList::Save(CString fileName)
{
       CFileException exc;
       CFile fl;
       fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << this;
       the_out_Archive.Close();
       fl.Close();
}


IMPLEMENT_SERIAL(CSoundBankObj, CObject, 0)
IMPLEMENT_SERIAL(CSoundBankList, CObList, 0)
IMPLEMENT_SERIAL(CSoundSampleObj, CObject, 0)
IMPLEMENT_SERIAL(CSoundSampleObjList, CObList, 0)


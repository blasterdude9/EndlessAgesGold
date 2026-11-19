#include "stdafx.h"

#include <d3d9.h>
#include <afxtempl.h>
#include "direct.h"

#include "CSoundTrackClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSoundTrackObj::CSoundTrackObj()
{
   m_url = false;
}

void CSoundTrackObj::SafeDelete()
{
 
}

BOOL CSoundTrackObj::LoadWaveFile(CString name,CString fileName)
{
	  m_trackName = name;
	  m_fileName  = fileName;
	 
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CSoundTrackObj::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){	

       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);

	   ar  << m_trackName
		   << m_fileName;
		   
	   ar  << m_url;
	}
	else
	{

	  int version = ar.GetObjectSchema();

      ar   >> m_trackName
		   >> m_fileName;

	  if(version == 2)
	     ar >> m_url;

	}
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Interpolation between animations BABY!! Imediate mode style-----------//
long CSoundTrackObjList::InterpolateLong(
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
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
CSoundTrackObjList::CSoundTrackObjList()
{
	InitializeCriticalSection(&m_mSection);
	m_initializedCorrectly = FALSE;
	m_trackInUse           = -1;//-1 = none
	m_fadeTime             = 0;
	m_pendingTrack         = -1;
	m_changeTrackStamp     = 0;
    m_fadingIn             = FALSE;
    m_fadingOut            = FALSE;
	m_pendingLoopingCount  = -1;//infinit 0=once count times
	m_musicPlayer          = NULL;
	m_acceptChangeFlag     = TRUE;
	this->m_pendingUrlShouldLoop = VARIANT_FALSE;
	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CSoundTrackObjList::Update()
{

    if(m_musicPlayer)
	   m_musicPlayer->Callback(GetTickCount());


	
}
BOOL CSoundTrackObjList::PlaySong(CString url, VARIANT_BOOL loop)
{
	EnterCriticalSection(&m_mSection);
	if(m_musicPlayer->m_disabled)
	{
		LeaveCriticalSection(&m_mSection);
        return FALSE;
	}

	if(m_lastSongPlayed.CompareNoCase(url)==0)
	{
		LeaveCriticalSection(&m_mSection);
		return FALSE;
	}
	
	m_pendingURL = url;
	this->m_pendingUrlShouldLoop = loop;
	m_lastSongPlayed = url;

	LeaveCriticalSection(&m_mSection);

    return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CSoundTrackObjList::RemoveTrack(int index)
{
   if(index < 0)
      return FALSE;

   POSITION posLoc = FindIndex(index);
     if(!posLoc)
		 return FALSE;
         CSoundTrackObj * trackObj = (CSoundTrackObj *)GetAt(posLoc);
         trackObj->SafeDelete();
		 delete trackObj;
		 trackObj=0;
		 RemoveAt(posLoc);
		 if(m_trackInUse == index)
		 m_trackInUse = -1;
		 return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CSoundTrackObjList::AddTrack(CString name,CString fileName,bool url)
{
   if(!m_initializedCorrectly)
	     return FALSE;
   CSoundTrackObj *trackObj = new CSoundTrackObj();
   trackObj->m_url = url;
   if(url == true)
      AddTail(trackObj);
   else if(trackObj->LoadWaveFile(name,fileName))
      AddTail(trackObj);
   else
   return FALSE;
 return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CSoundTrackObjList::SetVolume(long percent)
{
	long volume = m_musicPlayer->GetVolume();
	m_musicPlayer->SetVolume(percent);
	m_musicPlayer->UpdateLastSetVolume(volume);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CSoundTrackObjList::Stop()
{
     if(!m_initializedCorrectly)
	     return FALSE;

	 if(m_trackInUse == -1)
		 return FALSE;

	 EnterCriticalSection(&m_mSection);
	 if(this->m_pendingURL.GetLength() > 0 || this->m_currentURL.GetLength() > 0)
	 {
        this->m_pendingURL = "stop";//places command to stop streamer
		this->m_currentURL = "";
	 }
	 LeaveCriticalSection(&m_mSection);
     m_trackInUse = -1;

 return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CSoundTrackObjList::Play(int index,CString & path, VARIANT_BOOL loop)
{
     if(!m_initializedCorrectly)
	     return FALSE;

     POSITION posLoc = FindIndex(index);
     if(!posLoc)
		 return FALSE;
         CSoundTrackObj * trackObj = (CSoundTrackObj *)GetAt(posLoc);

		 if(trackObj->m_url)
			PlaySong(trackObj->m_fileName, loop);
		 else
			PlaySong("file://"+path+trackObj->m_fileName, loop);
 return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CSoundTrackObjList::Initialize(HWND hwnd,LPDIRECTSOUND pDS)
{

    if(m_initializedCorrectly)
	   return TRUE;

	if(m_musicPlayer)
	   delete m_musicPlayer;
	   m_musicPlayer = 0;

	m_musicPlayer = new CMusicAX();

	m_initializedCorrectly = TRUE;

	return TRUE;

	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CSoundTrackObjList::SafeDelete()
{
   if(m_musicPlayer)
   delete m_musicPlayer;
   m_musicPlayer = 0;

  

   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CSoundTrackObj * sndTPtr = (CSoundTrackObj *)GetNext(posLoc);
	   sndTPtr->SafeDelete();
	   delete sndTPtr;
	   sndTPtr=0;   
   }
   RemoveAll();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CSoundTrackObjList::Serialize( CArchive& ar )
{
	int integer = 0;
    CObList::Serialize( ar );
	if(ar.IsStoring() ){	
       
	   ar  << integer
		   << m_fadeTime;
	}
	else
	{
      ar   >> integer
		   >> m_fadeTime;

	  m_initializedCorrectly = FALSE;
	  m_trackInUse           = -1;
	  m_pendingTrack         = -1;
	  m_changeTrackStamp     = GetTickCount();
	  m_fadingIn             = FALSE;
      m_fadingOut            = FALSE;
	  m_acceptChangeFlag     = TRUE;
	}
}


IMPLEMENT_SERIAL(CSoundTrackObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CSoundTrackObjList, CObject, 0)
#include "CSSoundRange.h"
#include "matrixarb.h"
//#include <dmusicc.h>
//#include <dmusici.h>
//#include <dsound.h>
#include "dsound.h"
#include <mmsystem.h>
#include "CMusicClass.h"

#ifndef CSOUNDTRACKCLASS_H
#define CSOUNDTRACKCLASS_H

class   CSoundTrackObj : public CObject
{
	
     
	DECLARE_SERIAL( CSoundTrackObj );
	
    
public:
    CSoundTrackObj();
    void SafeDelete();
	
	CString                     m_trackName;
	CString                     m_fileName;
	bool                        m_url;
	//IDirectMusicSegment8      * m_directMusicSegment;
	//MUSIC_TIME                  m_duration;

    
    
	BOOL LoadWaveFile(CString name,CString fileName);

	void Serialize( CArchive& ar );	
};

class   CSoundTrackObjList : public CObList
{
public:
	CSoundTrackObjList();
	~CSoundTrackObjList(){DeleteCriticalSection(&m_mSection);};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CSoundTrackObjList );
	
	CMusicAX                     *m_musicPlayer;
	VARIANT_BOOL				  m_pendingUrlShouldLoop;
	CString                       m_pendingURL;//streaming music is valid length play then copy to current then truncate
	CString                       m_currentURL;//if streaming music this will be valid so we can switch to custome music also
    CString                       m_stationName;
	CString						  m_lastSongPlayed;


	//IDirectMusicPerformance8     *m_performaceModule;
	//IDirectMusicLoader8          *m_musicLoader;
	//IDirectMusicAudioPath8       *m_pAudioPath;

	BOOL                          m_initializedCorrectly;
    int                           m_trackInUse;
	int                           m_fadeTime;
	int                           m_pendingTrack;
	long                          m_changeTrackStamp;
	BOOL                          m_fadingIn;
	BOOL                          m_fadingOut;
	int                           m_pendingLoopingCount;
	BOOL                          m_acceptChangeFlag;
	CRITICAL_SECTION              m_mSection;

    long InterpolateLong(
						 long StartVec, 
						 long EndVec,
					     int NumOfStages, 
					     int ThisStage);
    void Update();

	void SetVolume(long percent);
	BOOL RemoveTrack(int index);
	BOOL Stop();
	BOOL Play(int index, CString & path, VARIANT_BOOL loop = VARIANT_FALSE);
    BOOL Initialize(HWND hwnd,LPDIRECTSOUND pDS);
	void Serialize( CArchive& ar );	
	BOOL AddTrack(CString name,CString fileName,bool url);
	BOOL PlaySong(CString url, VARIANT_BOOL loop = VARIANT_FALSE);
	
};

#endif
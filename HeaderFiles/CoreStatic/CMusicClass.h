#ifndef CMusicAXCLASS_H
#define CMusicAXCLASS_H
//#include "Chstring.h"
//#include <dshow.h>


class   CMusicAX
{

public:
    CMusicAX();
	~CMusicAX();
   
	/*IGraphBuilder *m_pGraph;
    IMediaControl *m_pControl;
    IMediaEvent   *m_pEvent;
	IBasicAudio   *m_audioControl;
	IMediaSeeking *m_mediaSeekingControl;*/
	byte           m_validForCallback;

	bool           m_looping;
	int            m_maxLoopCount;//0=forever >0 is= count
	int            m_currentLoopCount;
	LONG           m_evCode;
	LONG_PTR       m_evParam1, m_evParam2;
	int            m_lastUpdateTime;
	int            m_updateInterval;
	CString        m_targetStream;
    CString        m_currentStream;
	int            m_transitionMode;//0=none 1=fade out 2=fade in
	int            m_currentVolume;
	byte           m_quePlay;
	long           m_currentTargetVolume;
	long           m_volumeSetting;
	long           m_lastSetVolume;
	CString        m_overrideStation;//blank means no override
	BOOL           m_disabled;//this will disable music
	BOOL           m_disabledCurState;
	BOOL           m_musicStreamerEnabled;
	BOOL           m_urlMode;
	CRITICAL_SECTION m_cSection;
	

	//functions
	void SafeDelete();
	HRESULT PlayFile(CString filePath,int loopCount);
	void SetVolume(long volume);
	long InterpolateLong(
						 long StartVec, 
						 long EndVec,
					     int NumOfStages, 
					     int ThisStage);
	void SeekPosition(LONGLONG pos);
	void Callback(long currenttime);
	void InitializeInterfaces();
	void Stop();
	long GetVolume();
	void WriteCfgFile();
    void ReadCfgFile();
	void Disable();
    void Enable();
	void SetDisabledCurState(BOOL enabled);
	BOOL GetCurDisabledStateStatus();
	void UpdateLastSetVolume(long vol);
	BOOL VolumeUpdateNeeded();
	BOOL GetMusicStreamerStatus();
	void SetMusicStreamerEnable(BOOL enabled);
	BOOL GetDisabledStatus();
};

#endif
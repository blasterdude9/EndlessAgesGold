#include "dsound.h"
#include <mmsystem.h>
#include "matrixarb.h"
#include "CAxList.h"

#ifndef CSOUNDMANAGERCLASS_H
#define CSOUNDMANAGERCLASS_H
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
class CSoundSampleObj : public CObject
{
	
     
	DECLARE_SERIAL( CSoundSampleObj );
	
    
public:
    CSoundSampleObj();
	void Clone(CSoundSampleObj ** clone);

    IDirectSoundBuffer   *m_soundBuffer;
	IDirectSound3DBuffer *m_3dBuffer;
	CString               m_fileName;
	BOOL                  m_loopable;
	float                 m_soundFactor;
	BOOL                  m_selfDeleteing;
	BOOL                  m_playing;
	
	
    
    void Clone(CSoundSampleObj ** clone,IDirectSound8 * directSoundPtr);
	BOOL UpdateFreqBasedOnSpeed(D3DVECTOR velocityVector);
	BOOL UpdateFreqBasedOnSpeed(float maxSpeed, float curSpeed, float startFreq);
	float InterpolateFloatsHiRes(
						            float StartVec, 
						            float EndVec,
					                float NumOfStages, 
					                float ThisStage);
	BOOL Initialize(IDirectSound8 * directSoundPtr);
	BOOL InitFromFile(IDirectSound8 * directSoundPtr,CString fileName);
	IDirectSoundBuffer *DSLoad3DSoundBuffer(IDirectSound8 *pDS, LPCTSTR lpName);
	BOOL DSGetWaveFile(HMODULE hModule, LPCTSTR lpName,WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize,void **ppvBase);
	BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD cbWaveSize);
	BOOL DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData,DWORD *pcbWaveSize);
	BOOL DSGetWaveResource(HMODULE hModule, LPCTSTR lpName,WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize);
	 void Play(BOOL looping);
	 BOOL CloneSoundObjects(IDirectSoundBuffer *baseSoundBuffer,
				                        IDirectSound3DBuffer *buffer3DControl,
								        IDirectSoundBuffer **newSndBuf,
				                        IDirectSound3DBuffer **New3DSndBuf,
									    IDirectSound8 * DirSnd);
	 BOOL UpdatePosition(D3DVECTOR & positionalUpdate);
	 BOOL SetToDelete();
	void Serialize( CArchive& ar );
	void SafeDelete();
};
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
class CSoundSampleObjList : public CObList
{
public:
	CSoundSampleObjList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CSoundSampleObjList );
	
    

	 void CallBack();
};
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
class CSoundBankObj : public CObject
{
	
     
	DECLARE_SERIAL( CSoundBankObj );
	
    
public:
    CSoundBankObj();
    
	CSoundSampleObj     * m_primary;
 	CSoundSampleObjList * m_soundChain;
	int                   m_id;
	MatrixARB             GL_matrixFunctions;

	CSoundSampleObj * Play(BOOL looping,
		                   IDirectSound8 * directSoundPtr,
						   D3DVECTOR & initialPosition,
						   float & soundFactor,
						   float clipRadius,
						   D3DVECTOR & earCenter);
	float InterpolateFloatsHiRes(
						            float StartVec, 
						            float EndVec,
					                float NumOfStages, 
					                float ThisStage);
	 BOOL InitPrimary(CString fileName,IDirectSound8 * directSoundPtr);
	void Serialize( CArchive& ar );
	void SafeDelete();
	
};
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
class CSoundBankList : public CObList
{
public:
	CSoundBankList()
	{
		m_enableLogging=FALSE;
		m_soundListener=NULL;
		ZeroMemory(&m_listenerStruct,sizeof(DS3DLISTENER));
		m_listenerStruct.dwSize = sizeof(DS3DLISTENER);
	    ZeroMemory(&m_currentEarCenter,sizeof(D3DVECTOR));
	}

    
     
	DECLARE_SERIAL( CSoundBankList );
	BOOL			m_enableLogging;
	CString			m_logFileName;
	MatrixARB		m_matLib;
	D3DVECTOR       m_pos;
	D3DVECTOR		m_dir;
	D3DVECTOR		m_up;
	DS3DLISTENER	m_listenerStruct;
	D3DVECTOR		m_currentEarCenter;
    

	void SafeDelete();

	IDirectSound8 * m_directSoundPtr;
	LPDIRECTSOUND3DLISTENER m_soundListener;

	void Save(CString fileName);
	void Initialize(IDirectSound8 * directSoundPtr,LPDIRECTSOUND3DLISTENER listener);
	CSoundSampleObj * PlayByID(int id,
		                       BOOL looping,
							   D3DVECTOR & initialPosition,
							   float & soundFactor,
							   float clipRadius);
	BOOL CreateSoundBank(CString fileName,int id);
	int GetCurrentRuntimeSoundCount();
	void AppendInfoToFile(CString data);
	void CallBack();
	int GetUniqueID();
	void EnableLogger(CString filename);
	void DisableLogger();
	BOOL Get3DListenerInterface(  );
	BOOL UpdateListenerPosition(D3DVECTOR & soundCenter,D3DVECTOR & dirVect, D3DVECTOR & upVect);
	BOOL UpdateListenerPosition(D3DMATRIX & ori);
};

#endif
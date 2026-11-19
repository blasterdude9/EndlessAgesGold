#include <wincrypt.h>
#include "aes.h"

#pragma once
class CAXEncryptionLib
{
public:
	CAXEncryptionLib(void);
	CAXEncryptionLib(char * password,ALG_ID encryptionType,ALG_ID hashEncryption);
	CAXEncryptionLib(char * key);
	CAXEncryptionLib(char * key,int type);
	~CAXEncryptionLib(void);

    //params
	HCRYPTPROV				m_hCryptProv; 
	HCRYPTKEY				m_hKey; 
	HCRYPTKEY				m_hXchgKey; 
	HCRYPTHASH				m_hHash; 
	PBYTE					m_pbKeyBlob; 
	DWORD					m_dwKeyBlobLen;
	int						m_errorTrack;
	char					m_preBuffer[1024];
	char					m_preBufferTemp[1024];
	char					m_resultBuffer[1024];
	char				*   m_errorDecortaed;
	//CRijndael           *	m_oRijndael;
	char                    m_key[512];
	int						m_type;//0=ENCstatndard 1=2 m_oRijndaelw
	CRITICAL_SECTION m_criticalSection;

    //Functions
	bool EncryptString(char * data, char * retBuffer);
	char * EncryptString64(char * data);
	bool DecryptString(char * data, char * retData);
	void Init(char * password,ALG_ID encryptionType,ALG_ID hashEncryption);
	char * DecryptString(char * data);
	char * EncryptString(char * data);
	void ExternalInit(char * key);
	char * EncodeToNumeric(BYTE * data);
	char * DecodeToNumeric(char * data);
	char * DecryptString64(char * data);
	bool EncryptStringWkey(char * data, char * retBuffer, char * key);
	bool DecryptStringWKey(char * data, char * retData, char * key);
	//CString ConvToString(int number);
	int GetCharArrayLength(char * array);
	void UnLock();
	void Lock();
	
};

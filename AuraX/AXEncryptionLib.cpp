#include "StdAfx.h"
#include "axencryptionlib.h"
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void WINAPI ExternalInitX(char * key);
void WINAPI LockX();
void WINAPI UnLockX();
char * WINAPI DecryptStringX(char * data);
char * WINAPI EncryptStringX(char * data);
char * WINAPI EncryptString64X(char * data, char * buffer);
char * WINAPI DecryptString64X(char * data, char * buffer);
void WINAPI SetKey64X(char * data);
char * WINAPI DecryptString64wKey(char * data, char * buffer, char * key);
char * WINAPI EncryptString64wKey(char * data, char * buffer, char * key);
char * WINAPI DecodeToNumericX(char * data);
void WINAPI ExternalInitSHA1RC4(char * key);
static CAXEncryptionLib * g_global = NULL;


void WINAPI LockX()
{
         g_global->Lock();
}

void WINAPI UnLockX()
{
         g_global->UnLock();
}

void WINAPI ExternalInitX(char * key)
{
     if(!g_global)
         g_global = new CAXEncryptionLib(key); 
}

void WINAPI ExternalInitSHA1RC4(char * key)
{
     if(!g_global)
         g_global = new CAXEncryptionLib(key,1); 
}

char * WINAPI DecryptStringX(char * key)
{
       return g_global->DecryptString(key);
}

char * WINAPI DecodeToNumericX(char * key)
{
       return g_global->DecodeToNumeric(key);
}

char * WINAPI EncryptStringX(char * key)
{
       return g_global->EncryptString(key);
}

void WINAPI SetKey64X(char * data)
{
	   ZeroMemory(g_global->m_key,512);
	   CopyMemory(g_global->m_key,data,strlen(data));
}

char * WINAPI DecryptString64wKey(char * data, char * buffer, char * key)
{
       g_global->DecryptStringWKey(data, buffer, key);
	   return buffer;
}

char * WINAPI EncryptString64wKey(char * data, char * buffer, char * key)
{
	   g_global->EncryptStringWkey(data,buffer,key);
	   return buffer;
}


char * WINAPI DecryptString64X(char * data, char * buffer)
{
       
       g_global->DecryptString(data,buffer);
	   return buffer;
}

char * WINAPI EncryptString64X(char * data, char * buffer)
{
	   g_global->EncryptString(data,buffer);
	   return buffer;
}



// Every constructor starts from a clean state. The handles used to be left
// uninitialised by the default and key-only constructors, so the destructor
// called CryptDestroyHash()/CryptReleaseContext() on garbage and the client
// crashed on exit (CryptDestroyHash access violation).
static void AXEncZero(CAXEncryptionLib * e)
{
	e->m_hCryptProv    = 0;
	e->m_hKey          = 0;
	e->m_hXchgKey      = 0;
	e->m_hHash         = 0;
	e->m_pbKeyBlob     = 0;
	e->m_dwKeyBlobLen  = 0;
	e->m_errorTrack    = 0;
	e->m_errorDecortaed = 0;
	e->m_type          = -1;
	ZeroMemory(e->m_key, sizeof(e->m_key));
	InitializeCriticalSection(&e->m_criticalSection);
}

CAXEncryptionLib::CAXEncryptionLib()
{
	AXEncZero(this);
}
CAXEncryptionLib::CAXEncryptionLib(char * key)
{
	AXEncZero(this);
	m_type = 1;
    ZeroMemory(m_key,512);
    CopyMemory(m_key,key,strlen(key));
}


CAXEncryptionLib::CAXEncryptionLib(char * key,int type)
{
	AXEncZero(this);
    if(type == 1)
	   Init(key,CALG_RC4,CALG_SHA1);
    ZeroMemory(m_key,512);
	CopyMemory(m_key,key,strlen(key));
}

CAXEncryptionLib::CAXEncryptionLib(char * password,ALG_ID encryptionType,ALG_ID hashEncryption)
{
	AXEncZero(this);
	Init(password,encryptionType,hashEncryption);
	
}

void CAXEncryptionLib::ExternalInit(char * key)
{
    if(m_type == 1)
	   return;

	m_type = 1;   // (critical section is set up by the constructor)
    ZeroMemory(m_key,512);
	CopyMemory(m_key,key,strlen(key));
}

void CAXEncryptionLib::Init(char * password,ALG_ID encryptionType,ALG_ID hashEncryption)
{
	//CAXEncryptionLib * encLib = new CAXEncryptionLib("catcow",CALG_RC4,CALG_SHA1);
	// (critical section is set up by the constructor)
    int passLen = GetCharArrayLength(password);
	//m_oRijndael = NULL;
	m_type = 0;
	m_errorTrack = 0;
    //Get the handle to the default provider. 
	if(!CryptAcquireContext(
							&m_hCryptProv, 
							NULL, 
							MS_ENHANCED_PROV, 
							PROV_RSA_FULL, 
							0))
	{
	   m_errorTrack = 1;
	   return;
	}

	//Create a hash object. 
    if(!CryptCreateHash(
						m_hCryptProv, 
						hashEncryption, 
						0, 
						0, 
						&m_hHash))
    {
       m_errorTrack = 2;
	   return;
    }
	//Hash the password. 
	if(!CryptHashData(
		m_hHash, 
		(BYTE *)password, 
		passLen, 
		0))
	{
       m_errorTrack = 3;
	   return;
    }

	//Derive a session key from the hash object. 
    if(!CryptDeriveKey(
		m_hCryptProv, 
		encryptionType, 
		m_hHash, 
		0, 
		&m_hKey)) 
    {

    /*if(CryptGenKey(
		m_hCryptProv,
		encryptionType,
		0,
		&m_hKey))
	{*/


        DWORD res = GetLastError();
		
		switch(res)
		{
		    case ERROR_INVALID_HANDLE:
                 m_errorDecortaed = "ERROR_INVALID_HANDLE";
			break;
			case ERROR_INVALID_PARAMETER:
				 m_errorDecortaed = "ERROR_INVALID_PARAMETER";
			break;
			case NTE_BAD_ALGID:
                 m_errorDecortaed = "NTE_BAD_ALGID";
			break;
			case NTE_BAD_FLAGS:
                 m_errorDecortaed = "NTE_BAD_FLAGS";
			break;
			case NTE_BAD_HASH:
                 m_errorDecortaed = "NTE_BAD_HASH";
			break;
			case NTE_BAD_UID:
                 m_errorDecortaed = "NTE_BAD_UID";
			break;
            case NTE_BAD_HASH_STATE:
                 m_errorDecortaed = "NTE_BAD_HASH_STATE";
			break;
			case NTE_FAIL:
                 m_errorDecortaed = "NTE_FAIL";
			break;
			case NTE_SILENT_CONTEXT:
                 m_errorDecortaed = "NTE_SILENT_CONTEXT";
			break;
		}

        m_errorTrack = 4;
	    return;
    }

	
}





CAXEncryptionLib::~CAXEncryptionLib(void)
{
	BOOL result = FALSE;
	// Destroy key and hash before releasing the provider they belong to.
    if(m_hKey)
	   result = CryptDestroyKey(m_hKey);
	if(m_hHash) 
		result = CryptDestroyHash(m_hHash);	
	if(m_hCryptProv)
       result = CryptReleaseContext(m_hCryptProv, 0);
	m_hKey = 0; m_hHash = 0; m_hCryptProv = 0;

	DeleteCriticalSection(&m_criticalSection);
}

void CAXEncryptionLib::Lock()
{
     EnterCriticalSection(&m_criticalSection);
}

void CAXEncryptionLib::UnLock()
{
     LeaveCriticalSection(&m_criticalSection);
}

char * CAXEncryptionLib::EncryptString64(char * data)
{
    

	ZeroMemory(m_resultBuffer,1024);
	ZeroMemory(m_preBuffer,1024);
	int retLength = 0;
	
	EncryptString(data,m_resultBuffer);
    return m_resultBuffer;
}

char * CAXEncryptionLib::DecryptString64(char * data)
{
	ZeroMemory(m_resultBuffer,1024);
	ZeroMemory(m_preBuffer,1024);
	int retLength = 0;
	DecryptString(data,m_resultBuffer);
	

    return m_resultBuffer;
}

char * CAXEncryptionLib::EncryptString(char * data)
{
	ZeroMemory(m_resultBuffer,1024);
	int retLength = 0;
	EncryptString(data,m_resultBuffer);
    return m_resultBuffer;
}

int CAXEncryptionLib::GetCharArrayLength(char * array)
{
    int trace = 0;
    while(1)
	{
		if(array[trace] == '0')
		   if(array[trace+1] == '0')
		      break;

         trace++;
	}

	return trace;

}

bool CAXEncryptionLib::EncryptString(char * data, char * retBuffer)
{
	ZeroMemory(m_preBuffer,1024);
	DWORD			dwCount = 0;
	if(m_type == 0)
	{
    PBYTE			pbBuffer = (BYTE*)data; 
	DWORD			bufLen = 1024; 
	DWORD			dwBufferLen = GetCharArrayLength(data)+128; 
    dwCount			= (DWORD)strlen(data); 
 
	CopyMemory(m_preBuffer,pbBuffer,dwCount);

    //Encrypt data. 
    if(!CryptEncrypt(
		m_hKey, 
		0, 
		TRUE, 
		0, 
		(BYTE*)m_preBuffer, //input
		&dwCount, 
		1024))
    {
        

		DWORD res = GetLastError();
		
		switch(res)
		{
		    case ERROR_INVALID_HANDLE:
                 m_errorDecortaed = "ERROR_INVALID_HANDLE";
			break;
			case ERROR_INVALID_PARAMETER:
				 m_errorDecortaed = "ERROR_INVALID_PARAMETER";
			break;
			case NTE_BAD_ALGID:
                 m_errorDecortaed = "NTE_BAD_ALGID";
			break;
			case NTE_BAD_DATA:
                 m_errorDecortaed = "NTE_BAD_DATA";
			break;
			case NTE_BAD_FLAGS:
                 m_errorDecortaed = "NTE_BAD_FLAGS";
			break;
			case NTE_BAD_HASH:
                 m_errorDecortaed = "NTE_BAD_HASH";
			break;
            case NTE_BAD_HASH_STATE:
                 m_errorDecortaed = "NTE_BAD_HASH_STATE";
			break;
			case NTE_BAD_KEY:
                 m_errorDecortaed = "NTE_BAD_KEY";
			break;
			case NTE_BAD_LEN:
                 m_errorDecortaed = "NTE_BAD_LEN";
			break;
			case NTE_BAD_UID:
                 m_errorDecortaed = "NTE_BAD_UID";
			break;
			case NTE_DOUBLE_ENCRYPT:
                 m_errorDecortaed = "NTE_DOUBLE_ENCRYPT";
			break;
			case NTE_FAIL:
                 m_errorDecortaed = "NTE_FAIL";
			break;
			case NTE_NO_MEMORY:
                 m_errorDecortaed = "NTE_NO_MEMORY";
			break;
		}

	   return false;
    }
	}
	else if(m_type == 1)
	{
	   //m_oRijndael->EncryptBlock(data, m_preBuffer);
	   Encrypt64(data, m_preBuffer, m_key);
	   dwCount = (DWORD)strlen(m_preBuffer);
	}
	//*returnLength = dwCount;
	CopyMemory(retBuffer,m_preBuffer,dwCount);
	return true;
}

bool CAXEncryptionLib::EncryptStringWkey(char * data, char * retBuffer, char * key)
{
	ZeroMemory(m_preBuffer,1024);
	DWORD			dwCount = 0;
	if(m_type == 0)
	{
    PBYTE			pbBuffer = (BYTE*)data; 
	DWORD			bufLen = 1024; 
	DWORD			dwBufferLen = GetCharArrayLength(data)+128; 
    dwCount			= (DWORD)strlen(data); 
 
	CopyMemory(m_preBuffer,pbBuffer,dwCount);

    //Encrypt data. 
    if(!CryptEncrypt(
		m_hKey, 
		0, 
		TRUE, 
		0, 
		(BYTE*)m_preBuffer, //input
		&dwCount, 
		1024))
    {
        

		DWORD res = GetLastError();
		
		switch(res)
		{
		    case ERROR_INVALID_HANDLE:
                 m_errorDecortaed = "ERROR_INVALID_HANDLE";
			break;
			case ERROR_INVALID_PARAMETER:
				 m_errorDecortaed = "ERROR_INVALID_PARAMETER";
			break;
			case NTE_BAD_ALGID:
                 m_errorDecortaed = "NTE_BAD_ALGID";
			break;
			case NTE_BAD_DATA:
                 m_errorDecortaed = "NTE_BAD_DATA";
			break;
			case NTE_BAD_FLAGS:
                 m_errorDecortaed = "NTE_BAD_FLAGS";
			break;
			case NTE_BAD_HASH:
                 m_errorDecortaed = "NTE_BAD_HASH";
			break;
            case NTE_BAD_HASH_STATE:
                 m_errorDecortaed = "NTE_BAD_HASH_STATE";
			break;
			case NTE_BAD_KEY:
                 m_errorDecortaed = "NTE_BAD_KEY";
			break;
			case NTE_BAD_LEN:
                 m_errorDecortaed = "NTE_BAD_LEN";
			break;
			case NTE_BAD_UID:
                 m_errorDecortaed = "NTE_BAD_UID";
			break;
			case NTE_DOUBLE_ENCRYPT:
                 m_errorDecortaed = "NTE_DOUBLE_ENCRYPT";
			break;
			case NTE_FAIL:
                 m_errorDecortaed = "NTE_FAIL";
			break;
			case NTE_NO_MEMORY:
                 m_errorDecortaed = "NTE_NO_MEMORY";
			break;
		}

	   return false;
    }
	}
	else if(m_type == 1)
	{
	   //m_oRijndael->EncryptBlock(data, m_preBuffer);
	   Encrypt64(data, m_preBuffer, key);
	   dwCount = (DWORD)strlen(m_preBuffer);
	}
	//*returnLength = dwCount;
	CopyMemory(retBuffer,m_preBuffer,dwCount);
	return true;
}


char * CAXEncryptionLib::DecryptString(char * data)
{
    int retLength = 0;
	DecryptString(data,m_resultBuffer);

    return m_resultBuffer;
}

char * CAXEncryptionLib::EncodeToNumeric(BYTE * data)
{
	  /*m_tempHolder = "";
      int trace = 0;
	  int indexer = 0;
      while(data[trace] != 0)
	  {
		     indexer = 1;
          if(data[trace] > 9)
		     indexer = 2;
		  if(data[trace] > 99)
		     indexer = 3;

          m_tempHolder += ConvToString(indexer);
          m_tempHolder += ConvToString(data[trace]);
		  trace++;
	  }

	  ZeroMemory(m_preBuffer,1024);
	  CopyMemory(m_preBuffer,m_tempHolder.GetBuffer(),m_tempHolder.GetLength());*/
      
	  return (char*)m_preBuffer;
}

char * CAXEncryptionLib::DecodeToNumeric(char * data)
{
	  /*ZeroMemory(m_preBufferTemp,1024);
	  CString buildBuffer;
      int trace = 0;
	  int indexer = 0;
	  int dataTrace = 0;
	  int curStep = 0;
	  char tempXYZ[2];
	  tempXYZ[1] = 0;
      while(data[trace] != 0)
	  {
		  tempXYZ[0] = data[trace++];
          curStep = atoi(tempXYZ);
		  if(curStep == 3)
		  {
             buildBuffer = "";
             buildBuffer += data[trace++];
			 buildBuffer += data[trace++];
			 buildBuffer += data[trace++];
             
			 m_preBufferTemp[dataTrace++] = atoi(buildBuffer);
		     indexer = 3;
		  }
		  else if(curStep == 2)
		  {
			 buildBuffer = "";
             buildBuffer += data[trace++];
			 buildBuffer += data[trace++];
             
			 m_preBufferTemp[dataTrace++] = atoi(buildBuffer);
			 continue;
		  }
          else
		  {
             buildBuffer = "";
             buildBuffer += data[trace++];
             
			 m_preBufferTemp[dataTrace++] = atoi(buildBuffer);
		     indexer = 2;
		  }
	
	  }

	  CopyMemory(m_preBuffer,m_preBufferTemp,1024);*/
      
	  return (char*)m_preBuffer;
}


//-----converts a long into string-----------------------------//
/*CString CAXEncryptionLib::ConvToString(int number)
{    
              if(number==0)
				 return "0";
	 
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);
		
			  return temp;
}*/

bool CAXEncryptionLib::DecryptString(char * data, char * retData)
{
	if(m_type == 0)
	{
		PBYTE			pbBuffer = (BYTE*)data; 
		DWORD			dwBufferLen = (DWORD)strlen(data); 
		DWORD			dwCount = dwBufferLen; 
	 
		ZeroMemory(m_preBuffer,1024);
		
		if(dwBufferLen > 1024)
			return false;//bad error
		
		CopyMemory(m_preBuffer,pbBuffer,dwBufferLen);
		//Encrypt data. 
	   
		if(!CryptDecrypt(
			m_hKey, 
			0, 
			TRUE, 
			0, 
			(BYTE*)m_preBuffer, //input
			&dwCount))
		{
	       
		return false;
		}
		
	}
	else if(m_type == 1)
	{
	   ZeroMemory(m_preBuffer,1024);
		if(sizeof(data) < 1)
			return false;

	   Decrypt64(data,m_preBuffer,m_key);
	}
	//*returnLength = strlen(m_preBuffer);
	size_t sizeX = strlen(m_preBuffer);
	if(sizeX <= 1024)
	   CopyMemory(retData,m_preBuffer,sizeX);
	
	return true;
}

bool CAXEncryptionLib::DecryptStringWKey(char * data, char * retData, char * key)
{
	if(m_type == 0)
	{
		PBYTE			pbBuffer = (BYTE*)data; 
		DWORD			dwBufferLen = (DWORD)strlen(data); 
		DWORD			dwCount = dwBufferLen; 
	 
		ZeroMemory(m_preBuffer,1024);
		CopyMemory(m_preBuffer,pbBuffer,dwBufferLen);
		//Encrypt data. 
	   
		if(!CryptDecrypt(
			m_hKey, 
			0, 
			TRUE, 
			0, 
			(BYTE*)m_preBuffer, //input
			&dwCount))
		{
	       
		return false;
		}
		
	}
	else if(m_type == 1)
	{
	   ZeroMemory(m_preBuffer,1024);
	   //m_oRijndael->DecryptBlock(data, m_preBuffer);
	   Decrypt64(data,m_preBuffer,key);
	}
	//*returnLength = strlen(m_preBuffer);
	CopyMemory(retData,m_preBuffer,strlen(m_preBuffer));
	
	return true;
}





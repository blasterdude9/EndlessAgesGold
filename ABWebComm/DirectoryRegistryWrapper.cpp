#include "StdAfx.h"
#include "direct.h"
#include ".\directoryregistrywrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDirectoryRegistryWrapper::CDirectoryRegistryWrapper(void)
{
}

CDirectoryRegistryWrapper::~CDirectoryRegistryWrapper(void)
{
}

void CDirectoryRegistryWrapper::MakeDirectory(LPSTR path)
{

//parse directory
       CString directory = path;
	   CString dirArray[20];
	   int dirCount = 0;
       int fPathLength = directory.GetLength();
       
	   
	   for(int dLoop=(fPathLength-1);dLoop>0;dLoop--)
       {
           char xTest = directory.GetAt(dLoop);
	       if(xTest == '\\')
		   {
	         dirArray[dirCount++] = directory;
			   
			   //break;
			 directory.Delete(dLoop,1);
		   }
	       else
             directory.Delete(dLoop,1);
       }
       for(int i=(dirCount-1);i>-1;i--)
	   {
           CreateDirectory(dirArray[i],NULL);
	   }
}

DWORD64 CDirectoryRegistryWrapper::GetFreeSpaceMB(LPSTR path)
{
       
	DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;
	
	
	GetDiskFreeSpace(path,
         (DWORD*)&sectorsPerCluster,
         (DWORD*)&bytesPerSector,
         (DWORD*)&numberOfFreeClusters,
         (DWORD*)&totalNumberOfClusters
       );
  
       DWORD64 div = 1000000;
	   DWORD64 unit = (bytesPerSector*sectorsPerCluster);
	   DWORD64 count = numberOfFreeClusters;
	   DWORD64 total = totalNumberOfClusters;
	   
	   
	   
	   DWORD64 freeBytes = (unit*count)/div;
	   DWORD64 totalBytes = (unit*total)/div;
	   //long returnVal = freeBytes;
       return freeBytes;
}

void CDirectoryRegistryWrapper::SetSoftwareReg(LPSTR headerX,LPSTR valueName,LPSTR valueDataX)
{
   HKEY hk; 
   CString valueData = valueDataX;
   CString header = headerX;

   if(RegCreateKey(HKEY_LOCAL_MACHINE, 
                   "SOFTWARE\\"+header,
				   &hk))
   {
        
   }

   if(valueData.GetLength() < 1)
      return;

   char * szBuf = new char[valueData.GetLength()+1];
   ZeroMemory(szBuf,valueData.GetLength()+1);

   for(int i=0;i<valueData.GetLength();i++)
   {
       szBuf[i] = valueData.GetAt(i);
   }

   if (RegSetValueEx(hk,             //subkey handle 
                     valueName,       //value name 
                     0,                        //must be zero 
                     REG_EXPAND_SZ,            //value type 
                     (LPBYTE) szBuf,           //pointer to value data 
                     strlen(szBuf) + 1))       //length of value data 
   {

   }


   delete [] szBuf;
   szBuf = 0;

}

CString CDirectoryRegistryWrapper::GetSoftwareRegkeyVal(LPSTR headerX,LPSTR valueRequest)
{
	        CString header = headerX;
            HKEY hKey;
			DWORD BUFSIZE = 500;
            char szProductType[500];
			ZeroMemory(szProductType,BUFSIZE);
            DWORD dwBufLen=BUFSIZE;
            LONG lRet;

            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                 "SOFTWARE\\"+header,
                                 0, 
								 KEY_QUERY_VALUE, 
								 &hKey );
            
			if( lRet != ERROR_SUCCESS )
               return "";

            lRet = RegQueryValueEx( hKey, valueRequest, NULL, NULL,
                                 (LPBYTE) szProductType, &dwBufLen);
            if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
               return "";

            RegCloseKey( hKey );

            return szProductType;
}

BOOL CDirectoryRegistryWrapper::ValidateRegistryEntry(LPSTR headerX)
{
	        CString header = headerX;
            HKEY hKey;
			int BUFSIZE = 500;
            char szProductType[500];
			ZeroMemory(szProductType,BUFSIZE);
            DWORD dwBufLen=BUFSIZE;
            LONG lRet;

            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                 header,
                                 0, 
								 KEY_QUERY_VALUE, 
								 &hKey );
            
			if( lRet == ERROR_SUCCESS )
               return TRUE;

			RegCloseKey( hKey );

            return FALSE;
}

CString CDirectoryRegistryWrapper::GetSoftwareRegkeyValDef(LPSTR headerX,LPSTR valueRequest,LPSTR root)
{
	        CString header = headerX;
            HKEY hKey;
			DWORD BUFSIZE = 500;
            char szProductType[500];
			ZeroMemory(szProductType,BUFSIZE);
            DWORD dwBufLen=BUFSIZE;
            LONG lRet;

            CString combo = root;
			combo = combo + header;

            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                 combo.GetBuffer(),
                                 0, 
								 KEY_QUERY_VALUE, 
								 &hKey );
            
			if( lRet != ERROR_SUCCESS )
               return "";

            lRet = RegQueryValueEx( hKey, valueRequest, NULL, NULL,
                                 (LPBYTE) szProductType, &dwBufLen);
            if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
               return "";

            RegCloseKey( hKey );

            return szProductType;
}
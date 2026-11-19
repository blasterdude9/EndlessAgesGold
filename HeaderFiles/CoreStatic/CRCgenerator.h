#pragma once

class  CCRCgenerator
{
public:
	CCRCgenerator(void){crcgen( );};
	~CCRCgenerator(void){};
    __int64 crcTable[256];

	__int64 getcrc( CString path )
	{
		CFileException exc;
		CFile fl;
		BOOL res = fl.Open(path,CFile::modeRead, &exc);
		if(res == FALSE){
			return 0;
		}
		__int64 temp = getcrc( &fl );
		fl.Close();
		return temp;
	}

	__int64 getcrc( CFile *fp )
	{
		register __int64 crc;
		__int64 c;

		__int64 bufSz = 2048;
		BYTE buffer[2048];
		__int64 bytesRead = 0;
		crc = 0xFFFFFFFF;
		while(1) 
		{
			bytesRead = fp->Read(buffer,bufSz);
			if(bytesRead < 1)
			break;

			for(__int64 i=0;i<bytesRead;i++)
			{
				c = buffer[i];
				crc = ((crc>>8) & 0x00FFFFFF) ^ crcTable[ (crc^c) & 0xFF ];

			}
		}
		return( crc^0xFFFFFFFF );
	}

	void crcgen( )
	{
		

		__int64 crc, poly;
		__int64	i, j;

		poly = 0xEDB88320L;
		for (i=0; i<256; i++) {
			crc = i;
			for (j=8; j>0; j--) {
				if (crc&1) {
					crc = (crc >> 1) ^ poly;
				} else {
					crc >>= 1;
				}
			}
			crcTable[i] = crc;
		}
	}
};

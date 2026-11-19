#ifndef CCOMPRESSIONCLASS_H
#define CCOMPRESSIONCLASS_H

class CCompressedObj : public CObject
{
	
     
	DECLARE_SERIAL( CCompressedObj );
	
    
public:
    CCompressedObj();
    
    BYTE *   m_dataBlock;
	long     m_blockLength;
	long     m_originalFileLength;



	void SafeDelete();
	void CompressData(BYTE * rawDataBlock,long rawDataLength);	
	void AddData(BYTE * byteBlock,
				 long appendDataLength);
	void Serialize( CArchive& ar );
    void compressUseingElzd(BYTE * rawDataBlock,long rawDataLength);
	int  GetCharIncrement(BYTE * rawDataBlock,
						  long & rawDataLength,
						  long & curPosition);
    void exhaustlkbuff(int lkbc,
					   int * buf,
					   int & bc,
					   unsigned int * lkbuf,
					   int * buft,
					   int * buftv);
	void refreshbuffer(int & bc,
					   int * buf);
    void add2buff(unsigned int c,
				  int * buf,
				  int * buft,
				  int * buftv,
				  int & bc);
	void check4phrase(int & check,
								  int & countold,
								  int & posold,
								  int & count,
								  unsigned int * lkbuf,
								  int & txtbc,
								  unsigned int * txtbuf,
								  int & pos,
								  int & lkbc);
	void AddCharElzd(int c);
	int lkfill( int a,
			    int * c,
			    unsigned int * txtbuf,
			    unsigned int * lkbuf,
			    int & txtbc,
			    int & lkbc,
			    int & cc,
			    int & end,
			    int & ch,
			    BYTE * rawDataBlock,
			    long & dataReadPosition,
				long & infilelen,
				long & rawDataLength);
	int fillphrase2buff(int * buf,
					    int * buft,
					    int * buftv,
				        int & bc,
					    int & pos,
					    int & count);
	unsigned char oddchar(int * buf,int & bc);

    //decompression functions
	void decompressToData(BYTE ** outPutData,long * newLength);
	void WriteByteToDataBlock(int c,BYTE * newData,long & currentPos,long & maxSz);
	void fillbuffer(int & bc,
				    int * buf,
				    int * buft,
				    int * buftv,
				    int & end,
				    long & currentReadPos,
				    int & ct,
				    long & redlen,
				    long & filelen);

	unsigned int getcharfromp(int & bc,
				                          int * buf,
				                          int * buft,
				                          int * buftv,
				                          int & end,
				                          long & currentReadPos,
								          int & ct,
								          long & redlen,
								          long & filelen,
										  unsigned int * txtbuf,
										  unsigned int & txtbc);
	void getphrasefromp(int & bc,
				                    int * buf,
				                    int * buft,
				                    int * buftv,
				                    int & end,
				                    long & currentReadPos,
				                    int & ct,
				                    long & redlen,
				                    long & filelen,
				                    unsigned int * txtbuf,
				                    unsigned int & txtbc,
									int & pos,
									int & count,
									unsigned int * ch);
	void DecompressToFile(CString fileName);
	void InitializeFromFile(CString fileName);
	
};


#endif
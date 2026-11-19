#pragma once

struct CAX_XMLheader
{
   char catagorie;
   long length;
};

class CAX_XMLCommandQueObj : public CObject
{
public:
	CAX_XMLCommandQueObj()
	{
	  m_msgID				= 0;
	  m_msgLength			= 0;
      m_dataBlock			= 0;
	  m_currentDldProgress	= 0;
      m_msgCompleted		= false;
	};

	CAX_XMLCommandQueObj(CAX_XMLheader * header)
	{
	  m_msgID				= 0;
	  m_msgLength			= header->length;
      m_dataBlock			= new BYTE[m_msgLength+1];
	  ZeroMemory(m_dataBlock,header->length+1);
	  m_currentDldProgress	= 0;
      m_msgCompleted		= false;
	};

	~CAX_XMLCommandQueObj()
	{
		if(m_dataBlock)
		{
	       delete [] m_dataBlock;
		   m_dataBlock = 0;
		}
	}

    int m_msgID;
	long m_msgLength;
    BYTE * m_dataBlock;
	long m_currentDldProgress;
    bool m_msgCompleted;
};

class CAX_XMLCommandQueList : public CObList
{
public:
	CAX_XMLCommandQueList()
	{
		InitializeCriticalSection(&m_cSection);
		m_stream=NULL;
		m_currentLength=0;
		m_dataIncommingEvent = CreateEvent( NULL,TRUE,TRUE,"DataIncomming"); 
	};
	~CAX_XMLCommandQueList()
	{
        if(m_stream)
		   delete [] m_stream;
		m_stream = 0;

		CloseHandle(m_dataIncommingEvent);
		DeleteCriticalSection(&m_cSection);
	};

	CRITICAL_SECTION m_cSection;
	BYTE * m_stream;
	long m_currentLength;
	HANDLE m_dataIncommingEvent;

	CAX_XMLCommandQueObj * GetNextCompletedMsg()
	{
		CAX_XMLCommandQueObj * cmdObj = NULL;
		EnterCriticalSection(&m_cSection);
		//process option
         if(m_currentLength > 8)
		 {//pull block
            CAX_XMLheader * header = (CAX_XMLheader *)m_stream;
            if((m_currentLength-8)>=header->length) 
			{//message ready to be pulled
               cmdObj = new CAX_XMLCommandQueObj((CAX_XMLheader *)m_stream);
			   CopyMemory(cmdObj->m_dataBlock,&m_stream[8],header->length);
			   cmdObj->m_msgCompleted = true;
               //AddTail(cmdObj);
			   //delete part of stream that is used
			   long remainder = m_currentLength - (header->length+8);
			   if(remainder > 0)
			   {//more data
                  BYTE * newBlock = new BYTE[remainder];
                  CopyMemory(newBlock,&m_stream[header->length+8],remainder);
                  m_currentLength = remainder;
                  delete [] m_stream;
				  m_stream = newBlock;
			   }//end more data
			   else
			   {//clear it
                  m_currentLength = 0;
                  delete [] m_stream;
				  m_stream = 0;
			   }//end clear it
			}//end message ready to be pulled
		 }//end pull block
        LeaveCriticalSection(&m_cSection);
		return cmdObj;
	}

	void AppendDataX(BYTE * data, long length)
	{
		 EnterCriticalSection(&m_cSection);
         if(!m_stream)
		 {
            m_stream = new BYTE[length];
            CopyMemory(m_stream,data,length);
			m_currentLength = length;
			SetEvent(m_dataIncommingEvent);
		 }
		 else
		 {
			m_currentLength += length;
            BYTE * appendBlock = new BYTE[m_currentLength];
			CopyMemory(appendBlock,m_stream,m_currentLength-length);
            CopyMemory(&appendBlock[(m_currentLength-length)],data,length);
            delete m_stream;
			m_stream=appendBlock;
			SetEvent(m_dataIncommingEvent);
	     }
         

		 LeaveCriticalSection(&m_cSection);
	}

	void AppendData(BYTE * data, long length)
	{
	   POSITION posLast;
	   BOOL appedInProg = FALSE;
	   EnterCriticalSection(&m_cSection);
	   long dataPos = 0;
       //reconnect or create
	   for(POSITION posLoc = GetHeadPosition();(posLast=posLoc) != NULL;)
	   {
           CAX_XMLCommandQueObj * cmdObj = (CAX_XMLCommandQueObj *)GetNext(posLoc);
		   if(cmdObj->m_msgCompleted)
			  continue;

           appedInProg = TRUE;
	   }
	   if(!appedInProg)
	   {//add one
          CAX_XMLCommandQueObj * cmdObj = new CAX_XMLCommandQueObj((CAX_XMLheader *)data);
          AddTail(cmdObj);
		  //buff for header
		  dataPos += sizeof(CAX_XMLheader);
	   }//end add one
	   
	   if(dataPos >= length)
	   {
	      
	      return;
	   }

	   length -= dataPos;

	   long curLength = 0;
       for(POSITION posLoc = GetHeadPosition();(posLast=posLoc) != NULL;)
	   {
           CAX_XMLCommandQueObj * cmdObj = (CAX_XMLCommandQueObj *)GetNext(posLoc);
		   if(cmdObj->m_msgCompleted)
			  continue;

		   curLength = length;
		   if((cmdObj->m_currentDldProgress+length) > cmdObj->m_msgLength)
		   {
		       curLength = cmdObj->m_msgLength - cmdObj->m_currentDldProgress;
               //create remainder object
               CAX_XMLCommandQueObj * cmdObj = new CAX_XMLCommandQueObj((CAX_XMLheader *)&data[curLength]);
               AddTail(cmdObj);
		   }

		   CopyMemory(&cmdObj->m_dataBlock[cmdObj->m_currentDldProgress],&data[dataPos],curLength);
		   cmdObj->m_currentDldProgress += curLength;
		   
		   if(cmdObj->m_currentDldProgress == cmdObj->m_msgLength)
		      cmdObj->m_msgCompleted = true;

		   break;
	   }
	   LeaveCriticalSection(&m_cSection);
	}
};
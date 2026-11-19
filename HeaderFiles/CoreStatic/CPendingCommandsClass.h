#ifndef CPENDINGCOMOBJCLASS_H
#define CPENDINGCOMOBJCLASS_H



class   CPendingCommandObj : public CObject
{
	
public:
    CPendingCommandObj();
    
	int m_command;
	int m_done;
	long m_timeCreated;
	long m_timeToExec;
	int m_register0;
	int m_register1;
	CString m_specialString;
};

class   CPendingCommandObjList : public CObList
{
public:
   CPendingCommandObjList(){};
   CPendingCommandObj * CallBack();
   BOOL AddCommand(int command, 
	               int register0, 
				   int register1, 
				   long duration,
				   CString specialString);
   void SafeDelete();
   
};

#endif
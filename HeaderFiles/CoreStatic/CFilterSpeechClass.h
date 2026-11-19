#ifndef CFILTERSPEECHCLASS_H
#define CFILTERSPEECHCLASS_H

class CFilterSpeechObj : public CObject
{
	
     
	DECLARE_SERIAL( CFilterSpeechObj );
	
    
public:
    CFilterSpeechObj();
	CString                    m_playersName;
  
	void Serialize( CArchive& ar );	
};

class CFilterSpeechList : public CObList
{
public:
	CFilterSpeechList(){};
   
    
     
	DECLARE_SERIAL( CFilterSpeechList );
	
    

	void SafeDelete();
	BOOL ShouldStatmentBeBlocked(CString statment);
	void AddName(CString name);
	void UnBlockName(CString name);
	BOOL IsNameAlreadyInList(CString name);

};

#endif
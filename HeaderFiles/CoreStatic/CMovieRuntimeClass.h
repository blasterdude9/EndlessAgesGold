#ifndef MOVIERUNTIMECLASS_H
#define MOVIERUNTIMECLASS_H




class CMovieRuntimeObj : public CObject
{
	CMovieRuntimeObj(){}
	
     
    DECLARE_SERIAL( CMovieRuntimeObj );
    
    
public:
	
	CMovieRuntimeObj(BOOL L_playing,
		             BOOL L_looping,
		             CString L_fileName,
					 RECT L_rectMov,
					 RECT L_changingRect
				     );

	                 RECT List_changingRect;
	                 BOOL List_playing;
					 BOOL List_looping;
	                 CString List_fileName;
					 RECT List_rectMov;
    
	void Serialize( CArchive& ar );

};

class CMovieRuntimeObjList : public CObList
{
public:
   CMovieRuntimeObjList(){};
   
    
   DECLARE_SERIAL( CMovieRuntimeObjList );
   
   
};

#endif




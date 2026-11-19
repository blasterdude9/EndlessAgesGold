#ifndef CGROUPCLASS_H
#define CGROUPCLASS_H

//class is basically used for popout
class  CGroupObj : public CObject
{
	CGroupObj(){}
	DECLARE_SERIAL( CGroupObj );
public:
	
	CGroupObj(
		     float         L_groupRadius,
			 D3DVECTOR     L_groupCenter,
             BOOL          L_valid
		     );
    
             float         List_groupRadius;
			 D3DVECTOR     List_groupCenter;
             BOOL          List_valid;

	void Serialize( CArchive& ar );

};

class  CGroupList : public CObList
{
public:
   CGroupList(){};
   DECLARE_SERIAL( CGroupList )
};

#endif
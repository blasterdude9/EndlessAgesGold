#include "d3d.h"
#ifndef AIWEBCLASS_H
#define AIWEBCLASS_H
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
class CAIRouteToObj : public CObject
{
	//CAIRouteToObj(){}
	DECLARE_SERIAL( CAIRouteToObj );
public:	
    //Order of objects mimics web list itself
    CAIRouteToObj();
    int m_mapNodeCount;//
	int *m_map;//list of route positions in necessary order
       
	void SafeDelete();
	void Serialize( CArchive& ar );
};

class CAIRouteToObjList : public CObList
{
public:
    CAIRouteToObjList(){};
	void SafeDelete();
	DECLARE_SERIAL( CAIRouteToObjList );
};

//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
class CAIWebNode : public CObject
{
	//CAIWebObj(){}
	DECLARE_SERIAL( CAIWebNode );
public:
	CAIWebNode();
    CString                   m_locationName;//=}
    CAIRouteToObjList        *m_routeToAllFromLocationList;
	int                      *m_directLinks;
	int                       m_directLinkCount;
	D3DVECTOR                 m_position;
	float                     m_radius;
	
	void  AddDirectLink(int newInt);
	int * GetRoute(int index,int * mapNodeCount);
	void  SafeDelete();
	void  Serialize( CArchive& ar );
};

class CAIWebNodeList : public CObList
{
public:
    CAIWebNodeList(){};
	DECLARE_SERIAL( CAIWebNodeList );

//Functions
	float GetRadius(int index);
	D3DVECTOR GetPosition(int index);
	BOOL VerifyMoreInList(CAIWebNode* wbPtr);
	int * AddIntegerIntoArray(int * intArray,int * arrayCount,int newInt);
	int  GetIndexFromPointer(CAIWebNode* wbPtr);
	void SafeDelete();
	BOOL CompileAll(int querryDuration);//generates connections and routes
};
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
class CAIWebObj : public CObject
{
	//CAIWebObj(){}
	DECLARE_SERIAL( CAIWebObj );
public:
	CAIWebObj();
    
	CString          m_name;
	CAIWebNodeList * m_web;

    int * GetRoute(int currentPos,int dest,int * nodeCount);
	void  SafeDelete();
	void  Serialize( CArchive& ar );
};

class CAIWebObjList : public CObList
{
public:
    CAIWebObjList(){};
	void SafeDelete();
	DECLARE_SERIAL( CAIWebObjList );
};

#endif

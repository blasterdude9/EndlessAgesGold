#include "matrixarb.h"
#ifndef CAITREESEARCHCLASS_H
#define CAITREESEARCHCLASS_H
//-------------------------------
//Containment class
//Holds Class     CAIRoute
//-------------------------------
class AFX_EXT_CLASS CAIAlternateRoutes : public CObList
{
public:
   CAIAlternateRoutes(){};


   //Functions
   void SafeDelete();
};
//-------------------------------
//Base class
//Primary root node
//-------------------------------
class AFX_EXT_CLASS CAITreeNode : public CObject
{
public:
    CAITreeNode();

	int                        m_currentNode;
	CAIAlternateRoutes        *m_alternateRoutes;

    //Functions
	void SafeDelete();
};
//-------------------------------
//Base class
//Primary root list
//-------------------------------
class AFX_EXT_CLASS CAITree : public CObList
{
public:
   CAITree();

   int                         m_currentNodeCount;

   //Functions
   BOOL AddNode(int currentIndex);
   void SafeDelete();
};
//-------------------------------
//Route
//Holds a complete buildable tree
//-------------------------------
class AFX_EXT_CLASS CAIRoute : public CObject
{
public:
    CAIRoute();
    
    CAITree * m_tree;
    BOOL      m_routeIsValid;
	BOOL      m_growingStill;//there are still options


	//Functions
    void SafeDelete();
};


#endif
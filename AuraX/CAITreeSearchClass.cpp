#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CAITreeSearchClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//----------------------------------//
//----------------------------------//
//----------------------------------//
CAITreeNode::CAITreeNode()
{
    m_currentNode       = 0;
	m_alternateRoutes   = NULL;
}
//----------------------------------//
//----------------------------------//
//----------------------------------//
void CAITreeNode::SafeDelete()
{
    if(m_alternateRoutes)
	{
       m_alternateRoutes->SafeDelete();
	   delete m_alternateRoutes;
	   m_alternateRoutes = 0;
	}
}
//----------------------------------//
//----------------------------------//
//----------------------------------//
CAITree::CAITree()
{
     m_currentNodeCount = 0;
}
//----------------------------------//
//----------------------------------//
//----------------------------------//
BOOL CAITree::AddNode(int currentIndex)
{
     CAITreeNode * node = new CAITreeNode();
	 node->m_currentNode = currentIndex;
     m_currentNodeCount++;
     AddTail(node);

	 return TRUE;
}
//----------------------------------//
//----------------------------------//
//----------------------------------//
void CAITree::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CAITreeNode * spnPtr = (CAITreeNode *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}
//----------------------------------//
//----------------------------------//
//----------------------------------//
CAIRoute::CAIRoute()
{
   m_tree            = NULL;
   m_routeIsValid    = TRUE;
   m_growingStill    = TRUE;
}
//----------------------------------//
//----------------------------------//
//----------------------------------//
void CAIRoute::SafeDelete()
{
   if(m_tree)
   {
      m_tree->SafeDelete();
	  delete m_tree;
	  m_tree = 0;
   }
}
//----------------------------------//
//----------------------------------//
//----------------------------------//
void CAIAlternateRoutes::SafeDelete()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CAIRoute * spnPtr = (CAIRoute *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
   }
   RemoveAll();
}


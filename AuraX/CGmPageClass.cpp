#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CGmPageClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGmPageObj::CGmPageObj()
{
    m_handle = 0;
}


void CGmPageList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CGmPageObj * gmpPtr = (CGmPageObj *)GetNext(posLoc);
		delete gmpPtr;
		gmpPtr=0;
	}
	RemoveAll();
}

void CGmPageList::AddPage(int handle)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CGmPageObj * gmpPtr = (CGmPageObj *)GetNext(posLoc);
		if(gmpPtr->m_handle == handle)
		   return;
	}
    CGmPageObj * pageObject = new CGmPageObj();
	pageObject->m_handle = handle;

    AddTail(pageObject);
}

void CGmPageList::RemovePage(int handle)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CGmPageObj * gmpPtr = (CGmPageObj *)GetNext(posLoc);
		if(gmpPtr->m_handle == handle)
		{
           delete gmpPtr;
		   gmpPtr = 0;
           RemoveAt(posLast);
		}
	}
}


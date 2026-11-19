#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include "resource.h"
#include <afxtempl.h>

#include "CTableSearchDBClass.h"

//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
CTableSearchDBObj::CTableSearchDBObj()
{

}
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
int CTableSearchDBObj::IsBoxOnWall(BNDBOX & primary,BNDBOX & boxToCheck)// 0 = not on a wall 1=x upper 2=x lower 3=y Upper 4=y lower 5=z Upper 6=z lower
{
    if(primary.min.x != boxToCheck.min.x && primary.min.y != boxToCheck.min.y && primary.min.z != boxToCheck.min.z &&
       primary.max.x != boxToCheck.max.x && primary.max.y != boxToCheck.max.y && primary.max.z != boxToCheck.max.z)
	   return 0; //definetly not on a wall-quick filter 


	//check X upper/lower    
    if(primary.max.z >= boxToCheck.max.z && primary.min.z <= boxToCheck.min.z &&
	   primary.max.y >= boxToCheck.max.y && primary.min.y <= boxToCheck.min.y)
	{
	   if(primary.max.x == boxToCheck.min.x)
          return 1;
	   else if(primary.min.x == boxToCheck.max.x)
		  return 2;
	}
	//check Y upper/lower    
    if(primary.max.z >= boxToCheck.max.z && primary.min.z <= boxToCheck.min.z &&
	   primary.max.x >= boxToCheck.max.x && primary.min.x <= boxToCheck.min.x)
	{
	   if(primary.max.y == boxToCheck.min.y)
          return 3;
	   else if(primary.min.y == boxToCheck.max.y)
		  return 4;
	}
    //check Z upper/lower    
    if(primary.max.y >= boxToCheck.max.y && primary.min.y <= boxToCheck.min.y &&
	   primary.max.x >= boxToCheck.max.x && primary.min.x <= boxToCheck.min.x)
	{
	   if(primary.max.z == boxToCheck.min.z)
          return 5;
	   else if(primary.min.z == boxToCheck.max.z)
		  return 6;
	}

return 0;
}
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
int CTableSearchDBObj::GetBaseBoxCount(CCollisionObj * collisionObject)
{
  if(!collisionObject)
	  return 0;

  int traceCount = 0;

  int trace = 0;
         for (int xPtr=0; xPtr<collisionObject->SizeSquaredX[0]; xPtr++)
           for (int yPtr=0; yPtr<collisionObject->SizeSquaredY[0]; yPtr++)
            for (int zPtr=0; zPtr<collisionObject->SizeSquaredZ[0]; zPtr++)
			{//loop through all boxes in list
	         int oldCurSubLevel=-1;
             int CurSubLevel;
             REFBOX ***CurrentSubBox;
	         CurSubLevel = 0;
          
             CurrentSubBox = collisionObject->boxCollection[xPtr][yPtr][zPtr].SubBoxCollection;

			 if(CurrentSubBox != NULL){
                GetBaseBoxCountRecurse(&traceCount,
					                    CurrentSubBox,
										collisionObject);
			 }
			 else
			 traceCount++;
                
			}//end loop through all boxes in list

  return 0;
}
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
void CTableSearchDBObj::GetBaseBoxCountRecurse(int * curCount,
											   REFBOX ***CurrentSubBox,
											   CCollisionObj * collisionObject)
{
     for (int xPos=0; xPos<collisionObject->SizeSquaredX[0]; xPos++)
       for (int yPos=0; yPos<collisionObject->SizeSquaredY[0]; yPos++)
         for (int zPos=0; zPos<collisionObject->SizeSquaredZ[0]; zPos++)
		 {//loop through all boxes inside list 	   
               //save box data
		       if(CurrentSubBox[xPos][yPos][zPos].SubBoxCollection != NULL)
			      GetBaseBoxCountRecurse(curCount,
				                         CurrentSubBox[xPos][yPos][zPos].SubBoxCollection,
										 collisionObject);
			   else
				  *curCount++;
		 }//end loop through all boxes inside list 
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
void CTableSearchDBObj::LocateBox(int *FoundBoxes, 
								  D3DVECTOR Pstart, 
								  D3DVECTOR Pend, 
								  REFBOX BoxArray)
{
 /*int            curbox;
 int            endbox;
 int            newbox;

 double         Pdifx;
 double         Pdify;
 double         Pdifz;
 double         tx,ty,tz,ts;
 D3DVECTOR      Pmax,Pmin,Ptemp;

 Pdifx = Pend.x - Pstart.x;
 Pdify = Pend.y - Pstart.y;
 Pdifz = Pend.z - Pstart.z;

 curbox = wherept(Pstart, BoxArray, 1);
 endbox = wherept(Pend, BoxArray, 1);

 while (curbox != endbox)
 {
  Add2BoxList(FoundBoxes, curbox, AddBox);
  Pmax.x = BoxArray[curbox].Pmax.x;
  Pmax.y = BoxArray[curbox].Pmax.y;
  Pmax.z = BoxArray[curbox].Pmax.z;
  Pmin.x = BoxArray[curbox].Pmin.x;
  Pmin.y = BoxArray[curbox].Pmin.y;
  Pmin.z = BoxArray[curbox].Pmin.z;


  if (Pdifx > 0.0)
  {
   tx = (Pmax.x - Pstart.x)/Pdifx;
  }
  else if (Pdifx < 0.0)
  {
   tx = (Pmin.x - Pstart.x)/Pdifx;
  }

  if (Pdify > 0.0)
  {
   ty = (Pmax.y - Pstart.y)/Pdify;
  }
  else if (Pdify < 0.0)
  {
   ty = (Pmin.y - Pstart.y)/Pdify;
  }

  if (Pdifz > 0.0)
  {
   tz = (Pmax.z - Pstart.z)/Pdifz;
  }
  else if (Pdifz < 0.0)
  {
   tz = (Pmin.z - Pstart.z)/Pdifz;
  }

  if ((Pdifx != 0.0) && (Pdify != 0.0) && (Pdifz != 0.0))
  {
   if ((tx <= ty) && (tx <= tz))
   {
    ts = tx;
    if (Pdifx < 0.0)
    {
     newbox = BoxArray[curbox].xlow;
    }
    else
    {
     newbox = BoxArray[curbox].xhigh;
    }
   }
   else if ((ty <= tx) && (ty <= tz))
   {
    ts = ty;
    if (Pdify < 0.0)
    {
     newbox = BoxArray[curbox].ylow;
    }
    else
    {
     newbox = BoxArray[curbox].yhigh;
    }
   }
   else
   {
    ts = tz;
    if (Pdifz < 0.0)
    {
     newbox = BoxArray[curbox].zlow;
    }
    else
    {
     newbox = BoxArray[curbox].zhigh;
    }
   }
  }
  else if ((Pdifx != 0.0) && (Pdify != 0.0))
  {
   if (tx <= ty)
   {
    ts = tx;
    if (Pdifx < 0.0)
    {
     newbox = BoxArray[curbox].xlow;
    }
    else
    {
     newbox = BoxArray[curbox].xhigh;
    }
   }
   else
   {
    ts = ty;
    if (Pdify < 0.0)
    {
     newbox = BoxArray[curbox].ylow;
    }
    else
    {
     newbox = BoxArray[curbox].yhigh;
    }
   }
  }
  else if ((Pdifx != 0.0) && (Pdifz != 0.0))
  {
   if (tx <= tz)
   {
    ts = tx;
    if (Pdifx < 0.0)
    {
     newbox = BoxArray[curbox].xlow;
    }
    else
    {
     newbox = BoxArray[curbox].xhigh;
    }
   }
   else
   {
    ts = tz;
    if (Pdifz < 0.0)
    {
     newbox = BoxArray[curbox].zlow;
    }
    else
    {
     newbox = BoxArray[curbox].zhigh;
    }
   }
  }
  else if ((Pdifz != 0.0) && (Pdify != 0.0))
  {
   if (tz <= ty)
   {
    ts = tz;
    if (Pdifz < 0.0)
    {
     newbox = BoxArray[curbox].zlow;
    }
    else
    {
     newbox = BoxArray[curbox].zhigh;
    }
   }
   else
   {
    ts = ty;
    if (Pdify < 0.0)
    {
     newbox = BoxArray[curbox].ylow;
    }
    else
    {
     newbox = BoxArray[curbox].yhigh;
    }
   }
  }
  else if (Pdifx != 0.0)
  {
   ts = tx;
   if (Pdifx < 0.0)
   {
    newbox = BoxArray[curbox].xlow;
   }
   else
   {
    newbox = BoxArray[curbox].xhigh;
   }
  }
  else if (Pdify != 0.0)
  {
   ts = ty;
   if (Pdify < 0.0)
   {
    newbox = BoxArray[curbox].ylow;
   }
   else
   {
    newbox = BoxArray[curbox].yhigh;
   }
  }
  else
  {
   ts = tz;
   if (Pdifz < 0.0)
   {
    newbox = BoxArray[curbox].zlow;
   }
   else
   {
    newbox = BoxArray[curbox].zhigh;
   }
  }

  Ptemp.x = Pstart.x + (ts * Pdifx);
  Ptemp.y = Pstart.y + (ts * Pdify);
  Ptemp.z = Pstart.z + (ts * Pdifz);

  if ((Ptemp.x <= BoxArray[newbox].Pmax.x) &&
      (Ptemp.y <= BoxArray[newbox].Pmax.y) &&
      (Ptemp.z <= BoxArray[newbox].Pmax.z) &&
      (Ptemp.x >= BoxArray[newbox].Pmin.x) &&
      (Ptemp.y >= BoxArray[newbox].Pmin.y) &&
      (Ptemp.z >= BoxArray[newbox].Pmin.z))
  {
   curbox = newbox;
  }
  else
  {
   curbox = wherept(Ptemp, BoxArray, newbox);
  }
 }
 Add2BoxList(FoundBoxes, curbox, LastBox);*/
}
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
int CTableSearchDBObj::wherept( D3DVECTOR ThisPt, 
							    REFBOX * BoxArray, 
								int StartBox)
{
 /*int    thisbox;
 int    newbox;
 bool   notfoundx = true;
 bool   notfoundy = true;
 bool   notfoundz = true;

 thisbox = StartBox;

 while (notfoundx || notfoundy || notfoundz)
 {
  while (notfoundx)
  {
   if ((ThisPt.x >= BoxArray[thisbox].Pmin.x) &&
       (ThisPt.x <= BoxArray[thisbox].Pmax.x))
   {
    notfoundx = false;
   }
   else if (ThisPt.x > BoxArray[thisbox].Pmax.x)
   {
    newbox = BoxArray[thisbox].xhigh;
    if (newbox == thisbox)
    {
     notfoundx = false;
    }
    else
    {
     thisbox = newbox;
     notfoundy = true;
     notfoundz = true;
    }
   }
   else
   {
    newbox = BoxArray[thisbox].xlow;
    if (newbox == thisbox)
    {
     notfoundx = false;
    }
    else
    {
     thisbox = newbox;
     notfoundy = true;
     notfoundz = true;
    }
   }
  }

  while (notfoundy)
  {
   if ((ThisPt.y >= BoxArray[thisbox].Pmin.y) &&
       (ThisPt.y <= BoxArray[thisbox].Pmax.y))
   {
    notfoundy = false;
   }
   else if (ThisPt.y > BoxArray[thisbox].Pmax.y)
   {
    newbox = BoxArray[thisbox].yhigh;
    if (newbox == thisbox)
    {
     notfoundy = false;
    }
    else
    {
     thisbox = newbox;
     notfoundx = true;
     notfoundz = true;
    }
   }
   else
   {
    newbox = BoxArray[thisbox].ylow;
    if (newbox == thisbox)
    {
     notfoundy = false;
    }
    else
    {
     thisbox = newbox;
     notfoundx = true;
     notfoundz = true;
    }
   }
  }

  while (notfoundz)
  {
   if ((ThisPt.z >= BoxArray[thisbox].Pmin.z) &&
       (ThisPt.z <= BoxArray[thisbox].Pmax.z))
   {
    notfoundz = false;
   }
   else if (ThisPt.z > BoxArray[thisbox].Pmax.z)
   {
    newbox = BoxArray[thisbox].zhigh;
    if (newbox == thisbox)
    {
     notfoundz = false;
    }
    else
    {
     thisbox = newbox;
     notfoundx = true;
     notfoundy = true;
    }
   }
   else
   {
    newbox = BoxArray[thisbox].zlow;
    if (newbox == thisbox)
    {
     notfoundz = false;
    }
    else
    {
     thisbox = newbox;
     notfoundx = true;
     notfoundy = true;
    }
   }
  }
 }
 return (thisbox);*/
return 0;
}
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
void CTableSearchDBObj::Serialize( CArchive& ar )
{
/*CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << 
	}
	else
	{
       ar >> 
	}*/
}     

IMPLEMENT_SERIAL(CTableSearchDBObj, CObject, 0)



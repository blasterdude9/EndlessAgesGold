#include "dplay8.h"
#ifndef CSESSIONCLASS_H
#define CSESSIONCLASS_H

class   CSessionObj : public CObject
{
public:
    CSessionObj();
	void SafeDelete();
	
	DPN_APPLICATION_DESC   * m_applicationDesc;
    IDirectPlay8Address          * m_hostAddress;
	IDirectPlay8Address          * m_deviceInfo;
  
};

class   CSessionObjList : public CObList
{
public:
   CSessionObjList(){};
   void SafeDelete();
   BOOL AddObjectIfNotAdded(DPN_APPLICATION_DESC * applicationDesc,
                                          IDirectPlay8Address  * hostAddress,
	                                      IDirectPlay8Address  * deviceInfo);
};

#endif
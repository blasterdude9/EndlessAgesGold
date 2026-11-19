#ifndef CLIGHTTRACKER_H
#define CLIGHTTRACKER_H



class  CLightTrackerObj : public CObject
{
public:
	CLightTrackerObj();

    int List_hardwareIndex;
};

class  CLightTrackerObjList : public CObList
{
public:
   CLightTrackerObjList(){};

   int AddLight();
   void LightHasBeenRemoved(int hardwareIndex);
   void SafeDelete();
};

#endif
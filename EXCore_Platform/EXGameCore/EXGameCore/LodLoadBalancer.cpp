#include "StdAfx.h"
#include "LodLoadBalancer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLodLoadBalancer::CLodLoadBalancer(void)
{
    m_activeLodManagers = 0;
    ZeroMemory(m_lodTrackerArray,sizeof(lodTracker)*64);
}

CLodLoadBalancer::~CLodLoadBalancer(void)
{
}

void CLodLoadBalancer::Reset()
{
  for(int loop=0;loop<m_activeLodManagers;loop++)
  {
      m_lodTrackerArray[loop].m_currentThisRount = m_lodTrackerArray[loop].m_maxAllowed;
  }
}

void CLodLoadBalancer::ClearAll()
{
    m_activeLodManagers = 0;
    ZeroMemory(m_lodTrackerArray,sizeof(lodTracker)*64);
}

BOOL CLodLoadBalancer::AddLODAllowance(int lod, int max)
{
     if(lod >= 64 || lod < 0)
	    return FALSE;

	 m_lodTrackerArray[lod].m_maxAllowed = max;
	 if(m_activeLodManagers < (lod+1))
	    m_activeLodManagers = lod+1;

	 return TRUE;
}

void CLodLoadBalancer::TranslateLOD(int & currentLod)
{
     if(m_lodTrackerArray[currentLod].m_maxAllowed == 0)//disabled
	    return;

	 if(m_lodTrackerArray[currentLod].m_currentThisRount > 0)
	 {
	    m_lodTrackerArray[currentLod].m_currentThisRount--;//use one up
	 }
	 else
	 {//no more of this lod is allowed therefor me must default to the next one down
	    while(currentLod < 64)
		{//loop to find open lod to use
           currentLod++;
		   if(m_lodTrackerArray[currentLod].m_maxAllowed == 0)//disabled
	          break;

		   if(m_lodTrackerArray[currentLod].m_currentThisRount > 0)
		   {
				m_lodTrackerArray[currentLod].m_currentThisRount--;//use one up
				break;
		   }  
		}//end loop to find open lod to use
	 }
}

void CLodLoadBalancer::ApplyLODProcessing(CString strLodScript)
{//slow but doesnt matter
    ClearAll();//clear all 

	//If current LOD count, remove lod #s zero based //example: 10000,6000,3000,1500,500 0,1,2,3,4
    //4,4,2,0;3,0,2;3,0; example of string that gets parsed to generate thse structures in an array form
    //always make sure lod that you will keep are listed greatest to least, this will keep proper lod removal
    //second line/section defines by a \r\n(newline) of the script includes the comminted LOD ranges for entities based on its height, 
	//this is critical because you can have only one set of ranges per entity and if a lod doesnt exist it just wont render ie: weapon 
	//attachment that may only have 2 lods and the entity is on lod 3 the attachment wont render this is a advantage
	//however if not understood while designing lods or adding lods this can be a major problem
	//PROBLEM : lod ranges can be different on every entity, tailored by size genrally-if range isnt in script will keep whats currently there
	//script example: 
	//4,4,2,0;3,0,2;3,0
	//0-4,7,30,300
	//4-6,10,50,600
	//6-12,20,90,800
	//LOD LIMITS (LOAD BALANCING) LOD INDEX,HOW MANY ALLOWED
    //0,2
    //1,5

	int scriptLength = strLodScript.GetLength();
	char * scriptArray = strLodScript.GetBuffer();
	int trace = 0;
	int engaged = 0;
	CString temp;
	
    while(trace < scriptLength)
	{
            if(scriptArray[trace] == 'L')
			{//found the lod limits section
               while(scriptArray[trace] != '\r' && scriptArray[trace] != '\n' && trace < scriptLength)
		       {//eat 1 line

			      trace++;
			   }//end eat 1 line
			   trace++;
               if(scriptArray[trace] == '\n' && trace < scriptLength)
			      trace++;

			   while(trace < scriptLength)
		       {//line by line
                  //lod index
			      int lodIndex = 0;
				  int lodLimit = 0;
			      temp = "";
                  while(scriptArray[trace] != '\r' && scriptArray[trace] != '\n' && scriptArray[trace] != ' ' && scriptArray[trace] != ',' && trace < scriptLength)
		          {//line by line
                     temp += scriptArray[trace];
				     trace++;
				  }
                  lodIndex = atoi(temp);
			      trace++;
				  if(scriptArray[trace] == '\n' && trace < scriptLength)
			         trace++;
           
				  temp = "";
				  while(scriptArray[trace] != '\r' && scriptArray[trace] != '\n' && scriptArray[trace] != ' ' && scriptArray[trace] != ',' && trace < scriptLength)
		          {//line by line
                     temp += scriptArray[trace];
				     trace++;
				  }
                  lodLimit = atoi(temp);
			      trace++;
				  if(scriptArray[trace] == '\n' && trace < scriptLength)
			      trace++;

				  AddLODAllowance(lodIndex, lodLimit);

			   }//end line by line
			}//end found lod limits section
		trace++;
	}
}

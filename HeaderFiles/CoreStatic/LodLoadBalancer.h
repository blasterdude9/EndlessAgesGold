#pragma once
//this class will be used so frequently it must remain very fast
//this class will most likly be used by select entities, not all should be managed, there will probably be some simple ai
struct lodTracker
{
    int m_maxAllowed;//how many are allowed at this index that are under lod load balancing rules
    int m_currentThisRount;
};

class CLodLoadBalancer
{
public:
	CLodLoadBalancer(void);

    
    int m_activeLodManagers;
    lodTracker m_lodTrackerArray[64];//more than 64 lod configuration to track would be un-civilized
	                                 //the array index corrilates to the lod level, LOD 0 = 0 element in the array etc

	void TranslateLOD(int & currentLod);//this will either allow or modify current lod to a alowable lod
	void Reset();//after each scene assembly or viewport rendered this should be called
	BOOL AddLODAllowance(int lod, int max);//this adds lod maximums, if one type of lod is undefined there is no limit, like zombocom
	void ApplyLODProcessing(CString strLodScript);//initialize from script
	void ClearAll();//removes all limits
public:
	~CLodLoadBalancer(void);
};

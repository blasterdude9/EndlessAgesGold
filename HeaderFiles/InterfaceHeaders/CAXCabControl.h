#include "afxcmn.h"
#include "afxwin.h"
#include "fdi.h"
#ifndef CCABINETCONTROL_H
#define CCABINETCONTROL_H



class  CCabinetObj : public CObject
{
	
    
public:
	CCabinetObj(){};
//Vars
int ExtractCabinetToDirectory(CString destPath,CString CabinetFile,CProgressCtrl * progressRef);
int ExtractCabinetToDirectory(CString destPath,CString CabinetFile,int * percCompleted,CRITICAL_SECTION * criticalSection);
int ExtractCabinetToDirectory(CString destPath,
							CString CabinetFile,
							int * percCompleted,
							CRITICAL_SECTION * criticalSection,
							CArchive & archiveRef);
//Functions inline

//primary logic

//utilize


//Functions 2

	
};

#endif
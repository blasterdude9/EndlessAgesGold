/**********************************************************************
*<
	FILE: PluginTest.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

*>	Copyright (c) 1997, All Rights Reserved.
**********************************************************************/
#define  D3D_OVERLOADS
#include "afxwin.h"
#include "afxext.h"
#include "PluginTest.h"
#include "PHYEXP.h"
#include "Bipexp.h"
#include "MAXScrpt.h"
#include "Numbers.h"
#include "MAXObj.h"
#include "3DMath.h"

#include "IMPEXP.H"
#include "direct.h"
#include "decomp.h"

#include "modstack.h"
#include "MESHDLIB.H"

//DPD
#include "ID3D9GraphicsWindow.h"
#include "iparamm2.h"

#include "d3dx9effect.h"
#include "D3DX9.h"

#include <fstream>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>






//static declarations 
static ExporterClassDesc ExporterDesc;
ClassDesc2* GetExporterDesc() {return &ExporterDesc;}
DLGCommunication Exporter::GL_CommDlg;
//--- Exporter -------------------------------------------------------
Exporter::Exporter()
{
m_lightCache = NULL;
m_logFileName = "AurXpluginLog.TXT";
m_loggingType = 1;
WriteDebugFile("Exporter initialized",1,1);
m_webExport = NULL;
GL_currentPhyModIndex = 0;
m_skeletalExport = NULL;
GL_physiqueModCount = 0;
GL_CommDlg.nodeCount = 0;
GL_CommDlg.GL_SceneNodes = NULL;
GL_CommDlg.selectionCount = 0;
GL_index = 0;
GL_CommDlg.GL_animSpeedAdjust = 2;
GL_CommDlg.GL_exportType = 0;//0=character/bones 1=World export
GL_CommDlg.GL_outputMemoryType = 0;//0=memory 1=file
GL_CommDlg.GL_sightRange = 100;
GL_CommDlg.GL_generateNormals = FALSE;
GL_CommDlg.GL_exportAllKeys = FALSE;
GL_CommDlg.GL_enableBlendedExport = TRUE;
GL_CommDlg.GL_smShadeToleranceAngle = "180"; // Originally 45, Changed For Alan
GL_CommDlg.GL_miscLoadInstruction = 0;
GL_CommDlg.GL_generateTangents = FALSE;

m_autoLoader = new CPluginTransfer();


m_memBlock = NULL;
m_numMeshes = 0;
m_memSize = 0;
}

Exporter::~Exporter() 
{
if(GL_CommDlg.nodeCount != 0)
   delete [] GL_CommDlg.GL_SceneNodes;
   GL_CommDlg.GL_SceneNodes=0;
   if(GL_CommDlg.selectionCount > 0)
   delete [] GL_CommDlg.selectionBuffer;
   GL_CommDlg.selectionBuffer=0;
   GL_CommDlg.selectionCount = 0;

   if(m_autoLoader)
      delete m_autoLoader;
   m_autoLoader = 0;
}

int Exporter::ExtCount()
{
	CString h="HOE";
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *Exporter::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	return _T("ARB");
}

const TCHAR *Exporter::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("ARB - Exporter");
}
	
const TCHAR *Exporter::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("AURA EXPORT");
}

const TCHAR *Exporter::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("Aaron Boucher");
}

const TCHAR *Exporter::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("Aaron Boucher");
}

const TCHAR *Exporter::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *Exporter::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int Exporter::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 201;
}

void Exporter::ShowAbout(HWND hWnd)
{			
	// Optional
}

//---------------------------------------------------------------//
//----------------------struct helpers---------------------------//
//---------------------------------------------------------------//
BOOL Exporter::GetNodeNames(INode* node,BOOL hiddenFlags)
{
        /*
        OMNI_LIGHT_CLASS_ID - Omni Light
		SPOT_LIGHT_CLASS_ID - Spot Light
		DIR_LIGHT_CLASS_ID - Directional Light
		FSPOT_LIGHT_CLASS_ID - Free Spot Light
		TDIR_LIGHT_CLASS_ID - Target Directional Light
        CAMERA_CLASS_ID
        SIMPLE_CAM_CLASS_ID - Free Camera
        LOOKAT_CAM_CLASS_ID - Target Camera
		*/


		ObjectState os = node->EvalWorldState(0); 
		// The obj member of ObjectState is the actual object we will export.
		if (os.obj) {//info on node

			if(TestIfBipedObject (node)==FALSE){//filter Biped
			  if(GL_CommDlg.nodeCount < nTotalNodeCount)
			  {
				  BOOL proceed = TRUE;
				  Control *c = node->GetTMController();
				  Object* ObjectPtr = node->GetObjectRef();

				  if(hiddenFlags)
			         if(node->IsNodeHidden())
					    proceed = FALSE;

				  if(ObjectPtr)
					if(ObjectPtr->SuperClassID() == CAMERA_CLASS_ID || ObjectPtr->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
					    proceed = FALSE;

				  if(proceed)
				  {
				     GL_CommDlg.GL_SceneNodes[GL_CommDlg.nodeCount] = FixupName(node->GetName());
                     GL_CommDlg.nodeCount++;
				  }
			  }
			}//end filter Biped
            //Modifier* phyMod = FindPhysiqueModifier (node);	
	        //if(phyMod != NULL)//physique exists  
            //   GL_physiqueModCount++;

			//GL_CommDlg.GL_SceneNodes[GL_CommDlg.nodeCount] = FixupName(node->GetName());
			//GL_CommDlg.nodeCount++;
		}//end info on node
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!GetNodeNames(node->GetChildNode(c),hiddenFlags))
			return FALSE;
	}

	return TRUE;
}
//---------------------------------------------------------------//
//----------------------struct helpers---------------------------//
//---------------------------------------------------------------//



BOOL Exporter::GetPhysiqueCount(INode* node)
{

		ObjectState os = node->EvalWorldState(0); 
		// The obj member of ObjectState is the actual object we will export.
		if (os.obj) {//info on node
            Modifier* phyMod = FindPhysiqueModifier (node);	

			Modifier* skinMod = FindSkinModifier (node);
	        
			LPSTR nodeName = FixupName(node->GetName());
			if(phyMod != NULL || skinMod != NULL)
			for(int loop=0;loop<GL_CommDlg.selectionCount;loop++){//check for mesh output status
				//int result = strnicmp(nodeName,GL_CommDlg.GL_SceneNodes[GL_CommDlg.selectionBuffer[loop]].stringBuild,256);
                if(nodeName==GL_CommDlg.GL_SceneNodes[GL_CommDlg.selectionBuffer[loop]]){//exists*/
			       GL_physiqueModCount++;
			
			//if(phyMod != NULL)//physique exists  
               //GL_physiqueModCount++;
				}
			}
			//GL_CommDlg.GL_SceneNodes[GL_CommDlg.nodeCount] = FixupName(node->GetName());
			//GL_CommDlg.nodeCount++;
		}//end info on node
	    for (int c = 0; c < node->NumberOfChildren(); c++) {
		     if (!GetPhysiqueCount(node->GetChildNode(c)))
			      return FALSE;
	    }

	return TRUE;
}
//---------------------------------------------------------------//
//---------------fill struct now---------------------------------//
//---------------------------------------------------------------//
int Exporter::EnumDlgStructInfo()
{
	nTotalNodeCount = 0;
	PreProcess(ip->GetRootNode(), nTotalNodeCount);
	
	//GenRootNodeArray();
	BOOL hiddenFlag = TRUE;
	//allocate
	GL_CommDlg.GL_SceneNodes = new CString[nTotalNodeCount];
	//GL_CommDlg.nodeReferenceArray = new nodeReference[nTotalNodeCount];
	int tracer=0;
	GL_CommDlg.nodeCount = 0;
	int numChildren = ip->GetRootNode()->NumberOfChildren();
    for (int idx=0; idx<numChildren; idx++) {
		GetNodeNames(ip->GetRootNode()->GetChildNode(idx),hiddenFlag);
		//GL_CommDlg.GL_SceneNodes[GL_CommDlg.nodeCount] = FixupName(ip->GetRootNode()->GetChildNode(idx)->GetName());
		//GL_CommDlg.nodeCount++;
	}
return 1;
}
//---------------------------------------------------------------//
//----------------------Core Export------------------------------//
//---------------------------------------------------------------//
int Exporter::DoExport(const TCHAR *filename,ExpInterface *ei,Interface *gi, BOOL suppressPrompts, DWORD options) {
    //if(!suppressPrompts)
    ip = gi;//use this if you want the scene to be edited
    
	//ip = (Interface*)gi->CloneInterface(0);//use this if you want reconstructionm changes to not effect the scene

	EnumDlgStructInfo();
    // Open the stream
	DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), 
				ExporterOptionsDlgProc, (LPARAM)this);

	int numChildren = ip->GetRootNode()->NumberOfChildren();
    for (int idx=0; idx<numChildren; idx++) {
		GetPhysiqueCount(ip->GetRootNode()->GetChildNode(idx));
	}
	
	if(GL_CommDlg.GL_exportType == 9){//reference export
           
		     CReferenceImpExpList * referenceExporter = new CReferenceImpExpList();
	         int nTotalNodeCount = 0;
	         PreProcess(ip->GetRootNode(), nTotalNodeCount);
			 int numChildren = ip->GetRootNode()->NumberOfChildren();
			 int dummyTrace=0;
	         GL_index = 0;
			 //Build info
			 for (int idx=0; idx<numChildren; idx++) {
		          NodeEnumRefExporter(ip->GetRootNode()->GetChildNode(idx),dummyTrace,referenceExporter);
			 }
			 //save tree to file
			 CString cnvFilname = filename;
             CABLIB.RemoveFileExt(&cnvFilname);
			 //end remove path
			 cnvFilname = cnvFilname+".rne";
			 referenceExporter->SaveToFile(cnvFilname);
			 referenceExporter->SafeDelete();
             delete referenceExporter;
			 referenceExporter = 0; 
	}//end reference export
    if(GL_CommDlg.GL_exportType == 5){//ai tree export
             m_webExport = new CAIWebObj();
			 m_webExport->m_web = new CAIWebNodeList();
	         int nTotalNodeCount = 0;
	         PreProcess(ip->GetRootNode(), nTotalNodeCount);
			 int numChildren = ip->GetRootNode()->NumberOfChildren();
			 int dummyTrace=0;
	         GL_index = 0;
			 //Build info
			 int idx=0;
			 for (idx=0; idx<numChildren; idx++) {
		          nodeEnumAiTreeExport(ip->GetRootNode()->GetChildNode(idx),dummyTrace);
			 }
			 GL_index = 0;
			 for (idx=0; idx<numChildren; idx++) {
		          nodeEnumAiTreeLinksExport(ip->GetRootNode()->GetChildNode(idx),dummyTrace);
			 }
             //compile
             m_webExport->m_web->CompileAll(GL_CommDlg.treeTHDuration);
			 //save tree to file
             CFileException exc;
             CFile fl;
			 CString cnvFilname = filename;
             CABLIB.RemoveFileExt(&cnvFilname);
			 m_webExport->m_name = cnvFilname;
             //remove path
			 int stringLenth = m_webExport->m_name.GetLength();
             for(int trimLoop=stringLenth;trimLoop > 0;trimLoop--){
                 CString tempAlloc = m_webExport->m_name.GetAt(trimLoop);
				 if(tempAlloc == "\\"){
                    m_webExport->m_name.Delete(0,trimLoop+1);
					break;
				 }
			 }
			 //end remove path
			 cnvFilname = cnvFilname+".tre";
             BOOL res = fl.Open(cnvFilname,CFile::modeCreate | CFile::modeWrite, &exc);
             CArchive the_out_Archive(&fl,CArchive::store);
	         the_out_Archive << m_webExport;
			 the_out_Archive.Close();
			 fl.Close(); 
			 //end save tree to file
	}//end ai tree export
	if(GL_CommDlg.GL_exportType == 1){//world object export
             m_autoLoader->ClearBlock();
       // Open the stream
	         int nTotalNodeCount = 0;
	         PreProcess(ip->GetRootNode(), nTotalNodeCount);
			 int numChildren = ip->GetRootNode()->NumberOfChildren();
			 int dummyTrace=0;
	         GL_index = 0;
			 if(!GL_CommDlg.GL_exportAllKeys)
			 for (int idx=0; idx<numChildren; idx++) {
		          nodeEnumWorldExport(ip->GetRootNode()->GetChildNode(idx),dummyTrace,ip->GetTime(),"");
			 }

			 if(GL_CommDlg.GL_outputMemoryType==0)
			 {
			    m_autoLoader->BroadCastData();
				m_autoLoader->ClearBlock();
			 }

			 if(GL_CommDlg.GL_exportAllKeys)
			 {
                 Interval range = ip->GetAnimRange();
	             int frameCount = ((range.End()-range.Start()) / GetTicksPerFrame());
               
			     for (int idx=0; idx<numChildren; idx++) {
		              
					 for(int jCount = 0;jCount < frameCount;jCount++)
					 {
                         CString appTemp = "_frame"+CABLIB.ConvToString(jCount);
					     nodeEnumWorldExport(ip->GetRootNode()->GetChildNode(idx),dummyTrace,jCount*GetTicksPerFrame(),appTemp);
			         }
				 }
			 }

			 //if(GL_CommDlg.GL_outputMemoryType==0)
			    
				 //MemSend();
			 if(m_lightCache)
			    m_lightCache->SaveToFile("lightCache.rne");
	}//end world object export
	if(GL_CommDlg.GL_exportType == 0){//bones/hiarchy
	   
	   CString * animationNames = new CString[100];
	   int     * startFrames = new int[100];
	   int     * endFrames = new int[100];
	   int     * totalFramesLoc = new int[100];
	   BOOL    overrideMaxExport = FALSE;
	   int tracer = 0;
	   CFileException exc5;
       CFile fl5;
	   CArchive the_Archive5(&fl5,CArchive::load);
       BOOL res = fl5.Open("animation.txt",CFile::modeRead, &exc5);
	   if(res == TRUE)
	   {
	    overrideMaxExport = TRUE;
	    CString tempBuffer;
     
	    while(the_Archive5.ReadString(tempBuffer))
		{
           if(the_Archive5.ReadString(tempBuffer))//start frame
		      animationNames[tracer] = tempBuffer;

	       if(the_Archive5.ReadString(tempBuffer))//start frame
		      startFrames[tracer] = atoi(tempBuffer);

	       if(the_Archive5.ReadString(tempBuffer))//end frame
		      endFrames[tracer] = atoi(tempBuffer);

		   totalFramesLoc[tracer] = endFrames[tracer]-startFrames[tracer];

		   tracer++;
		}
        the_Archive5.Close();
	    fl5.Close();
		
	   }
	   //AfxMessageBox("Begin Export 1");
	 //test animation extract*/
	   m_autoLoader->ClearBlock();
       /*CString CurDir;
       char buffer[_MAX_PATH];
	   _getcwd( buffer, _MAX_PATH );
       CurDir = buffer;
       m_autoLoader->AppendStringSegToBlock(CurDir + "\\" + filename);*/

	   //m_autoLoader->AppendStringSegToBlock(filename);

       if(!overrideMaxExport)
	   {
		   m_autoLoader->AppendStringSegToBlock(filename,GL_CommDlg.GL_miscLoadInstruction);
	       ExportSkeleton(filename,FALSE,0,0);
	   }
	   else
	   {
           CString CurDir;
           char buffer[_MAX_PATH];
	       _getcwd( buffer, _MAX_PATH );
           CurDir = buffer;
           if(tracer > 0)
		   m_autoLoader->AppendStringSegToBlock(CurDir + "\\" +(animationNames[0]+".ARB"),GL_CommDlg.GL_miscLoadInstruction);
		   else
		   ExportSkeleton(filename,FALSE,0,0);

           for(int iCount = 0;iCount < tracer;iCount++)
               ExportSkeleton((animationNames[iCount]+".ARB"),overrideMaxExport,startFrames[iCount],endFrames[iCount]);
	   }
		
	   delete [] animationNames;
	   animationNames = 0;
	   delete [] startFrames;
	   startFrames = 0;
	   delete [] endFrames;
	   endFrames = 0;
	   delete [] totalFramesLoc;
	   totalFramesLoc = 0;

	   //export alt cfgs is available
		//GL_CommDlg.GL_SceneNodes = new CString[nTotalNodeCount];
       int altNumber = 2;
	   
       while(1)
	   {//EXPORT ALL ALTCFGs
          //clear selection (keep physique count)
          GL_CommDlg.selectionCount = 0;
          int traceNew = 0;
          CString curSearch = "ALT" + CABLIB.ConvToString(altNumber) + "_";
		  int minSearchLength = curSearch.GetLength();
		  int truncate = curSearch.GetLength();
          for(int altLoop=0;altLoop<nTotalNodeCount;altLoop++)
		  {//object loop
             CString temp = GL_CommDlg.GL_SceneNodes[altLoop];
             int maxRead = temp.GetLength();
			 if(temp.GetLength() < minSearchLength)
			    continue;

			 if(temp.GetAt(0) == 'A')
			    if(temp.GetAt(1) == 'L')
				   if(temp.GetAt(2) == 'T')
					  {
					     CString numberCmp = temp.GetAt(3);
					     for(int i=4;i<maxRead;i++)
						 {
							 if(temp.GetAt(i) == '_')
							    break;
                             numberCmp += temp.GetAt(i);
						 }
						 if(atoi(numberCmp) == altNumber)
						 {
                            GL_CommDlg.selectionBuffer[traceNew] = altLoop;
                            traceNew++;
						    GL_CommDlg.selectionCount++;
						 }
					  }
					     


			 /*char last = temp.GetAt(temp.GetLength()-1);
			 char last2 = temp.GetAt(temp.GetLength()-2);
			 char last3 = temp.GetAt(temp.GetLength()-3);

			 if(last == 'a' || last == 'b' || last == 'c' || last == 'd' || last == 'e')
			    if(last2 == 'T' && last3 == 'L')
				   continue;*/
			 
			 /*if(temp.GetLength() > truncate)
                temp.Delete(truncate,(temp.GetLength()-(truncate+1)));*/

			 /*if(temp.GetLength() < 7)
                continue;
             if(altNumber > 9)
			    temp.Delete(6,(temp.GetLength()-6));
			 else
			    temp.Delete(5,(temp.GetLength()-5));

		     if(curSearch == temp)
			 {
                GL_CommDlg.selectionBuffer[traceNew] = altLoop;
                traceNew++;
			 }*/
		  }//end object loop
		  //EXPORT
		  if(traceNew > 0)
		  {
		     CString finalNameAlt = curSearch+".ALT";
             ExportSkeleton(finalNameAlt,TRUE,0,3);
		  }
		  else
			 break;
		  
		  altNumber++;
	   }//END EXPORT ALL ALTCFGs

	   altNumber = 1;
	   int lodCurrent = 1;
       while(1)
	   {//EXPORT ALL LODCFGs
          //clear selection (keep physique count)
          
          int traceNew = 0;
          CString curSearch = "ALT" + CABLIB.ConvToString(altNumber) + "_LOD" + CABLIB.ConvToString(lodCurrent) + "_";
          for(int altLoop=0;altLoop<nTotalNodeCount;altLoop++)
		  {//object loop
             CString temp = GL_CommDlg.GL_SceneNodes[altLoop];
             if(temp.GetLength() < 7)
                continue;
             if(altNumber > 9)
			    temp.Delete(6,(temp.GetLength()-6));
			 else
			    temp.Delete(5,(temp.GetLength()-5));

		     if(curSearch == temp)
			 {
                GL_CommDlg.selectionBuffer[traceNew] = altLoop;
                traceNew++;
			 }
		  }//end object loop
		  //EXPORT
		  if(traceNew > 0)
		  {//FOUND LOD
			  ExportSkeleton((curSearch+".LOD"),TRUE,0,3);
		  }//END FOUND ALT PREFIX
		  else
			 break;
		  
		  altNumber++;
	   }//END EXPORT ALL LODCFGs
	}//end bones/hiarchy

	if(GL_CommDlg.GL_outputMemoryType==0)
	{
	   m_autoLoader->BroadCastData();
       m_autoLoader->ClearBlock();
	}
	
	return TRUE;
}


void Exporter::WriteDebugFile(long debugString,BOOL clearIt,BOOL returnCase)
{
	if(!m_loggingType)
		return;

	    char buffer[67];
        if(debugString==0)
		{
		   buffer[0] = '0';
		   buffer[1] = 0;
		}
		else
		{
        _ltoa(debugString,buffer,10);
		}

		WriteDebugFile(buffer,clearIt,returnCase);
}
//-----------------------------------------------------------------//
void Exporter::WriteDebugFile(LPSTR debugString,BOOL clearIt,BOOL returnCase)
{
	if(!m_loggingType)
		return;

    int oldFile = 0;
	if(clearIt)	
	   oldFile = open( m_logFileName.GetBuffer(), _O_WRONLY | _O_TRUNC  | _O_CREAT  |_O_BINARY , _S_IWRITE);  
	else
	{
	   oldFile = open( m_logFileName.GetBuffer(), _O_WRONLY  | _O_CREAT  |_O_BINARY , _S_IWRITE);  
	   _lseek( oldFile, 0L, SEEK_END );


	}
	
	int trace = 0;
    while(debugString[trace++]!= 0)
	{
        //trace++;
	}
	long bytesRead = _write( oldFile, debugString, trace );
	if(returnCase)
	{
	 LPSTR returnCase = "\r\n";
     _write( oldFile, returnCase, 2 );
	}
 
	_close( oldFile );

    
}

BOOL Exporter::ExportSkeleton(CString filename,BOOL override,int startFrame, int endFrame)
{
	   m_skeletalExport = new CSkeletonObject();
	   m_skeletalExport->m_bonesList = new CBoneList();
	   /*//preprocess all multisubs to simplified rolled out nodes===//
			 //Current Issue this changes the enviorment when exported-this is bad
			 GenBoxObject *objTemp = (GenBoxObject *)ip->CreateInstance(GEOMOBJECT_CLASS_ID,Class_ID(BOXOBJ_CLASS_ID,0)); 
             objTemp->SetParams(4,4,4,1,1,1,FALSE);
             INode* createdNode = ip->CreateObjectNode( objTemp );//Object *obj )
			 createdNode->SetName("TEST M");
	   //end pre process*/
	
	   GL_physiqueModCount = 0;
       int numChildren = ip->GetRootNode()->NumberOfChildren();
       for (int idx=0; idx<numChildren; idx++) {
		    GetPhysiqueCount(ip->GetRootNode()->GetChildNode(idx));
	   }
       GL_currentPhyModIndex = 0;
	   

	   frameCount=0;
	         //PreProcess(ip->GetRootNode(), nTotalNodeCount);
             //Global info
			 ExportGlobalInfo(override,startFrame,endFrame);
	         
			 int dummyTrace=0;
	         GL_index = 0;

			 

			 

			 dummyTrace=0;
	         GL_index = 0;
	         for (int idx=0; idx<numChildren; idx++)//export bones and meshes
		          nodeEnum(ip->GetRootNode()->GetChildNode(idx), 0,dummyTrace);

			 dummyTrace=0;
	         GL_index = 0;
			 GL_currentPhyModIndex = 0;
	         for (int idx=0; idx<numChildren; idx++) 
		          nodeEnumVertexAssignments(ip->GetRootNode()->GetChildNode(idx), 0,dummyTrace);
            
			 int shadeTol = atoi(GL_CommDlg.GL_smShadeToleranceAngle);
			 int dmLoop=0;
              //optimize before output
			  //hiarchle bones
              if(m_skeletalExport->m_bonesList)
                for(POSITION posLoc = m_skeletalExport->m_bonesList->GetHeadPosition();posLoc != NULL;){
                    CBoneObject * frmPtr = (CBoneObject *)m_skeletalExport->m_bonesList->GetNext(posLoc);
                    
	                if(frmPtr->m_hiarchyMesh){
                       for(POSITION posLocB = frmPtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();posLocB != NULL;)
					   {
						   CHiarcleVisObj * hVis = ( CHiarcleVisObj *) frmPtr->m_hiarchyMesh->m_lodChain->GetNext(posLocB);
                           hVis->m_mesh->NormalizeOnly();
						   hVis->m_mesh->OptimizeMesh(NULL,3);
						   hVis->m_mesh->NormalizeOnly();
					   }
					}
				}
			  //deformable
			  if(GL_CommDlg.GL_generateNormals == TRUE)
			  {
			    for(dmLoop=0;dmLoop<m_skeletalExport->m_deformableMeshArrayCount;dmLoop++)
				{
					   m_skeletalExport->m_deformableMeshArrayV2[dmLoop].m_defMeshVisualUnit->m_lodChain->NormilizeOnly();
					   m_skeletalExport->m_deformableMeshArrayV2[dmLoop].m_defMeshVisualUnit->m_lodChain->Optimize(2);
					   m_skeletalExport->m_deformableMeshArrayV2[dmLoop].m_defMeshVisualUnit->m_lodChain->NormilizeOnly();
				}
			  }
			  else
			  {
                for(dmLoop=0;dmLoop<m_skeletalExport->m_deformableMeshArrayCount;dmLoop++)
				{
					m_skeletalExport->m_deformableMeshArrayV2[dmLoop].m_defMeshVisualUnit->m_lodChain->NormilizeOnly();
					m_skeletalExport->m_deformableMeshArrayV2[dmLoop].m_defMeshVisualUnit->m_lodChain->Optimize(2);
					m_skeletalExport->m_deformableMeshArrayV2[dmLoop].m_defMeshVisualUnit->m_lodChain->NormilizeOnly();

				}
			  }
			  
			  //inverse normal based on bones
			  if(!GL_CommDlg.GL_enableBlendedExport)//Rigid inversion
			  for(int i=0;i<m_skeletalExport->m_deformableMeshArrayCount;i++)
			  {//formation loop Rigid Inversion
                if(m_skeletalExport->m_deformableMeshArrayV2[i].m_defMeshVisualUnit){
                   CDeformableVisObj * deformPtr = (CDeformableVisObj*)m_skeletalExport->m_deformableMeshArrayV2[i].m_defMeshVisualUnit->m_lodChain->GetByIndex(0);
                    if(deformPtr){
				     ABVERTEX *tempVertexMass = NULL;
			         int tempVertexMassCount = 0;
			         deformPtr->m_mesh->m_abVertexArray.GetABVERTEXStyle(&tempVertexMass,&tempVertexMassCount);
				     for(int x=0;x<deformPtr->m_assignmentCount;x++)
					 {//assign loop
                        POSITION bPos = m_skeletalExport->m_bonesList->FindIndex(deformPtr->m_vertexAssignments[x].boneIndex);
					    if(bPos)
						{//valid bone
			              CBoneObject * bonePtr = (CBoneObject *) m_skeletalExport->m_bonesList->GetAt(bPos);
						  D3DMATRIX boneMatrix;
						  bonePtr->m_boneFrame->GetInverseTransform(&boneMatrix);
						  
						  float vTempX = (boneMatrix.m[0][0]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][0]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][0]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][0]);
                          float vTempY = (boneMatrix.m[0][1]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][1]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][1]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][1]);
                          float vTempZ = (boneMatrix.m[0][2]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][2]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][2]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][2]);

						  float nTempX = (boneMatrix.m[0][0]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].nx)+(boneMatrix.m[1][0]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].ny)+(boneMatrix.m[2][0]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].nz);
                          float nTempY = (boneMatrix.m[0][1]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].nx)+(boneMatrix.m[1][1]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].ny)+(boneMatrix.m[2][1]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].nz);
                          float nTempZ = (boneMatrix.m[0][2]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].nx)+(boneMatrix.m[1][2]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].ny)+(boneMatrix.m[2][2]*tempVertexMass[deformPtr->m_vertexAssignments[x].vertexIndex].nz);
					      int indexTemp = deformPtr->m_vertexAssignments[x].vertexIndex;
						  deformPtr->m_mesh->m_abVertexArray.SetPositionXYZNXNYNZ(indexTemp,vTempX,vTempY,vTempZ,nTempX,nTempY,nTempZ);
						  
						   for(int ownedLoop=0;ownedLoop<deformPtr->m_vertexAssignments[x].carriedCount;ownedLoop++)
						   {//carry
						     indexTemp = deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop];
							 deformPtr->m_mesh->m_abVertexArray.SetPositionXYZNXNYNZ(indexTemp,vTempX,vTempY,vTempZ,nTempX,nTempY,nTempZ);
						   }//end carry
						}//end valid bone
					 }//end assign loop
					 if(tempVertexMass)
					 {
                        delete [] tempVertexMass;
                        tempVertexMass = 0;
					 }
				   }
				}
			  }//end formation loop Rigid Inversion
			  if(GL_CommDlg.GL_enableBlendedExport)//blended inversion
			  for(int i=0;i<m_skeletalExport->m_deformableMeshArrayCount;i++)
			  {//formation loop blended Inversion
			    D3DVECTOR tempArray[10];
				float     tempWeightsArray[10];
                if(m_skeletalExport->m_deformableMeshArrayV2[i].m_defMeshVisualUnit){
                   CDeformableVisObj * deformPtr = (CDeformableVisObj*)m_skeletalExport->m_deformableMeshArrayV2[i].m_defMeshVisualUnit->m_lodChain->GetByIndex(0);
                    if(deformPtr){
				     ABVERTEX *tempVertexMass = NULL;
					 D3DVECTOR finalPosition,normalPosition;
			         int tempVertexMassCount = 0;
			         deformPtr->m_mesh->m_abVertexArray.GetABVERTEXStyle(&tempVertexMass,&tempVertexMassCount);
				     for(int x=0;x<deformPtr->m_assignmentCount;x++)
					 {//assign loop
                        for(int calculateLoop = 0;calculateLoop<deformPtr->m_blendedVertexAssignments[x].boneIndexesCount;calculateLoop++)
	                    {
         
                            CBoneObject * bonePtr = m_skeletalExport->m_bonesList->GetBoneByIndex(deformPtr->m_blendedVertexAssignments[x].boneIndexes[calculateLoop]);
	                        if(bonePtr)
		                    {
		                       D3DMATRIX boneMatrix;
                               bonePtr->m_boneFrame->GetInverseTransform(&boneMatrix);
			                   deformPtr->m_blendedVertexAssignments[x].offsetVects[calculateLoop].x = (boneMatrix.m[0][0]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][0]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][0]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][0]);
                               deformPtr->m_blendedVertexAssignments[x].offsetVects[calculateLoop].y = (boneMatrix.m[0][1]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][1]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][1]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][1]);
                               deformPtr->m_blendedVertexAssignments[x].offsetVects[calculateLoop].z = (boneMatrix.m[0][2]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][2]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][2]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][2]);
			                   deformPtr->m_blendedVertexAssignments[x].offsetVects[calculateLoop].nx = (boneMatrix.m[0][0]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].nx)+(boneMatrix.m[1][0]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].ny)+(boneMatrix.m[2][0]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].nz);
                               deformPtr->m_blendedVertexAssignments[x].offsetVects[calculateLoop].ny = (boneMatrix.m[0][1]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].nx)+(boneMatrix.m[1][1]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].ny)+(boneMatrix.m[2][1]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].nz);
                               deformPtr->m_blendedVertexAssignments[x].offsetVects[calculateLoop].nz = (boneMatrix.m[0][2]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].nx)+(boneMatrix.m[1][2]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].ny)+(boneMatrix.m[2][2]*tempVertexMass[deformPtr->m_blendedVertexAssignments[x].vertexIndex].nz);

		                    }
	                   }



                       
					 }//end assign loop
					 if(tempVertexMass)
					 {
                        delete [] tempVertexMass;
                        tempVertexMass = 0;
					 }
				   }
				}
			  }//end formation loop blended Inversion
             
			 //EXPORT serialized
			 CFileException exc;
             CFile fl;
             BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
             CArchive the_out_Archive(&fl,CArchive::store);

			 m_skeletalExport->m_lastFrame = m_skeletalExport->m_lastFrame - m_skeletalExport->m_firstFrame;
			 m_skeletalExport->m_firstFrame = 0;
	         //the_out_Archive << m_skeletalExport;
	         the_out_Archive << m_skeletalExport->m_SkeletalName
	                         << m_skeletalExport->m_frameSpeed
	                         << m_skeletalExport->m_ticksPerFrame
                             << m_skeletalExport->m_bonesList
	                         << m_skeletalExport->m_deformableMeshArrayCount
                             << m_skeletalExport->m_deformableMeshArrayCount//m_physiqueModCount
	                         << m_skeletalExport->m_firstFrame
                             << m_skeletalExport->m_lastFrame
	                         << m_skeletalExport->m_frameCount;


                //export individual hiarch
			    int tracerBone = 0;
                for(POSITION bPos = m_skeletalExport->m_bonesList->GetHeadPosition();bPos != NULL;)
				{
				    CBoneObject * boneLoc = (CBoneObject *)m_skeletalExport->m_bonesList->GetNext(bPos);
					if(boneLoc->m_hiarchyMesh){
					    CString append = "dMesh_";
                        CString append2 = m_skeletalExport->m_SkeletalName;
                        CABLIB.RemoveFileExt(&append2);
					      append = append + append2;
					      append = append+"_";
					      append = append+CABLIB.ConvToString(tracerBone);
					      append = append+".him";
						  boneLoc->m_hiarchyMesh->SaveLevel(0,append);
						  tracerBone++;

					}
				}

	   
		        for(int loop=0;loop<m_skeletalExport->m_deformableMeshArrayCount;loop++)
				{
                    the_out_Archive << m_skeletalExport->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
				    
					//export deformable
					CString append = "";
                    CString append2 = m_skeletalExport->m_SkeletalName;
                    CABLIB.RemoveFileExt(&append2);
					append = append+append2;
					append = append+"_";
					append = append+CABLIB.ConvToString(loop);
					append = append+".dim";
					m_skeletalExport->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->SaveLevel(0,append);
					//end export deformable
				}    
			 
 
			 the_out_Archive.Close();
			 fl.Close(); 
			 m_skeletalExport->SafeDelete();
			 delete m_skeletalExport;
			 m_skeletalExport=0;

return TRUE;
}


// Return an indentation string
TSTR Exporter::GetIndent(int indentLevel)
{
	TSTR indentString = "";
	for (int i=0; i<indentLevel; i++) {
		indentString += "\t";
	}
	
	return indentString;
}

// Replace some characters we don't care for.
TCHAR* Exporter::FixupName(TCHAR* name)
{
	static char buffer[256];
	TCHAR* cPtr;
	
    _tcscpy(buffer, name);
    cPtr = buffer;
	
    while(*cPtr) {
		if (*cPtr == '"')
			*cPtr = SINGLE_QUOTE;
        else if (*cPtr <= CTL_CHARS)
			*cPtr = _T('_');
        cPtr++;
    }
	
	return buffer;
}

TCHAR* Exporter::FixupName2(TCHAR* name)
{
	static char buffer[256];
	TCHAR* cPtr;
	
    _tcscpy(buffer, name);
    cPtr = buffer;
	
    while(*cPtr) {
        cPtr++;
    }
	
	return buffer;
}



void Exporter::ExportNodeTM(INode* node, 
							int indentLevel,
							BOOL * physiqueExists,
							BOOL * skinExists)
{     
	TSTR indent = GetIndent(indentLevel);
	node->EvalWorldState(0);
	//evaluate phymod
	Modifier* phyMod = FindPhysiqueModifier (node);	
	if(phyMod != NULL){//physique exists  
	   *physiqueExists = TRUE;
       return;
	}
	else{
       *physiqueExists = FALSE;
	} 
    //evaluate skin mod
    Modifier* skinMod = FindSkinModifier (node);
    if(skinMod != NULL){//physique exists  
	   *skinExists = TRUE;
       return;
	}
	else{
       *skinExists = FALSE;
	} 


	Matrix3 pivot = node->GetNodeTM(0);
	// Export TM inheritance flags
	DWORD iFlags = node->GetTMController()->GetInheritanceFlags();
	// Dump the full matrix
	CString name = FixupName(node->GetName());
    if(name.CompareNoCase("nub")==0)
	   return;

	if(name.GetLength() > 13)
	{
		CString partCheck = name.Mid(0,13);
		if(partCheck.CompareNoCase("Particle View")==0)
		   return;
	}

	//check for particle view and skip it

       

    

	/*if(VerifyObjectForMeshExport(boneNode->m_boneName))
	{//mesh will export so we have to detect possible multi sub for additiona nodes
       CIDList * idList = NULL;
	   GetMeshIDList(&idList,mesh);
	   if(idList)
	      if(idList->GetCount() > 1)
		  {//confirmed mulisub
		     BOOL needDel;
             TriObject* tri = GetTriObjectFromNode(node,0, needDel);
	         if(tri)
			 {
			    Mesh* mesh = &tri->GetMesh();
				if(mesh)
				{
			      
			 
				}
			 }
		  }//end confirmed mulisub
	}//end possible multi sub*/

    int exportBoneCount = 1;//

	BOOL needDel;
    TriObject* tri = GetTriObjectFromNode(node,0, needDel);
	 if(tri)
	 {
	    Mesh* mesh = &tri->GetMesh();
		if(mesh)
		{
	      CIDList * idList = NULL;
	      GetMeshIDList(&idList,mesh);
	      if(idList)
		     if(idList->GetCount() > exportBoneCount)
			    exportBoneCount = idList->GetCount();
		}
	 }

    for(int ibc=0;ibc<exportBoneCount;ibc++)
	{//bone addition loop-handle sub object modifier break down requirements
		CBoneObject * boneNode = new CBoneObject();
		boneNode->m_boneName = FixupName(node->GetName());
		if(ibc > 0)//add extension
			boneNode->m_boneName += "_MSub:"+CABLIB.ConvToString(ibc+1);
		//parent info
		INode* parent = node->GetParentNode();
		boneNode->m_parentName = "NONE";
		if (parent && !parent->IsRootNode())
			boneNode->m_parentName = FixupName(parent->GetName());

		m_skeletalExport->m_bonesList->AddTail(boneNode);
		boneNode->m_animationList = new CBoneAnimationList;
		CBoneAnimationObject *animObject = new CBoneAnimationObject();
		animObject->m_totalFrames          = m_skeletalExport->m_lastFrame - m_skeletalExport->m_firstFrame;
		animObject->m_millisecondsPerFrame = m_skeletalExport->m_ticksPerFrame;
	    		
		//test animation export
		int startFrame = m_skeletalExport->m_firstFrame;
		int endFrame = m_skeletalExport->m_lastFrame;
		int totalFrames = animObject->m_totalFrames;
		
		 m_skeletalExport->m_firstFrame = startFrame;
		 m_skeletalExport->m_lastFrame = endFrame;
		 animObject->m_totalFrames = totalFrames;

		
		animObject->m_animationData        = new AnimationContent[frameCount+1];
		//get all animation data
		D3DVECTOR upVectTemp,dirVectTemp;
		float visibilityLoc = 1.0f;
		
	    

		int tracer = 0;

		for(int loop=m_skeletalExport->m_firstFrame;loop<(m_skeletalExport->m_lastFrame+1);loop++){
			node->EvalWorldState((loop*GetTicksPerFrame()));
			pivot = node->GetNodeTM((loop*GetTicksPerFrame()));
			D3DMATRIX currentMatrix = GetMatrixFromMaxStruct(&pivot);

			GL_matrixFunctions.GetOrientation(currentMatrix,
											  &dirVectTemp,
											  &upVectTemp);

			visibilityLoc = node->GetVisibility((loop*GetTicksPerFrame()));

			//animObject->m_animationData[loop].dirVect = dirVectTemp;
			//animObject->m_animationData[loop].upVect = upVectTemp;
			dirVectTemp = GL_matrixFunctions.Normalize(dirVectTemp);
			upVectTemp = GL_matrixFunctions.Normalize(upVectTemp);
			animObject->m_animationData[tracer].dirVect = dirVectTemp;
			animObject->m_animationData[tracer].upVect = upVectTemp;
			animObject->m_animationData[tracer].position.x = currentMatrix._41;
			animObject->m_animationData[tracer].position.y = currentMatrix._42;
			animObject->m_animationData[tracer].position.z = currentMatrix._43;
			animObject->m_animationData[tracer].visibility = visibilityLoc;

			if(tracer == 0){
			   if(!boneNode->m_boneFrame)
				   boneNode->m_boneFrame = new CFrameObj(NULL);
				   boneNode->m_boneFrame->SetOrientation(dirVectTemp,
														 upVectTemp);
				   boneNode->m_boneFrame->SetPosition(NULL,animObject->m_animationData[tracer].position);
				   boneNode->m_boneFrame->GetTransform(NULL,&boneNode->m_startPosition);
			   //boneNode->m_boneFrame->AddTransformReplace(currentMatrix);
			   boneNode->m_boneFrame->GetTransform(NULL,&boneNode->m_startPosition);
			}
			tracer++;
		}
		//add animation object
		boneNode->m_animationList->AddTail(animObject);
	}//end bone addition loop-handle sub object modifier break down requirements
}
//find skin modifier
Modifier* Exporter::FindSkinModifier (INode* node)
{
	// Get object from node. Abort if no object.
	Object* pObj = node->GetObjectRef();
	

	if (!pObj) return NULL;

	// Is derived object ?
	while (pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		// Yes -> Cast.
		IDerivedObject* pDerObj = static_cast<IDerivedObject*>(pObj);

		// Iterate over all entries of the modifier stack.
		int ModStackIndex = 0;
		while (ModStackIndex < pDerObj->NumModifiers())
		{
			// Get current modifier.
			Modifier* mod = pDerObj->GetModifier(ModStackIndex);

			// Is this Skin ?
			if (mod->ClassID() == SKIN_CLASSID )
			{
				// Yes -> Exit.
				return mod;
			}

			// Next modifier stack entry.
			ModStackIndex++;
		}
		pObj = pDerObj->GetObjRef();
	}

	// Not found.
	return NULL;
}
//---------------------------------------------------------------//
//--Detachment method based on ID doesnt interfere with scene----//
//---------------------------------------------------------------//
BitArray * Exporter::GetMeshGeneratedByID(int id,
									Mesh * mesh,
									Mesh ** meshOutput)
{
//detach test
    
	MeshDelta * meshDelta = new MeshDelta(*mesh);
    BitArray * bitArrayLoc = new BitArray(mesh->getNumFaces());
	bitArrayLoc->ClearAll();
	//int size
    int targetID = id;
	int detachCount = 0;
	for(int loop=0;loop<mesh->getNumFaces();loop++){
        if(targetID == mesh->faces[loop].getMatID()){
		   bitArrayLoc->Set(loop);
           detachCount++;
		}
	}
	if(detachCount > 0){
    //AfxMessageBox("Detaching");
    Mesh* newMesh = new Mesh();
	meshDelta->Detach(*mesh,newMesh,*bitArrayLoc,TRUE,TRUE,FALSE);
	*meshOutput = newMesh;
	}
	delete meshDelta;
	meshDelta=0;
	return bitArrayLoc;
	
}
//---------------------------------------------------------------//
//--Detachment method based on ID doesnt interfere with scene----//
//---------------------------------------------------------------//
void Exporter::GetMeshIDList(CIDList ** idList,
							 Mesh * mesh)
{
//detach test
    CIDList * idLocalList = new CIDList();
	MeshDelta * meshDelta = new MeshDelta(*mesh);
	//int size
	for(int loop=0;loop<mesh->getNumFaces();loop++){
        int id = mesh->faces[loop].getMatID();
		BOOL foundOne = FALSE;
		
		for(POSITION posLoc = idLocalList->GetHeadPosition();posLoc != NULL;){
			CIDObj* idObj = (CIDObj*)idLocalList->GetNext(posLoc);
            if(idObj->m_id == id){
			   foundOne = TRUE;
			   break;
			}
		}
		if(foundOne == FALSE){
		   CIDObj* newIdObj = new CIDObj();
		   newIdObj->m_id = id;
		   idLocalList->AddTail(newIdObj);
		}

	}
	*idList = idLocalList;
	delete meshDelta;
	meshDelta=0;
}


D3DMATRIX Exporter::GetMatrixFromMaxStruct(Matrix3* m)
{
	D3DMATRIX matrixCreation;

    GL_matrixFunctions.InitMat(matrixCreation.m);
	Point3 row;
	row = m->GetRow(0);
	matrixCreation._11 = row.x;
	matrixCreation._12 = row.z;
	matrixCreation._13 = row.y;
	row = m->GetRow(1);
	matrixCreation._21 = row.x;
	matrixCreation._22 = row.z;
	matrixCreation._23 = row.y;
	row = m->GetRow(2);
	matrixCreation._31 = row.x;
	matrixCreation._32 = row.z;
	matrixCreation._33 = row.y;
	row = m->GetRow(3);
	matrixCreation._41 = row.x;
	matrixCreation._42 = row.z;
	matrixCreation._43 = row.y;
		
    return matrixCreation;
}

// Dump some global animation information.
void Exporter::ExportGlobalInfo(BOOL override,int startFrame, int endFrame)
{
	Interval range = ip->GetAnimRange();
	// Start with a file identifier and format version
	frameCount = ((range.End()-range.Start()) / GetTicksPerFrame());
	

    m_skeletalExport->m_SkeletalName     = FixupName(ip->GetCurFileName());
	m_skeletalExport->m_frameSpeed       = GetFrameRate();
	m_skeletalExport->m_ticksPerFrame    = (GetTicksPerFrame()/GL_CommDlg.GL_animSpeedAdjust);
    //m_skeletalExport->m_physiqueModCount = GL_physiqueModCount;//defines how many def meshes
	m_skeletalExport->m_firstFrame       = range.Start() / GetTicksPerFrame();
	m_skeletalExport->m_lastFrame        = range.End() / GetTicksPerFrame();

	if(override)
	{
       frameCount = endFrame-startFrame;
       m_skeletalExport->m_firstFrame       = startFrame;
	   m_skeletalExport->m_lastFrame        = endFrame;
	}

	m_skeletalExport->m_deformableMeshArrayV2 = NULL;
	if(GL_physiqueModCount > 0)
	{
    m_skeletalExport->m_deformableMeshArrayV2 = new DeformableMeshStructV2[GL_physiqueModCount];
    ZeroMemory(m_skeletalExport->m_deformableMeshArrayV2,sizeof(DeformableMeshStructV2)*GL_physiqueModCount);

	}
	m_skeletalExport->m_deformableMeshArrayCount = GL_physiqueModCount;



	//AfxMessageBox(operator+("Phy Count",CABLIB.ConvToString(GL_physiqueModCount)));

}


void Exporter::PreProcess(INode* node, int& nodeCount)
{
	nodeCount++;
	
	// Add the nodes material to out material list
	// Null entries are ignored when added...
	//mtlList.AddMtl(node->GetMtl());

	// For each child of this node, we recurse into ourselves 
	// and increment the counter until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		PreProcess(node->GetChildNode(c), nodeCount);
	}
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
BOOL Exporter::nodeEnumAiTreeExport(INode* node,int tracer) 
{
	ObjectState os = node->EvalWorldState(0); 
	TSTR buf;
	node->GetUserPropBuffer(buf);
	CString buffer = buf;
	CString *arrayStrings = NULL;
	int      stringCount  = 1;
	//parse string
	int stringLength = buffer.GetLength();
	int strLoop=0;
	for(strLoop=0;strLoop<stringLength;strLoop++){//get length
	    CString tempChar = buffer.GetAt(strLoop);
		if(tempChar == "\n"){
           stringCount++;
		}
	}//end get length
	//AfxMessageBox(CABLIB.ConvToString(stringCount));
    arrayStrings = new CString[stringCount];
	int traceStore = 0;
	for(strLoop=0;strLoop<stringLength;strLoop++){
	    CString tempChar = buffer.GetAt(strLoop);
		if(tempChar == "\n"){
		   CString cutString = buffer;
           cutString.Delete(strLoop,(stringLength-strLoop));
		   arrayStrings[traceStore] = cutString;
		   traceStore++;
		   buffer.Delete(0,strLoop+1);
		   stringLength = buffer.GetLength();
		   strLoop = 0;
		   //AfxMessageBox(cutString);
		}
	}
	arrayStrings[traceStore] = buffer;
	//----------------------------------------//
	//---------filter non AI paths------------//
    //----------------------------------------//
	CString que = arrayStrings[0];
	que.TrimRight();
	que.TrimLeft();
	que.MakeUpper();
	CString radius = que;
	int fmt = que.GetLength();
	if(fmt > 2)
	que.Delete(2,(fmt-2));

    
	fmt = radius.GetLength();
	if(fmt > 2)
	   radius.Delete(0,2);
    float finalRadius = atof(radius);

	if(que == "AI")
	{
	CAIWebNode * webNode = new CAIWebNode();
	webNode->m_locationName = node->GetName();//location name
	webNode->m_locationName.TrimRight();
	webNode->m_locationName.TrimLeft();
	webNode->m_locationName.MakeUpper();
	//AfxMessageBox(webNode->m_locationName);
	Matrix3 pivot = node->GetNodeTM(0);
	//AfxMessageBox(webNode->m_locationName);
	webNode->m_radius = finalRadius;
	D3DMATRIX currentMatrix = GetMatrixFromMaxStruct(&pivot);
	webNode->m_position.x = currentMatrix._41;//position in world space
	webNode->m_position.y = currentMatrix._42;//position in world space
	webNode->m_position.z = currentMatrix._43;//position in world space
	m_webExport->m_web->AddTail(webNode);
	GL_index++;
	} 
	//----------------------------------------//
    
	if(arrayStrings){
       delete [] arrayStrings;
	   arrayStrings=0;
	}
	// For each child of this node, we recurse into ourselves 
	// until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!nodeEnumAiTreeExport(node->GetChildNode(c),tracer))
			return FALSE;
	}
	return TRUE;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
BOOL Exporter::nodeEnumAiTreeLinksExport(INode* node,int tracer) 
{
	ObjectState os = node->EvalWorldState(0); 
	TSTR buf;
	node->GetUserPropBuffer(buf);
	CString buffer = buf;
	CString *arrayStrings = NULL;
	int      stringCount  = 1;
	//parse string
	int stringLength = buffer.GetLength();
	int strLoop=0;
	for(strLoop=0;strLoop<stringLength;strLoop++){//get length
	    CString tempChar = buffer.GetAt(strLoop);
		if(tempChar == "\n"){
           stringCount++;
		}
	}//end get length
	//AfxMessageBox(CABLIB.ConvToString(stringCount));
    arrayStrings = new CString[stringCount];
	int traceStore = 0;
	for(strLoop=0;strLoop<stringLength;strLoop++){
	    CString tempChar = buffer.GetAt(strLoop);
		if(tempChar == "\n"){
		   CString cutString = buffer;
           cutString.Delete(strLoop,(stringLength-strLoop));
		   cutString.TrimRight();
	       cutString.TrimLeft();
	       cutString.MakeUpper();
		   arrayStrings[traceStore] = cutString;
		   traceStore++;
		   buffer.Delete(0,strLoop+1);
		   stringLength = buffer.GetLength();
		   strLoop = 0;
		   //AfxMessageBox(cutString);
		}
	}
	buffer.TrimRight();
    buffer.TrimLeft();
    buffer.MakeUpper();
	arrayStrings[traceStore] = buffer;
	//AfxMessageBox(buffer);
	//end parse string
	CString que = arrayStrings[0];
	que.TrimRight();
	que.TrimLeft();
	que.MakeUpper();
	
	int fmt = que.GetLength();
	if(fmt > 2)
	que.Delete(2,(fmt-2));

	if("AI" == que){//valid Ai node
	POSITION posLoc = m_webExport->m_web->FindIndex(GL_index);
	 if(posLoc){//valid
	 CAIWebNode * wNode = (CAIWebNode *)m_webExport->m_web->GetAt(posLoc);
	  
	  for(int loop=0;loop<stringCount;loop++){//1
	  int trace = 0;
	   for(POSITION assPos = m_webExport->m_web->GetHeadPosition();assPos != NULL;)
	   {
		CAIWebNode * cNode = (CAIWebNode *)m_webExport->m_web->GetNext(assPos);
		if(cNode->m_locationName == arrayStrings[loop]){
		   wNode->AddDirectLink(trace);
		   //AfxMessageBox("Assigned");
		}
	    trace++;
	   }
	  }//end 1
	 }//end valid
	GL_index++;
	}//end valid AI node

    if(arrayStrings){
       delete [] arrayStrings;
	   arrayStrings=0;
	}

	else{
       AfxMessageBox("error 1134");
	}
	//AfxMessageBox(test);
    
	// For each child of this node, we recurse into ourselves 
	// until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!nodeEnumAiTreeExport(node->GetChildNode(c),tracer))
			return FALSE;
	}
	return TRUE;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
BOOL Exporter::nodeEnumVertexAssignments(INode* node, int indentLevel,int tracer) 
{
	
	TSTR indent = GetIndent(indentLevel);
	if (node->IsGroupHead()) {
		indentLevel++;
	}
		ObjectState os = node->EvalWorldState(0); 
		// The obj member of ObjectState is the actual object we will export.
		if (os.obj) {
			
			LPSTR nodeName = FixupName(node->GetName());
			for(int loop=0;loop<GL_CommDlg.selectionCount;loop++){//check for mesh output status
				//int result = strnicmp(nodeName,GL_CommDlg.GL_SceneNodes[GL_CommDlg.selectionBuffer[loop]].stringBuild,256);
                if(nodeName==GL_CommDlg.GL_SceneNodes[GL_CommDlg.selectionBuffer[loop]]){//exists*/
				   //AfxMessageBox("export mesh");
			       //if(physiqueExists)
			       LogVertexAssignments(node,2);
				   //AfxMessageBox("end export mesh");
				   break;
				}
			}//end check for mesh output status
			
			
			
		}
    GL_index++;
	// For each child of this node, we recurse into ourselves 
	// until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!nodeEnumVertexAssignments(node->GetChildNode(c), indentLevel,tracer))
			return FALSE;
	}
	return TRUE;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
BOOL Exporter::NodeEnumRefExporter(INode* node, int tracer,CReferenceImpExpList * referenceExporter) 
{
	
		ObjectState os = node->EvalWorldState(0); 
		// The obj member of ObjectState is the actual object we will export.
		if (os.obj) {
			
			LPSTR nodeName = FixupName(node->GetName());
			for(int loop=0;loop<GL_CommDlg.selectionCount;loop++){//check for mesh output status
                if(nodeName==GL_CommDlg.GL_SceneNodes[GL_CommDlg.selectionBuffer[loop]]){//exists
                   //do export here
                    Matrix3 pivot = node->GetNodeTM(0);
					D3DMATRIX matrixCreation;
				    GL_matrixFunctions.InitMat(matrixCreation.m);
					Point3 row;
					row = pivot.GetRow(0);
					matrixCreation._11 = row.x;
					matrixCreation._12 = row.y;
					matrixCreation._13 = row.z;
					row = pivot.GetRow(1);
					matrixCreation._21 = row.x;
					matrixCreation._22 = row.y;
					matrixCreation._23 = row.z;
					row = pivot.GetRow(2);
					matrixCreation._31 = row.x;
					matrixCreation._32 = row.y;
					matrixCreation._33 = row.z;
					row = pivot.GetRow(3);
					matrixCreation._41 = row.x;
					matrixCreation._42 = row.y;
					matrixCreation._43 = row.z;
					D3DXMATRIX tempX = matrixCreation;
					//Transpose
					//swap column 1 and 2
					matrixCreation._12 = tempX._13;
					matrixCreation._22 = tempX._23;
					matrixCreation._32 = tempX._33;
					matrixCreation._42 = tempX._43;
					matrixCreation._13 = tempX._12;
					matrixCreation._23 = tempX._22;
					matrixCreation._33 = tempX._32;
					matrixCreation._43 = tempX._42;
                    tempX = matrixCreation;
					//swap then row 1 and 2 for matrices
				    matrixCreation._21 = tempX._31;
					matrixCreation._22 = tempX._32;
					matrixCreation._23 = tempX._33;
					matrixCreation._24 = tempX._34;
					matrixCreation._31 = tempX._21;
					matrixCreation._32 = tempX._22;
					matrixCreation._33 = tempX._23;
					matrixCreation._34 = tempX._24;

                   referenceExporter->AddNewReference(GL_CommDlg.GL_miscString,matrixCreation);
                   //end do export here
				   break;
				}
			}//end check for mesh output status
		}
    GL_index++;
	// For each child of this node, we recurse into ourselves 
	// until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!NodeEnumRefExporter(node->GetChildNode(c),tracer,referenceExporter))
			return FALSE;
	}
	return TRUE;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
BOOL Exporter::nodeEnum(INode* node, int indentLevel,int tracer) 
{
	TSTR indent = GetIndent(indentLevel);
	if (node->IsGroupHead()) {
		indentLevel++;
	}
		ObjectState os = node->EvalWorldState(m_skeletalExport->m_firstFrame*GetTicksPerFrame()); 
		// The obj member of ObjectState is the actual object we will export.
		if (os.obj) 
		{
			BOOL physiqueExists = FALSE;
			BOOL skinExists = FALSE;
            ExportNodeTM(node,0,&physiqueExists,&skinExists);//exports nodes as bones on structure
			
			LPSTR nodeName = FixupName(node->GetName());
			for(int loop=0;loop<GL_CommDlg.selectionCount;loop++){//check for mesh output status
				//int result = strnicmp(nodeName,GL_CommDlg.GL_SceneNodes[GL_CommDlg.selectionBuffer[loop]].stringBuild,256);
                if(nodeName==GL_CommDlg.GL_SceneNodes[GL_CommDlg.selectionBuffer[loop]]){//selected for mesh export
				   //AfxMessageBox("export mesh");*/
			       //if(physiqueExists)
			          ExportMesh(node,(m_skeletalExport->m_firstFrame*GetTicksPerFrame()),1,physiqueExists,skinExists);
				   //AfxMessageBox("end export mesh");
				   break;
				}
			}//end check for mesh output status
		}
    GL_index++;
	// For each child of this node, we recurse into ourselves 
	// until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!nodeEnum(node->GetChildNode(c), indentLevel,tracer))
			return FALSE;
	}
	return TRUE;
}

//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
INode * Exporter::GetRootNodeByName(CString name) 
{

    int numChildren = ip->GetRootNode()->NumberOfChildren();
    for (int idx=0; idx<numChildren; idx++) 
	{
		INode* nodeX = ip->GetRootNode()->GetChildNode(idx);
		if(nodeX)
		{
		   LPSTR nodeName = FixupName(nodeX->GetName());
	       if(name.CompareNoCase(nodeName)==0)
		   {
              return nodeX;
		   }
		}
	}

	/*if(!nodeRefArray)
		return NULL;

	if(m_nodeRefCount < 1)
		return NULL;

	for (int idx=0; idx<m_nodeRefCount; idx++) 
	{
		if(nodeRefArray[idx].nodeRef)
		{
		   
		   LPSTR nodeName = FixupName(nodeRefArray[idx].nodeRef->GetName());
	       if(name.CompareNoCase(nodeName)==0)
		   {
              return nodeRefArray[idx].nodeRef;
		   }
		}
	}*/

    return NULL;
}
//--------------------------------------------------------------------//
void Exporter::GenRootNodeArray() 
{
    nodeRefArray = NULL;

    int numChildren = ip->GetRootNode()->NumberOfChildren();
	if(numChildren < 1)
	   return;
	  m_nodeRefCount = numChildren;
	nodeRefArray = new nodeReference[numChildren];
    for (int idx=0; idx<numChildren; idx++) 
	{
		INode* nodeX = ip->GetRootNode()->GetChildNode(idx);
		nodeRefArray[idx].nodeRef = NULL;
		if(nodeX)
		{
              nodeRefArray[idx].nodeRef = nodeX;
		}
	}
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//------------------node enum world Export----------------------------//
BOOL Exporter::SetABVERTEXCoordinateSet(int level,
										WORD vCount,
										ABVERTEX * vertexArray,
										ABVERTEX * textureSetArray)
{
	if(level == 0){//texture level 1
		for(int loop=0;loop<vCount;loop++){
            vertexArray[loop].tx1.tu = textureSetArray[loop].tx0.tu;
		    vertexArray[loop].tx1.tv = textureSetArray[loop].tx0.tv;
		}
	}//end texture level one
    if(level == 1){//texture level 2
		for(int loop=0;loop<vCount;loop++){
            vertexArray[loop].tx2.tu = textureSetArray[loop].tx0.tu;
		    vertexArray[loop].tx2.tv = textureSetArray[loop].tx0.tv;
		}
	}//end texture level one
	if(level == 2){//texture level 3
		for(int loop=0;loop<vCount;loop++){
            vertexArray[loop].tx3.tu = textureSetArray[loop].tx0.tu;
		    vertexArray[loop].tx3.tv = textureSetArray[loop].tx0.tv;
		}
	}//end texture level one
	if(level == 3){//texture level 4
		for(int loop=0;loop<vCount;loop++){
            vertexArray[loop].tx4.tu = textureSetArray[loop].tx0.tu;
		    vertexArray[loop].tx4.tv = textureSetArray[loop].tx0.tv;
		}
	}//end texture level one
	if(level == 4){//texture level 5
		for(int loop=0;loop<vCount;loop++){
            vertexArray[loop].tx5.tu = textureSetArray[loop].tx0.tu;
		    vertexArray[loop].tx5.tv = textureSetArray[loop].tx0.tv;
		}
	}//end texture level one
	if(level == 5){//texture level 6
		for(int loop=0;loop<vCount;loop++){
            vertexArray[loop].tx6.tu = textureSetArray[loop].tx0.tu;
		    vertexArray[loop].tx6.tv = textureSetArray[loop].tx0.tv;
		}
	}//end texture level one
    if(level == 6){//texture level 7
		for(int loop=0;loop<vCount;loop++){
            vertexArray[loop].tx7.tu = textureSetArray[loop].tx0.tu;
		    vertexArray[loop].tx7.tv = textureSetArray[loop].tx0.tv;
		}
	}//end texture level one
return TRUE;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//------------------node enum world Export----------------------------//
BOOL Exporter::nodeEnumWorldExport(INode* node,int tracer,TimeValue t,CString appendString) 
{
        //Interval range = ip->GetAnimRange();
	      //(range.Start() / GetTicksPerFrame())
        //ip->
	    if(!node)
		    return FALSE;
	

		ObjectState os = node->EvalWorldState(t); 
		if (os.obj) {
                      
            LPSTR nodeName = FixupName(node->GetName());
			INode * refNode = node;

			for(int loop=0;loop<GL_CommDlg.selectionCount;loop++){//check for mesh output status
				//int result = strnicmp(nodeName,GL_CommDlg.GL_SceneNodes[GL_CommDlg.selectionBuffer[loop]].stringBuild,256);
                //if(result==0){//exists
				if(nodeName == GL_CommDlg.GL_SceneNodes[GL_CommDlg.selectionBuffer[loop]])
				{
				   


				   nodeName = FixupName(node->GetName());
                   WriteDebugFile("nodeEnumWorldExport() mesh name:",0,0);
                   WriteDebugFile(nodeName,0,1);

                   //GetLightTypeFromNode(node);//export if light exists
				   TSTR bufX;
	                node->GetUserPropBuffer(bufX);
	                CString bufferX = bufX;
					D3DVECTOR boxMin,boxMax;
					boxMin.x = -GL_CommDlg.GL_sightRange;
                    boxMin.y = -GL_CommDlg.GL_sightRange;
					boxMin.z = -GL_CommDlg.GL_sightRange;

					boxMax.x = GL_CommDlg.GL_sightRange;
                    boxMax.y = GL_CommDlg.GL_sightRange;
					boxMax.z = GL_CommDlg.GL_sightRange;
					BOOL boundingBoxValid = FALSE;
					if(bufferX.GetLength() > 0)
					{//valid popout box
                        INode * nodePopout = GetRootNodeByName(bufferX);
						if(nodePopout)
						{//get box
                            ObjectState osX = nodePopout->EvalWorldState(t); 
		                    if (osX.obj) 
							{
                            Box3 boundingBox;
							Matrix3 tm = nodePopout->GetObjTMAfterWSM(t);
                            osX.obj->GetDeformBBox(t, boundingBox, &tm);


							boxMin.x = boundingBox.pmin.x;
							boxMin.y = boundingBox.pmin.z;
							boxMin.z = boundingBox.pmin.y;

							boxMax.x = boundingBox.pmax.x;
							boxMax.y = boundingBox.pmax.z;
							boxMax.z = boundingBox.pmax.y;
							boundingBoxValid = TRUE;
							}
						}//end get box
					}//end valid popout box


			       GetLightTypeFromNode(node,boundingBoxValid,&boxMin,&boxMax);//export if light exists

				   nodeName = FixupName(node->GetName());

				   os = node->EvalWorldState(t); 
			       BOOL needDel;
	               TriObject* tri = GetTriObjectFromNode(node,t, needDel);
	               if(tri){
				      Mesh* mesh = &tri->GetMesh();

					  int uvCountInUse = mesh->getNumMaps();
                      Matrix3 tm = node->GetObjTMAfterWSM(t);
			
					  CIDList * idList = NULL;
					  GetMeshIDList(&idList,mesh);

					  int indexAlpha = 0;
					  if(idList)
                      for(POSITION posLocal=idList->GetHeadPosition();posLocal!=NULL;)
					  {//idloop
						  CIDObj * id_obj = (CIDObj *)idList->GetNext(posLocal);
						  D3DMATERIAL9 * d3dMaterialRet = NULL;
						  CString currentTexture2 = "NONE";
						  CString currentTexture3 = "NONE";
						  CString currentTexture4 = "NONE";
						  CString currentTexture5 = "NONE";
						  CString currentTexture6 = "NONE";
						  CString currentTexture7 = "NONE";
						  CString currentTexture8 = "NONE";
						  CString currentTexture = GetMaterialFromID(node,
							                                         id_obj->m_id,
																	 &d3dMaterialRet,
																	 &currentTexture2,
																	 &currentTexture3,
																	 &currentTexture4,
																	 &currentTexture5);
						  
						  //check for DXmanager on baked materials
						  


                          CString generatedFilename = nodeName;
						  CString baseName = nodeName;
						  //generatedFilename = generatedFilename+indexAlpha;
						  if(indexAlpha > 0)
						     generatedFilename = generatedFilename+CABLIB.ConvToString(indexAlpha);
						  
						  
						  generatedFilename = generatedFilename+appendString;
					      generatedFilename = generatedFilename+".ARW";
						  indexAlpha++;
						  //CABLIB.ConvToString(indexAlpha);
						  Mesh* meshFocus = NULL;
						  BitArray * bitArray = GetMeshGeneratedByID(id_obj->m_id,
									            mesh,
									            &meshFocus);



						 
					  //Mesh output
                      //mesh
                   


					    ABVERTEX * vArray = NULL;
					    WORD *     iArray = NULL;
					    int        iCount = 0;
					    int        vCount = 0;
					    GetMeshData(meshFocus, 
						            &vArray,
						            &iArray,
					                &vCount,
						            &iCount,
                                    tm,
									FALSE);
						
                        //assignLightmap
						char indexer = 'a';
						CString tempStr = nodeName;
						CString LightmapMeshName = tempStr+"LT";
                        for(int ltLoop=0;ltLoop<7;ltLoop++){
							
							CString finalName = LightmapMeshName+indexer;
							INode* lightmapNode = ip->GetINodeByName(finalName);
                            if(!lightmapNode)
								break;
							
							BOOL needDel;
	                        TriObject* intrfaceTri = GetTriObjectFromNode(lightmapNode,0, needDel);
							CString currentLightTexture = GetMaterialFromID(lightmapNode,0,NULL,NULL,NULL,NULL,NULL);
                            if(ltLoop==0)currentTexture2 = currentLightTexture;
							if(ltLoop==1)currentTexture3 = currentLightTexture;
							if(ltLoop==2)currentTexture4 = currentLightTexture;
							if(ltLoop==3)currentTexture5 = currentLightTexture;
							if(ltLoop==4)currentTexture6 = currentLightTexture;
							if(ltLoop==5)currentTexture7 = currentLightTexture;
							if(ltLoop==6)currentTexture8 = currentLightTexture;


	                        if(tri){//valid
                               Mesh* lightMesh = &intrfaceTri->GetMesh();
	                           //lightMesh->buildNormals();
                               Matrix3 ltmatrix = lightmapNode->GetObjTMAfterWSM(t);
                               Mesh* meshLightmapFocus = new Mesh();
							   MeshDelta * meshDelta = new MeshDelta(*lightMesh);
						       meshDelta->Detach(*lightMesh,meshLightmapFocus,*bitArray,TRUE,TRUE,FALSE);      
							   
							   ABVERTEX * ltVertexArray = NULL;
					           WORD *     ltIndecieArray = NULL;
					           int        ltIndecieCount = 0;
					           int        lyVertexCount = 0;
					           GetMeshData(meshLightmapFocus, 
						                   &ltVertexArray,
						                   &ltIndecieArray,
					                       &lyVertexCount,
						                   &ltIndecieCount,
                                           ltmatrix,
										   FALSE);
                               if(lyVertexCount != vCount){
                                  AfxMessageBox((baseName+indexer)+"::Inconsistent vertex counts");
							   }
                               else{
							   SetABVERTEXCoordinateSet(ltLoop,
										                lyVertexCount,
										                vArray,
										                ltVertexArray);
							   }
                               //delete stuff
                               delete meshLightmapFocus;
							   meshLightmapFocus=0;
							   delete meshDelta;
							   meshDelta=0;
							}//end valid
							indexer++;
						}


						if(vArray){//valid
                           CMaterialObject* materialExport = new CMaterialObject();

						   //DPD
						   if(NodeHasShader(node))
						   {
								GraphicsWindow		*GW;
								ViewExp				*View;
								LPDIRECT3DDEVICE9	device;

								View = GetCOREInterface()->GetActiveViewport();
								if(View) {
									GW = View->getGW();
									if(GW) {
										ID3D9GraphicsWindow *D3DGW = (ID3D9GraphicsWindow *)GW->GetInterface(D3D9_GRAPHICS_WINDOW_INTERFACE_ID);
										if(D3DGW) {
											device = D3DGW->GetDevice();
										}
									}
								}
								if(device) {
									LPD3DXEFFECT effect;
									CString fxRetCode = "";
									CString textures[20];
                                    if(!(effect = SetupEffectFile(node, device, &fxRetCode)))
										break;
									if(!materialExport->InitPeramBlockFXShadersHSL(effect))
										break;
									if(!GetShaderInfo(materialExport, node, effect, fxRetCode))
										break;
									int texCount = SetShaderTextures(effect, node, materialExport, textures);
									currentTexture  = textures[0];
									currentTexture2 = textures[1];
									currentTexture3 = textures[2];
									currentTexture4 = textures[3];
									currentTexture5 = textures[4];
									currentTexture6 = textures[5];
									currentTexture7 = textures[6];
									currentTexture8 = textures[7];

                                    effect->Release();
									effect = 0;
								}
						   }
						   else if(d3dMaterialRet)
						   {						   
                               CopyMemory(&materialExport->m_useMaterial,d3dMaterialRet,sizeof(D3DMATERIAL9));
                               CopyMemory(&materialExport->m_baseMaterial,d3dMaterialRet,sizeof(D3DMATERIAL9));
						   }
						   //if(GL_CommDlg.GL_outputMemoryType == 1){//save to file
						   //check to see if bound to a popout box
						   
						 
                              
						   
						   
						 
						   SaveMesh(0,
							        GL_CommDlg.GL_sightRange,
							        generatedFilename,
						            vArray,
						            iArray,
						            vCount,
						            iCount,
									currentTexture,
									currentTexture2,
									currentTexture3,
									currentTexture4,
									currentTexture5,
									currentTexture6,
									currentTexture7,
									currentTexture8,
									"3",//GL_CommDlg.GL_smShadeToleranceAngle,//-2 =  special new normals should take care of it
									GL_CommDlg.GL_miscLoadInstruction,
									materialExport,
									boxMin,
									boxMax);


						     CString CurDir;
                             char buffer[_MAX_PATH];
	                         _getcwd( buffer, _MAX_PATH );
                             CurDir = buffer;
                             m_autoLoader->AppendStringSegToBlock(CurDir + "\\" + generatedFilename,GL_CommDlg.GL_miscLoadInstruction);

						   //}//end save to file
                           /*if(GL_CommDlg.GL_outputMemoryType == 0){//send direct
							   MemBuildBlock
								   (
								   GL_CommDlg.GL_smoothShading,
							       GL_CommDlg.GL_sightRange,
							       generatedFilename,
								   vArray,
					               iArray,
					               vCount,
					               iCount,
					               currentTexture,
								   currentTexture2,
								   currentTexture3,
								   currentTexture4,
								   currentTexture5,
								   currentTexture6,
								   currentTexture7,
								   currentTexture8);
						   }//end send direct*/
						}//end valid
						
					  }//end idloop
					  if(idList)
						 delete idList;
					     idList=0;
				   }
				   else{//selected object failed

				   }//end selected object failed
				   break;
				}
			}//end check for mesh output status
		}
    GL_index++;
	// For each child of this node, we recurse into ourselves 
	// until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!nodeEnumWorldExport(node->GetChildNode(c),tracer,t,appendString))
			return FALSE;
	}
	return TRUE;
}

/****************************************************************************

  Send mesh through memory to Aura Engine
  
****************************************************************************/



void Exporter::MemSend()
{
	UINT linkMsg = RegisterWindowMessage("--landmesh export from 3D Studio Max to Aura--");
	
	HANDLE hMapFile = NULL;

	hMapFile = CreateFileMapping
		(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		m_memSize+sizeof(DWORD)*2,
		"3DStudioMaxToAura-Transfer"
		);

	if(hMapFile==NULL)
		{
		MessageBox(GetForegroundWindow(),"CreateFileMapping() failed!", "::memSend()",MB_OK);
		return;
		}
	
	LPVOID data = NULL;
	data = MapViewOfFile(hMapFile,FILE_MAP_WRITE,0,0,0);

	if(data==NULL) 	
		{
		MessageBox(GetForegroundWindow(),"MapViewOfFile() failed!", "::memSend()",MB_OK);
		return;
		}

	BYTE *dataByte = (BYTE*)data;

	CopyMemory(&dataByte[0], &m_numMeshes, sizeof(DWORD));
	CopyMemory(&dataByte[sizeof(DWORD)], &m_memSize, sizeof(DWORD));
	CopyMemory(&dataByte[sizeof(DWORD)*2], m_memBlock, m_memSize);
	
	DWORD wParam=0, lParam=0;//wParam indicates transfer type
	::PostMessage( HWND_BROADCAST, linkMsg, wParam, lParam );

	HANDLE auraDone = NULL;
	auraDone = CreateSemaphore( NULL, 0, 1, "-AuraTransferDone-");
	
	if(WaitForSingleObject(auraDone,30000)==WAIT_TIMEOUT)
		MessageBox(GetForegroundWindow(),"ERROR: transfer timed out","::memSend()",MB_OK);
			
	CloseHandle(auraDone);


	UnmapViewOfFile(data);
	CloseHandle(hMapFile);
}

/****************************************************************************

  Builds a block of memory to send with MemSend()
  
****************************************************************************/
void Exporter::MemBuildBlock
	(
	BOOL smoothShading,
	float sightRange,
	CString fileName,
	ABVERTEX * vArray,
	WORD * iArray,
	WORD vCount,
	WORD iCount,
	CString texture1,
	CString texture2,
	CString texture3,
	CString texture4,
	CString texture5,
	CString texture6,
	CString texture7,
	CString texture8
	)
{
	int sizeABVERTEX = sizeof(float)*(3 + 3 + 2*8);
	
	DWORD blockSize = 
		sizeABVERTEX*vCount + 
		sizeof(WORD) +
		sizeof(WORD)*iCount +
		sizeof(WORD) +
		sizeof(float) +
		sizeof(BOOL) +
		sizeof(char)*256*9;

	 //allocate memory for new block and copy over all old data
	BYTE *tmpPtr = NULL;
	tmpPtr = new BYTE[m_memSize+blockSize];
	CopyMemory(tmpPtr, m_memBlock, m_memSize);
	delete [] m_memBlock;
	m_memBlock = tmpPtr;
	
	// copy mesh info into memory

	DWORD curOffset = m_memSize;
		
	CopyMemory(&m_memBlock[curOffset], &vCount, sizeof(WORD));
	curOffset += sizeof(WORD);

	CopyMemory(&m_memBlock[curOffset], &iCount, sizeof(WORD));
	curOffset += sizeof(WORD);

	CopyMemory(&m_memBlock[curOffset], &smoothShading, sizeof(BOOL));
	curOffset += sizeof(BOOL);

	CopyMemory(&m_memBlock[curOffset], &sightRange, sizeof(float));
	curOffset += sizeof(float);

	CopyMemory(&m_memBlock[curOffset], fileName.GetBuffer(256), sizeof(char)*256);
	curOffset += sizeof(char)*256;
	
	int loop = 0;
	for(loop=0; loop < vCount; loop++)
		{
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].x, sizeof(float));
		curOffset += sizeof(float);
	    CopyMemory(&m_memBlock[curOffset], &vArray[loop].y, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].z, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].nx, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].ny, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].nz, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx0.tu, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx0.tv, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx1.tu, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx1.tv, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx2.tu, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx2.tv, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx3.tu, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx3.tv, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx4.tu, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx4.tv, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx5.tu, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx5.tv, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx6.tu, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx6.tv, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx7.tu, sizeof(float));
		curOffset += sizeof(float);
		CopyMemory(&m_memBlock[curOffset], &vArray[loop].tx7.tv, sizeof(float));		
		curOffset += sizeof(float);
		}

	for(loop = 0; loop < iCount; loop++)
		{
		CopyMemory(&m_memBlock[curOffset], &iArray[loop], sizeof(WORD));
		curOffset += sizeof(WORD);
		}

	CString nullTexture = "NONE";

	int strSize = sizeof(char)*256;
	CopyMemory(&m_memBlock[curOffset], texture1.GetBuffer(256), strSize);
	curOffset += strSize;
	CopyMemory(&m_memBlock[curOffset], texture2.GetBuffer(256), strSize);
	curOffset += strSize;
	CopyMemory(&m_memBlock[curOffset], texture3.GetBuffer(256), strSize);
	curOffset += strSize;
	CopyMemory(&m_memBlock[curOffset], texture4.GetBuffer(256), strSize);
	curOffset += strSize;
	CopyMemory(&m_memBlock[curOffset], texture5.GetBuffer(256), strSize);
	curOffset += strSize;
	CopyMemory(&m_memBlock[curOffset], texture6.GetBuffer(256), strSize);
	curOffset += strSize;
	CopyMemory(&m_memBlock[curOffset], texture7.GetBuffer(256), strSize);
	curOffset += strSize;
	CopyMemory(&m_memBlock[curOffset], texture8.GetBuffer(256), strSize);
	curOffset += strSize;

	m_memSize += blockSize;
	m_numMeshes++;
}
//-------------------------------------------------------------------------//
BOOL Exporter::VerifyObjectForMeshExport(CString nodeName)
{
     for(int loop=0;loop<GL_CommDlg.selectionCount;loop++)//check for mesh output status
		 if(nodeName.CompareNoCase(GL_CommDlg.GL_SceneNodes[GL_CommDlg.selectionBuffer[loop]])==0)
		 {
			return TRUE;
		 }

	 return FALSE;
}
/****************************************************************************

  Save Mesh to file
  
****************************************************************************/
void Exporter::SaveMesh(BOOL generateNormals,
						float distance,
						CString fileName,
						ABVERTEX * vArray,
						WORD * iArray,
						WORD vCount,
						WORD iCount,
						CString texture1,
						CString texture2,
						CString texture3,
						CString texture4,
						CString texture5,
						CString texture6,
						CString texture7,
						CString texture8,
						CString smoothAngleTol,
						int miscLoadInstruction,
						CMaterialObject * materialOption,
						D3DVECTOR boxMin,
						D3DVECTOR boxMax)
{

	 BOOL smoothShading = TRUE;
     //if(!smoothShading)
		// smoothAngleTol = "1";


     /*if(GL_CommDlg.GL_generateNormals == TRUE)
        hVis->m_mesh->GenerateNormals(0);
	    hVis->m_mesh->OptimizeMesh(NULL,shadeTol);
	    hVis->m_mesh->GenerateNormals(0);
	  if(GL_CommDlg.GL_generateNormals == FALSE)
	  {
	      hVis->m_mesh->AVGNormals(shadeTol);//newly enabled 1/18/06
	      hVis->m_mesh->UnifyNormals(shadeTol);//newly enabled 1/18/06
	  }
	  hVis->m_mesh->NormalizeOnly();*/



	 int i=0;
	 CString version = "4";
     CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
     CArchive the_out_Archive(&fl,CArchive::store);
	 the_out_Archive << vCount << iCount << distance << generateNormals;
	 //Vertexes
	 int loop=0;
	 for(loop=0;loop<vCount;loop++){//loop
         the_out_Archive << vArray[loop].x
			             << vArray[loop].y
						 << vArray[loop].z
						 << vArray[loop].nx
						 << vArray[loop].ny
						 << vArray[loop].nz
						 << vArray[loop].tx0.tu
						 << vArray[loop].tx0.tv
						 << vArray[loop].tx1.tu
						 << vArray[loop].tx1.tv
						 << vArray[loop].tx2.tu
						 << vArray[loop].tx2.tv
						 << vArray[loop].tx3.tu
						 << vArray[loop].tx3.tv
						 << vArray[loop].tx4.tu
						 << vArray[loop].tx4.tv
						 << vArray[loop].tx5.tu
						 << vArray[loop].tx5.tv
						 << vArray[loop].tx6.tu
						 << vArray[loop].tx6.tv
						 << vArray[loop].tx7.tu
						 << vArray[loop].tx7.tv;
	 }//end loop
     //indecies
	 for(loop=0;loop<iCount;loop++){//loop
         the_out_Archive << iArray[loop];
	 }//end loop
     //textures
		 the_out_Archive << texture1;
		 the_out_Archive << texture2;
		 the_out_Archive << texture3;
		 the_out_Archive << texture4;
		 the_out_Archive << texture5;
		 the_out_Archive << texture6;
		 the_out_Archive << texture7;
		 the_out_Archive << texture8;

	 //Extended perams
		 
		 the_out_Archive << version;
		 the_out_Archive << smoothAngleTol;

     //version 2+
	     //the_out_Archive << miscLoadInstruction;


		 the_out_Archive << materialOption;

         the_out_Archive << boxMin.x << boxMin.y << boxMin.z << boxMax.x << boxMax.y << boxMax.z;

	 
	 the_out_Archive.Close();
     fl.Close(); 
}

/****************************************************************************

  Mesh output
  
****************************************************************************/

void Exporter::ExportMesh(INode* node, TimeValue t, int indentLevel, BOOL physiqueMesh, BOOL skinMesh)
{//exports both skin and hiarch based mesh data
    if(!node)
		return;


	WriteDebugFile("ExportMesh() mesh name:",0,0);
    WriteDebugFile(FixupName(node->GetName()),0,1);


	Mtl* nodeMtl = node->GetMtl();
	
	Matrix3 tm = node->GetObjTMAfterWSM(t);
	
	BOOL negScale = TMNegParity(tm);
	int vx1, vx2, vx3;
	TSTR indent;
	
	indent = GetIndent(indentLevel+1);
	
	ObjectState os = node->EvalWorldState(t);
	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID) {
		return; // Safety net. This shouldn't happen.
	}
	
	// Order of the vertices. Get 'em counter clockwise if the objects is
	// negatively scaled.
	if (negScale) {
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;
	}
	else {
		vx1 = 0;
		vx2 = 1;
		vx3 = 2;
	}
	
	BOOL needDel;
	TriObject* tri = GetTriObjectFromNode(node, t, needDel);
	if (!tri) {
		return;
	}
	
	Mesh* meshOrig = &tri->GetMesh();
	if(!meshOrig)
	    return;
	
	Mesh * mesh = NULL;
	//new stuff direct export
    CIDList * idList = NULL;
    GetMeshIDList(&idList,meshOrig);
	if(!idList)
	{
		WriteDebugFile("ExportMesh() error 52 cannot generate id list",0,1);
	    return;
	}
	int meshCt = idList->GetCount();
    int lTrace = 0;
	CString extN;
    for(POSITION posML = idList->GetHeadPosition();posML != NULL;)
	{//mesh loop
		CIDObj * id_obj = (CIDObj *)idList->GetNext(posML);
		CString curBoneName = FixupName(node->GetName());
	    if(meshCt == 1)
		{
		   mesh = meshOrig;
		   WriteDebugFile("ExportMesh() single mat id mesh export:",0,0);
		   WriteDebugFile(curBoneName.GetBuffer(),0,1);
		   extN = "";
		}
		else
		{//special multi sub object processing
           WriteDebugFile("ExportMesh() multi-sub mat id mesh export:",0,0);
		   WriteDebugFile(curBoneName.GetBuffer(),0,1);
           BitArray * bitArray = GetMeshGeneratedByID(id_obj->m_id,
									                  meshOrig,
									                  &mesh);

		   if(lTrace == 0)
		      extN = "";
		   else
		      extN = "_MSub:"+CABLIB.ConvToString(lTrace+1);
		}//end special multi sub object processing


		if(mesh)
		{//valid mesh
		int numberOfUVsets = mesh->getNumMaps();
		ABVERTEX * vArray = NULL;
		   WORD *     iArray = NULL;
		   int        iCount = 0;
		   int        vCount = 0;
		   //AfxMessageBox("GetMeshData");
		   int bnIndex = 0;
		   CBoneObject * bonePtr = m_skeletalExport->m_bonesList->GetBoneObjectByName(curBoneName+extN,&bnIndex);


		   if(!bonePtr)
		   {//get deformable basis
		   GetMeshData(mesh, 
					&vArray,
					&iArray,
					&vCount,
					&iCount,
					tm,
					FALSE
					);
		   }//end get deformable basis
		   else
		   {//get hiartchy basis
		   GetMeshData(mesh, 
					   &vArray,
					   &iArray,
					   &vCount,
					   &iCount,
					   tm,
					   TRUE
					   );//gets mesh for both bone and deformable
		   }//end get hiarchy basis
		   
		   D3DMATERIAL9 * d3dMaterialRet = NULL;
		   CString currentTexture2 = "NONE";
		   CString currentTexture3 = "NONE";
		   CString currentTexture4 = "NONE";
		   CString currentTexture5 = "NONE";
		   CString currentTexture6 = "NONE";
		   CString currentTexture7 = "NONE";
		   CString currentTexture8 = "NONE";
		   CString mainTexture = GetMaterialFromID(node,
												   id_obj->m_id,
												   &d3dMaterialRet,
												   &currentTexture2,
												   &currentTexture3,
												   &currentTexture4,
												   &currentTexture5);//get first texture
		   
		   
		   
							//assignLightmap
							CString nodeName = FixupName(node->GetName());
							char indexer = 'a';
							CString tempStr = nodeName;
							CString LightmapMeshName = tempStr+"LT";
							for(int ltLoop=0;ltLoop<7;ltLoop++){
								
								CString finalName = LightmapMeshName+indexer;
								INode* lightmapNode = ip->GetINodeByName(finalName);
								if(!lightmapNode)
									break;
								
								BOOL needDel;
								TriObject* intrfaceTri = GetTriObjectFromNode(lightmapNode,0, needDel);
								CString currentLightTexture = GetMaterialFromID(lightmapNode,0,NULL,NULL,NULL,NULL,NULL);
								if(ltLoop==0)currentTexture2 = currentLightTexture;
								if(ltLoop==1)currentTexture3 = currentLightTexture;
								if(ltLoop==2)currentTexture4 = currentLightTexture;
								if(ltLoop==3)currentTexture5 = currentLightTexture;
								if(ltLoop==4)currentTexture6 = currentLightTexture;
								if(ltLoop==5)currentTexture7 = currentLightTexture;
								if(ltLoop==6)currentTexture8 = currentLightTexture;

								if(intrfaceTri){//valid
								   Mesh* lightMesh = &intrfaceTri->GetMesh();
								   //lightMesh->buildNormals();
								   Matrix3 ltmatrix = lightmapNode->GetObjTMAfterWSM(0);
	                           
								   ABVERTEX * ltVertexArray = NULL;
								   WORD *     ltIndecieArray = NULL;
								   int        ltIndecieCount = 0;
								   int        lyVertexCount = 0;
								   GetMeshData(lightMesh, 
											   &ltVertexArray,
											   &ltIndecieArray,
											   &lyVertexCount,
											   &ltIndecieCount,
											   ltmatrix,
											   FALSE);
								   if(lyVertexCount != vCount){
									  AfxMessageBox("Inconsistent vertex counts detected on ltmaps");
								   }
								   else{
								   SetABVERTEXCoordinateSet(ltLoop,
															lyVertexCount,
															vArray,
															ltVertexArray);
								   }
								}//end valid
								indexer++;
							}
		
		if(bonePtr){//valid mesh on bone
		   //AfxMessageBox("Hiarchy mesh");
		   bonePtr->m_hiarchyMesh = new CHiarchyMesh();
		   bonePtr->m_hiarchyMesh->m_lodChain = new CHiarcleVisList();
		   CHiarcleVisObj * hVis = new CHiarcleVisObj();
		   bonePtr->m_hiarchyMesh->m_lodChain->AddTail(hVis);
		   hVis->m_mesh = new CEXMeshObj();
		   hVis->m_mesh->List_materialObject = new CMaterialObject(); 

		   BOOL pass = 0;

		   if(d3dMaterialRet)
		   {
				//DPD
				if(NodeHasShader(node))
				{
					GraphicsWindow		*GW;
					ViewExp				*View;
					LPDIRECT3DDEVICE9	device;

					View = GetCOREInterface()->GetActiveViewport();
					if(View) {
						GW = View->getGW();
						if(GW) {
							ID3D9GraphicsWindow *D3DGW = (ID3D9GraphicsWindow *)GW->GetInterface(D3D9_GRAPHICS_WINDOW_INTERFACE_ID);
							if(D3DGW) {
								device = D3DGW->GetDevice();
							}
						}
					}
					if(device) {
						LPD3DXEFFECT effect;
						CString fxRetCode = "";
						if((effect = SetupEffectFile(node, device, &fxRetCode)))
							if(hVis->m_mesh->List_materialObject->InitPeramBlockFXShadersHSL(effect))
								GetShaderInfo(hVis->m_mesh->List_materialObject, node, effect, fxRetCode);

						CString textures[20];
						int texCount = SetShaderTextures(effect, node, hVis->m_mesh->List_materialObject, textures);
						mainTexture     = textures[0];
						currentTexture2 = textures[1];
						currentTexture3 = textures[2];
						currentTexture4 = textures[3];
						currentTexture5 = textures[4];
						currentTexture6 = textures[5];
						currentTexture7 = textures[6];
						currentTexture8 = textures[7];
						effect->Release();
						effect = 0;
					}
				}
				else
				{	
					CopyMemory(&hVis->m_mesh->List_materialObject->m_useMaterial,d3dMaterialRet,sizeof(D3DMATERIAL9));
					CopyMemory(&hVis->m_mesh->List_materialObject->m_baseMaterial,d3dMaterialRet,sizeof(D3DMATERIAL9));
				}
		   }

		   //if(d3dMaterialRet)
		      
		   //hVis->m_mesh->m_abVertexArray.m_uvCount = 8;
		   D3DMATRIX InverseBoneMatrix;
		   bonePtr->m_boneFrame->GetInverseTransform(&InverseBoneMatrix);
		   for(int invLoop=0;invLoop<vCount;invLoop++){//move verts
			   D3DVECTOR vectTrans,normTrans;
			   vectTrans.x = vArray[invLoop].x;
			   vectTrans.y = vArray[invLoop].y;
			   vectTrans.z = vArray[invLoop].z;
			   normTrans.x = vArray[invLoop].nx + vArray[invLoop].x;
			   normTrans.y = vArray[invLoop].ny + vArray[invLoop].y;
			   normTrans.z = vArray[invLoop].nz + vArray[invLoop].z;
			   vectTrans = GL_matrixFunctions.UpdatePolygonVertex(InverseBoneMatrix.m,vectTrans);
			   normTrans = GL_matrixFunctions.UpdatePolygonVertex(InverseBoneMatrix.m,normTrans);
			   
			   vArray[invLoop].x = vectTrans.x;
			   vArray[invLoop].y = vectTrans.y;
			   vArray[invLoop].z = vectTrans.z;
			   normTrans.x = normTrans.x - vectTrans.x;
			   normTrans.y = normTrans.y - vectTrans.y;
			   normTrans.z = normTrans.z - vectTrans.z;
			   normTrans = GL_matrixFunctions.Normalize(normTrans);
			   vArray[invLoop].nx = normTrans.x;
			   vArray[invLoop].ny = normTrans.y;
			   vArray[invLoop].nz = normTrans.z;
		   }//end move verts

		   if(currentTexture2 != "NONE")
		   hVis->m_mesh->List_BlendMethod[0] = 1;
		   hVis->m_mesh->List_storedTextureFromFile[1] = currentTexture2;
		   hVis->m_mesh->List_storedTextureFromFile[2] = currentTexture3;
		   hVis->m_mesh->List_storedTextureFromFile[3] = currentTexture4;
		   hVis->m_mesh->List_storedTextureFromFile[4] = currentTexture5;
		   hVis->m_mesh->List_storedTextureFromFile[5] = currentTexture6;
		   hVis->m_mesh->List_storedTextureFromFile[6] = currentTexture7;
		   hVis->m_mesh->List_storedTextureFromFile[7] = currentTexture8;

		   hVis->m_mesh->GetUVlayerCountString();
		   hVis->m_mesh->SetGeometry(mainTexture,vArray,vCount,iArray,iCount);
		    
		}//end valid mesh on bone
		/*if(skinMesh == TRUE)
		{//valid skin mesh
	  

		}//end valid skin mesh*/
		//----------------------------------------------
		if(physiqueMesh == TRUE || skinMesh == TRUE){//valid deformable mesh
		   //AfxMessageBox("Export Phy");
			   CDeformableVisObj * dPtr = new CDeformableVisObj();
			   m_skeletalExport->m_deformableMeshArrayV2[GL_currentPhyModIndex].m_defMeshVisualUnit = new CDeformableMesh();
			   m_skeletalExport->m_deformableMeshArrayV2[GL_currentPhyModIndex].m_defMeshVisualUnit->m_lodChain = new CDeformableVisList();
			   m_skeletalExport->m_deformableMeshArrayV2[GL_currentPhyModIndex].m_defMeshVisualUnit->m_lodChain->AddTail(dPtr);
			   m_skeletalExport->m_deformableMeshArrayV2[GL_currentPhyModIndex].m_defMeshVisualUnit->m_dimName = nodeName;
			   dPtr->m_mesh                        = new CEXMeshObj();
			   dPtr->m_mesh->List_materialObject   = new CMaterialObject(); 
			   
			   if(d3dMaterialRet)
			   {
			   		//DPD
					BOOL pass = 0;
					if(NodeHasShader(node))
					{
						GraphicsWindow		*GW;
						ViewExp				*View;
						LPDIRECT3DDEVICE9	device;

						View = GetCOREInterface()->GetActiveViewport();
						if(View) {
							GW = View->getGW();
							if(GW) {
								ID3D9GraphicsWindow *D3DGW = (ID3D9GraphicsWindow *)GW->GetInterface(D3D9_GRAPHICS_WINDOW_INTERFACE_ID);
								if(D3DGW) {
									device = D3DGW->GetDevice();
								}
							}
						}
						if(device) {
							LPD3DXEFFECT effect;
							CString fxRetCode = "";
							if((effect = SetupEffectFile(node, device, &fxRetCode)))
							{
								if(dPtr->m_mesh->List_materialObject->InitPeramBlockFXShadersHSL(effect))
								{
									GetShaderInfo(dPtr->m_mesh->List_materialObject, node, effect, fxRetCode);
						
									CString textures[20];
							
									int texCount = SetShaderTextures(effect, node, dPtr->m_mesh->List_materialObject, textures);
									mainTexture     = textures[0];
									currentTexture2 = textures[1];
									currentTexture3 = textures[2];
									currentTexture4 = textures[3];
									currentTexture5 = textures[4];
									currentTexture6 = textures[5];
									currentTexture7 = textures[6];
									currentTexture8 = textures[7];

									effect->Release();
									effect = 0;
								}
							}
						}
					}
					else
					{	
						CopyMemory(&dPtr->m_mesh->List_materialObject->m_useMaterial,d3dMaterialRet,sizeof(D3DMATERIAL9));
						CopyMemory(&dPtr->m_mesh->List_materialObject->m_baseMaterial,d3dMaterialRet,sizeof(D3DMATERIAL9));
					}
			   }
			   dPtr->m_nameTemp                    = nodeName;
			   
			   if(currentTexture2 != "NONE")
			   dPtr->m_mesh->List_BlendMethod[0] = 1;
			   dPtr->m_mesh->List_storedTextureFromFile[1] = currentTexture2;
			   dPtr->m_mesh->List_storedTextureFromFile[2] = currentTexture3;
			   dPtr->m_mesh->List_storedTextureFromFile[3] = currentTexture4;
			   dPtr->m_mesh->List_storedTextureFromFile[4] = currentTexture5;
			   dPtr->m_mesh->List_storedTextureFromFile[5] = currentTexture6;
			   dPtr->m_mesh->List_storedTextureFromFile[6] = currentTexture7;
			   dPtr->m_mesh->List_storedTextureFromFile[7] = currentTexture8;
			   
			   dPtr->m_mesh->GetUVlayerCountString();
			   dPtr->m_mesh->SetGeometry(mainTexture,vArray,vCount,iArray,iCount);
			   //dPtr->m_mesh->UnifyNormals();
			   dPtr->m_originalArrayVector         = new D3DVECTOR[vCount];
			   //CopyMemory(dPtr->m_originalArrayVector,vArray,sizeof(D3DVECTOR)*vCount);
			   for(int vArrayLoop=0;vArrayLoop<vCount;vArrayLoop++)
			   {
				   dPtr->m_originalArrayVector[vArrayLoop].x = vArray[vArrayLoop].x;
				   dPtr->m_originalArrayVector[vArrayLoop].y = vArray[vArrayLoop].y;
				   dPtr->m_originalArrayVector[vArrayLoop].z = vArray[vArrayLoop].z;
			   }
			   //AfxMessageBox(mainTexture);

	           

			   //dPtr->m_mesh->GetUVlayerCountString();

		   GL_currentPhyModIndex++;
		   //AfxMessageBox("completed Export Phy");
		}//end valid deformable mesh
		}//end valid mesh
		lTrace++;
		}//mesh loop
}
/****************************************************************************

  Mesh output
  
****************************************************************************/

void Exporter::GetMeshData(Mesh* mesh, 
						   ABVERTEX ** vArray,
						   WORD ** iArray,
					       int * vCount,
						   int * iCount,
                           Matrix3 tm,
						   BOOL useTransofmWNorm
						   )
{
	int i;
	TimeValue t = 0;
	//Mtl* nodeMtl = node->GetMtl();
	//Matrix3 tm = node->GetObjTMAfterWSM(t);
	Matrix3 tmRotation = tm;
	Point3 zeroVect;
	zeroVect.x = 0.0f;
	zeroVect.y = 0.0f;
	zeroVect.z = 0.0f;
	tmRotation.SetTrans(zeroVect);

	BOOL negScale = TMNegParity(tm);
	int vx1, vx2, vx3;
	// Order of the vertices. Get 'em counter clockwise if the objects is
	// negatively scaled.
	if (negScale) {
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;
	}
	else {
		vx1 = 0;
		vx2 = 1;
		vx3 = 2;
	}

	//mesh->buildNormals();
    int numFaces = mesh->getNumFaces();
	int currentUvCount = mesh->getNumMaps();
	int vertexCount = numFaces*3;
	WORD indecieCount = numFaces*3;
	ABVERTEX * vertexArray = NULL;
	WORD * indecieArray = NULL;

    //AfxMessageBox("GetMeshData2");

	if(vertexCount > 0){
	   vertexArray = new ABVERTEX[vertexCount];
       ZeroMemory(vertexArray,(sizeof(ABVERTEX)*vertexCount));
	}
	if(indecieCount > 0){
       indecieArray = new WORD[(numFaces*3)];
	   ZeroMemory(indecieArray,(sizeof(WORD)*(numFaces*3)));

	}
    
    int vertexCurrent = 0;
    int sceneVertCount = mesh->getNumVerts();

	//ExportMaterial(node, indentLevel);
	//int textureMapPresent = 0;
	int count = mesh->getNumTVerts();
	int indecieTrace = 0;

	UVVert * levelOneUvs = NULL;
	UVVert * levelTwoUvs = NULL;
	UVVert * levelThreeUvs = NULL;
	UVVert * levelFourUvs = NULL;
	UVVert * levelFiveUvs = NULL;
	TVFace * levelOneTVface = NULL;
	TVFace * levelTwoTVface = NULL;
	TVFace * levelThreeTVface = NULL;
	TVFace * levelFourTVface = NULL;
	TVFace * levelFiveTVface = NULL;

	   levelOneUvs = mesh->mapVerts(1);//default set
	   levelOneTVface = mesh->mapFaces(1);
	if(currentUvCount > 2)
	{
	   levelTwoUvs = mesh->mapVerts(2);//default set
	   levelTwoTVface = mesh->mapFaces(2);
	}
	if(currentUvCount > 3)
	{
	   levelThreeUvs = mesh->mapVerts(3);//default set
	   levelThreeTVface = mesh->mapFaces(3);
	}
	if(currentUvCount > 4)
	{
	   levelFourUvs = mesh->mapVerts(4);//default set
	   levelFourTVface = mesh->mapFaces(4);
	}
	if(currentUvCount > 5)
	{
	   levelFiveUvs = mesh->mapVerts(5);//default set
	   levelFiveTVface = mesh->mapFaces(5);
	}
    
	
	//TVFace * newtvface = mesh->mapFaces(2);
	//newtvface->getAllTVerts();


    //mesh->buildNormals();
	mesh->buildRenderNormals();
	Point3 nrml,cnml,onrml;
	int ci = 0;
	int indecieCmp = 0;
	int indecieCmp2 = 0;
	int indecieCmp3 = 0;
	bool compound = false;
	int finalIndecieCmp = -1;
	RVertex* rv = NULL;

	float shadeTol = atof(GL_CommDlg.GL_smShadeToleranceAngle);
	D3DVECTOR tempVect1,tempVect2;

	for (i=0; i<numFaces; i++) {
		 //vertex 1
		     int indecieCurrent = mesh->faces[i].v[vx1];
			 Point3 v = tm * mesh->verts[indecieCurrent];
			 
			 //Point3 nrmlFace = tmRotation * mesh->FaceNormal(i, TRUE);

			 
			 /*Point3 nrml = rv->rn.getNormal();
			 if(useTransofmWNorm)
			    nrml = tmRotation * rv->rn.getNormal();*/

			 

             nrml = mesh->FaceNormal(i, TRUE);
			 //rv = mesh->getRVertPtr(indecieCurrent);
			 //nrml = rv->rn.getNormal();
			 onrml = nrml;
			 for (ci=0; ci<numFaces; ci++)
			 {
                  if(ci == i)
				     continue;

                  if(mesh->faces[i].smGroup != mesh->faces[ci].smGroup)
				     continue;

				  finalIndecieCmp = -1;
				  indecieCmp = mesh->faces[ci].v[vx1];
				  indecieCmp2 = mesh->faces[ci].v[vx2];
				  indecieCmp3 = mesh->faces[ci].v[vx3];
                  if(mesh->verts[indecieCurrent].x == mesh->verts[indecieCmp].x && mesh->verts[indecieCurrent].y == mesh->verts[indecieCmp].y && mesh->verts[indecieCurrent].z == mesh->verts[indecieCmp].z)
                     finalIndecieCmp = indecieCmp;
				  else if(mesh->verts[indecieCurrent].x == mesh->verts[indecieCmp2].x && mesh->verts[indecieCurrent].y == mesh->verts[indecieCmp2].y && mesh->verts[indecieCurrent].z == mesh->verts[indecieCmp2].z)
                     finalIndecieCmp = indecieCmp2;
				  else if(mesh->verts[indecieCurrent].x == mesh->verts[indecieCmp3].x && mesh->verts[indecieCurrent].y == mesh->verts[indecieCmp3].y && mesh->verts[indecieCurrent].z == mesh->verts[indecieCmp3].z)
                     finalIndecieCmp = indecieCmp3;
                  
				  
				  if(finalIndecieCmp > -1)
				  {
				     cnml = mesh->FaceNormal(ci, TRUE);
					 //RVertex* rv = mesh->getRVertPtr(finalIndecieCmp);
			         //cnml = rv->rn.getNormal();
					  
					 tempVect1.x = cnml.x;
					 tempVect1.y = cnml.y;
					 tempVect1.z = cnml.z;
					 tempVect2.x = onrml.x;
					 tempVect2.y = onrml.y;
					 tempVect2.z = onrml.z;
                     
					 if(GL_matrixFunctions.GetAngleBetweenTwoVectors(tempVect1,tempVect2)<shadeTol)
					 {
					    nrml.x += cnml.x;
				        nrml.y += cnml.y;
				        nrml.z += cnml.z;
					 }
				  }
			 }
			 nrml.Normalize();
			 if(useTransofmWNorm)
			    nrml = tmRotation * nrml;
			 vertexArray[vertexCurrent].tx7.tu = mesh->faces[i].smGroup;//trick uv to make optimize not compact this


			 vertexArray[vertexCurrent].x = v.x;
			 vertexArray[vertexCurrent].y = v.z;
			 vertexArray[vertexCurrent].z = v.y;

			 vertexArray[vertexCurrent].nx = nrml.x;
			 vertexArray[vertexCurrent].ny = nrml.z;
			 vertexArray[vertexCurrent].nz = nrml.y;
			 //log texture coordinates
			 if(mesh->tvFace){
			    //vertexArray[vertexCurrent].tx0.tu = mesh->tVerts[mesh->tvFace[i].t[vx1]].x;
			    //vertexArray[vertexCurrent].tx0.tv = 1.0f-mesh->tVerts[mesh->tvFace[i].t[vx1]].y;
			    vertexArray[vertexCurrent].tx0.tu = levelOneUvs[levelOneTVface[i].t[vx1]].x;
			    vertexArray[vertexCurrent].tx0.tv = 1.0f-levelOneUvs[levelOneTVface[i].t[vx1]].y;
				if(levelTwoUvs)
				{
				   vertexArray[vertexCurrent].tx1.tu = levelTwoUvs[levelTwoTVface[i].t[vx1]].x;
			       vertexArray[vertexCurrent].tx1.tv = 1.0f-levelTwoUvs[levelTwoTVface[i].t[vx1]].y;
				}
				if(levelThreeUvs)
				{
				   vertexArray[vertexCurrent].tx2.tu = levelThreeUvs[levelThreeTVface[i].t[vx1]].x;
			       vertexArray[vertexCurrent].tx2.tv = 1.0f-levelThreeUvs[levelThreeTVface[i].t[vx1]].y;
				}
				if(levelFourUvs)
				{
				   vertexArray[vertexCurrent].tx3.tu = levelFourUvs[levelFourTVface[i].t[vx1]].x;
			       vertexArray[vertexCurrent].tx3.tv = 1.0f-levelFourUvs[levelFourTVface[i].t[vx1]].y;
				}
				if(levelFiveUvs)
				{
				   vertexArray[vertexCurrent].tx4.tu = levelFiveUvs[levelFourTVface[i].t[vx1]].x;
			       vertexArray[vertexCurrent].tx4.tv = 1.0f-levelFiveUvs[levelFourTVface[i].t[vx1]].y;
				}

			 }
			 else{
                vertexArray[vertexCurrent].tx0.tu = 0.0f;
			    vertexArray[vertexCurrent].tx0.tv = 0.0f;
			 }
			 indecieArray[indecieTrace] = indecieTrace;
			 indecieTrace++;
			 vertexCurrent++;
			 
		 //vertex 2
			 indecieCurrent = mesh->faces[i].v[vx3];
			 v = tm * mesh->verts[indecieCurrent];
			 //nrml = tmRotation * mesh->FaceNormal(i, TRUE);
             /*rv = mesh->getRVertPtr(indecieCurrent);
			 nrml = rv->rn.getNormal();
			 if(useTransofmWNorm)
			    nrml = tmRotation * rv->rn.getNormal();*/

			 nrml = mesh->FaceNormal(i, TRUE);
			 //rv = mesh->getRVertPtr(indecieCurrent);
			 //nrml = rv->rn.getNormal();
			 onrml = nrml;
			 for (ci=0; ci<numFaces; ci++)
			 {
                  if(ci == i)
				     continue;

                  if(mesh->faces[i].smGroup != mesh->faces[ci].smGroup)
				     continue;

				  finalIndecieCmp = -1;
				  indecieCmp = mesh->faces[ci].v[vx1];
				  indecieCmp2 = mesh->faces[ci].v[vx2];
				  indecieCmp3 = mesh->faces[ci].v[vx3];
                  if(mesh->verts[indecieCurrent].x == mesh->verts[indecieCmp].x && mesh->verts[indecieCurrent].y == mesh->verts[indecieCmp].y && mesh->verts[indecieCurrent].z == mesh->verts[indecieCmp].z)
                     finalIndecieCmp = indecieCmp;
				  else if(mesh->verts[indecieCurrent].x == mesh->verts[indecieCmp2].x && mesh->verts[indecieCurrent].y == mesh->verts[indecieCmp2].y && mesh->verts[indecieCurrent].z == mesh->verts[indecieCmp2].z)
                     finalIndecieCmp = indecieCmp2;
				  else if(mesh->verts[indecieCurrent].x == mesh->verts[indecieCmp3].x && mesh->verts[indecieCurrent].y == mesh->verts[indecieCmp3].y && mesh->verts[indecieCurrent].z == mesh->verts[indecieCmp3].z)
                     finalIndecieCmp = indecieCmp3;
                  
				  
				  if(finalIndecieCmp > -1)
				  {
				     cnml = mesh->FaceNormal(ci, TRUE);
					 //RVertex* rv = mesh->getRVertPtr(finalIndecieCmp);
			         //cnml = rv->rn.getNormal();
					  
					 tempVect1.x = cnml.x;
					 tempVect1.y = cnml.y;
					 tempVect1.z = cnml.z;
					 tempVect2.x = onrml.x;
					 tempVect2.y = onrml.y;
					 tempVect2.z = onrml.z;
                     
					 if(GL_matrixFunctions.GetAngleBetweenTwoVectors(tempVect1,tempVect2)<shadeTol)
					 {
					    nrml.x += cnml.x;
				        nrml.y += cnml.y;
				        nrml.z += cnml.z;
					 }
				  }
			 }
			 nrml.Normalize();
			 if(useTransofmWNorm)
			    nrml = tmRotation * nrml;
			 vertexArray[vertexCurrent].tx7.tu = mesh->faces[i].smGroup;//trick uv to make optimize not compact this


			 vertexArray[vertexCurrent].x = v.x;
			 vertexArray[vertexCurrent].y = v.z;
			 vertexArray[vertexCurrent].z = v.y;

			 vertexArray[vertexCurrent].nx = nrml.x;
			 vertexArray[vertexCurrent].ny = nrml.z;
			 vertexArray[vertexCurrent].nz = nrml.y;
             //log texture coords
			 if(mesh->tvFace){
			    //vertexArray[vertexCurrent].tx0.tu = mesh->tVerts[mesh->tvFace[i].t[vx3]].x;
			    //vertexArray[vertexCurrent].tx0.tv = 1.0f-mesh->tVerts[mesh->tvFace[i].t[vx3]].y;
			    vertexArray[vertexCurrent].tx0.tu = levelOneUvs[levelOneTVface[i].t[vx3]].x;
			    vertexArray[vertexCurrent].tx0.tv = 1.0f-levelOneUvs[levelOneTVface[i].t[vx3]].y;
				if(levelTwoUvs)
				{
				   vertexArray[vertexCurrent].tx1.tu = levelTwoUvs[levelTwoTVface[i].t[vx3]].x;
			       vertexArray[vertexCurrent].tx1.tv = 1.0f-levelTwoUvs[levelTwoTVface[i].t[vx3]].y;
				}
				if(levelThreeUvs)
				{
				   vertexArray[vertexCurrent].tx2.tu = levelThreeUvs[levelThreeTVface[i].t[vx3]].x;
			       vertexArray[vertexCurrent].tx2.tv = 1.0f-levelThreeUvs[levelThreeTVface[i].t[vx3]].y;
				}
				if(levelFourUvs)
				{
				   vertexArray[vertexCurrent].tx3.tu = levelFourUvs[levelFourTVface[i].t[vx3]].x;
			       vertexArray[vertexCurrent].tx3.tv = 1.0f-levelFourUvs[levelFourTVface[i].t[vx3]].y;
				}
				if(levelFiveUvs)
				{
				   vertexArray[vertexCurrent].tx4.tu = levelFiveUvs[levelFiveTVface[i].t[vx3]].x;
			       vertexArray[vertexCurrent].tx4.tv = 1.0f-levelFiveUvs[levelFiveTVface[i].t[vx3]].y;
				}
			 }
			 else{
                vertexArray[vertexCurrent].tx0.tu = 0.0f;
			    vertexArray[vertexCurrent].tx0.tv = 0.0f;
			 }
			 indecieArray[indecieTrace] = indecieTrace;
			 indecieTrace++;
			 vertexCurrent++;
		 //vertex 3
			 indecieCurrent = mesh->faces[i].v[vx2];
			 v = tm * mesh->verts[indecieCurrent];
			 //nrml = tmRotation * mesh->FaceNormal(i, TRUE);
             /*rv = mesh->getRVertPtr(indecieCurrent);
			 nrml = rv->rn.getNormal();
			 if(useTransofmWNorm)
			    nrml = tmRotation * rv->rn.getNormal();*/

			 nrml = mesh->FaceNormal(i, TRUE);
			 //rv = mesh->getRVertPtr(indecieCurrent);
			 //nrml = rv->rn.getNormal();
			 onrml = nrml;
			 for (ci=0; ci<numFaces; ci++)
			 {
                  if(ci == i)
				     continue;

                  if(mesh->faces[i].smGroup != mesh->faces[ci].smGroup)
				     continue;

				  finalIndecieCmp = -1;
				  indecieCmp = mesh->faces[ci].v[vx1];
				  indecieCmp2 = mesh->faces[ci].v[vx2];
				  indecieCmp3 = mesh->faces[ci].v[vx3];
                  if(mesh->verts[indecieCurrent].x == mesh->verts[indecieCmp].x && mesh->verts[indecieCurrent].y == mesh->verts[indecieCmp].y && mesh->verts[indecieCurrent].z == mesh->verts[indecieCmp].z)
                     finalIndecieCmp = indecieCmp;
				  else if(mesh->verts[indecieCurrent].x == mesh->verts[indecieCmp2].x && mesh->verts[indecieCurrent].y == mesh->verts[indecieCmp2].y && mesh->verts[indecieCurrent].z == mesh->verts[indecieCmp2].z)
                     finalIndecieCmp = indecieCmp2;
				  else if(mesh->verts[indecieCurrent].x == mesh->verts[indecieCmp3].x && mesh->verts[indecieCurrent].y == mesh->verts[indecieCmp3].y && mesh->verts[indecieCurrent].z == mesh->verts[indecieCmp3].z)
                     finalIndecieCmp = indecieCmp3;
                  
				  
				  if(finalIndecieCmp > -1)
				  {
				     cnml = mesh->FaceNormal(ci, TRUE);
					 //RVertex* rv = mesh->getRVertPtr(finalIndecieCmp);
			         //cnml = rv->rn.getNormal();
					  
					 tempVect1.x = cnml.x;
					 tempVect1.y = cnml.y;
					 tempVect1.z = cnml.z;
					 tempVect2.x = onrml.x;
					 tempVect2.y = onrml.y;
					 tempVect2.z = onrml.z;
                     
					 if(GL_matrixFunctions.GetAngleBetweenTwoVectors(tempVect1,tempVect2)<shadeTol)
					 {
					    nrml.x += cnml.x;
				        nrml.y += cnml.y;
				        nrml.z += cnml.z;
					 }
				  }
			 }
			 nrml.Normalize();
			 if(useTransofmWNorm)
			    nrml = tmRotation * nrml;
			 vertexArray[vertexCurrent].tx7.tu = mesh->faces[i].smGroup;//trick uv to make optimize not compact this



			 vertexArray[vertexCurrent].x = v.x;
			 vertexArray[vertexCurrent].y = v.z;
			 vertexArray[vertexCurrent].z = v.y;

			 vertexArray[vertexCurrent].nx = nrml.x;
			 vertexArray[vertexCurrent].ny = nrml.z;
			 vertexArray[vertexCurrent].nz = nrml.y;
			 //log texture coords
			   if(mesh->tvFace){
			    //vertexArray[vertexCurrent].tx0.tu = mesh->tVerts[mesh->tvFace[i].t[vx2]].x;
			    //vertexArray[vertexCurrent].tx0.tv = 1.0f-mesh->tVerts[mesh->tvFace[i].t[vx2]].y;
			    vertexArray[vertexCurrent].tx0.tu = levelOneUvs[levelOneTVface[i].t[vx2]].x;
			    vertexArray[vertexCurrent].tx0.tv = 1.0f-levelOneUvs[levelOneTVface[i].t[vx2]].y;
				if(levelTwoUvs)
				{
				   vertexArray[vertexCurrent].tx1.tu = levelTwoUvs[levelTwoTVface[i].t[vx2]].x;
			       vertexArray[vertexCurrent].tx1.tv = 1.0f-levelTwoUvs[levelTwoTVface[i].t[vx2]].y;
				}
				if(levelThreeUvs)
				{
				   vertexArray[vertexCurrent].tx2.tu = levelThreeUvs[levelThreeTVface[i].t[vx2]].x;
			       vertexArray[vertexCurrent].tx2.tv = 1.0f-levelThreeUvs[levelThreeTVface[i].t[vx2]].y;
				}
				if(levelFourUvs)
				{
				   vertexArray[vertexCurrent].tx3.tu = levelFourUvs[levelFourTVface[i].t[vx2]].x;
			       vertexArray[vertexCurrent].tx3.tv = 1.0f-levelFourUvs[levelFourTVface[i].t[vx2]].y;
				}
				if(levelFiveUvs)
				{
				   vertexArray[vertexCurrent].tx4.tu = levelFiveUvs[levelFiveTVface[i].t[vx2]].x;
			       vertexArray[vertexCurrent].tx4.tv = 1.0f-levelFiveUvs[levelFiveTVface[i].t[vx2]].y;
				}
			   }
			   else{
                vertexArray[vertexCurrent].tx0.tu = 0.0f;
			    vertexArray[vertexCurrent].tx0.tv = 0.0f;
			   }
			 indecieArray[indecieTrace] = indecieTrace;
			 indecieTrace++;
			 vertexCurrent++;
			 
			 
	}
    *vArray = vertexArray;
    *iArray = indecieArray;
    *vCount = vertexCount;
    *iCount = indecieCount;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
BOOL Exporter::LogVertexAssignments(INode* node, int indentLevel)
{
	
	int physiqueCurrentIndex = GL_currentPhyModIndex;
	TSTR indent = GetIndent(indentLevel);
	    BOOL needDel;
	    TriObject* tri = GetTriObjectFromNode(node,0, needDel);
	    if (!tri) {
		     return FALSE;
		}
	    Mesh* mesh = &tri->GetMesh();
		Matrix3 tm = node->GetObjTMAfterWSM(0);
		BOOL negScale = TMNegParity(tm);
	    int vx1, vx2, vx3;
		if (negScale) {
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;
		}
	    else {
		vx1 = 0;
		vx2 = 1;
		vx3 = 2;
		}
        //skin
        Modifier* skinMod = FindSkinModifier (node);
        if(skinMod != NULL)
		{//skin exists 

		   ISkin * skin = (ISkin*)skinMod->GetInterface(I_SKIN);
		   if(skin == NULL)
		   {
		    AfxMessageBox("Skin get interface error");
			return FALSE;
		   }
           ISkinContextData *skinMC = skin->GetContextInterface(node);
           if(skinMC == NULL)
		   {
		    AfxMessageBox("Get skin contex error");
			return FALSE;
		   }

		   Point3 offsetVector;
		   int vertexCount = mesh->getNumFaces()*3;
	       
		   int vertexTrace = 0;
		   for(int i=0;i<mesh->getNumFaces();i++)
		   {//get vertex assignments
		      int indecieCurrent = 0;
			   for(int faceLoop=0;faceLoop<3;faceLoop++)
			   {//face loop
			    int boneIndexesCount = 0;
			    float * wieghtsToBone = NULL;
			    byte * boneIndexes = NULL;
			    ABVERTEX0L * offsetVectors  =NULL;
			    float totalWeight = 0.0f;
			    CString boneTracerName;
				   
			    if(faceLoop == 0)
                indecieCurrent = mesh->faces[i].v[vx1];
				else if(faceLoop == 1)
		        indecieCurrent = mesh->faces[i].v[vx3];
				else
				indecieCurrent = mesh->faces[i].v[vx2];

				
				
           
                boneIndexesCount = skinMC->GetNumAssignedBones(indecieCurrent);
				wieghtsToBone = new float[boneIndexesCount];
			    offsetVectors = new ABVERTEX0L[boneIndexesCount];
			    boneIndexes = new byte[boneIndexesCount];
				ZeroMemory(wieghtsToBone,sizeof(float)*boneIndexesCount);
				ZeroMemory(boneIndexes,sizeof(byte)*boneIndexesCount);
				ZeroMemory(offsetVectors,sizeof(ABVERTEX0L)*boneIndexesCount);
                totalWeight = 0.0f;

				for (int x = 0; x<boneIndexesCount; x++)
			    {
					float weight = skinMC->GetBoneWeight(indecieCurrent,x);

					//skin->GetBoneInitTM

                    if(weight < 0.0f)
					   weight = 0.0f;
					if(weight > 1.0f)
                       weight = 1.0f;

					totalWeight += weight;

					//offsetVector = rb_vtx->GetOffsetVector(x);
					INode * iNodeBN = skin->GetBone(skinMC->GetAssignedBone(indecieCurrent,x));
					Matrix3 inv;
                    skin->GetBoneInitTM(iNodeBN,inv,TRUE);
					
					Point3 pointX = inv * mesh->verts[indecieCurrent];
					offsetVector.x = pointX.x;
					offsetVector.y = pointX.y;
					offsetVector.z = pointX.z;

					

					TSTR nodeName = iNodeBN->GetName();
					CString casted = nodeName;
					boneTracerName = nodeName;

                    int indexOfBone = 0;
					CBoneObject * bonePtr = m_skeletalExport->m_bonesList->GetBoneObjectByName(FixupName(iNodeBN->GetName()),&indexOfBone);
				    if(bonePtr)
				    {
					   if(bonePtr->m_boneName == "Dummy02" || bonePtr->m_boneName ==  "Dummy20")
					   {
                          wieghtsToBone[x] = weight;
					   }
								    
						  wieghtsToBone[x] = weight;
						  boneIndexes[x] = indexOfBone;
						  offsetVectors[x].x = offsetVector.x;
						  offsetVectors[x].y = offsetVector.y;
						  offsetVectors[x].z = offsetVector.z;
					}
					else
					   MessageBox(NULL,"Bone not found","",MB_OK);//failure message    
				}

				if(totalWeight > 1.2f)
			    {

                   MessageBox(NULL,"TotalWeight Issue","",MB_OK);//failure message
				}

			    if(boneIndexesCount > 0)
				   AddVertexAssignmentBlended(boneIndexes,
										      wieghtsToBone,
											  offsetVectors,
										      boneIndexesCount,
										      vertexTrace,
										      physiqueCurrentIndex,
											  vertexCount);
				vertexTrace++;
			   }//end face loop
		   }//end get vertex assignments
           //AfxMessageBox("Skin has been initialized correctly");
		   GL_currentPhyModIndex++;
		}//end skin exists
		//Physique
		Modifier* phyMod = FindPhysiqueModifier (node);	
	    if(phyMod != NULL){//physique exists 
		//AfxMessageBox("Log vertex assignments");
		   //MessageBox(NULL,"Begin vertex assignment calls","",MB_OK);//failure message
	       IPhysiqueExport *phyExport = (IPhysiqueExport *)phyMod->GetInterface(I_PHYINTERFACE);
		   int phyVersion = phyExport->Version();
		   CString phyDesc = FixupName(node->GetName());
           phyDesc = "*PHYSIQUE NODE "+phyDesc;
	       IPhyContextExport *mcExport = (IPhyContextExport *)phyExport->GetContextInterface(node);
	       mcExport->ConvertToRigid(true);
		   
           BOOL blendedExport = GL_CommDlg.GL_enableBlendedExport;
		   if(blendedExport)
		      mcExport->AllowBlending(true);
		   else
		      mcExport->AllowBlending(false);

           int x = 0;
	       //float normalizedWeight;
	       Point3 offsetVector;
		   D3DVECTOR offsetVect2;

	       //these are the different types of rigid vertices supported
	       IPhyBlendedRigidVertex *rb_vtx;
	       IPhyRigidVertex *r_vtx;
	       //IPhyFloatingVertex *f_vtx;


	       int phyVertexCount = mesh->getNumFaces()*3;
	       
		   int vertexTrace = 0;
		   for(int i=0;i<mesh->getNumFaces();i++)
		   {//get vertex assignments
			   //get correct vertex 1
			   int indecieCurrent = 0;
			   for(int faceLoop=0;faceLoop<3;faceLoop++)
			   {//face loop
			   //IPhyRigidVertex *vtxExport  = (IPhyRigidVertex  *)contextExport->GetVertexInterface(indecieCurrent);
				   
			    if(faceLoop == 0)
                indecieCurrent = mesh->faces[i].v[vx1];
				else if(faceLoop == 1)
		        indecieCurrent = mesh->faces[i].v[vx3];
				else
				indecieCurrent = mesh->faces[i].v[vx2];

                //Get the hierarchial vertex interface for this vertex
		        IPhyVertexExport* vi = mcExport->GetVertexInterface(indecieCurrent);

				if(vi){
                    int type = vi->GetVertexType();
					int boneIndexesCount = 0;
					float * wieghtsToBone = NULL;
					byte * boneIndexes = NULL;
					ABVERTEX0L * offsetVectors  =NULL;
					float totalWeight = 0.0f;
					CString boneTracerName;

					switch (type)
			        {//we have a vertex assigned to more than one link
				       case RIGID_BLENDED_TYPE: 
					        //MessageBox(NULL,"Begin Blended","",MB_OK);//failure message
					        //type-cast the node to the proper class		
					        rb_vtx = (IPhyBlendedRigidVertex*)vi;
					        //iterate through all links this vertex is assigned to
                            //AddVertexAssignmentBlended(byte * boneIndexes,float * wieghtsToBone,int boneIndexesCount,int vertexIndex);
                            
							boneIndexesCount = rb_vtx->GetNumberNodes();
					        wieghtsToBone = new float[boneIndexesCount];
							offsetVectors = new ABVERTEX0L[boneIndexesCount];
					        boneIndexes = new byte[boneIndexesCount];
							ZeroMemory(wieghtsToBone,sizeof(float)*boneIndexesCount);
							ZeroMemory(boneIndexes,sizeof(byte)*boneIndexesCount);
							ZeroMemory(offsetVectors,sizeof(ABVERTEX0L)*boneIndexesCount);
                            totalWeight = 0.0f;

                           

					        for (x = 0; x<boneIndexesCount; x++)
					        {
								 INode *phyNode = rb_vtx->GetNode(x);
								 float weight = rb_vtx->GetWeight(x);

                                 if(weight < 0.0f)
								    weight = 0.0f;
								 if(weight > 1.0f)
                                    weight = 1.0f;

								 totalWeight += weight;

								 offsetVector = rb_vtx->GetOffsetVector(x);


								 TSTR nodeName = phyNode->GetName();
								 CString casted = nodeName;
								 boneTracerName = nodeName;
								 //CString weightStr = CABLIB.FloatToStingCnv(weight);
                                 int indexOfBone = 0;
								 CBoneObject * bonePtr = m_skeletalExport->m_bonesList->GetBoneObjectByName(FixupName(phyNode->GetName()),&indexOfBone);
						         if(bonePtr)
								 {
									 if(bonePtr->m_boneName == "Dummy02" || bonePtr->m_boneName ==  "Dummy20")
									 {
                                        wieghtsToBone[x] = weight;
									 }
								    
								    wieghtsToBone[x] = weight;
									boneIndexes[x] = indexOfBone;
									offsetVectors[x].x = offsetVector.x;
									offsetVectors[x].y = offsetVector.y;
									offsetVectors[x].z = offsetVector.z;
								 }
						         else
						            MessageBox(NULL,"Bone not found","",MB_OK);//failure message
					        }

							if(totalWeight > 1.2f)
						    {

                               MessageBox(NULL,"TotalWeight Issue","",MB_OK);//failure message
						    }

							if(boneIndexesCount > 0)
							AddVertexAssignmentBlended(boneIndexes,
										               wieghtsToBone,
													   offsetVectors,
										               boneIndexesCount,
										               vertexTrace,
										               physiqueCurrentIndex,
													   phyVertexCount);

							/*if(wieghtsToBone)
							   delete wieghtsToBone;
							   wieghtsToBone = 0;
						    if(boneIndexes)
							   delete boneIndexes;
							   boneIndexes = 0;*/
					
					   break;
					   case RIGID_TYPE: 
						   r_vtx = (IPhyRigidVertex*)vi;
						   INode *phyNode = r_vtx->GetNode();
					       if(phyNode)
						   {
						      int indexOfBone = 0;
						      CBoneObject * bonePtr = m_skeletalExport->m_bonesList->GetBoneObjectByName(FixupName(phyNode->GetName()),&indexOfBone);
						      if(bonePtr)
							  {
                                 


                                 if(!blendedExport)
								 {
									 
									
									 /*offsetVector = r_vtx->GetOffsetVector();
									 offsetVect2.x = offsetVector.x;
									 offsetVect2.y = offsetVector.y;
									 offsetVect2.z = offsetVector.z;*/
                                     AddVertexAssignment(indexOfBone,bonePtr,vertexTrace,physiqueCurrentIndex,offsetVect2);
									 
								 }
								 else
								 { 

                                     

					                 wieghtsToBone = new float[1];
									 offsetVectors = new ABVERTEX0L[1];
					                 boneIndexes = new byte[1];  
									 wieghtsToBone[0] = 1.0f;
									 boneIndexes[0] = indexOfBone;
									 offsetVector = r_vtx->GetOffsetVector();
									 offsetVectors[0].x = offsetVector.x;
									 offsetVectors[0].y = offsetVector.y;
									 offsetVectors[0].z = offsetVector.z;

									 if(bonePtr->m_boneName == "Dummy02" || bonePtr->m_boneName ==  "Dummy20")
								     {
                                        wieghtsToBone[0] = 1.0f;
							         }
									 
									 AddVertexAssignmentBlended(boneIndexes,
										                        wieghtsToBone,
																offsetVectors,
										                        1,
										                        vertexTrace,
										                        physiqueCurrentIndex,
																phyVertexCount);

									 /*if(wieghtsToBone)
									    delete [] wieghtsToBone;
									    wieghtsToBone = 0;

									 if(boneIndexes)
									    delete [] boneIndexes;
									    boneIndexes = 0;*/

								 }
							  }
						      else
						         MessageBox(NULL,"Bone not found","",MB_OK);//failure message
						   }
						   else
						   MessageBox(NULL,"Node NULL","",MB_OK);//failure message
						//all cool
					    break;	

						AfxMessageBox("UnknownVertexData");
					}
					
               
					mcExport->ReleaseVertexInterface(vi);
					vi = NULL;
					vertexTrace++;
				} 
				else
				MessageBox(NULL,"Failed to get vertex interface","",MB_OK);//failure message
			
               }//end face loop
		   }//end get vertex assignments
           //AfxMessageBox("Begin Apply1");
           POSITION sLoc = m_skeletalExport->m_deformableMeshArrayV2[physiqueCurrentIndex].m_defMeshVisualUnit->m_lodChain->FindIndex(0);
           if(sLoc){
              CDeformableVisObj * dVisPtr = (CDeformableVisObj *)m_skeletalExport->m_deformableMeshArrayV2[physiqueCurrentIndex].m_defMeshVisualUnit->m_lodChain->GetAt(sLoc);
 
              dVisPtr->m_mesh->m_abVertexArray.FillD3DVECTORArrayData(dVisPtr->m_originalArrayVector);
		   }
 

           

		    
		   phyExport->ReleaseContextInterface(mcExport);
		   phyMod->ReleaseInterface(I_PHYINTERFACE, phyExport);
		   GL_currentPhyModIndex++;
		   //AfxMessageBox("end Log vertex assignments");
		}//end physique exists
         //end get physique modifier
		
   return TRUE;
}


BOOL Exporter::AddVertexAssignment(int boneIndex,CBoneObject * focusBone,int vertecieIndex,int curPhyObj,D3DVECTOR offsetVector)
{						   
 //D3DMATRIX InverseBoneMatrix;
 //focusBone->m_boneFrame->GetInverseTransform(&InverseBoneMatrix);


 D3DVECTOR vectTrans;

 POSITION posSkl = m_skeletalExport->m_deformableMeshArrayV2[curPhyObj].m_defMeshVisualUnit->m_lodChain->FindIndex(0);
 if(posSkl){
    CDeformableVisObj * dPtr = (CDeformableVisObj *)m_skeletalExport->m_deformableMeshArrayV2[curPhyObj].m_defMeshVisualUnit->m_lodChain->GetAt(posSkl);
	
	
	vectTrans.x = dPtr->m_originalArrayVector[vertecieIndex].x;
    vectTrans.y = dPtr->m_originalArrayVector[vertecieIndex].y;
    vectTrans.z = dPtr->m_originalArrayVector[vertecieIndex].z;
    //vectTrans.x = offsetVector.x;
    //vectTrans.y = offsetVector.y;
    //vectTrans.z = offsetVector.z;
    
	dPtr->m_mesh->m_abVertexArray.SetPositionXYZ(vertecieIndex,vectTrans.x,vectTrans.y,vectTrans.z);
 //end set vertex to child position on bone
 dPtr->AddVertexAssignment(boneIndex,vertecieIndex);
 }
 return TRUE;
}

BOOL Exporter::AddVertexAssignmentBlended(byte * boneIndexes,
										  float * wieghtsToBone,
										  ABVERTEX0L * offsetVect,
										  int boneIndexesCount,
										  int vertecieIndex,
										  int curPhyObj,
										  int totalVertCount)
{						   
 
 

 POSITION posSkl = m_skeletalExport->m_deformableMeshArrayV2[curPhyObj].m_defMeshVisualUnit->m_lodChain->FindIndex(0);
 if(posSkl){
    CDeformableVisObj * dPtr = (CDeformableVisObj *)m_skeletalExport->m_deformableMeshArrayV2[curPhyObj].m_defMeshVisualUnit->m_lodChain->GetAt(posSkl);
    dPtr->m_blendedType = TRUE;
	D3DVECTOR finalPosition;
	finalPosition.x = dPtr->m_originalArrayVector[vertecieIndex].x;
    finalPosition.y = dPtr->m_originalArrayVector[vertecieIndex].y;
    finalPosition.z = dPtr->m_originalArrayVector[vertecieIndex].z;

    /*D3DVECTOR tempArray[10];
	int tracerLog = 0;
    for(int xBonesLoop=0;xBonesLoop<boneIndexesCount;xBonesLoop++)
	{
        CBoneObject * focusBone = m_skeletalExport->m_bonesList->GetBoneByIndex(boneIndexes[xBonesLoop]);
        if(focusBone)
		{
           if(wieghtsToBone[xBonesLoop] == 0.0f)
		      continue;


           D3DMATRIX InverseBoneMatrix;
           focusBone->m_boneFrame->GetInverseTransform(&InverseBoneMatrix);
           tempArray[tracerLog].x = (InverseBoneMatrix.m[0][0]*dPtr->m_originalArrayVector[vertecieIndex].x)+(InverseBoneMatrix.m[1][0]*dPtr->m_originalArrayVector[vertecieIndex].y)+(InverseBoneMatrix.m[2][0]*dPtr->m_originalArrayVector[vertecieIndex].z)+(InverseBoneMatrix.m[3][0]);
           tempArray[tracerLog].y = (InverseBoneMatrix.m[0][1]*dPtr->m_originalArrayVector[vertecieIndex].x)+(InverseBoneMatrix.m[1][1]*dPtr->m_originalArrayVector[vertecieIndex].y)+(InverseBoneMatrix.m[2][1]*dPtr->m_originalArrayVector[vertecieIndex].z)+(InverseBoneMatrix.m[3][1]);
           tempArray[tracerLog].z = (InverseBoneMatrix.m[0][2]*dPtr->m_originalArrayVector[vertecieIndex].x)+(InverseBoneMatrix.m[1][2]*dPtr->m_originalArrayVector[vertecieIndex].y)+(InverseBoneMatrix.m[2][2]*dPtr->m_originalArrayVector[vertecieIndex].z)+(InverseBoneMatrix.m[3][2]);
		   tracerLog++;
		}
	}
	//Blend algorithm
    D3DVECTOR finalPosition;
	 if(tracerLog > 0)
     {
		 if(tracerLog > 2)
		 {
            finalPosition.x = 0.0f;
		 }
						 
		finalPosition.x = tempArray[0].x;
	    finalPosition.y = tempArray[0].y;
	    finalPosition.z = tempArray[0].z;
        for(int blendLoop=1;blendLoop<tracerLog;blendLoop++)
	    {
            finalPosition = GL_matrixFunctions.InterpolatePointsHiRes(finalPosition,//D3DVECTOR StartVec, 
						                                           tempArray[blendLoop],//D3DVECTOR EndVec,
					                                               1.0f,//float NumOfStages, 
					                                               wieghtsToBone[blendLoop]);//float ThisStage);

		    }
		}
	    //end blend algorithm

	dPtr->m_mesh->m_abVertexArray.SetPositionXYZ(vertecieIndex,finalPosition.x,finalPosition.y,finalPosition.z);
 //end set vertex to child position on bone*/
 if(!dPtr->m_blendedVertexAssignments)
 {
	 dPtr->m_blendedVertexAssignments = new VertexAssignmentBlended[totalVertCount];
     ZeroMemory(dPtr->m_blendedVertexAssignments,sizeof(VertexAssignmentBlended)*totalVertCount);
 }
     dPtr->m_mesh->m_abVertexArray.SetPositionXYZ(vertecieIndex,finalPosition.x,finalPosition.y,finalPosition.z);
	 dPtr->m_blendedVertexAssignments[vertecieIndex].offsetVects = offsetVect;
     dPtr->m_blendedVertexAssignments[vertecieIndex].boneIndexesCount = boneIndexesCount;
     dPtr->m_blendedVertexAssignments[vertecieIndex].boneIndexes = boneIndexes;
	 dPtr->m_blendedVertexAssignments[vertecieIndex].weightsToBone = wieghtsToBone;
	 dPtr->m_blendedVertexAssignments[vertecieIndex].vertexIndex = vertecieIndex;


	 /*for(int calculateLoop = 0;calculateLoop<boneIndexesCount;calculateLoop++)
     {
         
         CBoneObject * bonePtr = m_skeletalExport->m_bonesList->GetBoneByIndex(boneIndexes[calculateLoop]);
	     if(bonePtr)
		 {
		                       D3DMATRIX boneMatrix;
                               bonePtr->m_boneFrame->GetInverseTransform(&boneMatrix);
			                   offsetVect[calculateLoop].x = (boneMatrix.m[0][0]*dPtr->m_originalArrayVector[vertecieIndex].x)+(boneMatrix.m[1][0]*dPtr->m_originalArrayVector[vertecieIndex].y)+(boneMatrix.m[2][0]*dPtr->m_originalArrayVector[vertecieIndex].z)+(boneMatrix.m[3][0]);
                               offsetVect[calculateLoop].y = (boneMatrix.m[0][1]*dPtr->m_originalArrayVector[vertecieIndex].x)+(boneMatrix.m[1][1]*dPtr->m_originalArrayVector[vertecieIndex].y)+(boneMatrix.m[2][1]*dPtr->m_originalArrayVector[vertecieIndex].z)+(boneMatrix.m[3][1]);
                               offsetVect[calculateLoop].z = (boneMatrix.m[0][2]*dPtr->m_originalArrayVector[vertecieIndex].x)+(boneMatrix.m[1][2]*dPtr->m_originalArrayVector[vertecieIndex].y)+(boneMatrix.m[2][2]*dPtr->m_originalArrayVector[vertecieIndex].z)+(boneMatrix.m[3][2]);
			                   //offsetVect[calculateLoop].nx = (boneMatrix.m[0][0]*tempVertexMass[x].nx)+(boneMatrix.m[1][0]*tempVertexMass[x].ny)+(boneMatrix.m[2][0]*tempVertexMass[x].nz);
                               //offsetVect[calculateLoop].ny = (boneMatrix.m[0][1]*tempVertexMass[x].nx)+(boneMatrix.m[1][1]*tempVertexMass[x].ny)+(boneMatrix.m[2][1]*tempVertexMass[x].nz);
                               //offsetVect[calculateLoop].nz = (boneMatrix.m[0][2]*tempVertexMass[x].nx)+(boneMatrix.m[1][2]*tempVertexMass[x].ny)+(boneMatrix.m[2][2]*tempVertexMass[x].nz);
                               offsetVect[calculateLoop].nx = 0.0f;
							   offsetVect[calculateLoop].ny = 1.0f;
							   offsetVect[calculateLoop].nz = 0.0f;
	     }
	                   
	 }*/

     

	 dPtr->m_assignmentCount++;
	
	//dPtr->AddVertexAssignmentBlended(boneIndexes,wieghtsToBone,boneIndexesCount,vertecieIndex);
 }
 
 return TRUE;
}



BOOL Exporter::TMNegParity(Matrix3 &m)
{
	return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0)?1:0;
}

TriObject* Exporter::GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) { 
		TriObject *tri = (TriObject *) obj->ConvertToType(t, 
			Class_ID(TRIOBJ_CLASS_ID, 0));
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else {
		return NULL;
	}
}

Modifier* Exporter::FindPhysiqueModifier (INode* nodePtr)
{
	// Get object from node. Abort if no object.
	Object* ObjectPtr = nodePtr->GetObjectRef();
	

	if (!ObjectPtr) return NULL;

	// Is derived object ?
	if (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		// Yes -> Cast.
		IDerivedObject* DerivedObjectPtr = static_cast<IDerivedObject*>(ObjectPtr);

		// Iterate over all entries of the modifier stack.
		int ModStackIndex = 0;
		while (ModStackIndex < DerivedObjectPtr->NumModifiers())
		{
			// Get current modifier.
			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);

			// Is this Physique ?
			if (ModifierPtr->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
			{
				// Yes -> Exit.
				return ModifierPtr;
			}

			// Next modifier stack entry.
			ModStackIndex++;
		}
	}

	
	// Not found.
	return NULL;
}


BOOL Exporter::TestIfBipedObject (INode* nodePtr)
{
   if (nodePtr)
   {
       // Get the node's transform control
       Control *c = nodePtr->GetTMController();

       // You can test whether or not this is a biped controller with the following pseudo code:
       if ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
           (c->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
           (c->ClassID() == FOOTPRINT_CLASS_ID))
	   {
           return TRUE;
           // Get the Biped Export Interface from the controller 
           //IBipedExport *BipIface = (IBipedExport *) c->GetInterface(I_BIPINTERFACE);
 
           // Remove the non uniform scale
           //BipIface->RemoveNonUniformScale(1);
	   }
	   else
		   return FALSE;
   }
   return FALSE;
}
//-------------------------------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
BOOL Exporter::GetLightTypeFromNode(INode* node, BOOL boundingBox, D3DVECTOR * min, D3DVECTOR * max)
{
    if(!node)
		return FALSE;
    Object* objectPtr = node->GetObjectRef();

    


	if(!objectPtr)
	    return FALSE;

	//if(objectPtr->SuperClassID() != LIGHT_CLASS_ID)
		//return FALSE;

	        

	

	ObjectState os = node->EvalWorldState( 0 );

	if(os.obj->SuperClassID() != LIGHT_CLASS_ID)
	   return FALSE;
    LightState structLS;
    GenLight *genlight = (GenLight*)os.obj;
    if(!genlight)
		return FALSE;

    Interval valid = FOREVER;
    genlight->EvalLightState( 0, valid, &structLS );

	CString name = FixupName(node->GetName());

	Matrix3 pivot = node->GetNodeTM(0);
			D3DMATRIX matrixCreation;
			GL_matrixFunctions.InitMat(matrixCreation.m);
			Point3 row;
			row = pivot.GetRow(0);
			matrixCreation._11 = row.x;
			matrixCreation._12 = row.y;
			matrixCreation._13 = row.z;
			row = pivot.GetRow(1);
			matrixCreation._21 = row.x;
			matrixCreation._22 = row.y;
			matrixCreation._23 = row.z;
			row = pivot.GetRow(2);
			matrixCreation._31 = row.x;
			matrixCreation._32 = row.y;
			matrixCreation._33 = row.z;
			row = pivot.GetRow(3);
			matrixCreation._41 = row.x;
			matrixCreation._42 = row.y;
			matrixCreation._43 = row.z;
			D3DXMATRIX tempX = matrixCreation;
			//Transpose
			//swap column 1 and 2
			matrixCreation._12 = tempX._13;
			matrixCreation._22 = tempX._23;
			matrixCreation._32 = tempX._33;
			matrixCreation._42 = tempX._43;
			matrixCreation._13 = tempX._12;
			matrixCreation._23 = tempX._22;
			matrixCreation._33 = tempX._32;
			matrixCreation._43 = tempX._42;
            tempX = matrixCreation;
			//swap then row 1 and 2 for matrices
			matrixCreation._21 = tempX._31;
			matrixCreation._22 = tempX._32;
			matrixCreation._23 = tempX._33;
			matrixCreation._24 = tempX._34;
			matrixCreation._31 = tempX._21;
			matrixCreation._32 = tempX._22;
			matrixCreation._33 = tempX._23;
			matrixCreation._34 = tempX._24;

	//float structLS.attenStart;
    //float structLS.attenEnd;
	//structLS.color
    D3DVECTOR dir;
	ZeroMemory(&dir,sizeof(D3DVECTOR));
	//if(genlight->Type() == OMNI_LGT ||  genlight->Type() == SPOT_LGT)
	//{//omni
		if(!m_lightCache)
            m_lightCache = new CReferenceImpExpList();

		if(!boundingBox)
        m_lightCache->AddNewReference(name,//light name
			                          1,//export as light
									  matrixCreation,//matrix of the light
									  structLS.color.r,
									  structLS.color.g,
									  structLS.color.b,
									  structLS.attenStart,
									  structLS.attenEnd,
									  structLS.intens,
									  0,//structLS.type,
									  dir,
									  0.0f,//theta
									  0.0f,//phi
									  0.0f,//falloff
									  GL_CommDlg.GL_sightRange);

		if(boundingBox)
        m_lightCache->AddNewReference(name,//light name
			                          1,//export as light
									  matrixCreation,//matrix of the light
									  structLS.color.r,
									  structLS.color.g,
									  structLS.color.b,
									  structLS.attenStart,
									  structLS.attenEnd,
									  structLS.intens,
									  0,//structLS.type,
									  dir,
									  0.0f,//theta
									  0.0f,//phi
									  0.0f,//falloff
									  GL_CommDlg.GL_sightRange,
									  boundingBox,
									  max->x,
									  max->y,
									  max->z,
									  min->x,
									  min->y,
									  min->z
									  );
	/*}//end omni
	if(genlight->Type() == SPOT_LGT )
	{//spot

        //m_lightCache->AddNewReference(


	}//end spot*/
	

	return FALSE;
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
CString Exporter::GetMaterialFromID(INode* node,
									int id,
									D3DMATERIAL9 ** d3dMaterial,
									CString * refTexture2,
									CString * refTexture3,
									CString * refTexture4,
									CString * refTexture5)
{
	//Mesh*m=NULL;
	Mtl* mtl = node->GetMtl();
	if(!mtl)
		return "NONE";


    MtlBase* baseMat = mtl->GetActiveTexmap();

    //MAX 7 specific
	BOOL continueThis = TRUE;

	TSTR bufTemp;
    mtl->GetClassName(bufTemp);


    Class_ID classIDTemp = mtl->ClassID();

	if(classIDTemp.PartA() == 597)
	{//shell material
		int possibilities = mtl->NumSubMtls();
		if(possibilities > 1)
		   mtl = mtl->GetSubMtl(1);
		else if(possibilities > 0)
		   mtl = mtl->GetSubMtl(0);
	}//end shell material
	
	if(mtl->NumSubMtls() > 0){
       int subNum = mtl->NumSubMtls();
	   if(id > subNum){//adjust for a kntx mishap
          id = id - subNum;
	   }//end adjust for a kntx mishap
	   mtl = mtl->GetSubMtl(id);
	}

	if(!mtl)
	   return "NONE";

	if (mtl->ClassID() != Class_ID(DMTL_CLASS_ID, 0))
        return "NONE";

	if(d3dMaterial)
	{
	  D3DMATERIAL9 * newmaterial = new D3DMATERIAL9;
	  Color ambient = mtl->GetAmbient(0,FALSE);
	  Color diffuse = mtl->GetDiffuse(0,FALSE);
	  Color specular = mtl->GetSpecular(0,FALSE);
	  Color emmisive = mtl->GetSelfIllumColor(0,FALSE);
	  float specPower = 100.0f * mtl->GetShininess(0,FALSE);
	  float translucency = 1.0f - mtl->GetXParency(0,FALSE);
	  if(mtl->GetSelfIllumColorOn(0,FALSE)==FALSE)
	  {
         emmisive.r = 0.0f;
	     emmisive.g = 0.0f;
	     emmisive.b = 0.0f;
	  }
	  newmaterial->Diffuse.r = diffuse.r;
      newmaterial->Diffuse.g = diffuse.g;
      newmaterial->Diffuse.b = diffuse.b;
      newmaterial->Diffuse.a = translucency;
      newmaterial->Ambient.r = ambient.r;
      newmaterial->Ambient.g = ambient.g;
      newmaterial->Ambient.b = ambient.b;
      newmaterial->Ambient.a = 1.0f;
      newmaterial->Specular.r = specular.r;
      newmaterial->Specular.g = specular.g;
      newmaterial->Specular.b = specular.b;
      newmaterial->Specular.a = 0.0f;
      newmaterial->Emissive.r = emmisive.r;
      newmaterial->Emissive.g = emmisive.g;
      newmaterial->Emissive.b = emmisive.b;
      newmaterial->Emissive.a = 0.0f;
      newmaterial->Power =  specPower;	
	  *d3dMaterial = newmaterial;
	}
    
	// If the node does not have a material, export the wireframe color
	  if (mtl) {
        //mtl->ClassID();

		//if (mtl->ClassID() != Class_ID(DMTL_CLASS_ID, 0))
		/*{//find material that will work
            mtl = GetSubMtl(0);

		}//end find material that will work*/

	    //if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) {
      
		        //StdMat* std = (StdMat*)mtl;
				
				Texmap* subTex = mtl->GetSubTexmap(1);//get diffuse map
				CString diffuseTextMap = GetTextureFromSubTex(subTex);
				     
				if(refTexture2)
				{
				   Texmap* subTexTemp = mtl->GetSubTexmap(2);
				   *refTexture2 = GetTextureFromSubTex(subTexTemp);
				}
				if(refTexture3)
				{
				   Texmap* subTexTemp = mtl->GetSubTexmap(3);
				   *refTexture3 = GetTextureFromSubTex(subTexTemp);
				}
				if(refTexture4)
				{
				   Texmap* subTexTemp = mtl->GetSubTexmap(4);
				   *refTexture4 = GetTextureFromSubTex(subTexTemp);
				}
				if(refTexture5)
				{
				   Texmap* subTexTemp = mtl->GetSubTexmap(5);
				   *refTexture5 = GetTextureFromSubTex(subTexTemp);
				}




				return diffuseTextMap;
				     
					 /*if(subTex){
                        if (subTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00)){
		                  
						  LPSTR mapName = ((BitmapTex *)subTex)->GetMapName();
							//format texture name
                          int size = strlen(mapName);
						   if(size != 0){
							int startPoint = size;
							
							for(int loop=size;loop>0;loop--){
								if(mapName[loop] == '\\'){
								   startPoint++;
								   break;
								}
								startPoint--;
							}
							int trace = 0;
							CString FormatedFilename;
							
							for(loop=startPoint;loop<size;loop++){
								FormatedFilename = FormatedFilename+mapName[loop];
								trace++;
							}
							return FormatedFilename;
						   }
						}
					 }*/
				   //}
		//}
	  }
return "NONE";
}

//DPD Sort Alphabetically
static BOOL SortAlpha(Exporter *imp, CString *List)
{
	CString *temp = new CString;

	//bubblesort, yes it sucks, but this list is small
	for (;;) 
	{
		bool swapped = false;
		for (int i = 0; i < imp->GL_CommDlg.nodeCount - 1; i++) 
		{
			int j = i + 1;
			if (List[i].CompareNoCase(List[j]) > 0)
			{
				*temp = List[i];
				List[i] = List[j];
				List[j] = *temp;
				swapped = true;
			}
		}
    if (!swapped)
      break;
	}
	return TRUE;
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
CString Exporter::GetTextureFromSubTex(Texmap* subTex)
{
   if(subTex)
   {
    if (subTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
	{
	    LPSTR mapName = ((BitmapTex *)subTex)->GetMapName();
	    //format texture name
        int size = strlen(mapName);
	    if(size != 0){
		   int startPoint = size;
		   int loop=size;		
		   for(loop=size;loop>0;loop--){
		     if(mapName[loop] == '\\'){
		        startPoint++;
			 break;
		   }
		   startPoint--;
		   }
		   int trace = 0;
		   CString FormatedFilename;
		   for(loop=startPoint;loop<size;loop++)
		   {
			   FormatedFilename = FormatedFilename+mapName[loop];
			   trace++;
		   }
		   return FormatedFilename;
		}				
	}				 
   }
   return "NONE";
}

//DPD
BOOL Exporter::NodeHasShader(INode* node)
{
	//Get Material ptr
	Mtl* material = node->GetMtl();
	
	if((!material)||(material->NumParamBlocks() < 2))
		return FALSE;

	IParamBlock2* pb	= material->GetParamBlock(1);   //Material Block
	CString		  name  = (CString)pb->GetLocalName(0); //Effect File

	if(name != "EffectFile")
		return FALSE;
		
	//Has Shader 
	return TRUE;
}

//DPD
int Exporter::SetShaderTextures(LPD3DXEFFECT effect, INode* node, CMaterialObject *matObj, CString *textures)
{
	D3DXPARAMETER_DESC desc;

	for(int i=0; i<8; i++)
		textures[i] = "NONE";

	int paramCount = matObj->GetParameterCount(effect);
	int textureCount = 0;

	for(int i=0; i<paramCount; i++)
	{
		D3DXHANDLE handle = effect->GetParameter(NULL, i);
		if(handle)
		{
			if(effect->GetParameterDesc(handle, &desc) == S_OK)
			{
				if(desc.Type == D3DXPT_TEXTURE)//we have a texture
				{
					if(matObj->m_peramBlock[i].svData == 1)//user defined 
					{
						//textures[textureCount] = matObj->m_peramBlock[i].data;
						char* tempString = new char[matObj->m_peramBlock[i].dataSize+1];
						ZeroMemory(tempString,matObj->m_peramBlock[i].dataSize+1);
						CopyMemory(tempString,matObj->m_peramBlock[i].data, matObj->m_peramBlock[i].dataSize);
						textures[textureCount] = tempString;
						delete [] tempString;
						tempString = 0;
					} 
					else
					{
						LPDIRECT3DBASETEXTURE9 pTexture = NULL;
						int tmp = effect->GetTexture(handle, &pTexture);
//						D3DRESOURCETYPE tmpType = pTexture->GetType();
						tmp = tmp;
					}
					textureCount++;
				}
				else if(desc.Type == D3DXPT_FLOAT)
				{
					FLOAT f;
					int tmp = effect->GetFloat(handle, &f);
					f=f;
				}
			}
		}
	}

	return textureCount;
}


//DPD
LPD3DXEFFECT Exporter::SetupEffectFile( INode* node, LPDIRECT3DDEVICE9 device, CString * fxReturn)
{
	CString fxCompleteName, fxFile;

	Mtl* material = node->GetMtl();
	int  numParamBlocks	= material->NumParamBlocks();

	for(int j=0; j<numParamBlocks; j++)
	{
		if( (j==2) || (j==4) )//2=Software Render Block 4=Unused
			continue;

		IParamBlock2* pb = material->GetParamBlock(j);
		int numParams = pb->NumParams();
		
		for(int i=0; i<numParams; i++)
		{
			CString fname = (CString)pb->GetLocalName(i);
			ParamType2 ptype = pb->GetParameterType(i);
			switch(ptype)
			{
			case TYPE_FILENAME:
				if(fname == "EffectFile")
				{
					TCHAR* filename;

					pb->GetValue(i, 0, filename, FOREVER, 0);
					CString fxName = (CString)filename;
					fxCompleteName = fxName;

                    CFileException exc;
                    CFile fxFileHandle;
					BOOL res = fxFileHandle.Open(fxName,CFile::modeRead, &exc);
	                if(res == FALSE){
						AfxMessageBox("Cannot aquire:"+fxName);
					   return NULL;
	                }

					//CFile * fxFile = new CFile(filename, CFile::modeReadWrite );

                    if(fxFileHandle.GetLength() == 0)
					{
                       AfxMessageBox("File Zero Length:"+fxName);
                       return NULL;
					}

					BYTE * fileBytes = new BYTE[fxFileHandle.GetLength()+1];
					ZeroMemory(fileBytes,fxFileHandle.GetLength()+1);

		
	                fxFileHandle.Read(fileBytes,fxFileHandle.GetLength());
					fxFile = (char*)fileBytes;
	                fxFileHandle.Close();
				}
				break;

			default:
				break;
			}
		}
	}

    LPD3DXBUFFER pErrors;
	LPD3DXEFFECT effect;
	HRESULT result;
	DWORD dwFlags;
	dwFlags = D3DXSHADER_DEBUG;

	int fileSize = fxFile.GetLength();

	result = D3DXCreateEffect(device, fxFile, fileSize, NULL, NULL, dwFlags, NULL, &effect, &pErrors); 

	*fxReturn = fxFile;

	if(pErrors)
	{
		int tSize = pErrors->GetBufferSize();
		char* BufferE = (char*)pErrors->GetBufferPointer();
	}

	if(result != D3D_OK)
		return NULL;

	return effect;
}

//DPD function to search
int searchEffect(LPD3DXEFFECT effect, int numParams, CString searchTerm )
{
	D3DXPARAMETER_DESC desc;
	CString strTemp;
	int foundIdx = -1;

	for(int i=0; i<numParams; i++)
	{
		D3DXHANDLE handle = effect->GetParameter(NULL, i);
		if(handle)
		{
			if(effect->GetParameterDesc(handle, &desc) == S_OK)
			{
				strTemp = desc.Name;
				strTemp.MakeUpper();
				searchTerm.MakeUpper();
				if(searchTerm == strTemp)
				{
					foundIdx = i;
					break;
				}
			}
		}
	}

	return foundIdx;
}

//DPD
BOOL Exporter::GetShaderInfo(CMaterialObject* matObj, INode* node, LPD3DXEFFECT effect, CString & fxInput)
{
	ParamType2	ptype;
	PBBitmap*	bitmap;
	TCHAR*		filename;
	float		floatVal;
	AColor		colorVal;
	int			tech, intVal, effectIndex;
	CString		fname, cS, fxName, fxFile, stringVal;
	//byte		*tByte;

	Mtl* material = node->GetMtl();

	int numParamBlocks = material->NumParamBlocks();

	IParamBlock2* pb = material->GetParamBlock(1);
	
	//Get EffectFile Name first for BindString
	fname = (CString)pb->GetLocalName(0);
	if(fname == "EffectFile")
	{
		pb->GetValue(0, 0, filename, FOREVER, 0);
		fxName = (CString)filename;
		int a1,a2;
		a1 = fxName.GetLength();
		a2 = fxName.ReverseFind( '\\' );
		fxName = fxName.Right( a1 - a2 - 1 );
	}
	matObj->m_fxBindString = fxName + "*";
	fxName = "";
		
	for(int j=0; j<numParamBlocks; j++)
	{
		if( (j==2) || (j==4) )//2=Software Render Block 4=Unused
			continue;

		pb = material->GetParamBlock(j);
		int numParams = pb->NumParams();
		BOOL result = 0;
		for(int i=0; i<numParams; i++)
		{
			fname = (CString)pb->GetLocalName(i);
			ptype = pb->GetParameterType(i);
			switch(ptype)
			{
			case TYPE_FLOAT:
				result = pb->GetValue(i, 0, floatVal, FOREVER, 0);
				effectIndex = searchEffect(effect, matObj->m_peramCount, fname);
				if(effectIndex > -1 && effectIndex < matObj->m_peramCount)
				{
				   matObj->m_peramBlock[effectIndex].type = ARBP_USEDEFAULT;
				   matObj->m_peramBlock[effectIndex].dataSize = sizeof(float);
				   matObj->m_peramBlock[effectIndex].data = new byte[sizeof(float)];
				   CopyMemory(matObj->m_peramBlock[effectIndex].data, &floatVal, sizeof(float));
				   matObj->m_peramBlock[effectIndex].svData = 1;
				   break;
				}
				
			case TYPE_INT:
				pb->GetValue(i, 0, intVal, FOREVER, 0);
				effectIndex = searchEffect(effect, matObj->m_peramCount, fname);
				if(effectIndex > -1 && effectIndex < matObj->m_peramCount)
				{
				   matObj->m_peramBlock[effectIndex].type = ARBP_USEDEFAULT;
				   matObj->m_peramBlock[effectIndex].dataSize = sizeof(int);
				   matObj->m_peramBlock[effectIndex].data = new byte[sizeof(int)];
				   CopyMemory(matObj->m_peramBlock[effectIndex].data, &intVal, sizeof(int));
				   matObj->m_peramBlock[effectIndex].svData = 1;
				}
				break;

			case TYPE_RGBA:
			case TYPE_FRGBA://NOT SURE WHY? BUT THIS IS WHAT IT RETURNS
				pb->GetValue(i, 0, colorVal, FOREVER, 0);
				effectIndex = searchEffect(effect, matObj->m_peramCount, fname);
				if(effectIndex > -1 && effectIndex < matObj->m_peramCount)
				{
				   matObj->m_peramBlock[effectIndex].type = ARBP_USEDEFAULT;
				   matObj->m_peramBlock[effectIndex].dataSize = sizeof(AColor);
				   matObj->m_peramBlock[effectIndex].data = new byte[sizeof(AColor)];
				   CopyMemory(matObj->m_peramBlock[effectIndex].data, &colorVal, sizeof(AColor));
				   matObj->m_peramBlock[effectIndex].svData = 1;
				}
				break;

			//case TYPE_POINT3:
			//case TYPE_BOOL:
			//case TYPE_ANGLE:
			//case TYPE_STRING:
			case TYPE_FILENAME:
				/*if(fname == "EffectFile")
				{
					pb->GetValue(i, 0, filename, FOREVER, 0);
					fxName = (CString)filename;
                    fxName = "";
					int a1,a2;
					a1 = fxName.GetLength();
					a2 = fxName.ReverseFind( '\\' );
					fxName = fxName.Right( a1 - a2 - 1 );
					
					CStdioFile file(LPCTSTR(filename), CFile::modeRead);
					CString strTemp;
					while(file.ReadString(strTemp))
					{
						fxFile += strTemp;
					}
				}*/
				break;

			//case TYPE_TEXMAP:
			case TYPE_BITMAP:
				pb->GetValue(i, 0, bitmap, FOREVER, 0);
				stringVal = (CString)bitmap->bi.Filename();
				effectIndex = searchEffect(effect, matObj->m_peramCount, fname);
				if(effectIndex > -1 && effectIndex < matObj->m_peramCount)
				{//verify range
				   matObj->m_peramBlock[effectIndex].type = ARBP_Texture;//revised ARB
				   matObj->m_peramBlock[effectIndex].dataSize = stringVal.GetLength();
				   if(stringVal.GetLength() > 0)
					  matObj->m_peramBlock[effectIndex].data = new byte[stringVal.GetLength()];
				   else
					  break;

				   for(int i=0; i<stringVal.GetLength(); i++)
				   {
                      matObj->m_peramBlock[effectIndex].data[i] = stringVal[i];
				   }
				   matObj->m_peramBlock[effectIndex].svData = 1;
				}//end verify range

				break;
			
			//case TYPE_INDEX:
			//case TYPE_MATRIX3:
			default:
				break;
			}
		}
	}

	//set alpha blending on/off
	if((fxInput.Find("AlphaBlendEnable = TRUE",0)) != -1)
		matObj->m_blendMode = 0;



	//Finish Bind String
	matObj->m_fxBindString += fxInput;	
	//pb->ReleaseInterface()
	
	return TRUE;
}



//WINAPI
//Callback
BOOL WINAPI CALLBACK Exporter::ExporterOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) 
{
    static Exporter *imp = NULL;
	int loop=0;
	int selectedMeshOutput=0;
	char bufferChar[250];

	        switch(message) {
		      case WM_INITDIALOG:
				  
			    imp = (Exporter *)lParam;
			    CenterWindow(hWnd,GetParent(hWnd));
				     SendDlgItemMessage(hWnd,IDC_RADIO_AnimSpeed1X,BM_SETCHECK ,BST_CHECKED,0);
					 SendDlgItemMessage(hWnd,IDC_CHECK_SMOOTHSHADING,BM_SETCHECK ,BST_UNCHECKED,0);
					 SetDlgItemInt(hWnd,IDC_EDIT_NODECOUNT,GL_CommDlg.nodeCount,TRUE);
					 SetDlgItemInt(hWnd,IDC_EDIT_TREE_THDuration,300,TRUE);
					 SetDlgItemInt(hWnd,IDC_EDIT_SIGHTRANGEBASIS,50000,TRUE);
					 SetDlgItemText(hWnd,IDC_EDIT_toleranceAngle,GL_CommDlg.GL_smShadeToleranceAngle);
				     
					 //DPD Sort alphabetically
					 SortAlpha(imp, GL_CommDlg.GL_SceneNodes);
					 
					 for(loop=0;loop<GL_CommDlg.nodeCount;loop++){
					     SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)GL_CommDlg.GL_SceneNodes[loop]);
					 }
					 SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_SETCURSEL ,0,0);
					 SendDlgItemMessage(hWnd,IDC_CHECK_enableBlending,BM_SETCHECK ,BST_CHECKED,0);
					 
			   return TRUE;

		       case WM_CLOSE:
			   EndDialog(hWnd, 0);

               case WM_COMMAND:
            

		         switch (LOWORD(wParam)) {
		         case IDOK:
					  
					  //------------------------------
					  //handle list box node selection
					  GL_CommDlg.selectionCount = SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELCOUNT ,0,0);
					  if(GL_CommDlg.selectionCount > 0){//somthin selected
						 GL_CommDlg.selectionBuffer = new int[GL_CommDlg.selectionCount+1];
						 SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELITEMS ,(GL_CommDlg.selectionCount+1),(LPARAM)(LPINT)GL_CommDlg.selectionBuffer);
					  }//end somthin selected
					  if(SendDlgItemMessage(hWnd,IDC_CHECK_enableBlending,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_enableBlendedExport = TRUE;
					  else
						 GL_CommDlg.GL_enableBlendedExport = FALSE;
					  //end handle list box node selection
                      if(SendDlgItemMessage(hWnd,IDC_CHECK_GenerateTangents,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_generateTangents = TRUE;
					  else
						 GL_CommDlg.GL_generateTangents = FALSE;

					  

					  GetDlgItemText(hWnd,IDC_EDIT_toleranceAngle,bufferChar,250);
                      GL_CommDlg.GL_smShadeToleranceAngle = bufferChar;
					  //------------------------------
					  //animation speed handle
                      if(SendDlgItemMessage(hWnd,IDC_RADIO_AnimSpeed1X,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_animSpeedAdjust = 2;
                      if(SendDlgItemMessage(hWnd,IDC_RADIO_AnimSpeed2X,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_animSpeedAdjust = 4;
					  if(SendDlgItemMessage(hWnd,IDC_RADIO_AnimSpeed4X,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_animSpeedAdjust = 8;
					  if(SendDlgItemMessage(hWnd,IDC_RADIO_AnimSpeed1_2X,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_animSpeedAdjust = 1;
					  
					  
					  if(SendDlgItemMessage(hWnd,IDC_CHECK_SMOOTHSHADING,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_generateNormals = TRUE;
					  else
						 GL_CommDlg.GL_generateNormals = FALSE;
					  GL_CommDlg.GL_outputMemoryType = 1;//to file
					  
					  
					  //end animation speed handle
					  //if(SendDlgItemMessage(hWnd,IDC_RADIO_HIARCHYBASED,   BM_GETSTATE,0,0)==BST_CHECKED)
					   //  GL_CommDlg.GL_exportType = 0;//hiarchy based
                      //if(SendDlgItemMessage(hWnd,IDC_RADIO_WORLDEXPORTTYPE,BM_GETSTATE,0,0)==BST_CHECKED)
					  //   GL_CommDlg.GL_exportType = 2;//world export based
                      

			          EndDialog(hWnd, 1);
			          break;
		         
				 case IDC_BUTTON_SelectAll: 
				      for(loop=0;loop<GL_CommDlg.nodeCount;loop++)
					     SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_SETSEL,(WPARAM) (BOOL)TRUE,(LPARAM)(UINT)loop);
					 
				      break;

                 case IDC_BUTTON_UnselectAll:
                      for(loop=0;loop<GL_CommDlg.nodeCount;loop++)
					     SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_SETSEL,(WPARAM) (BOOL)FALSE,(LPARAM)(UINT)loop);
					 
				      break;

                 case IDC_BUTTON_SENDOBJECTVISUALS:

                      if(SendDlgItemMessage(hWnd,IDC_CHECK_ExportFrameStates,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_exportAllKeys = TRUE;
					  else
						 GL_CommDlg.GL_exportAllKeys = FALSE;

                      if(SendDlgItemMessage(hWnd,IDC_CHECK_SMOOTHSHADING,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_generateNormals = TRUE;
					  else
						 GL_CommDlg.GL_generateNormals = FALSE;

                      if(SendDlgItemMessage(hWnd,IDC_CHECK_enableBlending,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_enableBlendedExport = TRUE;
					  else
						 GL_CommDlg.GL_enableBlendedExport = FALSE;

					  if(SendDlgItemMessage(hWnd,IDC_CHECK_GenerateTangents,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_generateTangents = TRUE;
					  else
						 GL_CommDlg.GL_generateTangents = FALSE;

					  

                      GetDlgItemText(hWnd,IDC_EDIT_SIGHTRANGEBASIS,bufferChar,250);
                      GL_CommDlg.GL_sightRange = atof(bufferChar);

					  GetDlgItemText(hWnd,IDC_EDIT_toleranceAngle,bufferChar,250);
                      GL_CommDlg.GL_smShadeToleranceAngle = bufferChar;
                      
                      GL_CommDlg.selectionCount = SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELCOUNT ,0,0);
					  if(GL_CommDlg.selectionCount > 0){//somthin selected
						 GL_CommDlg.selectionBuffer = new int[GL_CommDlg.selectionCount+1];
						 SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELITEMS ,(GL_CommDlg.selectionCount+1),(LPARAM)(LPINT)GL_CommDlg.selectionBuffer);
					  }//end somthin selected
					  GL_CommDlg.GL_exportType = 1;
					  GL_CommDlg.GL_outputMemoryType = 0;//memory mapping
					  EndDialog(hWnd, 1);
					  break;

                 case IDC_BUTTON_SENDOBJECTVISUALSTOFILE:
					  
                      if(SendDlgItemMessage(hWnd,IDC_CHECK_ExportFrameStates,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_exportAllKeys = TRUE;
					  else
						 GL_CommDlg.GL_exportAllKeys = FALSE;


					  if(SendDlgItemMessage(hWnd,IDC_CHECK_SMOOTHSHADING,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_generateNormals = TRUE;
					  else
						 GL_CommDlg.GL_generateNormals = FALSE;

                      if(SendDlgItemMessage(hWnd,IDC_CHECK_enableBlending,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_enableBlendedExport = TRUE;
					  else
						 GL_CommDlg.GL_enableBlendedExport = FALSE;

					  if(SendDlgItemMessage(hWnd,IDC_CHECK_GenerateTangents,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_generateTangents = TRUE;
					  else
						 GL_CommDlg.GL_generateTangents = FALSE;

                      GetDlgItemText(hWnd,IDC_EDIT_SIGHTRANGEBASIS,bufferChar,250);
                      GL_CommDlg.GL_sightRange = atof(bufferChar);

					  GetDlgItemText(hWnd,IDC_EDIT_toleranceAngle,bufferChar,250);
                      GL_CommDlg.GL_smShadeToleranceAngle = bufferChar;

                      GL_CommDlg.selectionCount = SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELCOUNT ,0,0);
					  if(GL_CommDlg.selectionCount > 0){//somthin selected
						 GL_CommDlg.selectionBuffer = new int[GL_CommDlg.selectionCount+1];
						 SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELITEMS ,(GL_CommDlg.selectionCount+1),(LPARAM)(LPINT)GL_CommDlg.selectionBuffer);
					  }//end somthin selected
					  GL_CommDlg.GL_exportType = 1;
					  GL_CommDlg.GL_outputMemoryType = 1;//to file
					  GL_CommDlg.GL_miscLoadInstruction = 0;
					  EndDialog(hWnd, 1);
					  break;

			     case IDC_BUTTON_UPDATEGROUP:
					  
                      if(SendDlgItemMessage(hWnd,IDC_CHECK_ExportFrameStates,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_exportAllKeys = TRUE;
					  else
						 GL_CommDlg.GL_exportAllKeys = FALSE;


					  if(SendDlgItemMessage(hWnd,IDC_CHECK_SMOOTHSHADING,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_generateNormals = TRUE;
					  else
						 GL_CommDlg.GL_generateNormals = FALSE;

                      if(SendDlgItemMessage(hWnd,IDC_CHECK_enableBlending,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_enableBlendedExport = TRUE;
					  else
						 GL_CommDlg.GL_enableBlendedExport = FALSE;

					  if(SendDlgItemMessage(hWnd,IDC_CHECK_GenerateTangents,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_generateTangents = TRUE;
					  else
						 GL_CommDlg.GL_generateTangents = FALSE;

                      GetDlgItemText(hWnd,IDC_EDIT_SIGHTRANGEBASIS,bufferChar,250);
                      GL_CommDlg.GL_sightRange = atof(bufferChar);

					  GetDlgItemText(hWnd,IDC_EDIT_toleranceAngle,bufferChar,250);
                      GL_CommDlg.GL_smShadeToleranceAngle = bufferChar;

                      GL_CommDlg.selectionCount = SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELCOUNT ,0,0);
					  if(GL_CommDlg.selectionCount > 0){//somthin selected
						 GL_CommDlg.selectionBuffer = new int[GL_CommDlg.selectionCount+1];
						 SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELITEMS ,(GL_CommDlg.selectionCount+1),(LPARAM)(LPINT)GL_CommDlg.selectionBuffer);
					  }//end somthin selected
					  GL_CommDlg.GL_exportType = 1;
					  GL_CommDlg.GL_outputMemoryType = 0;//to file
					  GL_CommDlg.GL_miscLoadInstruction = 1;
					  EndDialog(hWnd, 1);
					  break;

				 case IDC_ExportARBToEngine:
					  
                      //------------------------------
					  //handle list box node selection
					  GL_CommDlg.selectionCount = SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELCOUNT ,0,0);
					  if(GL_CommDlg.selectionCount > 0){//somthin selected
						 GL_CommDlg.selectionBuffer = new int[GL_CommDlg.selectionCount+1];
						 SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELITEMS ,(GL_CommDlg.selectionCount+1),(LPARAM)(LPINT)GL_CommDlg.selectionBuffer);
					  }//end somthin selected
					  if(SendDlgItemMessage(hWnd,IDC_CHECK_enableBlending,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_enableBlendedExport = TRUE;
					  else
						 GL_CommDlg.GL_enableBlendedExport = FALSE;

					  if(SendDlgItemMessage(hWnd,IDC_CHECK_GenerateTangents,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_generateTangents = TRUE;
					  else
						 GL_CommDlg.GL_generateTangents = FALSE;
					  //end handle list box node selection
					  //------------------------------
					  //animation speed handle
                      if(SendDlgItemMessage(hWnd,IDC_RADIO_AnimSpeed1X,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_animSpeedAdjust = 2;
                      if(SendDlgItemMessage(hWnd,IDC_RADIO_AnimSpeed2X,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_animSpeedAdjust = 4;
					  if(SendDlgItemMessage(hWnd,IDC_RADIO_AnimSpeed4X,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_animSpeedAdjust = 8;
					  if(SendDlgItemMessage(hWnd,IDC_RADIO_AnimSpeed1_2X,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_animSpeedAdjust = 1;

					  GetDlgItemText(hWnd,IDC_EDIT_toleranceAngle,bufferChar,250);
                      GL_CommDlg.GL_smShadeToleranceAngle = bufferChar;
					  
					  
					  if(SendDlgItemMessage(hWnd,IDC_CHECK_SMOOTHSHADING,BM_GETSTATE,0,0)==BST_CHECKED)
                         GL_CommDlg.GL_generateNormals = TRUE;
					  else
						 GL_CommDlg.GL_generateNormals = FALSE;

					  GL_CommDlg.GL_outputMemoryType = 0;//auto load
					  
					  
					  //end animation speed handle
					  //if(SendDlgItemMessage(hWnd,IDC_RADIO_HIARCHYBASED,   BM_GETSTATE,0,0)==BST_CHECKED)
					   //  GL_CommDlg.GL_exportType = 0;//hiarchy based
                      //if(SendDlgItemMessage(hWnd,IDC_RADIO_WORLDEXPORTTYPE,BM_GETSTATE,0,0)==BST_CHECKED)
					  //   GL_CommDlg.GL_exportType = 2;//world export based
                      

			          EndDialog(hWnd, 1);
			          break;

			     case IDC_BUTTON_ExportReferenceFile:
				      GL_CommDlg.GL_exportType = 9;
					  GetDlgItemText(hWnd,IDC_EDIT_refIdName,bufferChar,250);
					  GL_CommDlg.GL_miscString = bufferChar;
					   GL_CommDlg.selectionCount = SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELCOUNT ,0,0);
					  if(GL_CommDlg.selectionCount > 0){//somthin selected
						 GL_CommDlg.selectionBuffer = new int[GL_CommDlg.selectionCount+1];
						 SendDlgItemMessage(hWnd,IDC_LIST_NODES,LB_GETSELITEMS ,(GL_CommDlg.selectionCount+1),(LPARAM)(LPINT)GL_CommDlg.selectionBuffer);
					  }//end somthin selected
					  EndDialog(hWnd, 1);

					  break;
				 case IDC_BUTTON_EXPORTAITREE:
				      
					  GL_CommDlg.GL_exportType = 5;
					  GL_CommDlg.treeTHDuration = GetDlgItemInt(hWnd,IDC_EDIT_TREE_THDuration,NULL,TRUE);
					  EndDialog(hWnd, 1);
					  break;
				 	 
				 case IDCANCEL:
					  GL_CommDlg.GL_exportType = 99;
			          EndDialog(hWnd, 0);
			          break;
				 }

			return TRUE;
			}
	return FALSE;
}





/**********************************************************************
 *<
	FILE: PluginTest.h

	DESCRIPTION:	Template Utility

	CREATED BY:

	HISTORY:

 *>	Copyright (c) 1997, All Rights Reserved.
 **********************************************************************/

#ifndef __PLUGINTEST__H
#define __PLUGINTEST__H

#include "afxwin.h"
#include "afxext.h"
#include "matrixarb.h"
#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "IMPEXP.H"
#include "d3dx9.h"
#include "CSkeletalDataClass.h"
#include "CEXMeshClass.h"
#include "CAIWebClass.h"
#include "iskin.h"
#include "CPluginTransfer.h"
#include "IViewportManager.h"
#include "IHardwareShader.h"
#include "IHardwareRenderer.h"
#include <direct.h>
#include <commdlg.h>
#include "bmmlib.h"

#include "CReferenceImporterExporter.h"

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;


#define EXPORTER_CLASS_ID	Class_ID(0x314d7ad2, 0x599ce9da)


#define CTL_CHARS  31
#define SINGLE_QUOTE 39
// Node related ID's
#define ID_NODE_TM				_T("*NODE_TM")
#define ID_NODE_NAME			_T("*NODE_NAME") 
#define ID_NODE_PARENT			_T("*NODE_PARENT")

// Object (node) properties
#define ID_PROP_MOTIONBLUR		_T("*PROP_MOTIONBLUR")
#define ID_PROP_CASTSHADOW		_T("*PROP_CASTSHADOW")
#define ID_PROP_RECVSHADOW		_T("*PROP_RECVSHADOW")

#define ID_INHERIT_POS			_T("*INHERIT_POS")
#define ID_INHERIT_ROT			_T("*INHERIT_ROT")
#define ID_INHERIT_SCL			_T("*INHERIT_SCL")

// TM related ID's				
#define ID_TM_ROW0				_T("*TM_ROW0") 
#define ID_TM_ROW1				_T("*TM_ROW1") 
#define ID_TM_ROW2				_T("*TM_ROW2") 
#define ID_TM_ROW3				_T("*TM_ROW3") 
#define ID_TM_POS				_T("*TM_POS")
#define ID_TM_ROTAXIS			_T("*TM_ROTAXIS") 
#define ID_TM_ROTANGLE			_T("*TM_ROTANGLE") 
#define ID_TM_SCALE				_T("*A_SCALE") 
#define ID_TM_SCALEAXIS			_T("*TM_SCALEAXIS")
#define ID_TM_SCALEAXISANG		_T("*TM_SCALEAXISANG") 
#define ID_TM_ANIMATION			_T("*TM_ANIMATION")

// Scene related ID's			
#define ID_FILENAME				_T("*SCENE_FILENAME") 
#define ID_FIRSTFRAME			_T("*SCENE_FIRSTFRAME") 
#define ID_LASTFRAME			_T("*SCENE_LASTFRAME") 
#define ID_FRAMESPEED			_T("*SCENE_FRAMESPEED") 
#define ID_TICKSPERFRAME		_T("*SCENE_TICKSPERFRAME") 
#define ID_ENVMAP				_T("*SCENE_ENVMAP")
#define ID_STATICBGCOLOR		_T("*SCENE_BACKGROUND_STATIC") 
#define ID_ANIMBGCOLOR			_T("*SCENE_BACKGROUND_ANIM") 
#define ID_STATICAMBIENT		_T("*SCENE_AMBIENT_STATIC") 
#define ID_ANIMAMBIENT			_T("*SCENE_AMBIENT_ANIM")

#define ID_GROUP				_T("*GROUP")
//mesh
#define ID_MESH					_T("*MESH")
#define ID_MESH_NORMALS			_T("*MESH_NORMALS") 
#define ID_MESH_NUMVERTEX		_T("*MESH_NUMVERTEX") 
#define ID_MESH_NUMFACES		_T("*MESH_NUMFACES") 
#define ID_MESH_VERTEX_LIST		_T("*MESH_VERTEX_LIST") 
#define ID_MESH_VERTEX			_T("*MESH_VERTEX")
#define ID_MESH_FACE_LIST		_T("*MESH_FACE_LIST") 
#define ID_MESH_FACE			_T("*MESH_FACE")
#define ID_MESH_SMOOTHING		_T("*MESH_SMOOTHING") 
#define ID_MESH_MTLID			_T("*MESH_MTLID")

#define ID_MESH_NUMTVERTEX		_T("*MESH_NUMTVERTEX") 
#define ID_MESH_NUMTVFACES		_T("*MESH_NUMTVFACES") 
#define ID_MESH_TVERTLIST		_T("*MESH_TVERTLIST") 
#define ID_MESH_TVERT			_T("*MESH_TVERT") 
#define ID_MESH_TFACELIST		_T("*MESH_TFACELIST") 
#define ID_MESH_TFACE			_T("*MESH_TFACE")

#define ID_MESH_NUMCVERTEX		_T("*MESH_NUMCVERTEX")
#define ID_MESH_NUMCVFACES		_T("*MESH_NUMCVFACES") 
#define ID_MESH_CVERTLIST		_T("*MESH_CVERTLIST")
#define ID_MESH_VERTCOL			_T("*MESH_VERTCOL")
#define ID_MESH_CFACELIST		_T("*MESH_CFACELIST")
#define ID_MESH_CFACE			_T("*MESH_CFACE")

// Generic ID's that can show up here and there 
#define ID_TIMEVALUE			_T("*TIMEVALUE")
#define ID_COMMENT				_T("*COMMENT")
#define ID_FILEID				_T("*3DSMAX_ASCIIEXPORT")
#define ID_BOUNDINGBOX_MIN		_T("*BOUNDINGBOX_MIN")
#define ID_BOUNDINGBOX_MAX		_T("*BOUNDINGBOX_MAX")

#define VWRAP (1<<11)		/* Texture coord V wraps on this face */
#define UWRAP (1<<3)		/* Texture coord U wraps on this face */


/*struct ABString
{
    char stringBuild[256];
};*/

struct nodeReference
{
   INode * nodeRef;

};


struct DLGCommunication
{
	CString  *GL_SceneNodes;
	int       nodeCount;
	int *     selectionBuffer;
	//nodeReference *    nodeReferenceArray;
	int       selectionCount;
	int       GL_animSpeedAdjust;
	int       GL_exportType;
	int       GL_outputMemoryType;
	float     GL_sightRange;
	BOOL      GL_generateNormals;
	BOOL      GL_enableBlendedExport;
	int       treeTHDuration;
	BOOL      GL_exportAllKeys;
    CString   GL_smShadeToleranceAngle;
	int       GL_miscLoadInstruction;
	BOOL      GL_generateTangents;
	CString   GL_miscString;
};


//id tracking
class CIDObj : public CObject
{
	
public:
	CIDObj(){}
	int m_id;
};

class CIDList : public CObList
{
public:
   CIDList(){};
};


class Exporter : public SceneExport {
	public:
		static HWND hParams;
		nodeReference * nodeRefArray;
		int             m_nodeRefCount;
		//BOOL m_generateNormals;
  
		int nTotalNodeCount;
		//variables
        static DLGCommunication GL_CommDlg;
		CPluginTransfer * m_autoLoader;
		CReferenceImpExpList * m_lightCache;
        int GL_index;
		int GL_currentPhyModIndex;
		int GL_physiqueModCount;
        MatrixARB GL_matrixFunctions;
        CABFunctionLib CABLIB;
        //functions
		BOOL            AddVertexAssignment(int boneIndex,
			                                CBoneObject * focusBone,
											int vertecieIndex,
											int curPhyObj,
											D3DVECTOR offsetVector);
		int				ExtCount();					// Number of extensions supported
		const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *	AuthorName();				// ASCII Author name
		const TCHAR *	CopyrightMessage();			// ASCII Copyright message
		const TCHAR *	OtherMessage1();			// Other message #1
		const TCHAR *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void            ExportNodeTM(INode* node, int indentLevel,BOOL * physiqueExists,BOOL * skinExists);
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
		TSTR            GetIndent(int indentLevel);
		TCHAR*          FixupName(TCHAR* name);
		BOOL            GetPhysiqueCount(INode* node);
		BOOL            nodeEnumVertexAssignments(INode* node, int indentLevel,int tracer);
		void            ExportGlobalInfo(BOOL override,int startFrame, int endFrame);
		void            PreProcess(INode* node, int& nodeCount);
		BOOL            nodeEnum(INode* node, int indentLevel,int tracer);
		BOOL            nodeEnumAiTreeExport(INode* node,int tracer); 
		BOOL            nodeEnumAiTreeLinksExport(INode* node,int tracer);
		BOOL            nodeEnumWorldExport(INode* node,int tracer,TimeValue t,CString appendString);
		void            ExportMesh(INode* node, TimeValue t, int indentLevel, BOOL physiqueMesh, BOOL skinMesh);
		BOOL            TMNegParity(Matrix3 &m);
		TriObject*      GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt);
		Modifier*       FindPhysiqueModifier (INode* nodePtr);
		BOOL            TestIfBipedObject (INode* nodePtr);
		static BOOL WINAPI CALLBACK ExporterOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	    BOOL            GetNodeNames(INode* node,BOOL hiddenFlags);
		BOOL            LogVertexAssignments(INode* node,int indentLevel);
		BOOL            NodeEnumRefExporter(INode* node, int tracer,CReferenceImpExpList * referenceExporter);
		BOOL			GetLightTypeFromNode(INode* node, BOOL boundingBox, D3DVECTOR * min, D3DVECTOR * max);
		CString         GetMaterialFromID(INode* node,
			                              int id,
			                              D3DMATERIAL9 ** d3dMaterial,
			                              CString * refTexture2,
									      CString * refTexture3,
									      CString * refTexture4,
									      CString * refTexture5);
		D3DMATRIX       GetMatrixFromMaxStruct(Matrix3* m);
		Modifier*       FindSkinModifier (INode* node);
		BOOL            AddVertexAssignmentBlended(byte * boneIndexes,
										  float * wieghtsToBone,
										  ABVERTEX0L * offsetVect,
										  int boneIndexesCount,
										  int vertecieIndex,
										  int curPhyObj,
										  int totalVertCount);
		BOOL            SetABVERTEXCoordinateSet(int level,
										WORD vCount,
										ABVERTEX * vertexArray,
										ABVERTEX * textureSetArray);
		void            SaveMesh(
			            BOOL generateNormals,
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
						D3DVECTOR boxMax);

		void            MemBuildBlock(
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
	                    );

		void            MemSend();

		BYTE            *m_memBlock;
		DWORD           m_numMeshes;
		DWORD           m_memSize;

		void WriteDebugFile(long debugString,BOOL clearIt,BOOL returnCase);
        void WriteDebugFile(LPSTR debugString,BOOL clearIt,BOOL returnCase);
		CString m_logFileName;
		BOOL m_loggingType;

		void GenRootNodeArray();
		BitArray *      GetMeshGeneratedByID(int id,
									         Mesh * mesh,
									         Mesh ** meshOutput);
		void            GetMeshIDList(CIDList ** idList,
							          Mesh * mesh);
		void            GetMeshData(Mesh* mesh, 
							        ABVERTEX ** vArray,
							        WORD ** iArray,
							        int * vCount,
							        int * iCount,
                                    Matrix3 tm,
									BOOL useTransofmWNorm
							        );
		/*int				DoExport(const TCHAR *name,
			                     ExpInterface *i,
								 Interface *gi, 
								 BOOL suppressPrompts=FALSE);// Export file*/
		int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);	// Export file
		int             EnumDlgStructInfo();
		BOOL            ExportSkeleton(CString filename,BOOL override,int startFrame, int endFrame);
		TCHAR*          FixupName2(TCHAR* name);
		CString         GetTextureFromSubTex(Texmap* subTex);
		INode *         GetRootNodeByName(CString name);

		//DPD
		BOOL			NodeHasShader(INode *node);
		BOOL			GetShaderInfo(CMaterialObject* matObj, INode *node, LPD3DXEFFECT effect, CString & fxInput);
		LPD3DXEFFECT	SetupEffectFile(INode *node, LPDIRECT3DDEVICE9 device, CString * fxReturn);
		int				SetShaderTextures(LPD3DXEFFECT effect, INode* node, CMaterialObject *matObj, CString *textures);
		BOOL            VerifyObjectForMeshExport(CString nodeName);
		int             frameCount;
		long            frameLength;
		Interface*	    ip;
		TCHAR		    szFmtStr[16];

		//new bones export
        CSkeletonObject * m_skeletalExport;
		//ai web export
		CAIWebObj * m_webExport;

		//Constructor/Destructor
		Exporter();
		~Exporter();

};




class ExporterClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic() {return 1;}
	void *			Create(BOOL loading = FALSE) {return new Exporter();}
	const TCHAR *	ClassName() {return GetString(IDS_CLASS_NAME);}
	SClass_ID		SuperClassID() {return SCENE_EXPORT_CLASS_ID;}
	Class_ID		ClassID() {return EXPORTER_CLASS_ID;}
	const TCHAR* 	Category() {return GetString(IDS_CATEGORY);}
	const TCHAR*	InternalName() { return _T("Exporter"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle
};






#endif // __PLUGINTEST__H

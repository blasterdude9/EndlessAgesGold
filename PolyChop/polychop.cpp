/*===========================================================================*\
 |    File: polychop.cpp
 |
 |
\*===========================================================================*/
#include "max.h"			// Main MAX include file
#include "iparamm.h"		// Parameter Map include file
#include "polychop.h"		// Resource editor include file
#include "progmesh.h"

/*===========================================================================*\
 | Misc Defines
\*===========================================================================*/
// The unique ClassID
#define POLYCHOP_CLASS_ID Class_ID(0x2e6753d9, 0x3b6e12a3)

// This is the name that will appear in the Modifier stack
#define INIT_MOD_NAME			GetString(IDS_POLYCHOP)

// Name used for debugging
#define CLASSNAME				GetString(IDS_POLYCHOPMOD)

// This is the name on the creation button
#define POLYCHOP_CLASSNAME		GetString(IDS_POLYCHOP)

// This is the category the button goes into
#define CATEGORY_NAME			GetString(IDS_MAXADDITIONAL)

// The name of the parameter block sub-anim
#define PARAMETERS_NAME			GetString(IDS_PARAMETERS)


// Parameter block indicies
#define PB_USEEDGELENGTH    0
#define PB_USECURVATURE     1
#define PB_PROTECTTEXTURE   2
#define PB_PROTECTSMOOTH    3
#define PB_GENTEXTURE       4
#define PB_MAINTAINSMOOTH   5
#define PB_PERCENT          6
#define PB_PROTECTVC        7
#define PB_LOCKBORDER       8
#define PB_LOCKSELECTED     9


// The DLL instance handle
HINSTANCE hInstance;

TCHAR *GetString(int id)
{
	static TCHAR buf[256];
	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}

/*===========================================================================*\
 | Class Definitions:
\*===========================================================================*/
class PolyChopMod : public Modifier
{
public:
	PolyChopMod();

	// Class vars
	static Interface *ip;
	static IParamMap *pmap;

	// The parameter block for the percentage parameter
	IParamBlock *pblock;

	// ---- Methods from Animatable ----
	void DeleteThis() { delete this; }
	void GetClassName(TSTR& s) { s= CLASSNAME; }  
	virtual Class_ID ClassID() { return POLYCHOP_CLASS_ID;}
	SClass_ID SuperClassID() { return OSM_CLASS_ID; }
	void BeginEditParams(IObjParam *ip, ULONG flags, Animatable *prev);
	void EndEditParams(IObjParam *ip,ULONG flags, Animatable *next);
	int NumSubs() { return 1; }  
	Animatable* SubAnim(int i) { return pblock; }
	TSTR SubAnimName(int i) { return PARAMETERS_NAME;}		

	// ---- Methods from ReferenceMaker ----
	RefResult NotifyRefChanged( Interval changeInt,RefTargetHandle hTarget,
	   PartID& partID, RefMessage message );
	int NumRefs() { return 1; }
	RefTargetHandle GetReference(int i) { return pblock; }
    void SetReference(int i, RefTargetHandle rtarg) { pblock = (IParamBlock*)rtarg; }

	// ---- Methods from ReferenceTarget ----
	RefTargetHandle Clone(RemapDir& remap = NoRemap());

	// ---- Inherited virtual methods from BaseObject ----
	// This is the name that appears in the history list (modifier stack).
	TCHAR *GetObjectName() { return INIT_MOD_NAME; }
	CreateMouseCallBack* GetCreateMouseCallBack() { return NULL; } 

	// ---- Methods from Modifier ----
    ChannelMask ChannelsUsed()  { return OBJ_CHANNELS; }
    ChannelMask ChannelsChanged() { return PART_GEOM|PART_TOPO|TEXMAP_CHANNEL|VERTCOLOR_CHANNEL|SELECT_CHANNEL
; }
	Class_ID InputType() { return Class_ID(TRIOBJ_CLASS_ID,0); }
	void ModifyObject(TimeValue t, ModContext &mc, 
		ObjectState *os, INode *node);
    void NotifyInputChanged(Interval changeInt, PartID partID, RefMessage message, ModContext *mc);

	// ---- Local methods ----
	float GetPercent(TimeValue t);
	Interval PolyChopValidity(TimeValue t);
	List<int> order;
	List<int> map;
	int regenerate;
};


/*===========================================================================*\
 | Parameter Map and Parameter Block User Interface Stuff
\*===========================================================================*/
#define DESC_LENGTH (10)
static ParamUIDesc descParam[DESC_LENGTH] = {
	ParamUIDesc(PB_USEEDGELENGTH,  TYPE_SINGLECHEKBOX, IDC_USEEDGELENGTH),
	ParamUIDesc(PB_USECURVATURE,   TYPE_SINGLECHEKBOX, IDC_USECURVATURE),
	ParamUIDesc(PB_PROTECTTEXTURE, TYPE_SINGLECHEKBOX, IDC_PROTECTTEXTURE),
	ParamUIDesc(PB_PROTECTSMOOTH,  TYPE_SINGLECHEKBOX, IDC_PROTECTSMOOTH),
	ParamUIDesc(PB_GENTEXTURE,     TYPE_SINGLECHEKBOX, IDC_GENTEXTURE),
	ParamUIDesc(PB_MAINTAINSMOOTH, TYPE_SINGLECHEKBOX, IDC_MAINTAINSMOOTH),
	ParamUIDesc(PB_PERCENT, EDITTYPE_FLOAT, IDC_PERCENT, IDC_PERCENT_SPIN,
		0.0f, 100.0f, 1.0f),
	ParamUIDesc(PB_PROTECTVC,      TYPE_SINGLECHEKBOX, IDC_PROTECTVC),
	ParamUIDesc(PB_LOCKBORDER,     TYPE_SINGLECHEKBOX, IDC_LOCKBORDER),
	ParamUIDesc(PB_LOCKSELECTED,   TYPE_SINGLECHEKBOX, IDC_LOCKSELECTED),
};

#define PBLOCK_LENGTH		(DESC_LENGTH)
static ParamBlockDescID descVer0[PBLOCK_LENGTH] = {
	{ TYPE_BOOL, NULL, FALSE, PB_USEEDGELENGTH },
	{ TYPE_BOOL, NULL, FALSE, PB_USECURVATURE },
	{ TYPE_BOOL, NULL, FALSE, PB_PROTECTTEXTURE },
	{ TYPE_BOOL, NULL, FALSE, PB_PROTECTSMOOTH },
	{ TYPE_BOOL, NULL, FALSE, PB_GENTEXTURE },
	{ TYPE_BOOL, NULL, FALSE, PB_MAINTAINSMOOTH },
	{ TYPE_FLOAT, NULL, TRUE, PB_PERCENT },
	{ TYPE_BOOL, NULL, FALSE, PB_PROTECTVC },
	{ TYPE_BOOL, NULL, FALSE, PB_LOCKBORDER },
	{ TYPE_BOOL, NULL, FALSE, PB_LOCKSELECTED },
};
#define CURRENT_VERSION		0

/*===========================================================================*\
 | User Interface stuff...
\*===========================================================================*/
void PolyChopMod::BeginEditParams(IObjParam *ip, ULONG flags, Animatable *prev)
{
	this->ip = ip;
	// Create and add the Parameters rollup to the command panel
	pmap = CreateCPParamMap(descParam, DESC_LENGTH,
		pblock, ip, hInstance, MAKEINTRESOURCE(IDD_POLYCHOP), 
		GetString(IDS_PARAMETERS), 0);
}

void PolyChopMod::EndEditParams(IObjParam *ip, ULONG flags, Animatable *next)
{
	// Delete the parameter map
	if (pmap) { DestroyCPParamMap(pmap); }
	this->ip = NULL;
	this->pmap = NULL;
}

PolyChopMod::PolyChopMod()
{
	regenerate=1;
	MakeRefByID(FOREVER, 0, CreateParameterBlock(descVer0, PBLOCK_LENGTH, CURRENT_VERSION));
	pblock->SetValue(PB_USEEDGELENGTH, 0, 1);
    pblock->SetValue(PB_USECURVATURE, 0, 1);
    pblock->SetValue(PB_PROTECTTEXTURE, 0, 1);
    pblock->SetValue(PB_PROTECTSMOOTH, 0, 0);
    pblock->SetValue(PB_GENTEXTURE, 0, 1);
    pblock->SetValue(PB_MAINTAINSMOOTH, 0, 1);
	pblock->SetValue(PB_PERCENT, 0, 100.0f);
    pblock->SetValue(PB_PROTECTVC, 0, 0);
    pblock->SetValue(PB_LOCKBORDER, 0, 0);
    pblock->SetValue(PB_LOCKSELECTED, 0, 0);
}

// Initialize the class variables.
Interface *PolyChopMod::ip = NULL;
IParamMap *PolyChopMod::pmap = NULL;

/*===========================================================================*\
 | Modification stuff...
\*===========================================================================*/

// This method, called by the system, modifies the items...
void PolyChopMod::ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node)
{	
	TriObject *obj = (TriObject *) os->obj;
	Mesh *mesh = &obj->mesh;

    Interval valid;

	float detail = 0.01f * GetPercent(t);
    if (detail < 0.0f) detail = 0.0f;
    if (detail > 1.0f) detail = 1.0f; // must clamp

	// I think this is the only routine that you can get access to the mesh
	// if you want to recompute the progressivemesh 
	// then set regenerate to 1
	if(regenerate){
		regenerate=0;
		PMarg pmarg;
	    pblock->GetValue(PB_USEEDGELENGTH, 0, pmarg.useedgelength, valid);
	    pblock->GetValue(PB_USECURVATURE,  0, pmarg.usecurvature, valid);
		pblock->GetValue(PB_PROTECTTEXTURE,0, pmarg.protecttexture,valid);
		pblock->GetValue(PB_PROTECTSMOOTH, 0, pmarg.protectsmooth,valid);
		pblock->GetValue(PB_PROTECTVC,     0, pmarg.protectvc,valid);
		pblock->GetValue(PB_LOCKBORDER,    0, pmarg.lockborder,valid);
		pblock->GetValue(PB_LOCKSELECTED,  0, pmarg.lockselected,valid);
 		ComputeProgressiveMesh(mesh,order,map,pmarg);
	}
	BOOL gentexture;
	BOOL maintainsmooth;
	pblock->GetValue(PB_GENTEXTURE    ,0, gentexture,valid);
	pblock->GetValue(PB_MAINTAINSMOOTH,0, maintainsmooth,valid);
	DoProgressiveMesh(mesh,order,map,detail,gentexture,maintainsmooth);

	/*valid = LocalValidity(t);
	obj->UpdateValidity(GEOM_CHAN_NUM,valid);
	obj->UpdateValidity(TOPO_CHAN_NUM,valid);*/
	obj->UpdateValidity(GEOM_CHAN_NUM, PolyChopValidity(t));
	obj->UpdateValidity(TOPO_CHAN_NUM, PolyChopValidity(t));
}

void PolyChopMod::NotifyInputChanged(Interval changeInt, PartID partID, RefMessage message, ModContext *mc)
{
	regenerate=1;
    // called when modified object changes
}

Interval PolyChopMod::PolyChopValidity(TimeValue t)
{
	float p;
	Interval valid = FOREVER;
	pblock->GetValue(PB_PERCENT, t, p, valid);
	return valid;
}

RefTargetHandle PolyChopMod::Clone(RemapDir& remap)
{
	PolyChopMod* newmod = new PolyChopMod();	
	newmod->ReplaceReference(0, pblock->Clone(remap));
	return(newmod);
}

RefResult PolyChopMod::NotifyRefChanged(Interval changeInt,
	RefTargetHandle hTarget, PartID& partID, RefMessage message)
{

	switch (message) {
		case REFMSG_CHANGE:
			if (pmap && pmap->GetParamBlock()==pblock) {
			    pmap->Invalidate();
                if (hTarget==pblock) {
			        int np = pblock->LastNotifyParamNum();
                    switch (np)
                    {
							// np = PB_...
						case PB_USEEDGELENGTH:
						case PB_USECURVATURE:
						case PB_PROTECTTEXTURE:
						case PB_PROTECTSMOOTH:
						case PB_PROTECTVC:
						case PB_LOCKBORDER:
						case PB_LOCKSELECTED:
							regenerate=1;
							break;
                    }
                }
		    }
			break;

		/*case REFMSG_GET_PARAM_DIM: { 
			GetParamDim *gpd = (GetParamDim*)partID;
			switch (gpd->index) {
				case PB_PERCENT:
					gpd->dim = stdPercentDim;
					break;
			};
			return REF_STOP; 
		}*/

		case REFMSG_GET_PARAM_NAME: {
			GetParamName *gpn = (GetParamName*)partID;
			switch (gpn->index) {
				case PB_PERCENT:
					gpn->name = GetString(IDS_PERCENT);
					break;
			};
			return REF_STOP; 
		}
	}
	return REF_SUCCEED;
}

float PolyChopMod::GetPercent(TimeValue t)
{
	float f;
	Interval valid;
	pblock->GetValue(PB_PERCENT, t, f, valid);
	return f;
}


/*===========================================================================*\
 | Class Descriptor
\*===========================================================================*/
class PolyChopClassDesc : public ClassDesc
{
public:
	int				IsPublic() { return 1; }
	void			*Create(BOOL loading=FALSE) { return new PolyChopMod(); }
	const TCHAR		*ClassName() { return POLYCHOP_CLASSNAME; }
	SClass_ID		SuperClassID() { return OSM_CLASS_ID; }
	Class_ID		ClassID() { return POLYCHOP_CLASS_ID; }
	const TCHAR		*Category() { return CATEGORY_NAME;  }
};
static PolyChopClassDesc PolyChopCD;

/*===========================================================================*\
 | DLL/Lib Functions
\*===========================================================================*/
int controlsInit = FALSE;
BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved) 
{	
	hInstance = hinstDLL;

	if (!controlsInit) {
		controlsInit = TRUE;
		InitCustomControls(hInstance);
		InitCommonControls();
	}
	
	return(TRUE);
}

__declspec( dllexport ) int LibNumberClasses() { 
	return 1; 
}

__declspec( dllexport ) ClassDesc *LibClassDesc(int i) { 
	return &PolyChopCD; 
}

__declspec( dllexport ) const TCHAR *LibDescription() {
	return GetString(IDS_POLYCHOPTITLE); 
}

__declspec( dllexport ) ULONG LibVersion() { 
	return VERSION_3DSMAX; 
}

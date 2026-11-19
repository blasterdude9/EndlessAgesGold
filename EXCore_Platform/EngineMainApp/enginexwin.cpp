#ifndef VC_EXTRALEAN 
#define VC_EXTRALEAN 
#define FLOATEQ0(n) ( n >= -0.01f && n <= 0.01f )

#endif
#define D3D_OVERLOADS
#include "stdafx.h"
#define INITGUID
#include <d3d9.h>
#include <d3dX9.h>
#include "resource.h"
#include "CrashReporter.h"
#include <direct.h>
#include <math.h>

 //#include <afxinet.h>
#include "CPolyObj.h"
#include "EngineXWin.h"
#include "rmxftmpl.h"
#include "CPluginTransfer.h"
//dialog includes
#include "ColorDlg.h"
#include "AnimSettingsDlg.h"
#include "MotionSettingsDlg.h"
#include "AddEntityDlg.h"
#include "MaterialDlg.h"
#include "PlacementDlg.h"
#include "CameraAdvDlg.h"
#include "MovieSettingsDlg.h"
#include "ControlAddObjectDlg.h"
#include "AddControlEventDlg.h"
#include "GlobalAdvancedDlg.h"
#include "LightSettingsDlg.h"
#include "alphatexturedlg.h"
#include "ShadowDlg.h"
#include "AnimPropertiesDlg.h"
#include "SoundSettingsDlg.h"
#include "ContentPropertiesDlg.h"
#include "ConnectionMethodDlg.h"
#include "ServerSetupDlg.h"
#include "LoginDlg.h"
#include "AnimTextureSwpAddDlg.h"
#include "AddScriptEventDlg.h"
#include "AdvancedNetworkDlg.h"
#include "HudAddDisplayUnitDlg.h"
#include "AddHud.h"
#include "AddMenu.h"//dlg
#include "MenuAddUnitDlg.h"
#include "AddParticleSystemDLG.h"
#include "matrixArb.h"
#include "AddSpawnGeneratorDlg.h"
#include "CollisionResolutionDlg.h"
#include "WorldObjectPropertiesDlg.h"
#include "WorldGroupDlg.h"
#include "GroupModifierDlg.h"
#include "EntityPropertiesDlg.h"
#include "NewAccountDlg.h"
#include "TriggerEditorDlg.h"
#include "AddTextureDlg.h"
#include "AITraceMapDlg.h"
#include "LodSystemDlg.h"
#include "SkeletalAnimationDlg.h"
#include "EditTextureDlg.h"
#include "AppendageCustDlg.h"
#include "EntityLODSystemsDlg.h"
#include "SkillEditorDlg.h"
#include "ResourceEditorDlg.h"
#include "MissileBaseMeshEdit.h"
#include "MissileAddBillboardDlg.h"
#include "EditStartInventoryDlg.h"
#include "GenerateMeshDLG.h"
#include "ParticleSystem_Both.h"
#include "ParticleSystem_Directional.h"
#include "ParticleSystem_NonDirectional.h"
#include "ScriptEventAdd.h"
#include "ScriptAdd.h"
#include "AiSettingsDlg.h"
#include "ExplosionBasicDlg.h"
#include "CExplosionsDatabaseDlg.h"
#include "SoundSettingsDlg.h"
#include "CursorScaleDlg.h"
#include "MenuClickZoneDlg.h"
#include "CharonaDlg.h"
#include "AddItemDlg.h"
#include "EntityItemTestDlg.h"
#include "TransformationDlg.h"
#include "GroupBoundingSetDlg.h"
#include "ObjectLibraryDlg.h"
#include "TimeSystemDlg.h"
#include "LaserDlg.h"
#include "AIPathDlg.h"
#include "CursorLimits.h"
#include "SkillBuilderDlg.h"
#include "SkillsOnCharEditorDlg.h"
#include "ReferenceLibrary.h"
#include "SoundDatabaseDlg.h"
#include "CurrencyDlg.h"
#include "CommerceDlg.h"
#include "BankZonesDlg.h"
#include "SafeZoneDlg.h"
#include "LightInterfaceDlg.h"
#include "StatisticsDBDlg.h"
#include "SpawnGenDlg.h"
#include "PatchSystemDlg.h"
//#include "BlockIpDlg.h"
#include "WaterZoneDBDLG.h"
#include "PortalsDlg.h"
#include "ElementsDlg.h"
#include "UpgradeWormDlg.h"
#include "SkillSystemDlg.h"
#include "SymbolMapDlg.h"
#include "GetSignificantOptionsDlg.h"
#include "IconSysDlg.h"
#include "HousingSysDlg.h"
#include "HousePlacementZoneDlg.h"
#include "ItemConflictDlg.h"
#include "AIRaidDbClass.h"
#include "MergeOptionsDlg.h"
#include "CommCrtlEditorDlg.h"
#include "WorldChannelDlg.h"
#include "ArenaManagerDlg.h"
#include "ShaderLibDlg.h"
#include "MaterialShaderLibDlg.h"
#include "VertexAnimSettingsDlg.h"
#include "TestExportDlg.h"
#include "FXDynPerametersDlg.h"
#include "dbconnectionsystem.h"
#include "SqlConnectionDlg.h"
#include "PatchSystemUpdateDlg.h"
#include "effectSystemDlg.h"
#include "ParticleInterfaceDlg.h"
#include "AddDotdlg.h"
#include "ItemManagerDlg.h"

//#import "msado15.dll" no_namespace rename("EOF", "EndOfFile")//server only pls
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//global thread functions

BEGIN_MESSAGE_MAP(EngineXWin, RMWin)
#ifndef _ClientOutput
	ON_COMMAND(ID_QUICK_PLAYHERE, OnQuickPlayHere)
	ON_COMMAND(ID_QUICK_BACKTOEDITOR, OnQuickBackToEditor)
	ON_COMMAND(ID_QUICK_WHEREAMI, OnQuickWhereAmI)
	ON_COMMAND(ID_QUICK_GOTO, OnQuickGoTo)
	ON_COMMAND(ID_QUICK_EXPORTCHARACTER, OnQuickExportCharacter)
	ON_COMMAND(ID_QUICK_EXPORTOUTFITS, OnQuickExportOutfits)
	ON_COMMAND(ID_QUICK_COPYENTITY, OnQuickCopyEntity)
	ON_COMMAND(ID_QUICK_COPYITEMRACE, OnQuickCopyItemRace)
	ON_COMMAND(ID_QUICK_NEWRACEWIZARD, OnQuickNewRaceWizard)
	ON_COMMAND(ID_QUICK_SENDTOGAME, OnQuickSendToGame)
	ON_COMMAND(ID_QUICK_SETGAMEPATHS, OnQuickSetGamePaths)
	ON_COMMAND(ID_QUICK_IMPORTCHARACTER, OnQuickImportCharacter)
	ON_COMMAND(ID_QUICK_GIVETOPSJIGGLE, OnQuickGiveTopsJiggle)
#endif
	//{{AFX_MSG_MAP(EngineXWin)
    ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, OnButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_COMMAND(ID_FILE_RUNSCRIPT, OnFileRunScript)
	ON_COMMAND(ID_FILE_LOADZONE, OnFileLoadZone)
	ON_BN_CLICKED(IDC_BUTTON_MENU, OnButtonMenu)
	ON_BN_CLICKED(IDC_BUTTON_TREETEST, OnButtonTreetest)
	ON_BN_CLICKED(IDC_BUTTON_OBJECTLOAD, OnButtonObjectload)
	ON_BN_CLICKED(IDC_BUTTON_OBJECTDELETE, OnButtonObjectdelete)
	ON_BN_CLICKED(IDC_BUTTON_SWITCH_OBJECT, OnButtonSwitchObject)
	ON_BN_CLICKED(IDC_BUTTON_BKEDITORswtch, OnBUTTONBKEDITORswtch)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_MV_UP, OnButtonMvUp)
	ON_BN_CLICKED(IDC_BUTTON_MV_DOWN, OnButtonMvDown)
	ON_BN_CLICKED(IDC_BUTTON_MV_LEFT, OnButtonMvLeft)
	ON_BN_CLICKED(IDC_BUTTON_MV_RIGHT, OnButtonMvRight)
	ON_BN_CLICKED(IDC_BUTTON_ADDNEWSTATIC_BK, OnButtonAddnewstaticBk)
	ON_BN_CLICKED(IDC_BUTTON_BK_SETTINGSapply, OnBUTTONBKSETTINGSapply)
	ON_BN_CLICKED(IDC_BUTTON_CONTROLS, OnButtonControls)
	ON_BN_CLICKED(IDC_BUTTON_TEST_EFFECT, OnButtonTestEffect)
	ON_BN_CLICKED(IDC_BUTTON_STOP_EFFECT, OnButtonStopEffect)
	ON_BN_CLICKED(IDC_BUTTON_MOVIES, OnButtonMovies)
	ON_BN_CLICKED(IDC_BUTTON_TESTMOVIE, OnButtonTestmovie)
	ON_BN_CLICKED(IDC_BTN_ENTITY_CREATENEW, OnBtnEntityCreatenew)
	ON_BN_CLICKED(IDC_BTN_ENTITY_ACTIVATE, OnBtnEntityActivate)
	ON_BN_CLICKED(IDC_BUTTON_SW_ENTITIES, OnButtonSwEntities)
	ON_BN_CLICKED(IDC_BUTTON_TESTFULLSCREEN, OnButtonTestfullscreen)
	ON_BN_CLICKED(IDC_BTN_VIEWWIREFRAME, OnBtnViewwireframe)
	ON_BN_CLICKED(IDC_BUTTON_VIEWSMOOTHSHADED, OnButtonViewsmoothshaded)
	ON_BN_CLICKED(IDC_BTN_ENTITY_MOVEMENT, OnBtnEntityMovement)
	ON_BN_CLICKED(IDC_BTN_ENTITY_DEACTIVATE, OnBtnEntityDeactivate)
	ON_BN_CLICKED(IDC_BTN_DELETE_ENTITY, OnBtnDeleteEntity)
	ON_BN_CLICKED(IDC_BTN_ENTITY_deleteAnim, OnBTNENTITYdeleteAnim)
	ON_BN_CLICKED(IDC_BTN_ENTITY_DeleteChld, OnBTNENTITYDeleteChld)
	ON_BN_CLICKED(IDC_BTN_ENTITY_ADDCHILD, OnBtnEntityAddchild)
	ON_BN_CLICKED(IDC_BTN_NEWALL, OnBtnNewall)
	ON_BN_CLICKED(IDB_BTN_CameraViewportsSw, OnBTNCameraViewportsSw)
	ON_BN_CLICKED(IDB_CAMERA_CreateNewCamera, OnCAMERACreateNewCamera)
	ON_BN_CLICKED(IDC_BTN_CreateNewViewport, OnBTNCreateNewViewport)
	ON_BN_CLICKED(IDC_BTN_CameraVportActivate, OnBTNCameraVportActivate)
	ON_BN_CLICKED(IDC_BTN_vportDeactivate, OnBTNvportDeactivate)
	ON_BN_CLICKED(IDC_BTN_MOUSEcreate, OnBTNMOUSEcreate)
	ON_BN_CLICKED(IDC_BTN_UImouseSW, OnBTNUImouseSW)
	ON_BN_CLICKED(IDC_BTN_MOUSEfileBrowse, OnBTNMOUSEfileBrowse)
	ON_BN_CLICKED(IDC_BTN_MOUSEactivate, OnBTNMOUSEactivate)
	ON_BN_CLICKED(IDC_BTN_MOUSEdelete, OnBTNMOUSEdelete)
	ON_BN_CLICKED(IDC_BTN_MOUSEdeactivate, OnBTNMOUSEdeactivate)
	ON_BN_CLICKED(IDC_BTN_2DINTERFACEsw, OnBTN2DINTERFACEsw)
	ON_BN_CLICKED(IDC_BTN_2D_CreateCollection, OnBTN2DCreateCollection)
	ON_BN_CLICKED(IDC_BTN_2D_ADDunit, OnBTN2DADDunit)
	ON_BN_CLICKED(IDC_BTN_2D_ACTIVATE, OnBtn2dActivate)
	ON_BN_CLICKED(IDC_BTN_2D_DEACTIVATE, OnBtn2dDeactivate)
	ON_BN_CLICKED(IDC_BTN_2D_BrowseFileUp, OnBTN2DBrowseFileUp)
	ON_BN_CLICKED(IDC_BTN_2D_BrowseFileDown, OnBTN2DBrowseFileDown)
	ON_BN_CLICKED(IDC_BTN_2DgetSelectedInfo, OnBTN2DgetSelectedInfo)
	ON_BN_CLICKED(IDC_BTN_2D_ApplyModifications, OnBTN2DApplyModifications)
	ON_BN_CLICKED(IDC_BTN_2D_DeleteCollection, OnBTN2DDeleteCollection)
	ON_BN_CLICKED(IDC_BTN_2D_DeleteUnit, OnBTN2DDeleteUnit)
	ON_BN_CLICKED(IDC_BUTTON_EnviormentSw, OnBUTTONEnviormentSw)
	/*
	ON_BN_CLICKED(IDC_BTN_ENV_BckRgb, OnBTNENVBckRgb)
	ON_BN_CLICKED(IDC_BTN_ENV_FogRgb, OnBTNENVFogRgb)
	ON_BN_CLICKED(IDC_BTN_ENV_ApplyChanges, OnBTNENVApplyChanges)
	ON_BN_CLICKED(IDC_BTN_ENV_Match, OnBTNENVMatch)
	ON_BN_CLICKED(IDC_CHECK_ENV_FogOn, OnCHECKENVFogOn)
	ON_BN_CLICKED(IDC_BTN_ENV_BrowseSurObj, OnBTNENVBrowseSurObj)
	ON_BN_CLICKED(IDC_BTN_ENV_emmisive, OnBTNENVemmisive)
	ON_BN_CLICKED(IDC_CHECK_ENV_enableSun, OnCHECKENVenableSun)
	ON_BN_CLICKED(IDC_BTN_ENV_sunColor, OnBTNENVsunColor)
	ON_BN_CLICKED(IDC_CHECK_EnableSurrounding, OnCHECKEnableSurrounding)
	*/
	ON_BN_CLICKED(IDC_BTN_Object_Material, OnBTNObjectMaterial)
	ON_BN_CLICKED(IDC_BTN_Entity_Material, OnBTNEntityMaterial)
	ON_BN_CLICKED(IDC_BUTTON_text_AddTextCollection, OnBUTTONtextAddTextCollection)
	ON_BN_CLICKED(IDC_BTN_textFontSW, OnBTNtextFontSW)
	ON_BN_CLICKED(IDC_BUTTON_text_AddTextObject, OnBUTTONtextAddTextObject)
	ON_BN_CLICKED(IDC_BTN_text_Activate, OnBTNtextActivate)
	ON_BN_CLICKED(IDC_BTN_text_Deactivate, OnBTNtextDeactivate)
	ON_BN_CLICKED(IDC_BUTTON_text_TextColor, OnBUTTONtextTextColor)
	ON_BN_CLICKED(IDC_BTN_text_ApplyChanges, OnBTNtextApplyChanges)
	ON_BN_CLICKED(IDC_BTN_text_GetCurrentInfo, OnBTNtextGetCurrentInfo)
	ON_BN_CLICKED(IDC_BTN_text_ApplyFont, OnBTNtextApplyFont)
	ON_BN_CLICKED(IDC_BUTTON_text_AddFont, OnBUTTONtextAddFont)
	ON_BN_CLICKED(IDC_BTN_text_ApplySizeChange, OnBTNtextApplySizeChange)
	ON_BN_CLICKED(IDC_BUTTON_text_DeleteFont, OnBUTTONtextDeleteFont)
	ON_BN_CLICKED(IDC_BUTTON_text_DeleteTextCollection, OnBUTTONtextDeleteTextCollection)
	ON_BN_CLICKED(IDC_BUTTON_text_DeleteTextObject, OnBUTTONtextDeleteTextObject)
	ON_BN_CLICKED(IDC_BTN_test_Save, OnBTNtestSave)
	ON_BN_CLICKED(IDC_BTN_test_Load, OnBTNtestLoad)
	ON_BN_CLICKED(IDC_BTN_camVport_Save, OnBTNcamVportSave)
	ON_BN_CLICKED(IDC_BTN_camVport_Load, OnBTNcamVportLoad)
	ON_BN_CLICKED(IDC_BTN_DeleteCamera, OnBTNDeleteCamera)
	ON_BN_CLICKED(IDC_BTN_DeleteViewport, OnBTNDeleteViewport)
	ON_BN_CLICKED(IDC_BTN_2D_SaveConfig, OnBTN2DSaveConfig)
	ON_BN_CLICKED(IDC_BTN_2D_LoadConfig, OnBTN2DLoadConfig)
	ON_BN_CLICKED(IDC_BTN_ExplosionDBSW, OnBTNExplosionDBSW)
	/*
	ON_BN_CLICKED(IDC_BTN_EXP_AddExplosion, OnBTNEXPAddExplosion)
	ON_BN_CLICKED(IDC_BTN_EXP_LoadExplosions, OnBTNEXPLoadExplosions)
	ON_BN_CLICKED(IDC_BTN_EXP_SaveExplosions, OnBTNEXPSaveExplosions)
	ON_BN_CLICKED(IDC_BTN_EXP_DeleteExplosion, OnBTNEXPDeleteExplosion)
	*/
	ON_BN_CLICKED(IDC_BTN_CameraAdvanced, OnBTNCameraAdvanced)
	ON_BN_CLICKED(IDC_BTN_Movie_AddMovie, OnBTNMovieAddMovie)
	ON_BN_CLICKED(IDC_BTN_Movie_TestMovie, OnBTNMovieTestMovie)
	ON_BN_CLICKED(IDC_BTN_Movie_EditMovie, OnBTNMovieEditMovie)
	ON_BN_CLICKED(IDC_BTN_Movie_DeleteMovie, OnBTNMovieDeleteMovie)
	ON_BN_CLICKED(IDC_BTN_MoviesSaveDB, OnBTNMoviesSaveDB)
	ON_BN_CLICKED(IDC_BTN_MoviesLoadDB, OnBTNMoviesLoadDB)
	ON_BN_CLICKED(IDC_BTN_Movie_StopMovies, OnBTNMovieStopMovies)
	ON_BN_CLICKED(IDC_BTN_ControlAddObject, OnBTNControlAddObject)
	ON_BN_CLICKED(IDC_BTN_ControlAddEvent, OnBTNControlAddEvent)
	ON_BN_CLICKED(IDC_BTN_ControlCfgSave, OnBTNControlCfgSave)
	ON_BN_CLICKED(IDC_BTN_ControlCfgLoad, OnBTNControlCfgLoad)
	ON_BN_CLICKED(IDC_BTN_ControlDeleteObject, OnBTNControlDeleteObject)
	ON_BN_CLICKED(IDC_BTN_ControlDeleteEvent, OnBTNControlDeleteEvent)
	ON_LBN_SELCHANGE(IDC_LIST_ControlDB, OnSelchangeLISTControlDB)
	ON_BN_CLICKED(IDC_BTN_ControlEditObject, OnBTNControlEditObject)
	ON_BN_CLICKED(IDC_BTN_ControlEditEvent, OnBTNControlEditEvent)
	ON_BN_CLICKED(IDC_BTN_ObjectArrayAdd, OnBTNObjectArrayAdd)
	ON_BN_CLICKED(IDC_BTN_UI_BaseAdvanced, OnBTNUIBaseAdvanced)
	ON_BN_CLICKED(IDC_BTN_ENTITY_Properties, OnBTNENTITYProperties)
	ON_BN_CLICKED(IDC_BTN_OBJECT_Properties, OnBTNOBJECTProperties)
	ON_BN_CLICKED(IDC_BTN_OBJ_DeleteLight, OnBTNOBJDeleteLight)
	ON_BN_CLICKED(IDC_BTN_OBJ_ADDLIGHT, OnBtnObjAddlight)
	ON_BN_CLICKED(IDC_BTN_ENT_DestroyLight, OnBTNENTDestroyLight)
	ON_BN_CLICKED(IDC_BTN_ENT_AddLight, OnBTNENTAddLight)
	ON_BN_CLICKED(IDC_BTN_UI_Music, OnBTNUIMusic)
	ON_BN_CLICKED(IDC_BTN_Music_AssignTrack, OnBTNMusicAssignTrack)
	ON_BN_CLICKED(IDC_BTN_Music_AssignWav, OnBTNMusicAssignWav)
	ON_BN_CLICKED(IDC_BTN_Music_UnassignTrack, OnBTNMusicUnassignTrack)
	ON_BN_CLICKED(IDC_BTN_Music_UnassignWav, OnBTNMusicUnassignWav)
	ON_BN_CLICKED(IDC_BTN_MusicBrowseWav, OnBTNMusicBrowseWav)
	ON_BN_CLICKED(IDC_BTN_UI_TextureAssign, OnBTNUITextureAssign)
	ON_BN_CLICKED(IDC_BTN_UI_MissileDatabase, OnBTNUIMissileDatabase)
	/*
	ON_BN_CLICKED(IDC_BTN_MIS_AddMissile, OnBTNMISAddMissile)
	ON_BN_CLICKED(IDC_BTN_MIS_BrowseFileName, OnBTNMISBrowseFileName)
	ON_BN_CLICKED(IDC_BTN_MIS_DeleteMissile, OnBTNMISDeleteMissile)
	ON_BN_CLICKED(IDC_BTN_MIS_save, OnBTNMISsave)
	ON_BN_CLICKED(IDC_BTN_MIS_Load, OnBTNMISLoad)
	*/
	ON_BN_CLICKED(IDC_BTN_UI_BulletHole, OnBTNUIBulletHole)
	ON_BN_CLICKED(IDC_BTN_BH_AddHole, OnBTNBHAddHole)
	ON_BN_CLICKED(IDC_BTN_BH_Browse, OnBTNBHBrowse)
	ON_BN_CLICKED(IDC_BTN_BH_AlphaTexture, OnBTNBHAlphaTexture)
	ON_BN_CLICKED(IDC_BTN_BH_DeleteHole, OnBTNBHDeleteHole)
	ON_BN_CLICKED(IDC_BTN_BH_load, OnBTNBHload)
	ON_BN_CLICKED(IDC_BTN_BH_Material, OnBTNBHMaterial)
	ON_BN_CLICKED(IDC_BTN_BH_Save, OnBTNBHSave)
	ON_BN_CLICKED(IDC_BTN_Entity_shadow, OnBTNEntityshadow)
	ON_BN_CLICKED(IDC_BTN_Object_Shadow, OnBTNObjectShadow)
	ON_BN_CLICKED(IDC_BTN_ENT_AnimProperties, OnBTNENTAnimProperties)
	ON_BN_CLICKED(IDC_BTN_Entity_Sound, OnBTNEntitySound)
	ON_BN_CLICKED(IDC_BTN_Entity_setSoundCenter, OnBTNEntitysetSoundCenter)
	ON_BN_CLICKED(IDC_BTN_OBJ_Sound, OnBTNOBJSound)
	ON_BN_CLICKED(IDC_BTN_OBJ_AlphaTexture, OnBTNOBJAlphaTexture)
	ON_BN_CLICKED(IDC_BTN_ENT_AnimSound, OnBTNENTAnimSound)
	ON_BN_CLICKED(IDC_BTN_TCPIP_StartHost, OnBTNTCPIPStartHost)
	ON_BN_CLICKED(IDC_BTN_TCPIP_StopHost, OnBTNTCPIPStopHost)
	ON_BN_CLICKED(IDC_BTN_TCPIP_JoinHost, OnBTNTCPIPJoinHost)
	ON_BN_CLICKED(IDC_BTN_UI_Multiplayer, OnBTNUIMultiplayer)
	ON_BN_CLICKED(IDC_BTN_Entity_CombineEntityVis, OnBTNEntityCombineEntityVis)
	ON_BN_CLICKED(IDC_BTN_ENT_ContentProperties, OnBTNENTContentProperties)
	ON_BN_CLICKED(IDC_BTN_TEX_AddCollection, OnBTNTEXAddCollection)
	ON_BN_CLICKED(IDC_BTN_TEX_DeleteCollection, OnBTNTEXDeleteCollection)
	ON_BN_CLICKED(IDC_BTN_UI_AnimTextureDB, OnBTNUIAnimTextureDB)
	ON_BN_CLICKED(IDC_BTN_TEX_LoadCollection, OnBTNTEXLoadCollection)
	ON_BN_CLICKED(IDC_BTN_TEX_SaveCollection, OnBTNTEXSaveCollection)
	ON_BN_CLICKED(IDC_BTN_ENT_LoadDB, OnBTNENTLoadDB)
	ON_BN_CLICKED(IDC_BTN_ENT_SaveDB, OnBTNENTSaveDB)
	ON_BN_CLICKED(IDC_BTN_UI_SCRIPT, OnBtnUiScript)
	ON_BN_CLICKED(IDC_BTN_SCR_AddScript, OnBTNSCRAddScript)
	ON_BN_CLICKED(IDC_BTN_SCR_DeleteEvent, OnBTNSCRDeleteEvent)
	ON_BN_CLICKED(IDC_BTN_SCR_LoadExecFile, OnBTNSCRLoadExecFile)
	ON_BN_CLICKED(IDC_BTN_SCR_Build, OnBTNSCRBuild)
	ON_BN_CLICKED(IDC_BTN_SCR_EditEvent, OnBTNSCREditEvent)
	ON_BN_CLICKED(IDC_BTN_OBJ_EditLight, OnBTNOBJEditLight)
	ON_BN_CLICKED(IDC_BTN_ENT_LoadEntity, OnBTNENTLoadEntity)
	ON_BN_CLICKED(IDC_BTN_ENT_SaveEntity, OnBTNENTSaveEntity)
	ON_BN_CLICKED(IDC_BTN_MS_SaveDB, OnBTNMSSaveDB)
	ON_BN_CLICKED(IDC_BTN_MS_LoadDB, OnBTNMSLoadDB)
	/*
	ON_BN_CLICKED(IDC_BTN_TXTDB_ADD, OnBtnTxtdbAdd)
	ON_BN_CLICKED(IDC_BTN_TXTDB_DELETE, OnBtnTxtdbDelete)
	ON_BN_CLICKED(IDC_BTN_TXTDB_EDIT, OnBtnTxtdbEdit)
	ON_BN_CLICKED(IDC_BTN_TXTDB_REINIT_ALL, OnBtnTxtdbReinitAll)
	ON_BN_CLICKED(IDC_BTN_TXTDB_Browse, OnBTNTXTDBBrowse)
	ON_BN_CLICKED(IDC_BTN_TXTDB_TextureReport, OnBTNTXTDBTextureReport)
	ON_BN_CLICKED(IDC_BTN_TXTDB_AddUrl, OnBtnTxtdbAddUrl)
	*/
	ON_BN_CLICKED(IDC_BTN_OBJ_ClearTexCoords, OnBTNOBJClearTexCoords)
	ON_BN_CLICKED(IDC_BTN_OBJ_AddTexCoords, OnBTNOBJAddTexCoords)
	ON_BN_CLICKED(IDC_BTN_SCR_SetToScene, OnBTNSCRSetToScene)
	ON_BN_CLICKED(IDC_BTN_SCR_ClearScene, OnBTNSCRClearScene)
	ON_BN_CLICKED(IDC_BTN_SCR_LoadFromScene, OnBTNSCRLoadFromScene)
	ON_BN_CLICKED(IDC_BTN_NT_Advanced, OnBTNNTAdvanced)
	ON_BN_CLICKED(IDC_BTN_NT_BuildTxtRecord, OnBTNNTBuildTxtRecord)
	ON_BN_CLICKED(IDC_BTN_UI_HudInterface, OnBTNUIHudInterface)
	ON_BN_CLICKED(IDC_BTN_HUD_AddDisplayUnit, OnBTNHUDAddDisplayUnit)
	ON_BN_CLICKED(IDC_BTN_HUD_AddNewHud, OnBTNHUDAddNewHud)
	ON_BN_CLICKED(IDC_BTN_HUD_SaveDB, OnBTNHUDSaveDB)
	ON_BN_CLICKED(IDC_BTN_HUD_LoadDB, OnBTNHUDLoadDB)
	ON_LBN_SELCHANGE(IDC_LIST_HUD_DB, OnSelchangeListHudDb)
	ON_BN_CLICKED(IDC_BTN_HUD_DeleteHud, OnBTNHUDDeleteHud)
	ON_BN_CLICKED(IDC_BTN_HUD_DeleteDisplayUnit, OnBTNHUDDeleteDisplayUnit)
	ON_BN_CLICKED(IDC_BTN_HUD_ReplaceUnit, OnBTNHUDReplaceUnit)
	ON_BN_CLICKED(IDC_BTN_UI_MenuSystem, OnBTNUIMenuSystem)
	//ON_BN_CLICKED(IDC_BTN_MENU_AddMenu, OnBTNMENUAddMenu)
	//ON_BN_CLICKED(IDC_BTN_MENU_AddUnit, OnBTNMENUAddUnit)
	//ON_BN_CLICKED(IDC_BTN_MENU_Save, OnBTNMENUSave)
	//ON_BN_CLICKED(IDC_BTN_MENU_Load, OnBTNMENULoad)
	//ON_BN_CLICKED(IDC_BTN_MENU_EditMenu, OnBTNMENUEditMenu)
	//ON_BN_CLICKED(IDC_BTN_MENU_EditDisplayUnit, OnBTNMENUEditDisplayUnit)
	//ON_BN_CLICKED(IDC_BTN_MENU_DeleteMenu, OnBTNMENUDeleteMenu)
	//ON_BN_CLICKED(IDC_BTN_MENU_DeleteUnit, OnBTNMENUDeleteUnit)
	ON_BN_CLICKED(IDC_BTN_ENT_Update, OnBTNENTUpdate)
	
	ON_BN_CLICKED(IDC_CHECK_TEX_BltOn, OnCHECKTEXBltOn)
	ON_BN_CLICKED(IDC_BTN_Menu_Clone, OnBTNMenuClone)
	ON_BN_CLICKED(IDC_BTN_Menu_Material3D, OnBTNMenuMaterial3D)
	ON_BN_CLICKED(IDC_BTN_Menu_AddAnimation, OnBTNMenuAddAnimation)
	ON_LBN_SELCHANGE(IDC_LIST_Menu_Animations, OnSelchangeLISTMenuAnimations)
	ON_LBN_SELCHANGE(IDC_LIST_MENU_UnitList, OnSelchangeLISTMENUUnitList)
	ON_BN_CLICKED(IDC_BTN_Menu_properties, OnBTNMenuproperties)
	/*
	ON_BN_CLICKED(IDC_BTN_ENV_FogDay, OnBTNENVFogDay)
	ON_BN_CLICKED(IDC_CHECK_ENV_AmbientLightOn, OnCHECKENVAmbientLightOn)
	ON_BN_CLICKED(IDC_BTN_ENV_ambientDayLight, OnBTNENVambientDayLight)
	ON_BN_CLICKED(IDC_CHECK_ENV_TimeSystem, OnCHECKENVTimeSystem)
	ON_BN_CLICKED(IDC_CHECK_ENV_WindRandomizer, OnCHECKENVWindRandomizer)
	ON_LBN_SELCHANGE(IDC_LIST_TXTDB_Database, OnSelchangeLISTTXTDBDatabase)
	*/
	ON_BN_CLICKED(IDC_BTN_UI_ParticleSys, OnBTNUIParticleSys)
	ON_BN_CLICKED(IDC_BTN_PART_AddParticle, OnBTNPARTAddParticle)
	ON_BN_CLICKED(IDC_BTN_PART_EditSystem, OnBTNPARTEditSystem)
	ON_BN_CLICKED(IDC_BTN_PART_DeleteSystem, OnBTNPARTDeleteSystem)
	ON_BN_CLICKED(IDC_BTN_PART_LoadDatabase, OnBTNPARTLoadDatabase)
	ON_BN_CLICKED(IDC_BTN_PART_SaveDatabase, OnBTNPARTSaveDatabase)
	ON_BN_CLICKED(IDC_BTN_PART_TestSystem, OnBTNPARTTestSystem)
	ON_BN_CLICKED(IDC_BTN_PART_StopTest, OnBTNPARTStopTest)
	ON_BN_CLICKED(IDC_BTN_UI_SpawnGen, OnBTNUISpawnGen)
	/*
	ON_BN_CLICKED(IDC_BTN_Spawn_AddGen, OnBTNSpawnAddGen)
	ON_BN_CLICKED(IDC_BTN_Spawn_DeleteGen, OnBTNSpawnDeleteGen)
	ON_BN_CLICKED(IDC_BTN_Spawn_EditGen, OnBTNSpawnEditGen)
	*/
	ON_BN_CLICKED(IDC_BTN_Object_BuildCollisionMap, OnBTNObjectBuildCollisionMap)
	ON_NOTIFY(NM_CLICK, IDC_OBJECT_TREE, OnClickObjectTree)
	ON_NOTIFY(NM_RCLICK, IDC_OBJECT_TREE, OnClickObjectTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_OBJECT_TREE, OnSelchangedObjectTree)
	ON_BN_CLICKED(IDC_BTN_Objects_AddGroup, OnBTNObjectsAddGroup)
	ON_BN_CLICKED(IDC_BTN_Objects_DeleteGroup, OnBTNObjectsDeleteGroup)
	ON_BN_CLICKED(IDC_BTN_ObjectsEditGroup, OnBTNObjectsEditGroup)
	ON_LBN_SELCHANGE(IDC_LIST_Objects_GroupList2Box, OnSelchangeLISTObjectsGroupList2Box)
	ON_LBN_DBLCLK(IDC_LIST_Objects_GroupList2Box, OnDblclkLISTObjectsGroupList2Box)
	ON_NOTIFY(NM_DBLCLK, IDC_OBJECT_TREE, OnDblclkObjectTree)
	ON_BN_CLICKED(IDC_BTN_UI_Merge, OnBTNUIMerge)
	ON_BN_CLICKED(IDC_BTN_TCP_DisconnectPlayer, OnBTNTCPDisconnectPlayer)
	ON_BN_CLICKED(IDC_BTN_TCP_ServerLoad, OnBTNTCPServerLoad)
	ON_BN_CLICKED(IDC_BTN_TCP_ServerSave, OnBTNTCPServerSave)
	ON_BN_CLICKED(IDC_BTN_OptimizeStates, OnBTNOptimizeStates)
	ON_BN_CLICKED(IDC_BTN_TCP_NewAccount, OnBTNTCPNewAccount)
	ON_BN_CLICKED(IDC_BTN_TCP_DeleteAccount, OnBTNTCPDeleteAccount)
	ON_BN_CLICKED(IDC_BTN_OBJ_AddTrigger, OnBTNOBJAddTrigger)
	ON_BN_CLICKED(IDC_BTN_OBJ_UpdateObject, OnBTNOBJUpdateObject)
	ON_BN_CLICKED(IDC_CHECK_TCP_Registered, OnCHECKTCPRegistered)
	ON_LBN_SELCHANGE(IDC_LIST_TCP_IP_CurrentPlayers, OnSelchangeLISTTCPIPCurrentPlayers)
	/*
	ON_BN_CLICKED(IDC_BTN_TX_SaveTextureBank, OnBTNTXSaveTextureBank)
	ON_BN_CLICKED(IDC_BTN_TX_LoadTextureBank, OnBTNTXLoadTextureBank)
	*/
	ON_BN_CLICKED(IDC_BTN_MNU_AnimProperties, OnBTNMNUAnimProperties)
	ON_BN_CLICKED(IDC_BTN_MNU_DeleteAnimation, OnBTNMNUDeleteAnimation)
	ON_BN_CLICKED(IDC_BTN_OBJ_GetSignificant, OnBTNOBJGetSignificant)
	ON_BN_CLICKED(IDC_BTN_OBJ_TextureSetMacro, OnBTNOBJTextureSetMacro)
	ON_BN_CLICKED(IDC_BTN_UI_AI, OnBtnUiAi)
	/*
	ON_BN_CLICKED(IDC_BTN_AI_CreateNew, OnBTNAICreateNew)
	ON_BN_CLICKED(IDC_BTN_AI_Delete, OnBTNAIDelete)
	*/
	ON_BN_CLICKED(IDC_BTN_OBJ_LODADD, OnBtnObjLodadd)
	ON_BN_CLICKED(IDC_BTN_OBJ_RemoveLODLevel, OnBTNOBJRemoveLODLevel)
	ON_BN_CLICKED(IDC_BTN_OBJ_EditLOD, OnBTNOBJEditLOD)
	ON_BN_CLICKED(IDC_BTN_ENTITY_Animations, OnBTNENTITYAnimations)
	ON_BN_CLICKED(IDC_BTN_ENT_AppendCustomDB, OnBTNENTAppendCustomDB)
	ON_LBN_SELCHANGE(IDC_LIST_ENTITY_RuntimeList, OnSelchangeLISTENTITYRuntimeList)
	ON_LBN_DBLCLK(IDC_LIST_ENTITY_RuntimeList, OnDblclkLISTENTITYRuntimeList)
	ON_BN_CLICKED(IDC_BTN_ENT_AppendArm, OnBTNENTAppendArm)
	ON_BN_CLICKED(IDC_BTN_ENT_AppendDisarm, OnBTNENTAppendDisarm)
	ON_BN_CLICKED(IDC_BTN_UI_HaloEffects, OnBTNUIHaloEffects)
	ON_BN_CLICKED(IDC_BTN_HaloEffects_AddHalo, OnBTNAddHalo)
	ON_BN_CLICKED(IDC_BTN_HaloEffects_DeleteHalo, OnBTNDeleteHalo)
	ON_BN_CLICKED(IDC_BTN_HaloEffects_EditHalo, OnBTNEditHalo)
	ON_BN_CLICKED(IDC_BTN_HaloEffects_LoadDatabase, OnBTNHaloLoad)
	ON_BN_CLICKED(IDC_BTN_HaloEffects_SaveDatabase, OnBTNHaloSave)
	ON_BN_CLICKED(IDC_BTN_HaloEffects_AddLensFlare, OnBTNAddLensFlare)
	ON_BN_CLICKED(IDC_BTN_HaloEffects_EditLensFlare, OnBTNEditLensFlare)
	ON_BN_CLICKED(IDC_BTN_HaloEffects_DeleteLensFlare, OnBTNDeleteLensFlare)
	ON_BN_CLICKED(IDC_BTN_HaloEffects_GenerateMesh, OnBTNGenerateMesh)
	ON_BN_CLICKED(IDC_BTN_ENT_LODSystems, OnBTNENTLODSystems)
	ON_BN_CLICKED(IDC_BTN_ENT_BaseSkills, OnBTNENTBaseSkills)
	ON_BN_CLICKED(IDC_BTN_ENT_ResourceBase, OnBTNENTResourceBase)
	ON_BN_CLICKED(IDC_BTN_ENT_ViewRuntimeStats, OnBTNENTViewRuntimeStats)
	ON_BN_CLICKED(IDC_BTN_ENT_ViewRuntimeResources, OnBTNENTViewRuntimeResources)
	/*
	ON_LBN_SELCHANGE(IDC_LIST_MIS_missileDataBase, OnSelchangeLISTMISmissileDataBase)
	ON_BN_CLICKED(IDC_BTN_MIS_ReplaceValues, OnBTNMISReplaceValues)
	ON_BN_CLICKED(IDC_BTN_MIS_EditVisual, OnBTNMISEditVisual)
	ON_BN_CLICKED(IDC_BTN_MIS_AddBillboardMissile, OnBTNMISAddBillboardMissile)
	ON_BN_CLICKED(IDC_BTN_MIS_EditDynamics, OnBTNMISEditDynamics)
	*/
	ON_BN_CLICKED(IDC_BTN_ENT_EditStartInven, OnBTNENTEditStartInven)
	ON_CBN_DROPDOWN(IDC_CMBO_CGTEX_FontTableTexture, OnDropdownCMBOCGTEXFontTableTexture)
	ON_BN_CLICKED(IDC_BTN_CGTEX_Add, OnBTNCGTEXAdd)
	ON_BN_CLICKED(IDC_BTN_CGTEX_Delete, OnBTNCGTEXDelete)
	/*
	ON_BN_CLICKED(IDC_BTN_AI_AddEvent, OnBTNAIAddEvent)
	ON_BN_CLICKED(IDC_BTN_AI_AddScript, OnBTNAIAddScript)
	ON_BN_CLICKED(IDC_BTN_AI_EditScript, OnBTNAIEditScript)
	ON_BN_CLICKED(IDC_BTN_AI_DeleteScript, OnBTNAIDeleteScript)
	ON_BN_CLICKED(IDC_BTN_AI_EditEvent, OnBTNAIEditEvent)
	ON_BN_CLICKED(IDC_BTN_AI_DeleteEvent, OnBTNAIDeleteEvent)
	ON_BN_CLICKED(IDC_BTN_AI_AddTree, OnBTNAIAddTree)
	ON_BN_CLICKED(IDC_BTN_AI_DeleteTree, OnBTNAIDeleteTree)
	ON_BN_CLICKED(IDC_BTN_AI_Edit, OnBTNAIEdit)
	ON_BN_CLICKED(IDC_BTN_AI_SaveAiData, OnBTNAISaveAiData)
	ON_BN_CLICKED(IDC_BTN_AI_LoadAiData, OnBTNAILoadAiData)
	ON_LBN_SELCHANGE(IDC_LIST_AI_Scripts, OnSelchangeLISTAIScripts)
	*/
	ON_BN_CLICKED(IDC_BTN_ENT_SaveAppendages, OnBTNENTSaveAppendages)
	ON_BN_CLICKED(IDC_BTN_ENT_LoadAppendages, OnBTNENTLoadAppendages)
	ON_BN_CLICKED(IDC_BTN_ENT_AddAppendages, OnBTNENTAddAppendages)
	ON_BN_CLICKED(IDC_BTN_ENT_MOUNT, OnBtnEntMount)
	ON_BN_CLICKED(IDC_BTN_ENT_DEMOUNT, OnBtnEntDemount)
	//ON_BN_CLICKED(IDC_BTN_EXP_EditExplosionObj, OnBTNEXPEditExplosionObj)
	//ON_BN_CLICKED(IDC_BTN_MIS_Sound, OnBTNMISSound)
	ON_BN_CLICKED(IDC_BTN_MUSIC_AddTrack, OnBTNMUSICAddTrack)
	ON_BN_CLICKED(IDC_BTN_MUSIC_DeleteTrack, OnBTNMUSICDeleteTrack)
	ON_BN_CLICKED(IDC_BTN_MUSIC_LoadDB, OnBTNMUSICLoadDB)
	ON_BN_CLICKED(IDC_BTN_MUSIC_SaveDB, OnBTNMUSICSaveDB)
	ON_BN_CLICKED(IDC_BTN_MUSIC_TestTrack, OnBTNMUSICTestTrack)
	ON_BN_CLICKED(IDC_BTN_MUSIC_StopSoundTrack, OnBTNMUSICStopSoundTrack)
	//ON_BN_CLICKED(IDC_BTN_MenuCursor, OnBTNMenuCursor)
	//ON_BN_CLICKED(IDC_BTN_Menu_Test, OnBTNMenuTest)
	//ON_BN_CLICKED(IDC_BTN_MenuCloseAllMenus, OnBTNMenuCloseAllMenus)
	//ON_LBN_SELCHANGE(IDC_LIST_MENU_MainDB, OnSelchangeLISTMENUMainDB)
	//ON_BN_CLICKED(IDC_BTN_Menu_cursorScale, OnBTNMenucursorScale)
	//ON_BN_CLICKED(IDC_BTN_Menu_cursorDelete, OnBTNMenucursorDelete)
	//ON_BN_CLICKED(IDC_BTN_MNU_AddClickZone, OnBTNMNUAddClickZone)
	//ON_BN_CLICKED(IDC_BTN_MNU_editClickZone, OnBTNMNUeditClickZone)
	//ON_BN_CLICKED(IDC_BTN_MNU_GetCursor, OnBTNMNUGetCursor)
	ON_BN_CLICKED(IDC_BTN_TX_PurgeTextures, OnBTNTXPurgeTextures)
	ON_EN_CHANGE(IDC_EDIT_TCP_IPadress, OnChangeEDITTCPIPadress)
	ON_EN_CHANGE(IDC_TCP_userName, OnChangeTCPuserName)
	ON_EN_CHANGE(IDC_TCP_password, OnChangeTCPpassword)
	ON_BN_CLICKED(IDC_BTN_SystemGoingDown, OnBTNSystemGoingDown)
	ON_BN_CLICKED(IDC_BTN_ENT_AddItem, OnBTNENTAddItem)
	ON_BN_CLICKED(IDC_BTN_ENT_ItemTest, OnBTNENTItemTest)
	ON_BN_CLICKED(IDC_BTN_ENT_DeleteItem, OnBTNENTDeleteItem)
	ON_BN_CLICKED(IDC_BTN_ENT_DisarmItem, OnBTNENTDisarmItem)
	ON_BN_CLICKED(IDC_BTN_ENT_EditItem, OnBTNENTEditItem)
	ON_BN_CLICKED(IDC_BTN_OBJ_Clone, OnBTNOBJClone)
	ON_BN_CLICKED(IDC_BTN_OBJ_Transformation, OnBTNOBJTransformation)
	ON_BN_CLICKED(IDC_BTN_OBJ_BoundingGroupSet, OnBTNOBJBoundingGroupSet)
	ON_BN_CLICKED(IDC_BTN_OBJ_LibraryAccess, OnBTNOBJLibraryAccess)
	//ON_BN_CLICKED(IDC_BTN_ENV_TimeSystem, OnBTNENVTimeSystem)
	/*
	ON_BN_CLICKED(IDC_BTN_MIS_AddSkeletalMissile, OnBTNMISAddSkeletalMissile)
	ON_BN_CLICKED(IDC_BTN_MIS_AddLaser, OnBTNMISAddLaser)
	*/
	ON_BN_CLICKED(IDC_BTN_ENT_defSwapNext, OnBTNENTdefSwapNext)
	ON_BN_CLICKED(IDC_BTN_ENT_dimPrev, OnBTNENTdimPrev)
	//ON_BN_CLICKED(IDC_BTN_AI_EditAITree, OnBTNAIEditAITree)
	ON_BN_CLICKED(IDC_BTN_TCP_SupportedWorlds, OnBTNTCPSupportedWorlds)
	ON_BN_CLICKED(IDC_BTN_ENT_EditDeathCfg, OnBTNENTEditDeathCfg)
	ON_BN_CLICKED(IDC_BTN_TCP_ViewAccount, OnBTNTCPViewAccount)
	ON_BN_CLICKED(IDC_CHECK_MNU_renderClizeZoneInfo, OnCHECKMNUrenderClizeZoneInfo)
	//ON_BN_CLICKED(IDC_BTN_MNU_DeleteClickZone, OnBTNMNUDeleteClickZone)
	ON_BN_CLICKED(IDC_CHECK_TXB_BitmapLoadOverride, OnCHECKTXBBitmapLoadOverride)
	ON_BN_CLICKED(IDC_BTN_UI_SkillBuilder, OnBTNUISkillBuilder)
	ON_BN_CLICKED(IDC_BTN_OBJ_AddNode, OnBTNOBJAddNode)
	ON_BN_CLICKED(IDC_BTN_OBJ_ReferenceLibrary, OnBTNOBJReferenceLibrary)
	ON_BN_CLICKED(IDC_BTN_UI_SoundDB, OnBTNUISoundDB)
	ON_BN_CLICKED(IDC_BTN_UI_GlInventoryDB, OnBTNUIGlInventoryDB)
	ON_BN_CLICKED(IDC_BTN_UI_CURRENCYSYS, OnBtnUiCurrencysys)
	ON_BN_CLICKED(IDC_BTN_UI_CommerceDB, OnBTNUICommerceDB)
	ON_BN_CLICKED(IDC_BTN_UI_BankZones, OnBTNUIBankZones)
	ON_BN_CLICKED(IDC_BTN_UI_SAFEZONES, OnBtnUiSafezones)
	ON_BN_CLICKED(IDC_BTN_ENT_RefreshList, OnBTNENTRefreshList)
	ON_BN_CLICKED(IDC_BTN_ENT_UnifySleletalMesh, OnBTNENTUnifySleletalMesh)
	/*
	ON_BN_CLICKED(IDC_BTN_MIS_DeleteLight, OnBTNMISDeleteLight)
	ON_BN_CLICKED(IDC_BTN_MIS_LightSys, OnBTNMISLightSys)
	*/
	ON_BN_CLICKED(IDC_BTN_UI_STATDB, OnBtnUiStatdb)
	ON_BN_CLICKED(IDC_BTN_TCP_ItemGeneratorDB, OnBTNTCPItemGeneratorDB)
	ON_BN_CLICKED(IDC_BTN_PATCHSYS, OnBtnPatchsys)
	ON_BN_CLICKED(IDC_BTN_TCP_UpdateCurrentOnlinePlayers, OnBTNTCPUpdateCurrentOnlinePlayers)
	ON_BN_CLICKED(IDC_BTN_ENT_ScaleEntity, OnBTNENTScaleEntity)
	//ON_BN_CLICKED(IDC_CHECK_SPN_EnableSystem, OnCHECKSPNEnableSystem)
	//ON_BN_CLICKED(IDC_BTN_TXB_SetMipLevels, OnBTNTXBSetMipLevels)
	ON_BN_CLICKED(IDC_BTN_TCP_UpdateAllAcounts, OnBTNTCPUpdateAllAcounts)
	ON_BN_CLICKED(IDC_BTN_ENT_ReplaceEntityWSIE, OnBTNENTReplaceEntityWSIE)
	ON_BN_CLICKED(IDC_BTN_TCP_IPBlocking, OnBTNTCPIPBlocking)
	ON_BN_CLICKED(IDC_BTN_UI_WATERZONES, OnBtnUiWaterzones)
	//ON_BN_CLICKED(IDC_BTN_MNU_SaveUnit, OnBTNMNUSaveUnit)
	//ON_BN_CLICKED(IDC_BTN_MNU_ImportUnit, OnBTNMNUImportUnit)
	ON_BN_CLICKED(IDC_BTN_UI_PORTALZONESDB, OnBtnUiPortalzonesdb)
	ON_BN_CLICKED(IDC_BTN_UI_ElementDB, OnBTNUIElementDB)
	ON_BN_CLICKED(IDC_BTN_UI_DotDB, OnBTNUIDotDB)
	ON_BN_CLICKED(IDC_BTN_TCP_UpgradeWorm, OnBTNTCPUpgradeWorm)
	/*
	ON_BN_CLICKED(IDC_BTN_EXP_SaveUnit, OnBTNEXPSaveUnit)
	ON_BN_CLICKED(IDC_BTN_EXP_LoadUnit, OnBTNEXPLoadUnit)
	*/
	ON_BN_CLICKED(IDC_BTN_TCP_DisconnectAll, OnBTNTCPDisconnectAll)
	ON_BN_CLICKED(IDC_BTN_PRT_SaveUnit, OnBTNPRTSaveUnit)
	ON_BN_CLICKED(IDC_BTN_PRT_LoadUnit, OnBTNPRTLoadUnit)
	ON_BN_CLICKED(IDC_BTN_TCP_PrintReports, OnBTNTCPPrintReports)
	//ON_BN_CLICKED(IDC_BTN_TXB_RemoveDuplicates, OnBTNTXBRemoveDuplicates)
	/*
	ON_BN_CLICKED(IDC_BTN_AI_LoadAiUnit, OnBTNAILoadAiUnit)
	ON_BN_CLICKED(IDC_BTN_AI_SaveAiUnit, OnBTNAISaveAiUnit)
	ON_BN_CLICKED(IDC_BTN_AI_ImportCollisionSys, OnBTNAIImportCollisionSys)
	ON_BN_CLICKED(IDC_BTN_AI_DeleteSys, OnBTNAIDeleteSys)
	ON_BN_CLICKED(IDC_BTN_AI_ExportCurrentCol, OnBTNAIExportCurrentCol)
	ON_BN_CLICKED(IDC_BTN_AI_SaveColDB, OnBTNAISaveColDB)
	ON_BN_CLICKED(IDC_BTN_AI_LoadColDB, OnBTNAILoadColDB)
	*/
	//ON_BN_CLICKED(IDC_BTN_MNU_ExportVisual, OnBTNMNUExportVisual)
	//ON_BN_CLICKED(IDC_BTN_MNU_ImportVisual, OnBTNMNUImportVisual)
	/*
	ON_BN_CLICKED(IDC_BTN_MIS_SaveUnit, OnBTNMISSaveUnit)
	ON_BN_CLICKED(IDC_BTN_MIS_LoadUnit, OnBTNMISLoadUnit)
	*/
	/*
	ON_BN_CLICKED(IDC_BTN_SPN_ExportSys, OnBTNSPNExportSys)
	ON_BN_CLICKED(IDC_BTN_SPN_ImportSys, OnBTNSPNImportSys)
	*/
	//ON_BN_CLICKED(IDC_BTN_AI_IntegrityCheck, OnBTNAIIntegrityCheck)
	ON_BN_CLICKED(IDC_BTN_TCP_SearchForAccount, OnBTNTCPSearchForAccount)
	ON_BN_CLICKED(IDC_BTN_TCP_RestoreBackup, OnBTNTCPRestoreBackup)
	ON_BN_CLICKED(IDC_BTN_TXT_SymbolDB, OnBTNTXTSymbolDB)
	ON_BN_CLICKED(IDC_CHECK_EditorTxtMode, OnCHECKEditorTxtMode)
	ON_BN_CLICKED(IDC_BTN_ENT_MassFormatNames, OnBTNENTMassFormatNames)
	ON_BN_CLICKED(IDC_CHK_OBJ_EnablePlaementHotKey, OnCHKOBJEnablePlaementHotKey)
	ON_BN_CLICKED(IDC_BTN_OBJ_ReportCurRenderCache, OnBTNOBJReportCurRenderCache)
	ON_BN_CLICKED(IDC_BTN_UI_IconDBACCESS, OnBTNUIIconDBACCESS)
	ON_BN_CLICKED(IDC_BTN_TCP_SnapShotPacketQue, OnBTNTCPSnapShotPacketQue)
	ON_BN_CLICKED(IDC_BTN_TCP_GenerateLogBasedOnTime, OnBTNTCPGenerateLogBasedOnTime)
	ON_BN_CLICKED(IDC_BTN_UI_HousingSys, OnBTNUIHousingSys)
	ON_BN_CLICKED(IDC_BTN_MNU_TestIcon, OnBTNMNUTestIcon)
	ON_BN_CLICKED(IDC_BTN_UI_HszoneDB, OnBTNUIHszoneDB)
	ON_BN_CLICKED(IDC_BTN_Chk_showSpeed, OnBTNChkshowSpeed)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_UI_AIRainSys, OnBTNUIAIRainSys)
	/*
	ON_BN_CLICKED(IDC_BTN_AI_newTree, OnBTNAInewTree)
	ON_BN_CLICKED(IDC_BTN_AI_Integrity_Check, OnBTNAIIntegrityCheck)
	ON_LBN_SELCHANGE(IDC_LIST_AI_WebTrees, OnSelchangeLISTAItree)
	*/
	ON_BN_CLICKED(IDC_BTN_TCP_ExportAccount, OnBTNTCPExportAccount)
	ON_BN_CLICKED(IDC_BTN_TCP_ImportAccount, OnBTNTCPImportAccount)
	ON_BN_CLICKED(IDC_BTN_TCP_GenerateClanDatabase, OnBTNTCPGenerateClanDatabase)
	ON_BN_CLICKED(IDC_BTN_TCP_ClearInactive, OnBTNTCPClearInactive)
	ON_BN_CLICKED(IDC_BTN_ENT_saveItemDB, OnBTNENTsaveItemDB)
	ON_BN_CLICKED(IDC_BTN_ENT_LoadItemDB, OnBTNENTLoadItemDB)
	ON_BN_CLICKED(IDC_BTN_UI_HaloEffects, OnBTNUIHaloEffects)
	//ON_BN_CLICKED(IDC_BTN_SPN_AppendSys, OnBTNSPNAppendSys)
	ON_BN_CLICKED(IDC_BTN_UI_testassign, OnBTNTestAssign)
    ON_BN_CLICKED(IDC_BTN_CommCntrlLibBtnA, OnBTNCommCntrlLibBtnA)
    ON_BN_CLICKED(IDD_BTN_UI_WorldRules, OnBTNWorldRule)
	ON_BN_CLICKED(IDD_BTN_UI_ArenaManager, OnBTNArenaManager)
	//ON_BN_CLICKED(IDC_BUTTON_UpdateAllTextures, OnBtnUpdateAllTextures)
    //ON_BN_CLICKED(IDC_BUTTON_AIExportToSql, OnBTNAIExportToSql)
	ON_BN_CLICKED(IDC_CHECK_WireMode, OnCHECKWireMode)
    ON_BN_CLICKED(IDC_CHECK_ShadeMode, OnCHECKShadeMode)
	ON_EN_CHANGE(IDC_EDIT_forceLODGlobal, OnChangeEDITForceLodGlobal)
	ON_BN_CLICKED(IDC_BTN_UI_MainEnvironment, OnBTNMainEnvironment)
	ON_BN_CLICKED(IDC_BTN_UI_MainCharacter, OnBTNMainCharacter)
	ON_BN_CLICKED(IDC_BTN_UI_MainMenus, OnBTNMainMenus)
	ON_BN_CLICKED(IDC_BTN_UI_MainFX, OnBTNMainFX)
	/*
	ON_BN_CLICKED(IDC_BTN_MIS_AddChannel, OnBTNAddMissileChannel)
	ON_BN_CLICKED(IDC_BTN_MIS_RemoveChannel, OnBTNRemoveMissileChannel)
	*/
	
	//New Menu System Buttons
	ON_LBN_SETFOCUS(IDC_LIST_MenuThemes, OnRefreshListThemes)
	ON_LBN_SELCHANGE(IDC_LIST_MenuThemes, OnRefreshListThemes)
	ON_BN_CLICKED(IDC_BUTTON_AddTheme, OnButtonAddTheme)
	ON_BN_CLICKED(IDC_BUTTON_EditTheme, OnButtonEditTheme)
	ON_BN_CLICKED(IDC_BUTTON_TestTheme, OnButtonTestTheme)
	ON_BN_CLICKED(IDC_BUTTON_ApplyTheme, OnButtonApplyTheme)
	ON_BN_CLICKED(IDC_BUTTON_ExportTheme, OnButtonExportTheme)
	ON_BN_CLICKED(IDC_BUTTON_ImportTheme, OnButtonImportTheme)
	ON_BN_CLICKED(IDC_BUTTON_DeleteTheme, OnButtonDeleteTheme)
	ON_LBN_SETFOCUS(IDC_LIST_Menus, OnRefreshListMenus)
	ON_LBN_SELCHANGE(IDC_LIST_Menus, OnRefreshListMenus)
	ON_BN_CLICKED(IDC_BUTTON_AddMenu, OnButtonAddMenu)
	ON_BN_CLICKED(IDC_BUTTON_EditMenu, OnButtonEditMenu)
	ON_BN_CLICKED(IDC_BUTTON_MoveMenuUp, OnButtonMoveMenuUpList)
	ON_BN_CLICKED(IDC_BUTTON_MoveMenuDown, OnButtonMoveMenuDownList)
	ON_BN_CLICKED(IDC_BUTTON_TestMenu, OnButtonTestMenu)
	ON_BN_CLICKED(IDC_BUTTON_CloseAllMenus, OnButtonCloseAllMenus)
	ON_BN_CLICKED(IDC_BUTTON_ExportMenu, OnButtonExportMenu)
	ON_BN_CLICKED(IDC_BUTTON_ImportMenu, OnButtonImportMenu)
	ON_BN_CLICKED(IDC_BUTTON_CloneMenu, OnButtonCloneMenu)
	ON_BN_CLICKED(IDC_BUTTON_DeleteMenu, OnButtonDeleteMenu)
	ON_LBN_SETFOCUS(IDC_LIST_MenuVisuals, OnRefreshListMenuVisuals)
	ON_LBN_SELCHANGE(IDC_LIST_MenuVisuals, OnRefreshListMenuVisuals)
	ON_BN_CLICKED(IDC_BUTTON_AddVisual, OnButtonAddVisual)
	ON_BN_CLICKED(IDC_BUTTON_EditVisual, OnButtonEditVisual)
	ON_BN_CLICKED(IDC_BUTTON_MoveVisualUp, OnButtonMoveVisualUpList)
	ON_BN_CLICKED(IDC_BUTTON_MoveVisualDown, OnButtonMoveVisualDownList)
	ON_BN_CLICKED(IDC_BUTTON_ExportVisual, OnButtonExportVisual)
	ON_BN_CLICKED(IDC_BUTTON_ImportVisual, OnButtonImportVisual)
	ON_BN_CLICKED(IDC_BUTTON_DeleteVisual, OnButtonDeleteVisual)
	ON_BN_CLICKED(IDC_BUTTON_CloneVisual, OnButtonCloneVisual)
	ON_BN_CLICKED(IDC_BUTTON_IDCheck, OnButtonIDCheck)
    ON_BN_CLICKED(IDC_BUTTON_TestMenuAsInstance, OnButtonTestAsInstance)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuSystemFonts, OnButtonEditMenuSystemFonts)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuIcons, OnButtonEditMenuIcons)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuSystemCursor, OnButtonEditMenuSystemCursor)
	ON_BN_CLICKED(IDC_BUTTON_SaveMenuSystem, OnButtonSaveMenuSystem)
	ON_BN_CLICKED(IDC_BUTTON_LoadMenuSystem, OnButtonLoadMenuSystem)
	ON_BN_CLICKED(IDC_BUTTON_SaveKeyConfig, OnButtonSaveKeyConfig)
	ON_BN_CLICKED(IDC_BUTTON_CopyMenu, OnButtonCopyMenuWindow)
	ON_BN_CLICKED(IDC_BUTTON_PasteMenu, OnButtonPasteMenuWindow)
	ON_BN_CLICKED(IDC_BUTTON_CopyVisual, OnButtonCopyMenuVisual)
	ON_BN_CLICKED(IDC_BUTTON_PasteVisual, OnButtonPasteMenuVisual)

	//ON_BN_CLICKED(IDC_BUTTON_explosionTest, OnBTNEXPTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//extensions for organization in win class
#include "ControlsWinExt.h"
#include "UserInterface.h"
#include "QuickTools.h"
#include "ui_menus.h"
#include "CleanUp.h"
#include "NetworkFunctionsExt.h"


//extern "C" void WINAPI InitAuraXExtDLL();
//extern "C" void WINAPI InitAXNetworkExtDLL();
//--------------Basic Construction code-----------------------------------------------//
EngineXWin::EngineXWin()
{
	//MAIN INTERFACE HOOK (Windows dependant libs line)
    /*#ifdef _axDynDll
	   InitAuraXExtDLL();
	   InitAXNetworkExtDLL();
       m_newMalInterface = new CMalInterface();
    #endif*/
    CountGlob = GetTickCount();
	m_newMalInterface = NULL;
	m_newMalInterface = new CMalInterface();

	this->m_pClipboardMenuVisual = NULL;
	this->m_pClipboardMenuWindow = NULL;

	// Initialize Explosions Database dialog
	this->m_pExplosionsDbDlg = 0;

	// Initialize Weapon System dialog
	this->m_pWeaponSystemDlg = 0;

	// Initialize Environment Settings dialog
	this->m_pEnvironmentSettingsDlg = 0;

	// Initialize AI System dialog
	this->m_pAISystemDlg = 0;

	// Initialize Spawn Generator dialog
	this->m_pSpawnGeneratorDlg = 0;

	// Initialize Texture Database dialog
	this->m_pTextureDatabaseDlg = 0;
    
	ZeroMemory(&g_materialInit7,sizeof(D3DMATERIAL9));
	g_materialInit7.Ambient.r = 1.0f;
	g_materialInit7.Ambient.g = 1.0f;
	g_materialInit7.Ambient.b = 1.0f;
	g_materialInit7.Ambient.a = 1.0f;
	g_materialInit7.Diffuse.r = 1.0f;
	g_materialInit7.Diffuse.g = 1.0f;
	g_materialInit7.Diffuse.b = 1.0f;
	g_materialInit7.Diffuse.a = 1.0f;
	m_refreshInterface = FALSE;
	m_dbgSequencer = 0;
	m_mapBatchIndex = -1;
	m_quickEditorTrace = -1;
	m_quickPlayTrace = -1;
	m_mapBatchPhase = 0;
	m_mapBatchStamp = 0;
	m_mapBatchOk = m_mapBatchFail = 0;
	//DEBUGGING
	//g_bumpmap = NULL;
	g_switch = 0;
	m_axmedia           = NULL;
	m_globalSqlHandle   = NULL;
	m_progressInternal   = NULL;
	testLength          = 0.0f;
	GL_physicsAdjust    = 1.0f;
    GL_physicsLastAdjust= 1.0f;
	m_snapshotPacketQue = FALSE;
    m_totalEntitypolys = 0;
	m_placementHotKeyEnabled = FALSE;
    m_editorTextModeOn = FALSE;
	m_skillTimeStampLocal = GetTickCount();
	
	m_accountModule = NULL;
    m_itemRequestDuration = 1000;
    m_itemRequestStamp = GetTickCount();
	m_respawnTimeStamp = GetTickCount();
	m_restrictMovement = FALSE;	
	m_triggerTestTemp  = FALSE;
	m_miscIndex = 0;
    //Modules
	m_currentWldsSpawnPointModule = NULL;
	m_accountViewerModule         = NULL;
	m_entityViewerModule          = NULL;  
    m_forceSpeed = 15;
	m_lastKnownFov = 0.0f;
	
	setIt = TRUE;
	GL_counterThing = 0;
	m_movementStamp = GetTickCount();
	
	m_networkPingDuration = 5000;
	m_networkPingStamp = GetTickCount();
	GL_skeletalTest = NULL;
//querry high res timer

//SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST );
GL_exactFPS = 0;

GL_mislTimeStampTest = GetTickCount();

gl_indexTemp=1;
GL_mouseHiResX = 0.0f;
GL_mouseHiResY = 0.0f;

GL_specularOn = FALSE;
//GLOBAL WORLD COORD
GL_showBoundingBoxs = FALSE; 
GL_TracerMan=0;
GL_dedicatedRenderLoop = FALSE;
GL_pingFrequency = 20;
GL_pingFrequencyVar = 0;
GL_dumpMessagesVar = 0;
//GLOBAL Text File defined Issues
InvertMouse = TRUE;
GL_QueOutPut = FALSE;
//MOuse
GL_mouseCurrent = 0;
//GLOBAL OPTIMIZERS
GL_lastMultitextureMode1 = -1;
GL_lastMultitextureMode2 = -1;
GL_lastMultitextureMode3 = -1;
GL_lastMultitextureMode4 = -1;
GL_lastMultitextureMode5 = -1;
GL_lastMultitextureMode6 = -1;
GL_lastMultitextureMode7 = -1;
GLHUDTEST = NULL;
//EFFECTS
Blend_TwoXValid = FALSE;
EntityCollisionOn = TRUE;
//Multiplayer
GL_RecordNetworkActivity = FALSE;
RENDERALL = TRUE;
//timing 
updateTimeBasis = 30;
lastUpdatesdTimeBasis = 0;
GL_RenderTimingBasis = 32;
GL_LastUpdatedRenderTime = 0;
GL_MovTimingBasis = 15;
GL_AnimTimingBasis = 20;
GL_LastUpdatedAnimTime = 0;
GL_FpsTimeTrack = 0;
GL_FpsLastTime = 0;
GL_MouseState = 20;
GL_MouseStateVarial = 0;
skipVarial = 0;
//Global


VicinityObjLoadCalled = FALSE;
VicinityAdditionalUnits = 3000.0f;
//script temp file
GL_scriptList = NULL;
SelectionModeOn = FALSE;
//client
LagCount = 0;
MaxLagCount = 1000;
//Screen Test resoulution varial
ResolutionHieght = 480;
ResolutionWidth = 640;
GL_totalTimePassed = 0;
GL_interpolateFpsBasis = 100;
//network 
PINGLAST=0;

RENDERLOOPTIME = 0;
GlobalMouseSensitivity = .5f;
maxBulletHoleObjects = 40;
ErrorMessage = "";
musicLoop = NULL;
//box init
D3DboxInit.min.x = 0.0f;
D3DboxInit.max.x = 0.0f;
D3DboxInit.min.y = 0.0f;
D3DboxInit.max.y = 0.0f;
D3DboxInit.min.z = 0.0f;
D3DboxInit.max.z = 0.0f;

//surface temp

updateEditorVarial=GetTickCount();

for(int loop=0;loop<4;loop++)
for(int loop2=0;loop2<4;loop2++){
        MatrixInit[loop][loop2]=0.0f;	    
}

//Mirrors
GL_MirrorRendering = FALSE;

initOnce = FALSE;

GlobalIndex = 0;
SKIPFACTOR = 0;//for frame skipping
PREVTICCOUNT = 0;
CURRTICCOUNT = 0;
LIGHTSINUSE = 0;
FPSCURACTUAL = 0;
//FPSLIMIT = 40;
host = 0;
//Global timing varials
EnvironmentAnimationTimingCurIndex=0;
EnvironmentAnimationTimingVarial=0;
AnimationTimingCurIndex=0;
AnimationTimingVarial=5;
//sound
sound3DListener = NULL;
//lists
NetworkRecordList			= NULL;
m_currencyObject            = NULL;

NetworkRecordList			= new CNetworkKeyObjList;
m_currencyObject            = new CCurrencyObj();


//Global Bowl Ship for mouse
GlobalLastPosMouseX=0;
GlobalLastPosMouseY=0;
GlobalMouseRect.left = 0;
GlobalMouseRect.right = 0;
GlobalMouseRect.top = 0;
GlobalMouseRect.bottom = 0;
GlobalMouseCenterPoint.x = 0;
GlobalMouseCenterPoint.y = 0;


mouseButtonOneDown = FALSE;
mouseButtonTwoDown = FALSE;
MouseControlX=0;
MouseControlY=0;
//search paths

GL_takingDamage      = FALSE;
GL_takingDamageStamp = GetTickCount();
m_useItemTimeStamp   = GetTickCount();

	//New Menu System Stuff
	m_addMenuDlg		= NULL;
	m_cursorsDlg		= NULL;
	m_fontsDlg			= NULL;
	m_iconsDlg			= NULL;
	m_editComboBoxDlg	= NULL;
	m_editListBoxDlg	= NULL;
	m_editTextBoxDlg	= NULL;
	m_editTextDlg		= NULL;
	m_editScrollBarDlg	= NULL;
	m_editButtonDlg		= NULL;
	m_editToggleDlg		= NULL;
	m_editProgBarDlg	= NULL;
	m_editImageDlg		= NULL;
	m_editInventoryDlg	= NULL;
	m_editEntityDlg		= NULL;
	m_menuThemeDatabase	= new CMenuVisualList();
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Initial scene create--------------------------------------------------//
BOOL EngineXWin::CreateScene()
{  
	BaseSceneSetup();
    


	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Base Scene setup---------------------------------------------------//
void EngineXWin::BaseSceneSetup()
{
	
		
}
//--------------Effects Log-----------------------------------------------------------//
void EngineXWin::Effects()
{

}
//--------------Main Render Loop------------------------------------------------------//

void EngineXWin::RenderLoop()
{//BEGIN FUNCTION

  if(!m_clientEngine)
	  return;
  
  if(!m_clientEngine->m_initObject->m_gStatus)
	  return;
  // the engine is torn down (CleanUp.h) while the message loop is still running:
  // never touch a renderer / device that is already gone
  if(!m_gStatus || !m_gStatus->m_dxRenderer || !m_gStatus->m_3dDevice)
	  return;
	
	
  long curTickCount = GetTickCount();
  //musicPlayer->Callback(curTickCount);
  if(initOnce == FALSE)//init once
	 initOnce = TRUE;

  if(m_clientEngine->m_initObject->m_gStatus->m_idOfWorldObjectLastHit > -1)
  {
     SetCursorToSelectedObject(m_clientEngine->m_initObject->m_gStatus->m_idOfWorldObjectLastHit);
     m_clientEngine->m_initObject->m_gStatus->m_idOfWorldObjectLastHit = -1;
  }

  BOOL objBarVis = (m_dlgBarObject.m_hWnd && m_dlgBarObject.IsWindowVisible());
  BOOL entBarVis = (m_entityDlgBar.m_hWnd && m_entityDlgBar.IsWindowVisible());
  if(objBarVis)
  {
     int pickedIdx = m_gStatus->m_tgSys_indexOfObjectSelected;
     if(pickedIdx != g_editorLast3DPickedObjIndex)
     {
        g_editorLast3DPickedObjIndex = pickedIdx;
        if(pickedIdx >= 0 && m_gStatus->m_collisionBase &&
           m_gStatus->m_collisionBase->m_wldObjectReferenceArray &&
           m_gStatus->m_wldObjList &&
           pickedIdx < m_gStatus->m_wldObjList->GetCount())
        {
           CWldObject * pickedObj = m_gStatus->m_collisionBase->m_wldObjectReferenceArray[pickedIdx].worldObjectReference;
           if(pickedObj)
              SetCursorToSelectedObject(pickedObj->List_identity);
        }
     }
     UpdateEditorObjectHighlight(m_gStatus, TRUE, FALSE);
  }
  else if(entBarVis && g_editorHighlightEntityIndex != -1)
  {
     UpdateEditorObjectHighlight(m_gStatus, FALSE, TRUE);
  }
     
  m_clientEngine->CallBackTimeCap();      
  m_clientEngine->CallBackTimeOpen();


  if(m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL.GetLength() > 0)
  {//streaming music time
     if(m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL.CompareNoCase("stop")==0)
	    m_axmedia->Stop();
	 else
	 {
        BOOL resMerror = m_axmedia->PlayURL(m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL.GetBuffer());
		m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_currentVolume = 0;
        
		if(m_gStatus->m_systemReadout)
			m_gStatus->m_systemReadout->AddLine("Now Playing " + m_axmedia->WhatsPlaying(), MENU_TEXTCOLOR_MUSIC);
	 }
	 //handle volume
	
	 m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_currentURL =  m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL;
     m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL = "";

	 m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_musicStreamerEnabled = TRUE;
  }//end streaming music time

  if(m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_musicStreamerEnabled)
  {
     if(m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabled != m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabledCurState)
	 {
        m_axmedia->Stop();
		m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_trackInUse = -1;
        m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabledCurState = m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabled;
	 }

	 if(m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_currentTargetVolume != m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_lastSetVolume)
	 {
			m_axmedia->SetVolume(m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_currentTargetVolume);
			m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_lastSetVolume = m_clientEngine->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_currentTargetVolume;
	 }
  }


  if((curTickCount-m_gStatus->GL_spawnGeneratorTimeVarial)>5000){//deal once a minute
	  SpawnGeneratorCallback();
	  m_gStatus->GL_spawnGeneratorTimeVarial = GetTickCount();
  }//end deal once a minute
  if((GetTickCount() - lastUpdatesdTimeBasis) > updateTimeBasis)
  {//timed basis thingys
	  lastUpdatesdTimeBasis = GetTickCount();
  }//end timed basis thingys
	
  ControlsInLoop();//DigitalControls Callback in loop
  //ServerCallback();InitializeEnvironment(FALSE); 
  m_clientEngine->m_initObject->m_userControls->MouseManagerCallback();
  m_clientEngine->ClientCallback(); 
  
  //Sleep(50);//REMOVE ME
  
  RenderLoopMessageHandling();	
  UpdateInfoEverySecond();
  MapBatchStep();

  if(m_gStatus->m_closeDownCall)
	  PostMessage( WM_CLOSE );


	 if(m_refreshInterface)
	 {
	    
	    WorldGroupInList(-1);
	    EnitityInList();
		m_gStatus->InListTextureDB(0);
		m_gStatus->SpawnGenDatabaseInList(0);
		m_refreshInterface = FALSE;
		if(this->m_pAISystemDlg)
			this->m_pAISystemDlg->AITreeInList(-1);
	 } 

return;
}//END FUNCTION

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------on create-------------------------------------------------------------//
int EngineXWin::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (RMWin::OnCreate(lpCreateStruct) == -1)
		return -1;
    
	HWND hwndLoc = GetSafeHwnd();
	//initialize organized
	
	m_axmedia = new CAXmediaFrame();
    BOOL rs = m_axmedia->Create(IDD_DIALOG_axMedia,this);
	
    //--------------------------------------------------//
   //--------------------------------------------------//
   //-------------TOOL BARS FOR EDITOR-----------------//
	#ifndef _ClientOutput 
	
		//EnableDocking(CBRS_ALIGN_ANY);
	//initialize all bars
    UINT styleDock = CBRS_RIGHT;

	//styleDock = CBRS_NOALIGN;   
    //#ifndef _LimitedEngine
	if (!m_wndDlgBar.Create(this, IDD_DIALOG_DLGBARUI,
		styleDock, IDD_DIALOG_DLGBARUI))
	{
		ReportDlgBarFailure("IDD_DIALOG_DLGBARUI", IDD_DIALOG_DLGBARUI, "m_wndDlgBar");
		return -1;
	}
	m_wndDlgBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndDlgBar.SendDlgItemMessage(IDC_PROGRESS_UI,PBM_SETBARCOLOR,0,RGB(0,255,0));
	m_wndDlgBar.SendDlgItemMessage(IDC_PROGRESS_UI,PBM_SETBKCOLOR,0,RGB(0,0,0));

    //object interface
	if (!m_dlgBarObject.Create(this, IDD_DIALOG_OBJECTADD,
		styleDock, IDD_DIALOG_OBJECTADD))
	{
		ReportDlgBarFailure("IDD_DIALOG_OBJECTADD", IDD_DIALOG_OBJECTADD, "m_dlgBarObject");
		return -1;
	}
	m_dlgBarObject.EnableDocking(CBRS_ALIGN_ANY);
	m_dlgBarObject.SendDlgItemMessage(IDC_PROGRESS_OBJ,PBM_SETBARCOLOR,0,RGB(0,255,0));
	m_dlgBarObject.SendDlgItemMessage(IDC_PROGRESS_OBJ,PBM_SETBKCOLOR,0,RGB(0,0,0));

    //background interface
    if (!m_backgroundDlgBar.Create(this, IDD_DIALOG_BACKGROUND,
		styleDock, IDD_DIALOG_BACKGROUND))
	{
		ReportDlgBarFailure("IDD_DIALOG_BACKGROUND", IDD_DIALOG_BACKGROUND, "m_backgroundDlgBar");
		return -1;
	}
	m_backgroundDlgBar.EnableDocking(CBRS_ALIGN_ANY);
   
	//controls interface
    
	if (!m_controlsDlgBar.Create(this, IDD_DIALOG_CONTROLERS,
		styleDock, IDD_DIALOG_CONTROLERS))
	{
		ReportDlgBarFailure("IDD_DIALOG_CONTROLERS", IDD_DIALOG_CONTROLERS, "m_controlsDlgBar");
		return -1;
	}
	m_controlsDlgBar.EnableDocking(CBRS_ALIGN_ANY);
	UpdateControlList();
    
	//movies interface
    
	if (!m_moviesDlgBar.Create(this, IDD_DIALOG_MOVIES,
		styleDock, IDD_DIALOG_MOVIES))
	{
		ReportDlgBarFailure("IDD_DIALOG_MOVIES", IDD_DIALOG_MOVIES, "m_moviesDlgBar");
		return -1;
	}
	m_moviesDlgBar.EnableDocking(CBRS_ALIGN_ANY);
    //entity movement interface
    if (!m_entityDlgBar.Create(this, IDD_DIALOG_ENTITY,
		styleDock, IDD_DIALOG_ENTITY))
	{
		ReportDlgBarFailure("IDD_DIALOG_ENTITY", IDD_DIALOG_ENTITY, "m_entityDlgBar");
		return -1;
	}
	m_entityDlgBar.EnableDocking(CBRS_ALIGN_ANY);

	//cameras and viwports interface
    if (!m_camVptDlgBar.Create(this, IDD_DIALOG_CAMERAviewports,
		styleDock, IDD_DIALOG_CAMERAviewports))
	{
		ReportDlgBarFailure("IDD_DIALOG_CAMERAviewports", IDD_DIALOG_CAMERAviewports, "m_camVptDlgBar");
		return -1;
	}
	m_camVptDlgBar.EnableDocking(CBRS_ALIGN_ANY);

	// Main Menus Options Creation
    if (!m_mainMenus.Create(this, IDD_DIALOG_MAIN_MENUS,
		styleDock, IDD_DIALOG_MAIN_MENUS))
	{
		ReportDlgBarFailure("IDD_DIALOG_MAIN_MENUS", IDD_DIALOG_MAIN_MENUS, "m_mainMenus");
		return -1;
	}
	m_mainMenus.EnableDocking(CBRS_ALIGN_ANY);

	// Main Character Options Creation
    if (!m_mainCharacter.Create(this, IDD_DIALOG_MAIN_CHARACTER,
		styleDock, IDD_DIALOG_MAIN_CHARACTER))
	{
		ReportDlgBarFailure("IDD_DIALOG_MAIN_CHARACTER", IDD_DIALOG_MAIN_CHARACTER, "m_mainCharacter");
		return -1;
	}
	m_mainCharacter.EnableDocking(CBRS_ALIGN_ANY);

	// Main Environment Options Creation
	if (!m_mainEnvironment.Create(this, IDD_DIALOG_MAIN_ENVIRONMENT,
		styleDock, IDD_DIALOG_MAIN_ENVIRONMENT))
	{
		ReportDlgBarFailure("IDD_DIALOG_MAIN_ENVIRONMENT", IDD_DIALOG_MAIN_ENVIRONMENT, "m_mainEnvironment");
		return -1;
	}
	m_mainEnvironment.EnableDocking(CBRS_ALIGN_ANY);

	// Main FX Options Creation
    if (!m_mainFX.Create(this, IDD_DIALOG_MAIN_FX,
		styleDock, IDD_DIALOG_MAIN_FX))
	{
		ReportDlgBarFailure("IDD_DIALOG_MAIN_FX", IDD_DIALOG_MAIN_FX, "m_mainFX");
		return -1;
	}
	m_mainFX.EnableDocking(CBRS_ALIGN_ANY);

	m_camVptDlgBar.SetDlgItemInt( IDC_EDIT_TOPpercent,0);
    m_camVptDlgBar.SetDlgItemInt( IDC_EDIT_BottomPercent,100);
    m_camVptDlgBar.SetDlgItemInt( IDC_EDIT_LeftPercent,0);
    m_camVptDlgBar.SetDlgItemInt( IDC_EDIT_RightPercent,100);
	m_camVptDlgBar.SetDlgItemText( IDC_EDIT_FOV,"0.7");
    m_camVptDlgBar.SetDlgItemText( IDC_EDIT_CLIPPLANE,"1000.0");
	m_camVptDlgBar.SetDlgItemText( IDC_COMBO_CAMERAtype,"Fixed");
	m_camVptDlgBar.SendDlgItemMessage(IDC_LIST_VIEWPORTS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Standard Vport");
	m_camVptDlgBar.SendDlgItemMessage(IDC_LIST_CAMERAS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Standard Camera");
	//mouse interface
	if (!m_mouseDlgBar.Create(this, IDD_DIALOG_MOUSEsettings,
		styleDock, IDD_DIALOG_MOUSEsettings))
	{
		ReportDlgBarFailure("IDD_DIALOG_MOUSEsettings", IDD_DIALOG_MOUSEsettings, "m_mouseDlgBar");
		return -1;
	}
	m_mouseDlgBar.EnableDocking(CBRS_ALIGN_ANY);
    m_mouseDlgBar.SetDlgItemInt( IDC_EDIT_MOUSESrcRed,0);
	m_mouseDlgBar.SetDlgItemInt( IDC_EDIT_MOUSESrcGreen,0);
	m_mouseDlgBar.SetDlgItemInt( IDC_EDIT_MouseSrcBlue,0);
    m_mouseDlgBar.SetDlgItemInt( IDC_EDIT_MOUSESizeX,5);
	m_mouseDlgBar.SetDlgItemInt( IDC_EDIT_MOUSESizeY,5);
	m_mouseDlgBar.SetDlgItemInt( IDC_EDIT_MOUSEframeCount,1);
	m_mouseDlgBar.SetDlgItemText( IDC_EDIT_MOUSEfile,"Mouse Filename");
	//2Dinterface
	if (!m_2DinterfaceBar.Create(this, IDD_DIALOG_2Dinterface,
		styleDock, IDD_DIALOG_2Dinterface))
	{
		ReportDlgBarFailure("IDD_DIALOG_2Dinterface", IDD_DIALOG_2Dinterface, "m_2DinterfaceBar");
		return -1;
	}
	m_2DinterfaceBar.EnableDocking(CBRS_ALIGN_ANY);
	m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_RedSrc,"-1");
	m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_GreenSrc,"0");
	m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_BlueSrc,"0");
	m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_Attribute,"0");
	m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_TOP,"0");
	m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_BOTTOM,"10");
	m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_LEFT,"0");
	m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_RIGHT,"10");
	m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_TRANSLUCENCY,".5");
	m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2DcollectionName,"Unique ID");
	m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_FadeInSpeed,".02");
    //Enviorment
	/* DEPRECATED /////////////////////////////////////////////////////////////
	if (!m_enviormentBar.Create(this, IDD_DIALOG_ENVIORMENT,
		styleDock, IDD_DIALOG_ENVIORMENT))
	{
		ReportDlgBarFailure("IDD_DIALOG_ENVIORMENT", IDD_DIALOG_ENVIORMENT, "m_enviormentBar");
		return -1;
	}
	m_enviormentBar.EnableDocking(CBRS_ALIGN_ANY);
    m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_StartRange,"10.0");
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_EndRange,"1000.0");
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_Density,"1.0");
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogRed,"0.0");
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogGreen,"0.0");
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogBlue,"0.0");
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckRed,"0.0");
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckGreen,"0.0");
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckBlue,"0.0");

	m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_enableSun,BM_SETCHECK ,BST_CHECKED,0);
	/////////////////////////////////////////////////////////////////////////*/
    //text and font dlg
	if (!m_textFontDlgBar.Create(this, IDD_DIALOG_TEXTinterface,
		styleDock, IDD_DIALOG_TEXTinterface))
	{
		ReportDlgBarFailure("IDD_DIALOG_TEXTinterface", IDD_DIALOG_TEXTinterface, "m_textFontDlgBar");
		return -1;
	}
	m_textFontDlgBar.EnableDocking(CBRS_ALIGN_ANY);
    TextDisplayTreeROOT = AddTreeRoot("Text Display Database",IDC_TREE_TEXTdisplays,&m_textFontDlgBar);
	//m_textFontDlgBar.SendDlgItemMessage(IDC_CMBO_CGTEX_TargetTex,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"HELLO");
	m_textFontDlgBar.SetDlgItemText( IDC_EDIT_text_percX,"0");
	m_textFontDlgBar.SetDlgItemText( IDC_EDIT_text_percY,"0");
    m_textFontDlgBar.SetDlgItemText( IDC_EDIT_text_SizeX,"2");
	m_textFontDlgBar.SetDlgItemText( IDC_EDIT_text_SizeY,"2");
	m_textFontDlgBar.SetDlgItemText( IDC_EDIT_text_timeLimit,"-1");
	//explosion dlg
	/*if (!m_explosionDlgBar.Create(this, (UINT)IDD_DIALOG_EXPLOSIONS,
		styleDock, (UINT)IDD_DIALOG_EXPLOSIONS))
	{
		TRACE0("Failed to create DlgBar\n");
		return -1;
	}
	m_explosionDlgBar.EnableDocking(CBRS_ALIGN_ANY);
	m_explosionDlgBar.SetDlgItemText( IDC_EDIT_EXP_VisCount,"10");
	m_explosionDlgBar.SetDlgItemText( IDC_EDIT_EXP_FadeSpeed,".05");
    m_explosionDlgBar.SetDlgItemText( IDC_EDIT_EXP_FadeStart,"0");
	m_explosionDlgBar.SetDlgItemText( IDC_EDIT_EXP_radiusExplosion,"0.0");*/
    //music dlg UI
    if (!m_musicDlgBar.Create(this, IDD_DIALOG_MUSIC,
		styleDock, IDD_DIALOG_MUSIC))
	{
		ReportDlgBarFailure("IDD_DIALOG_MUSIC", IDD_DIALOG_MUSIC, "m_musicDlgBar");
		return -1;
	}
	m_musicDlgBar.EnableDocking(CBRS_ALIGN_ANY);
    //missile interface
    /*if (!m_missileDlgBar.Create(this, IDD_DIALOG_MISSILE_Interface,
		styleDock, IDD_DIALOG_MISSILE_Interface))
	{
		ReportDlgBarFailure("IDD_DIALOG_MISSILE_Interface", IDD_DIALOG_MISSILE_Interface, "m_missileDlgBar");
		return -1;
	}
	m_missileDlgBar.EnableDocking(CBRS_ALIGN_ANY);
	m_missileDlgBar.SetDlgItemText( IDC_EDIT_MIS_MissileName,           "Name");
	m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_LifeTime,             2000,TRUE);
	m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_Damage,               10,TRUE);
	m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_ParticleDBlink,       -1,TRUE);
	m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_ExplosionAttribute,   0,TRUE);
	m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_AbilityUseType,       -1,TRUE);
	m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_SkillUseType,         -1,TRUE);	 
	m_missileDlgBar.SetDlgItemText( IDC_EDIT_MIS_CollisionRadius,m_gStatus->m_cabLib.FloatToStingCnv(5.0f));
	m_missileDlgBar.SetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,m_gStatus->m_cabLib.FloatToStingCnv(0.0f));
	m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_damPercentage,		0, TRUE);
	m_missileDlgBar.SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_SETCURSEL ,0,0);*/
	 
	 
	//bullet hole interface
	if (!m_bulletHoleDlgBar.Create(this, IDD_DIALOG_BULLETHOLEInterface,
		styleDock, IDD_DIALOG_BULLETHOLEInterface))
	{
		ReportDlgBarFailure("IDD_DIALOG_BULLETHOLEInterface", IDD_DIALOG_BULLETHOLEInterface, "m_bulletHoleDlgBar");
		return -1;
	}
	m_bulletHoleDlgBar.EnableDocking(CBRS_ALIGN_ANY);
    
	
	//Animated Texture Interface
	if (!m_animatedTextureDBDlgBar.Create(this, IDD_DIALOG_AnimTextures,
		styleDock, IDD_DIALOG_AnimTextures))
	{
		ReportDlgBarFailure("IDD_DIALOG_AnimTextures", IDD_DIALOG_AnimTextures, "m_animatedTextureDBDlgBar");
		return -1;
	}
	m_animatedTextureDBDlgBar.EnableDocking(CBRS_ALIGN_ANY);
	//Script Interface
	if (!m_scriptDlgBar.Create(this, IDD_DIALOG_SCRIPTING,
		styleDock, IDD_DIALOG_SCRIPTING))
	{
		ReportDlgBarFailure("IDD_DIALOG_SCRIPTING", IDD_DIALOG_SCRIPTING, "m_scriptDlgBar");
		return -1;
	}
	m_scriptDlgBar.EnableDocking(CBRS_ALIGN_ANY);
    //texture database
	/*
	if (!m_textureDBDlgBar.Create(this, IDD_DIALOG_TEXTUREDATA,
		styleDock, IDD_DIALOG_TEXTUREDATA))
	{
		ReportDlgBarFailure("IDD_DIALOG_TEXTUREDATA", IDD_DIALOG_TEXTUREDATA, "m_textureDBDlgBar");
		return -1;
	}
	m_textureDBDlgBar.EnableDocking(CBRS_ALIGN_ANY);
	m_textureDBDlgBar.SendDlgItemMessage(IDC_CHECK_TEX_BltOn,BM_SETCHECK ,BST_CHECKED,0);
	*/
    //hud interface
	if (!m_hudDBDlgBar.Create(this, IDD_DIALOG_HUDInterface,
		styleDock, IDD_DIALOG_HUDInterface))
	{
		ReportDlgBarFailure("IDD_DIALOG_HUDInterface", IDD_DIALOG_HUDInterface, "m_hudDBDlgBar");
		return -1;
	}
	m_hudDBDlgBar.EnableDocking(CBRS_ALIGN_ANY);
    //
	if (!m_menuDBDlgBar.Create(this, IDD_DIALOG_MenuSysMain,
		styleDock, IDD_DIALOG_MenuSysMain))
	{
		ReportDlgBarFailure("IDD_DIALOG_MenuSysMain", IDD_DIALOG_MenuSysMain, "m_menuDBDlgBar");
		return -1;
	}
	m_menuDBDlgBar.EnableDocking(CBRS_ALIGN_ANY);

    if (!m_particleDBDlgBar.Create(this, IDD_DIALOG_ParticleSystem,
		styleDock, IDD_DIALOG_ParticleSystem))
	{
		ReportDlgBarFailure("IDD_DIALOG_ParticleSystem", IDD_DIALOG_ParticleSystem, "m_particleDBDlgBar");
		return -1;
	}
	m_particleDBDlgBar.EnableDocking(CBRS_ALIGN_ANY);
	
	/* DEPRECATED /////////////////////////////////////////////////////////
    if (!m_spawnGenDBDlgBar.Create(this, IDD_DIALOG_SpawnGeneratorUI,
		styleDock, IDD_DIALOG_SpawnGeneratorUI))
	{
		ReportDlgBarFailure("IDD_DIALOG_SpawnGeneratorUI", IDD_DIALOG_SpawnGeneratorUI, "m_spawnGenDBDlgBar");
		return -1;
	}
	m_spawnGenDBDlgBar.EnableDocking(CBRS_ALIGN_ANY);
	
    if (!m_aiDBDlgBar.Create(this, IDD_DIALOG_AI_BOTS,
		styleDock, IDD_DIALOG_AI_BOTS))
	{
		ReportDlgBarFailure("IDD_DIALOG_AI_BOTS", IDD_DIALOG_AI_BOTS, "m_aiDBDlgBar");
		return -1;
	}
	m_aiDBDlgBar.EnableDocking(CBRS_ALIGN_ANY);
	/////////////////////////////////////////////////////////////////////*/
	
	//multiplayer
    if (!m_multiPlayer.Create(this, IDD_DIALOG_TCP_IP,
		styleDock, IDD_DIALOG_TCP_IP))
	{
		ReportDlgBarFailure("IDD_DIALOG_TCP_IP", IDD_DIALOG_TCP_IP, "m_multiPlayer");
		return -1;
	}
	m_multiPlayer.EnableDocking(CBRS_ALIGN_ANY);
	m_multiPlayer.SetDlgItemInt(IDC_EDIT_TCP_CameraIndex,0,TRUE);
	//#endif
	//editor prep

      CloseAllToolDialogs();
      OpenToolBar(&m_wndDlgBar);

    #endif

    if(m_gStatus)
    if(m_gStatus->m_editorModeOn == TRUE)
	{//refresh lists 
	   EnitityInList();//
	   SoundTrackInList(0);//
	   ParticleDatabaseInList(-1);//
	   RefreshMenuList(0);//
	   WorldGroupInList(-1);//
	   EnitityInList();//
	   //InListTextureDB(0);//
	   InListCGFonts(0);//
	   TextFontInterfaceInlist();//
	   InListControlEvents();
       
		/* DEPRECATED /////////////////////////////////////////////////////////
		InListExplosionDB(0);
		MissileDatabaseInList(-1);
		this->m_pAISystemDlg->SpawnGenerator()->SpawnGenDatabaseInList(0);
		if(this->m_pAISystemDlg)
		{
			this->m_pAISystemDlg->AI_DBInList(-1);
			this->m_pAISystemDlg->ScriptInList(-1);
			this->m_pAISystemDlg->AITreeInList(-1);
		}
		/////////////////////////////////////////////////////////////////////*/

		if(this->m_pEnvironmentSettingsDlg)
		{
			this->m_pEnvironmentSettingsDlg->RefreshEnviorment();
		}


		if(m_newMalInterface)
		{
		   m_newMalInterface->InitializeInterfaces(&m_gStatus->m_waterZoneDB,
													m_gStatus->m_dxRenderer->m_pd3dDevice,
												   &m_gStatus->m_aiList,
												   &m_gStatus->m_textureDataBase,
												   &m_gStatus->m_postProcessor,
												   &m_gStatus->m_pxShaderSystem,
												   &m_userControls,
												   &m_gStatus->m_soundTrackList,
												   &m_gStatus,
												   m_gStatus->m_dirSnd,
												   m_gStatus->m_dxRenderer->m_hwnd);
		   m_newMalInterface->OpenMenuBar();
		   // Nav UI lives in the main menu bar (File | World | Characters | ... | Help),
		   // not in a floating window; fall back to the window if the frame has no menu.
		   if(!m_newMalInterface->AttachNavMenuToFrame(this))
		      m_newMalInterface->ShowNavWindow();
		}

	   //INVOKE IT
	   /*#ifndef _ClientOutput 
	   m_newMalInterface->InitializeInterfaces(&m_gStatus->m_waterZoneDB,
		                                        m_gStatus->m_dxRenderer->m_pd3dDevice,
											   &m_gStatus->m_multiplayerObject->m_kingdomManager,
											   &m_gStatus->m_aiList,
											   &m_gStatus->m_textureDataBase,
											   &m_gStatus->m_postProcessor,
											   &m_gStatus->m_pxShaderSystem,
											   &m_userControls,
											   &m_gStatus->m_soundTrackList,
											   m_gStatus->m_dirSnd,
										       m_gStatus->m_dxRenderer->m_hwnd);

	   m_newMalInterface->OpenMenuBar();
       #endif*/
	}//end refresh lists

	
	//m_axmedia->PlayURL("http://www.digitallyimported.com/wma/chillout.asx");
  
	
	::SetForegroundWindow(hwndLoc);
	::SetFocus(hwndLoc);
	ShowWindow(SW_SHOW);
	if(m_clientEngine)
	{
		m_clientEngine->CallBackTimeOpen();
		if(m_clientEngine->m_initObject->m_gStatus)
		{
			m_clientEngine->m_initObject->m_gStatus->m_dxRenderer->EnableManagerCallback();
			// Initialize default controls (WASD, mouse, jump, etc.) so entities can be moved offline in the editor
			m_clientEngine->m_initObject->m_gStatus->LoadControlCfgList("");
		}
	}

	RefreshMenuList(-1);

	//SetForegroundWindow();
	return 0;

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----Resets display----if you specify it will search and diregard videomode----------//
BOOL EngineXWin::CreateDisplay(BOOL fulscreenLoc,int driverLoc,int videoModeLoc,int widthLoc,int hieghtLoc,int depthLoc){

return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------Movement update---------------------------------------------//
BOOL EngineXWin::MovementUpdate(DWORD curTickCount)
{//TIMED MOV
    curTickCount = GetTickCount();
    if((curTickCount-m_gStatus->GL_spawnGeneratorTimeVarial)>5000){//deal once a minute
        SpawnGeneratorCallback();
        m_gStatus->GL_spawnGeneratorTimeVarial = curTickCount;
	}//end deal once a minute
    if((curTickCount - lastUpdatesdTimeBasis) > updateTimeBasis){//timed basis thingys
    //MULTIPLAYER
        lastUpdatesdTimeBasis = GetTickCount();
	}//end timed basis thingys
	ControlsInLoop();//DigitalControls Callback in loop
	//ServerCallback();
    m_clientEngine->ClientCallback();
  
 	
	return TRUE;
}
//----------------------------------------------------------//
void EngineXWin::BoundingBoxesRender(D3DVECTOR offset)
{
    if(GL_showBoundingBoxs == TRUE){
	   m_gStatus->m_renderStateObject->SetFogFilter(m_gStatus->m_dxRenderer->m_pd3dDevice,TRUE);	
	   for(POSITION posLocal = m_gStatus->m_wldObjList->GetHeadPosition();posLocal != NULL;){//loop through objects
	       CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLocal);
	       if(ptrLoc->List_showBox == TRUE)
		      DrawBox(m_gStatus->m_dxRenderer->m_pd3dDevice,
			  ptrLoc->List_popoutBox.max,
			  ptrLoc->List_popoutBox.min,
			  offset);
	   }//end loop through objects
	   m_gStatus->m_renderStateObject->SetFogFilter(m_gStatus->m_dxRenderer->m_pd3dDevice,FALSE);
	}
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPGenerateLogBasedOnTime() 
{
   
}
//-----------------------------------------------------------//
void EngineXWin::OnBTNUIHousingSys() 
{
#ifndef _ClientOutput
    CHousingSysDlg dlg;
	dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	dlg.m_houseDBREF = m_gStatus->m_housingDB;
	dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
	dlg.DoModal();
	m_gStatus->m_housingDB = dlg.m_houseDBREF;
	if(dlg.m_spwnHouseOnClose)
	{
       POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
       if(!entPosition){
           MessageBox("no runtime spawned");
	       return;
	   }
	   CMovementObj * entityPtr = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
	   D3DVECTOR position;
	   entityPtr->List_baseFrame->GetPosition(NULL,&position);
       m_gStatus->m_houseingCache->PlaceHouse(position,dlg.m_houseSelection,22);

	}
#endif
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void EngineXWin::OnBTNMNUTestIcon() 
{
/*	CMenu3DObj * menuObj = NULL;
	   for(POSITION rMenuPos = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();rMenuPos != NULL;)
	   {
           menuObj  = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(rMenuPos);
		   if(menuObj->m_specialOperation != 0)
              break;
	   }
	 
	   CString tempString; 
       m_menuDBDlgBar.GetDlgItemText( IDC_EDIT_MNU_scrPosX,tempString);
  
       int dbIndex = m_menuDBDlgBar.GetDlgItemInt(IDC_EDIT_MNU_IconDBIndex,NULL,TRUE);
	   //IDC_EDIT_MNU_IconDBIndex
	 
	   D3DVECTOR screenPos;
	   screenPos.x = (float)atof((LPCTSTR)tempString);
        
       m_menuDBDlgBar.GetDlgItemText( IDC_EDIT_MNU_scrPosY,tempString);
	   
	   screenPos.y = (float)atof((LPCTSTR)tempString);
	   screenPos.z = 3.0f;

           if(!menuObj->m_iconDynamicOverlays)
			   menuObj->m_iconDynamicOverlays = new CIconSysList();

		       menuObj->m_iconDynamicOverlays->AddIconFromDBtoThisDB(m_gStatus->m_iconDatabase,
										                             dbIndex,
										                             screenPos,
										                             m_gStatus->m_dxRenderer->m_pd3dDevice);
*/}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
// File > Run Startup Script (.aut)...  - pick any .aut and execute it now (the
// same thing the engine does with autoexec.aut at start-up). Lets you load a
// different data set / world without renaming files or restarting.
void EngineXWin::OnFileRunScript()
{
#ifndef _ClientOutput
	if(!m_gStatus) return;
	static char BASED_CODE filter[] = "Autoexec Files|*.aut|All Files (*.*)|*.*||";
	CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
	if ( opendialog.DoModal() != IDOK )
		return;
	CString fileName = opendialog.GetPathName();
	SetCurrentDirectory(m_gStatus->GL_workingDirectory);
	if(!m_gStatus->RunScript(fileName))
		MessageBox("Could not run script:\n" + fileName + "\n\nSee GeneralClientLog.txt", "Run Startup Script", MB_ICONERROR);
	SetCurrentDirectory(m_gStatus->GL_workingDirectory);
	m_refreshInterface = TRUE;      // repopulate the editor lists from the new data
	RefreshMenuList(0);
	SoundTrackInList(0);
	ParticleDatabaseInList(-1);
	InListCGFonts(0);
	TextFontInterfaceInlist();
	InListControlEvents();
#endif
}
//-----------------------------------------------------------//
// File > Load Zone (.EXG)...  - same as the LOAD button on the main tool bar.
void EngineXWin::OnFileLoadZone()
{
#ifndef _ClientOutput
	OnButtonLoad();
#endif
}
//-----------------------------------------------------------//
BOOL EngineXWin::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if(RMWin::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	// Nav UI menu commands merged into our menu bar are handled by the (hidden) Nav UI dialog
	if(m_newMalInterface && m_newMalInterface->RouteNavCommand(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	return FALSE;
}
//-----------------------------------------------------------//
void EngineXWin::OnBTNUIHszoneDB() 
{
#ifndef _ClientOutput
	// "Hs Zone DB" button: house placement zone editor. The handler was left
	// empty in the original tree (the button never worked); wire it up the
	// same way the Housing Sys / Bank Zones buttons are. The zone DB is what
	// script attribute 41 loads from EARevolution.HPB.
	if(!m_gStatus || !m_gStatus->m_multiplayerObject)
		return;
	if(!m_gStatus->m_multiplayerObject->m_houseingDB)
		m_gStatus->m_multiplayerObject->m_houseingDB = new CHousingZoneDB();

	CHousePlacementZoneDlg dlg;
	dlg.m_housingZonesDB  = m_gStatus->m_multiplayerObject->m_houseingDB;
	dlg.m_houseObjListRef = m_gStatus->m_housingDB;
	dlg.g_pd3dDeviceRef   = m_gStatus->m_dxRenderer ? m_gStatus->m_dxRenderer->m_pd3dDevice : NULL;
	dlg.DoModal();
	// the dialog's Load button replaces the list object
	m_gStatus->m_multiplayerObject->m_houseingDB = dlg.m_housingZonesDB;
#endif
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void EngineXWin::OnBTNChkshowSpeed() 
{
	if(m_gStatus->m_speedOmeterOn)
	   m_gStatus->m_speedOmeterOn = FALSE;
	else
	   m_gStatus->m_speedOmeterOn = TRUE;
	
}
//-----------------------------------------------------------//
void EngineXWin::OnTimer(UINT nIDEvent) 
{
	//RenderLoop();
	//AfxMessageBox("TEST");
	
	RMWin::OnTimer(nIDEvent);
}
//-----------------------------------------------------------//
/*void EngineXWin::OnBTNAIExportToSql()
{
     CSqlConnectionDlg dlg;
     if(dlg.DoModal() == IDOK)
	 {
        if(!m_gStatus->m_aiList->ExportToSQL(dlg.m_ipAddress, dlg.m_database, dlg.m_username, dlg.m_password))
			AfxMessageBox("Connection Failed");
		else
		    AfxMessageBox("Completed");
	 }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNAIIntegrityCheck() 
{
#ifndef _ClientOutput
    //output AII system to sql
    

	//check web boundries
	if(m_gStatus->GL_aiWebDatabase)
	for(POSITION posLoc = m_gStatus->GL_aiWebDatabase->GetHeadPosition();posLoc != NULL;)
	{
        CAIWebObj * web = (CAIWebObj *)m_gStatus->GL_aiWebDatabase->GetNext(posLoc);
        for(POSITION wPos = web->m_web->GetHeadPosition();wPos != NULL;)
		{
            CAIWebNode * nodePtr = (CAIWebNode *) web->m_web->GetNext(wPos);

            if(fabs(nodePtr->m_position.x) > 29000)
			   MessageBox(operator+("Out of range: ",web->m_name));

			if(fabs(nodePtr->m_position.y) > 29000)
			   MessageBox(operator+("Out of range: ",web->m_name));

			if(fabs(nodePtr->m_position.z) > 29000)
			   MessageBox(operator+("Out of range: ",web->m_name));


		}
	}
	AfxMessageBox("Beginning DIM cfg check");
	AIdimIntegrityCheck();
	AfxMessageBox("Completed Bounds tree check");
	AfxMessageBox("Begginning item integrity check");
	//check inventory validity
	int lastSelectedPersist = 0;

	if(m_gStatus->m_aiList)
       m_gStatus->m_aiList->RefreshData(m_gStatus->m_itemManager);

    if(m_gStatus->m_aiList)
    for(POSITION posLoc = m_gStatus->m_aiList->GetHeadPosition();posLoc != NULL;){
        CAIObj * partPtr = (CAIObj *)m_gStatus->m_aiList->GetNext(posLoc);
        
        if(!partPtr->m_inventoryCfg)
			continue;

		POSITION rPosLast;
		for(POSITION rPos = partPtr->m_inventoryCfg->GetHeadPosition();(rPosLast = rPos) != NULL;)
		{//1
            CInventoryObj * invPtr = (CInventoryObj *)partPtr->m_inventoryCfg->GetNext(rPos);

            CItemObj * glidPtr = m_gStatus->m_itemManager->GetItemByID(invPtr->m_itemData->m_globalID);

            int glidOld = invPtr->m_itemData->m_globalID;

            BOOL callRepairMan = FALSE;
            if(!glidPtr)
			    callRepairMan = TRUE;
		    else if(glidPtr->m_itemName != invPtr->m_itemData->m_itemName)
			    callRepairMan = TRUE;
            
		    if(callRepairMan)
			{//r

		    CItemConflictDlg cnDlg;
	        cnDlg.m_lastSelected			= lastSelectedPersist;
	        cnDlg.m_currentInventory		= partPtr->m_inventoryCfg;
	        cnDlg.m_currentInvItem			= invPtr;
	        cnDlg.m_currentItemGlid			= invPtr->m_itemData->m_globalID;
	        cnDlg.m_gStatusMem				= m_gStatus;
	        cnDlg.m_currentName				= invPtr->m_itemData->m_itemName;
	        cnDlg.m_reference				= partPtr->m_nameOvveride;
            int result = cnDlg.DoModal();
			lastSelectedPersist = cnDlg.m_lastSelected;
	        if(cnDlg.m_returnCommand == 5)
		       return;
			if(cnDlg.m_returnCommand == 0)
			{//delete current
               invPtr->SafeDelete();
			   delete invPtr;
			   invPtr = 0;
               partPtr->m_inventoryCfg->RemoveAt(rPosLast);
			}//end delete current
            if(cnDlg.m_returnCommand == 10)
			{
               //SetAllItemData(invPtr->m_itemData,glidOld);
			}
			}//end r

		}//end 1
	}
	//
	Sleep(4000);
	AfxMessageBox("Begginning armed item confict checks");
	if(m_gStatus->m_aiList)
    for(POSITION posLoc = m_gStatus->m_aiList->GetHeadPosition();posLoc != NULL;){
        CAIObj * partPtr = (CAIObj *)m_gStatus->m_aiList->GetNext(posLoc);
        
        if(!partPtr->m_inventoryCfg)
			continue;

		POSITION rPosLast;
		for(POSITION rPos = partPtr->m_inventoryCfg->GetHeadPosition();(rPosLast = rPos) != NULL;)
		{//1
            CInventoryObj * invPtr = (CInventoryObj *)partPtr->m_inventoryCfg->GetNext(rPos);
			if(!invPtr->m_armed)
			   continue;
            for(POSITION rPos2 = partPtr->m_inventoryCfg->GetHeadPosition();rPos2 != NULL;)
			{//2
                CInventoryObj * invPtr2 = (CInventoryObj *)partPtr->m_inventoryCfg->GetNext(rPos2);
				if(!invPtr2->m_armed)
					continue;

				if(invPtr == invPtr2)
				    continue;

				if(invPtr2->m_itemData->m_locationID == invPtr->m_itemData->m_locationID)
				{//conflict exists
					CString build = "armed location ID conflict:";
                    build = build+invPtr2->m_itemData->m_itemName;
					build = build+"  ";
                    build = build+invPtr->m_itemData->m_itemName;
					AfxMessageBox(build);
				}//end conflict exists
			}//end 2
		}//end 1
	}
	Sleep(4000);
	AfxMessageBox("Begginning weapon armed checks-will notify if he has no attack");
	int tracerItTemp = 0;
	if(m_gStatus->m_aiList)
    for(POSITION posLoc = m_gStatus->m_aiList->GetHeadPosition();posLoc != NULL;){
        CAIObj * partPtr = (CAIObj *)m_gStatus->m_aiList->GetNext(posLoc);
        tracerItTemp++;
        if(!partPtr->m_inventoryCfg)
			continue;

		BOOL isArmed = FALSE;
		POSITION rPosLast;
		for(POSITION rPos = partPtr->m_inventoryCfg->GetHeadPosition();(rPosLast = rPos) != NULL;)
		{//1
            CInventoryObj * invPtr = (CInventoryObj *)partPtr->m_inventoryCfg->GetNext(rPos);
			if(!invPtr->m_armed)
			   continue;

			if(invPtr->m_itemData->m_locationID == 12)
				isArmed = TRUE;
            
		}//end 1
		if(!isArmed)
			AfxMessageBox(operator+("This ai index has no armed attack item on id 12:",m_gStatus->m_cabLib.ConvToString(tracerItTemp-1)));
	}

	Sleep(4000);
	AfxMessageBox("Completed");
#endif
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void EngineXWin::OnBTNAInewTree() 
{
     CAIWebObj * webObj = new CAIWebObj();
	 webObj->m_web = new CAIWebNodeList();
	 m_gStatus->GL_aiWebDatabase->AddTail(webObj);
	 AITreeInList(m_gStatus->GL_aiWebDatabase->GetCount()-1);
}*/
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void EngineXWin::OnBTNTCPExportAccount() 
{
   
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void EngineXWin::OnBTNTCPImportAccount() 
{
	
}
//-----------------------------------------------------------//
void EngineXWin::OnBTNTCPGenerateClanDatabase() 
{

}
//-----------------------------------------------------------//
void EngineXWin::OnBTNTCPClearInactive() 
{
	
}
//-----------------------------------------------------------//
void EngineXWin::OnBTNUIAIRainSys() 
{
#ifndef _ClientOutput
	D3DVECTOR optionalInitializer;
	optionalInitializer.x = 0.0f;
	optionalInitializer.y = 0.0f;
	optionalInitializer.z = 0.0f;
   POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
   if(entPosition)
   {//valid entity   
	       CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
		   ptrLoc->List_baseFrame->GetPosition(NULL,&optionalInitializer);
   }

    CAIRaidDbClass dlg;
	dlg.m_posX = optionalInitializer.x;
	dlg.m_posY = optionalInitializer.y;
	dlg.m_posZ = optionalInitializer.z;
	dlg.m_aiRaidRefList = m_gStatus->m_aiRaidDatabase;
	dlg.DoModal();
	m_gStatus->m_aiRaidDatabase = dlg.m_aiRaidRefList;
#endif
}

void EngineXWin::OnBTNENTsaveItemDB() 
{
	int indexLoc=0;
    BOOL foundIt = FALSE; 
	POSITION posLoc;
    for(posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
		HWND treeHwnd = NULL;
        m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
        HTREEITEM item = TreeView_GetSelection(treeHwnd);
		CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->List_traceNumber))) == temp){
           foundIt = TRUE;
		   break;
		}
		indexLoc++;
	}
	if(foundIt == FALSE){
       MessageBox("Must select a entity base object");
	   return;
	}
	//get database object
    posLoc = m_gStatus->m_entObjList->FindIndex(indexLoc);
    if(!posLoc)
      return;
      CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjList->GetAt(posLoc);
      if(!ptrLoc->m_itemDB){
		  MessageBox("no items on Entity");
          return;
	  }

	  //use gamefiles folder for organization
      CString CurDir;
      CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "ITD Files|*.ITD|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "ITD", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << ptrLoc->m_itemDB;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  
}

void EngineXWin::OnBTNENTLoadItemDB() 
{
	int indexLoc=0;
    BOOL foundIt = FALSE; 
	POSITION posLoc;
    for(posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
		HWND treeHwnd = NULL;
        m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
        HTREEITEM item = TreeView_GetSelection(treeHwnd);
		CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->List_traceNumber))) == temp){
           foundIt = TRUE;
		   break;
		}
		indexLoc++;
	}
	if(foundIt == FALSE){
       MessageBox("Must select a entity base object");
	   return;
	}
	//get database object
    posLoc = m_gStatus->m_entObjList->FindIndex(indexLoc);
    if(!posLoc)
      return;
      CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjList->GetAt(posLoc);
	  //use gamefiles folder for organization
      CString CurDir;
      CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
      //end use gamefiles folder for organization 	 
	  static char BASED_CODE filter[] = "ITD Files|*.ITD|All Files (*.*)|*.*||";
	  CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
      if ( opendialog.DoModal() != IDOK ){
           SetCurrentDirectory(MainDir); 
	  }
	  CString fileName = opendialog.GetFileName();	 
	  CFileException exc;
      CFile fl;
      BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	  if(res == FALSE){
		SetCurrentDirectory(MainDir); 
		return;
	  }
	  if(ptrLoc->m_itemDB){
         ptrLoc->m_itemDB->SafeDelete();
	     delete ptrLoc->m_itemDB;
	     ptrLoc->m_itemDB=0;
	  }
	  CArchive the_in_Archive(&fl,CArchive::load);
	  the_in_Archive >> ptrLoc->m_itemDB;
	  the_in_Archive.Close();
      fl.Close();
      SetCurrentDirectory(MainDir);
}

//-------------------------------------------//
//-------------------------------------------//
//-------------------------------------------//
/*
void EngineXWin::OnBTNSPNAppendSys() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "SDB Files|*.SDB|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
       if(!m_gStatus->m_spawnGeneratorDB)
	   {
           m_gStatus->m_spawnGeneratorDB = new CSpawnObjList();
	   }
	   if(!m_gStatus->GL_aiWebDatabase)
	   { 
           m_gStatus->GL_aiWebDatabase = new CAIWebObjList();
	   }

	   CAIWebObjList * aiWebHolder = NULL;
	   CSpawnObjList * spawnGenHolder = NULL;

	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> spawnGenHolder >> aiWebHolder;
	   the_in_Archive.Close();
       fl.Close();  

	   int offset = m_gStatus->GL_aiWebDatabase->GetCount();

       if(aiWebHolder)
       for(POSITION posBuild = aiWebHolder->GetHeadPosition();posBuild != NULL;)
	   {
           CAIWebNode * webNode = (CAIWebNode *)aiWebHolder->GetNext(posBuild);
           m_gStatus->GL_aiWebDatabase->AddTail(webNode);
	   }
	   
	   if(spawnGenHolder)
       for(POSITION posBuild = spawnGenHolder->GetHeadPosition();posBuild != NULL;)
	   {
           CSpawnObj * spawnNode = (CSpawnObj *)spawnGenHolder->GetNext(posBuild);
		   spawnNode->m_treeUse += offset;
           m_gStatus->m_spawnGeneratorDB->AddTail(spawnNode);
	   }
	   
	   SetCurrentDirectory(MainDir);	
	   m_gStatus->SpawnGenDatabaseInList(0);
	   this->m_pAISystemDlg->AITreeInList(-1);
	 }
     SetCurrentDirectory(MainDir);	
}
*/
//-------------------------------------------
void EngineXWin::OnButtonViewsmoothshaded() 
{

	//int runtimeIndex = m_explosionDlgBar.SendDlgItemMessage(IDC_LIST_ExplosionList,LB_GETCURSEL ,0,0);
	int runtimeIndex = this->m_pExplosionsDbDlg->SendDlgItemMessageA(IDC_LIST_ExplosionList, LB_GETCURSEL, 0, 0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must select an explosion");
		return;
	}

	CExplosionObj * expObj = m_gStatus->m_explosionDBList->GetPtr(runtimeIndex);
	if(expObj)
		m_gStatus->DoExpEffect(runtimeIndex,0,m_gStatus->m_explosionRuntimeList,expObj->m_pushPower,expObj->m_damageRadius);
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void EngineXWin::OnBTNTestAssign()
{
//m_gStatus->m_serverItemGenerator->OutPutTxt();
//SqlItemUpdateToServer();
int * test = new int[5];
    ZeroMemory(test,sizeof(int)*5);

    
    if(test[7] > 0)
	{

	}

    //test[7] = 1;
	 
	//delete [] test;
	//test = 0;
    AfxMessageBox("Test1");
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void EngineXWin::SqlAIToServer()
{	   
     CSqlConnectionDlg dlg;
     if(dlg.DoModal() == IDOK)
	 {
        CDBConnectionSystem * dbConnection = new CDBConnectionSystem(dlg.m_ipAddress,
			                                                         dlg.m_database,
																     dlg.m_username,
																     dlg.m_password,
																     NULL);


		if(!dbConnection->m_connectionEstablished)
		{
			delete dbConnection;
		    dbConnection = 0;
			AfxMessageBox("Connection could not be established to the database-check your connection info");
			return;
		}
		//remove/add table
		dbConnection->ExecQuery("drop table AI");
		dbConnection->ExecQuery("drop table AIInventory");


		dbConnection->ExecQuery("Create table AI(SpawnID INTEGER PRIMARY KEY,Name NVARCHAR(150), Aggressive int);");
		dbConnection->ExecQuery("Create table AIInventory(SpawnID INTEGER PRIMARY KEY,ItemID int,Item_Name NVARCHAR(150));");
		//ItemID INTEGER PRIMARY KEY IDENTITY
		CString addAIStencil = "INSERT INTO AI(SpawnID,Name,Aggressive) VALUES('$SpawnID','$Name','$Aggressive')";
		CString addInventoryItemStencil = "INSERT INTO AIInventory(SpawnID,ItemID,Item_Name) VALUES('$SpawnID','$ItemID','$Item_Name')";
		CString execQry;
	    //loop through all AI
		int traceID = 0;
		if(m_gStatus->m_aiList)
	    for(POSITION posAI = m_gStatus->m_aiList->GetHeadPosition();posAI != NULL;)
		{
		    CAIObj * aiPtr = (CAIObj *)m_gStatus->m_aiList->GetNext(posAI);
			execQry = addAIStencil;
			execQry.Replace("$SpawnID",m_gStatus->m_cabLib.ConvToString(traceID));
			execQry.Replace("$Name",dbConnection->SanitizeFormatIn(aiPtr->m_aiCfgName));
			execQry.Replace("$Aggressive",m_gStatus->m_cabLib.ConvToString(aiPtr->m_agressive));
            dbConnection->ExecQuery(execQry);
			if(aiPtr->m_inventoryCfg)
			   for(POSITION posInv = aiPtr->m_inventoryCfg->GetHeadPosition();posInv != NULL;)
			   {
                   CInventoryObj * iPtr = (CInventoryObj *) aiPtr->m_inventoryCfg->GetNext(posInv);
                   execQry = addInventoryItemStencil;
				   execQry.Replace("$SpawnID",m_gStatus->m_cabLib.ConvToString(traceID));
				   execQry.Replace("$ItemID",m_gStatus->m_cabLib.ConvToString(iPtr->m_itemData->m_globalID));
				   execQry.Replace("$Item_Name",dbConnection->SanitizeFormatIn(iPtr->m_itemData->m_itemName));
                   dbConnection->ExecQuery(execQry);
			   }
			traceID++;
		}
		   
		delete dbConnection;
		dbConnection = 0;
 
		
		return;
		}
	
return;

}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
DWORD EngineXWin::SqlItemUpdateToServer()
		{
	 CSqlConnectionDlg dlg;
     if(dlg.DoModal() == IDOK)
	 {
        CDBConnectionSystem * dbConnection = new CDBConnectionSystem(dlg.m_ipAddress,
			                                                         dlg.m_database,
																     dlg.m_username,
																     dlg.m_password,
																     NULL);
	   

		if(!dbConnection->m_connectionEstablished)
		{
			delete dbConnection;
		    dbConnection = 0;
			AfxMessageBox("Connection could not be established to the database-check your connection info");
			return 0;
		}
		//remove/add table
		dbConnection->ExecQuery("drop table ItemOutput");
		dbConnection->ExecQuery("drop table ItemAttacks");
		dbConnection->ExecQuery("drop table ItemStatBonuses");
        CString dynamicElementAppend,defAppend,valuesAppend,eleNameSani;
		if(m_gStatus->m_elementDB)
        for(POSITION posEle = m_gStatus->m_elementDB->GetHeadPosition();posEle != NULL;)
		{
            CElementObj * elementPtr = (CElementObj *) m_gStatus->m_elementDB->GetNext(posEle);
            //clean name
			eleNameSani = elementPtr->m_elementName;
			eleNameSani.Replace(" ","_");
			dynamicElementAppend += "," + eleNameSani + " INT";
			defAppend += "," + eleNameSani;
            valuesAppend += ",'$" + eleNameSani + "'";
		}

		dbConnection->ExecQuery("Create table ItemStatBonuses(ItemID INTEGER PRIMARY KEY,ItemName NVARCHAR(150),Stat_Name NVARCHAR(50), Stat_Value float);");
		dbConnection->ExecQuery("Create table ItemAttacks(ItemID INTEGER PRIMARY KEY,ItemName NVARCHAR(150),Projectile int,Damage_Channel NVARCHAR(150),Damage_Amount int);");
		dbConnection->ExecQuery("Create table ItemOutput(ItemID INTEGER PRIMARY KEY,ItemName NVARCHAR(150),STR int,DEX int,WIS int,Credits int"+dynamicElementAppend+");");
		//ItemID INTEGER PRIMARY KEY IDENTITY
		CString addStatBonusStencil = "INSERT INTO ItemStatBonuses(ItemID,ItemName,Stat_Name,Stat_Value) VALUES('$ItemID','$ItemName','$Stat_Name','$Stat_Value')";
		CString addItemStencil = "INSERT INTO ItemOutput(ItemID,ItemName,STR,DEX,WIS,Credits"+defAppend+") VALUES('$ItemID','$ItemName','$STR','$DEX','$WIS','$Credits'"+valuesAppend+")";
		CString addAttackStencil = "INSERT INTO ItemAttacks(ItemID,ItemName,Projectile,Damage_Channel,Damage_Amount) VALUES('$ItemID','$ItemName','$Projectile','$Damage_Channel','$Damage_Amount')";
		CString execQry,attackBuild,execAttackTemp,execStatTemp;
	    //loop through items
		for(unsigned int cLoop=0;cLoop<m_gStatus->m_itemManager->m_cellCount;cLoop++)
	    {//go through all items
		  for(CAxLinkObj * pos = m_gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
		  {//inner loop
		    CItemObj * glPtr = (CItemObj *) m_gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
        
		    execQry = addItemStencil;
			//stats
			execQry.Replace("$ItemName",dbConnection->SanitizeFormatIn(glPtr->m_itemName));
			execQry.Replace("$ItemID",m_gStatus->m_cabLib.ConvToString(glPtr->m_globalID));
            execQry.Replace("$STR",m_gStatus->m_cabLib.ConvToString(glPtr->m_requiredStr));
			execQry.Replace("$DEX",m_gStatus->m_cabLib.ConvToString(glPtr->m_requiredDex));
			execQry.Replace("$WIS",m_gStatus->m_cabLib.ConvToString(glPtr->m_requiredKnowledge));
			execQry.Replace("$Credits",m_gStatus->m_cabLib.ConvToString(glPtr->m_marketCost));
			//verify if attacks exists
			attackBuild = "";
	                CMovementObj * dbPtr = m_gStatus->m_entObjList->GetObjByIndex(glPtr->m_raceID);
					if(dbPtr)
					{//entity edb index aquired 
					  if(glPtr->m_attachmentRefList)
					   for(POSITION posAtt = glPtr->m_attachmentRefList->GetHeadPosition();posAtt != NULL;)
					   {   
                        CAttachmentRefObj * attachmentPtr = (CAttachmentRefObj *)glPtr->m_attachmentRefList->GetNext(posAtt);
                        if(m_gStatus->m_itemManager->m_visuals)
						{
						   CArmedInventoryObj * visObj = m_gStatus->m_itemManager->m_visuals->GetByType(attachmentPtr->m_attachmentType);
                           if(visObj)
				           {//valid attachment reference
							if(visObj->m_thirdPersonSystem)
							if(visObj->m_thirdPersonSystem->m_fireQueList)
							{
                               for(POSITION firePos = visObj->m_thirdPersonSystem->m_fireQueList->GetHeadPosition();firePos != NULL;)
				               {//damage paths
							       CFireRangeObject * fireRange = (CFireRangeObject *) visObj->m_thirdPersonSystem->m_fireQueList->GetNext(firePos);
                                   CMissileObj * misPtr = m_gStatus->m_missileDB->GetPtr(fireRange->m_missileIndex);
								   if(misPtr)
								   {
                                      execAttackTemp = addAttackStencil;
                                      execAttackTemp.Replace("$ItemName",dbConnection->SanitizeFormatIn(glPtr->m_itemName));
									  execAttackTemp.Replace("$ItemID",m_gStatus->m_cabLib.ConvToString(glPtr->m_globalID));
                                      execAttackTemp.Replace("$Projectile","1");
									  execAttackTemp.Replace("$Damage_Amount",m_gStatus->m_cabLib.ConvToString(misPtr->m_minDamageValue));
									  execAttackTemp.Replace("$MaxDamageAmount",m_gStatus->m_cabLib.ConvToString(misPtr->m_maxDamageValue));
									  execAttackTemp.Replace("$DotRef",m_gStatus->m_cabLib.ConvToString(misPtr->m_dotRefValue));
                                      CElementObj * elementPtr = m_gStatus->m_elementDB->GetByIndex(misPtr->m_damageChannel);
							          if(elementPtr)
									     execAttackTemp.Replace("$Damage_Channel",elementPtr->m_elementName);
							          else
									     execAttackTemp.Replace("$Damage_Channel","Error Unknown");

									  dbConnection->ExecQuery(execAttackTemp);//Commit
								   } 
							   }
							}

							if(visObj->m_firstPersonSystem)
							if(visObj->m_firstPersonSystem->m_fireQueList)
							{
                               for(POSITION firePos = visObj->m_firstPersonSystem->m_fireQueList->GetHeadPosition();firePos != NULL;)
				               {//damage paths
							       CFireRangeObject * fireRange = (CFireRangeObject *) visObj->m_firstPersonSystem->m_fireQueList->GetNext(firePos);
                                   CMissileObj * misPtr = m_gStatus->m_missileDB->GetPtr(fireRange->m_missileIndex);
								   if(misPtr)
								   {
                                      execAttackTemp = addAttackStencil;
                                      execAttackTemp.Replace("$ItemName",dbConnection->SanitizeFormatIn(glPtr->m_itemName));
									  execAttackTemp.Replace("$ItemID",m_gStatus->m_cabLib.ConvToString(glPtr->m_globalID));
                                      execAttackTemp.Replace("$Projectile","1");
									  execAttackTemp.Replace("$Damage_Amount",m_gStatus->m_cabLib.ConvToString(misPtr->m_minDamageValue));
									  execAttackTemp.Replace("$MaxDamageAmount",m_gStatus->m_cabLib.ConvToString(misPtr->m_maxDamageValue));
									  execAttackTemp.Replace("$DotRef",m_gStatus->m_cabLib.ConvToString(misPtr->m_dotRefValue));
                                      CElementObj * elementPtr = m_gStatus->m_elementDB->GetByIndex(misPtr->m_damageChannel);
							          if(elementPtr)
									     execAttackTemp.Replace("$Damage_Channel",elementPtr->m_elementName);
							          else
									     execAttackTemp.Replace("$Damage_Channel","Error Unknown");

									  dbConnection->ExecQuery(execAttackTemp);//Commit

								   } 
							   }
							}
							   
							   
							if(visObj->m_thirdPersonSystem)
							if(visObj->m_thirdPersonSystem->m_damageSphereList)
							{
			                  for(POSITION dPos = visObj->m_thirdPersonSystem->m_damageSphereList->GetHeadPosition();dPos != NULL;)
				              {//damage paths
                               CDamagePathObj* damagePathObj = (CDamagePathObj*)visObj->m_thirdPersonSystem->m_damageSphereList->GetNext(dPos);
							      execAttackTemp = addAttackStencil;
								  execAttackTemp.Replace("$ItemName",dbConnection->SanitizeFormatIn(glPtr->m_itemName));
								  execAttackTemp.Replace("$ItemID",m_gStatus->m_cabLib.ConvToString(glPtr->m_globalID));
								  execAttackTemp.Replace("$Projectile","0");
								  execAttackTemp.Replace("$Damage_Amount",m_gStatus->m_cabLib.ConvToString(damagePathObj->m_damageMinValue));
								  execAttackTemp.Replace("$Damage_MaxAmount",m_gStatus->m_cabLib.ConvToString(damagePathObj->m_damageMaxValue));
								  execAttackTemp.Replace("$Dot_Ref",m_gStatus->m_cabLib.ConvToString(damagePathObj->m_damageDotValue));
							   CElementObj * elementPtr = m_gStatus->m_elementDB->GetByIndex(damagePathObj->m_damageChannel);
							   if(elementPtr)
									 execAttackTemp.Replace("$Damage_Channel",elementPtr->m_elementName);
							   else
									 execAttackTemp.Replace("$Damage_Channel","Error Unknown");

								  dbConnection->ExecQuery(execAttackTemp);//Commit
							  }//end damage paths
				            }//end valid attachment reference
                            if(visObj->m_firstPersonSystem)
							if(visObj->m_firstPersonSystem->m_damageSphereList)
							{
                              for(POSITION dPos = visObj->m_firstPersonSystem->m_damageSphereList->GetHeadPosition();dPos != NULL;)
				              {//damage paths
                               CDamagePathObj* damagePathObj = (CDamagePathObj*)visObj->m_firstPersonSystem->m_damageSphereList->GetNext(dPos);
							      execAttackTemp = addAttackStencil;
                                  execAttackTemp.Replace("$ItemName",dbConnection->SanitizeFormatIn(glPtr->m_itemName));
								  execAttackTemp.Replace("$ItemID",m_gStatus->m_cabLib.ConvToString(glPtr->m_globalID));
                                  execAttackTemp.Replace("$Projectile","0");
								  execAttackTemp.Replace("$Damage_Amount",m_gStatus->m_cabLib.ConvToString(damagePathObj->m_damageMinValue));
								  execAttackTemp.Replace("$Damage_MaxAmount",m_gStatus->m_cabLib.ConvToString(damagePathObj->m_damageMaxValue));
								  execAttackTemp.Replace("$Dot_Ref",m_gStatus->m_cabLib.ConvToString(damagePathObj->m_damageDotValue));
							   CElementObj * elementPtr = m_gStatus->m_elementDB->GetByIndex(damagePathObj->m_damageChannel);
							   if(elementPtr)
								     execAttackTemp.Replace("$Damage_Channel",elementPtr->m_elementName);
							   else
								     execAttackTemp.Replace("$Damage_Channel","Error Unknown");

								  dbConnection->ExecQuery(execAttackTemp);//Commit
				              }//end damage paths
							}
						   }
						}
					   }
			}//entity edb index aquired
			//end aquire attacks

			//defenses
            if(glPtr->m_elementDefenses)
			for(POSITION posDef = glPtr->m_elementDefenses->GetHeadPosition();posDef != NULL;)
			{
                CElementStatusObj * statusPtr = (CElementStatusObj *)glPtr->m_elementDefenses->GetNext(posDef);
                CElementObj * elementPtr = m_gStatus->m_elementDB->GetByIndex(statusPtr->m_elementIndex);
				if(elementPtr)
				{
				   eleNameSani = elementPtr->m_elementName;
				   eleNameSani.Replace(" ","_");
				   execQry.Replace("$"+eleNameSani,m_gStatus->m_cabLib.ConvToString(statusPtr->m_currentValue));
			 }
		}
            //zero unset defenses
			if(m_gStatus->m_elementDB)
			for(POSITION posEle = m_gStatus->m_elementDB->GetHeadPosition();posEle != NULL;)
		{
				CElementObj * elementPtr = (CElementObj *) m_gStatus->m_elementDB->GetNext(posEle);
				eleNameSani = elementPtr->m_elementName;
				eleNameSani.Replace(" ","_");
				execQry.Replace("$"+eleNameSani,"0");
		}
		
			//stat bonuses
			if(m_gStatus->m_statDatabase)
			if(glPtr->m_statBonuses)
			    for(POSITION posStat = glPtr->m_statBonuses->GetHeadPosition();posStat != NULL;)
				{
					CStatBonusObj * statBonus = (CStatBonusObj *) glPtr->m_statBonuses->GetNext(posStat);
		 
					CStatObj * statPtr = m_gStatus->m_statDatabase->GetStatByID(statBonus->m_statID);
					if(statPtr)
					{
						execStatTemp = addStatBonusStencil;
						execStatTemp.Replace("$ItemName",dbConnection->SanitizeFormatIn(glPtr->m_itemName));
						execStatTemp.Replace("$ItemID",m_gStatus->m_cabLib.ConvToString(glPtr->m_globalID));
			            execStatTemp.Replace("$Stat_Name",statPtr->m_statName);
						execStatTemp.Replace("$Stat_Value",m_gStatus->m_cabLib.ConvToString(statBonus->m_statBonus));
						dbConnection->ExecQuery(execStatTemp);//Commit
					}
				}


			dbConnection->ExecQuery(execQry);//Commit
		   }//inner loop
		}//end go through all items
	
		delete dbConnection;
		dbConnection = 0;
 
		
return 1;
}
	
return 1;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void EngineXWin::OnBTNCommCntrlLibBtnA()
{

   #ifndef _ClientOutput
	CCommCrtlEditorDlg dlg;
	dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
	dlg.g_directSoundRef = m_gStatus->m_dirSnd;
	dlg.m_controlDatabase = m_gStatus->m_commCtrlDB;
	dlg.DoModal();
	m_gStatus->m_commCtrlDB = dlg.m_controlDatabase;
#endif
  
}
//---------------------------------------------------------------------//
void EngineXWin::CheckAccountDBSyncronization()
{
 
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void EngineXWin::OnBTNWorldRule() 
{
#ifndef _ClientOutput
	CWorldChannelDlg dlg;
	dlg.channelDatabaseRef = m_gStatus->m_worldChannelRuleDB;
    dlg.DoModal();
	m_gStatus->m_worldChannelRuleDB = dlg.channelDatabaseRef;
#endif
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void EngineXWin::OnBTNArenaManager() 
{
#ifndef _ClientOutput
     CArenaManagerDlg dlg;
	 dlg.m_battleSysListRef = m_gStatus->m_battleSchedulerSys;
	 dlg.DoModal();
	 
	 
	 m_gStatus->m_battleSchedulerSys = dlg.m_battleSysListRef;
	 m_gStatus->m_battleSchedulerSys->Initialize();
#endif
}

//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
void EngineXWin::OnBtnViewwireframe() 
{
     m_gStatus->m_debuggingOn = TRUE;
     CFileException e;
     if(!m_gStatus->m_debugfl.Open( "ErrorLive.txt", CFile::modeCreate | CFile::modeWrite, &e ))
		 m_gStatus->m_debuggingOn = FALSE;
} 
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
void EngineXWin::ComputeLight(D3DXVECTOR3 &lightDir, 
    D3DXMATRIXA16 *pObject, D3DXMATRIXA16 *pView, D3DXMATRIXA16 *pProj)
{

    D3DXMATRIXA16  m_matObject;
    D3DXMATRIXA16  m_matView;
    D3DXMATRIXA16  m_matProj;
	 
    D3DXMATRIXA16  m_matTot;
    D3DXVECTOR3 U,V,W,Pos;
    D3DXVECTOR4 light,one(1,1,1,1),half(0.5f,0.5f,.5f,.5f);//.5f);
   

    light.x = lightDir.x;
    light.y = lightDir.y;
    light.z = lightDir.z;
    light.w = 0;

    D3DXVec4Normalize(&light,&light);
    
    m_matObject = *pObject;
    m_matView   = *pView;
    m_matProj   = *pProj;

    D3DXMATRIXA16  temp;

    D3DXMatrixMultiply(&m_matTot, pObject, pView);

    D3DXMatrixTranspose(&temp, &m_matTot);

    //the inverse transform
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetVertexShaderConstantF(   16,(float*)pObject, 4);
    
    D3DXMatrixMultiply(&m_matTot,&m_matTot,pProj);

    D3DXMatrixTranspose(&m_matTot,&m_matTot);
    D3DXMatrixTranspose(&m_matObject,pObject);

    //load the values into the registers
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetVertexShaderConstantF(  12, (float*)&light,  1 );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetVertexShaderConstantF(  32, (float*)&one,    1);
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetVertexShaderConstantF(  33, (float*)&half,  1);
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetVertexShaderConstantF(   0, (float*)&m_matObject, 4);
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetVertexShaderConstantF(   8, (float*)&m_matTot, 4);
}
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
void EngineXWin::OnBTNUImouseSW() 
{
#ifndef _ClientOutput 
    //CShaderLibDlg dlg;
	CShaderLibDlg * dlg = new CShaderLibDlg();
	dlg->m_gstatusMem = m_gStatus;
	dlg->m_shaderLibRef = m_gStatus->m_shaderSystem;
	dlg->m_pd3dDevice = m_gStatus->m_dxRenderer->m_pd3dDevice;
	dlg->m_shaderTypeMode = 0;
	//dlg.DoModal();
	
     dlg->Create(IDD_DIALOG_ShaderLibEditor,NULL);
     dlg->ShowWindow(SW_SHOW);

	/*m_gStatus->m_shaderSystem = dlg->m_shaderLibRef;
    m_gStatus->m_shaderSystem->Compile(m_gStatus->m_dxRenderer->m_pd3dDevice);
	m_gStatus->m_renderStateObject->m_shderSystemRef = m_gStatus->m_shaderSystem;*/
	//m_gStatus->m_shaderSystem->SetShaderByIndex(m_gStatus->m_dxRenderer->m_pd3dDevice,0);
#endif
}

//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
void EngineXWin::MigradeEDBToGLAppendages()
{
   int offsetID = 0;
   for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;)
   {
	   CMovementObj * dbPtr = (CMovementObj *) m_gStatus->m_entObjList->GetNext(posLoc);
	   if(dbPtr->List_armedInventoryDB)
       if(dbPtr->m_itemDB)
	   {
	      for(POSITION itemPos = dbPtr->m_itemDB->GetHeadPosition();itemPos != NULL;)
	      {//item list
             CItemObj * itemPtr = (CItemObj *)dbPtr->m_itemDB->GetNext(itemPos);
			 if(itemPtr->m_attachmentRefList)
			    for(POSITION posAtt = itemPtr->m_attachmentRefList->GetHeadPosition();posAtt != NULL;)
			    {//attachment list
                     CAttachmentRefObj * attachmentPtr = (CAttachmentRefObj *)itemPtr->m_attachmentRefList->GetNext(posAtt);
					 
					 CArmedInventoryObj * armedObj = dbPtr->List_armedInventoryDB->GetByType(attachmentPtr->m_attachmentType);
					 if(armedObj)
					 {//valid
						dbPtr->List_armedInventoryDB->RemoveByType(attachmentPtr->m_attachmentType,FALSE);
						armedObj->m_type = offsetID;
                        m_gStatus->m_itemManager->m_visuals->AddTail(armedObj);
						attachmentPtr->m_attachmentType = offsetID;
						offsetID++;
					 }//end valid
				}//end attachment list
		  }//end item list
		  dbPtr->List_armedInventoryDB->SafeDelete();
		  delete dbPtr->List_armedInventoryDB;
		  dbPtr->List_armedInventoryDB = 0;
	   }

   }
   AfxMessageBox("Migration Completed");
}
//-----------------------------------------------------------------//
void EngineXWin::GetShadowPolyMesh( CCollisionObj *collisionObject, 
									  ABVERTEX1LNT * buffer, 
									  int & returnVCount,
									  D3DXVECTOR3 objectPos,
									  float radius,
									  float extentUp,
									  float extentDown)
{
	
    returnVCount = 0;
	
	
    //make collision vects
	//Creates a square pyramid
	float zmax = objectPos.z+(radius*2.5f);
	float zmin = objectPos.z-(radius*2.5f);
	float xmin = objectPos.x-(radius*2.5f);
	float xmax = objectPos.x+(radius*2.5f);
	float ymin = objectPos.y-extentDown;
	float ymax = objectPos.y+extentUp;
		
	D3DXVECTOR3 collisionVects[12];
	int collisionVectCount = 12;
	int cCollPolys = collisionVectCount/3; //number of polys in CollisionVert structure

	CPolyRetStruct *PolyList = NULL;
	PolyList = new CPolyRetStruct[cCollPolys*20]; //maximum number of polygons that can be returned
	int cPolyList=0;

	D3DXVECTOR3 *Normals = NULL;
	Normals = new D3DXVECTOR3[cCollPolys*20]; //maximum number of normals that can be returned

 	collisionVects[0] = D3DXVECTOR3(objectPos.x, ymax, objectPos.z);
	collisionVects[1] = D3DXVECTOR3(xmin , ymin, zmax);
	collisionVects[2] = D3DXVECTOR3(xmax , ymin, zmin);

	collisionVects[3] = D3DXVECTOR3(objectPos.x, ymax, objectPos.z);
	collisionVects[4] = D3DXVECTOR3(xmin , ymin, zmin);
	collisionVects[5] = D3DXVECTOR3(xmax , ymin, zmax);

	collisionVects[6] = D3DXVECTOR3(objectPos.x, ymax, objectPos.z);
	collisionVects[7] = D3DXVECTOR3(xmin , ymin, objectPos.z);
	collisionVects[8] = D3DXVECTOR3(xmax , ymin, objectPos.z);

	collisionVects[9] = D3DXVECTOR3(objectPos.x, ymax, objectPos.z);
	collisionVects[10] = D3DXVECTOR3(objectPos.x, ymin, zmax);
	collisionVects[11] = D3DXVECTOR3(objectPos.x, ymin,  zmin);



	//get all polygons that collide with the collision shape
	for(int CPolyIndex=0; CPolyIndex < cCollPolys; CPolyIndex++)
	{
		int cNormals=0;
		CPolyRetStruct CollisionPolys[20];
		D3DXVECTOR3      CollisionNormals[20];

		//dummy variables for CollisionPolyRetrieval()
		int dummyCollisions=0, dummyConsidered=0;
		D3DXVECTOR3 dummyLatestNormal, dummyVirtualNormal, dummyOrigin;
		float dummyX=0, dummyY=0, dummyZ=0, dummyYMax=0;
		float dummyDistances[20];
		
		collisionObject->CollisionPolyRetrieval
						(
							&collisionVects[CPolyIndex*3],
							&dummyX,
							&dummyY,
							&dummyZ,
							&dummyCollisions,
							&dummyConsidered,
							CollisionNormals,
							dummyDistances,
        					cNormals,
							&dummyYMax,
							TRUE, //RecordAllInfo,
							&dummyLatestNormal,
							dummyVirtualNormal,
							dummyOrigin,
							FALSE, //UseAngle,
							CollisionPolys
						); //cNormals returns the number of polys if RecordAllInfo = TRUE

		//add all polygons that are not duplicates
		for(int addpoly=0; addpoly < cNormals; addpoly++) 
		{

			if(CollisionNormals[addpoly].y < 0.5f && CollisionNormals[addpoly].y > -0.5f)
				continue; //ignore polys that are mostly vertical

			if(!DuplicateTri(PolyList, cPolyList, CollisionPolys[addpoly]))
			{
				PolyList[cPolyList] = CollisionPolys[addpoly];
				Normals[cPolyList]  = CollisionNormals[addpoly];
				cPolyList++;
				//returnVCount = returnVCount + 3;
				
			}

		}
		
	}


	if(cPolyList <= 0)
	{
	   if(PolyList)  delete [] PolyList;
	   if(Normals) delete [] Normals;
	   return;
	}
	////////////////////////
	////////////////////////
	
	
	returnVCount = 3*cPolyList;
	//m_vertices = new ABVERTEX1L[3*cPolyList];
	//m_normals  = new D3DXVECTOR3[cPolyList];

	//copy polys and normals to class variables
	//ABVERTEX1L *vtx = NULL;
	for(int poly=0; poly < cPolyList; poly++) {
		
		//if(Normals[poly].y > -.01f && Normals[poly].y < .01f) continue; //ignore vertical polys
		//m_vertices[(poly*3)+0] = ToVertex(PolyList[poly].polygon[0]);
		//m_vertices[(poly*3)+1] = ToVertex(PolyList[poly].polygon[1]);
		//m_vertices[(poly*3)+2] = ToVertex(PolyList[poly].polygon[2]);
		buffer[(poly*3)+0].x = PolyList[poly].polygon[0].x;
		buffer[(poly*3)+0].y = PolyList[poly].polygon[0].y + .02f;
		buffer[(poly*3)+0].z = PolyList[poly].polygon[0].z;
		buffer[(poly*3)+1].x = PolyList[poly].polygon[1].x;
		buffer[(poly*3)+1].y = PolyList[poly].polygon[1].y + .02f;
		buffer[(poly*3)+1].z = PolyList[poly].polygon[1].z;
		buffer[(poly*3)+2].x = PolyList[poly].polygon[2].x;
		buffer[(poly*3)+2].y = PolyList[poly].polygon[2].y + .02f;
		buffer[(poly*3)+2].z = PolyList[poly].polygon[2].z;
 
		/*buffer[(poly*3)+0].nx = 0.0f;
	    buffer[(poly*3)+0].ny = 1.0f;
		buffer[(poly*3)+0].nz = 0.0f;
		buffer[(poly*3)+1].nx = 0.0f;
		buffer[(poly*3)+1].ny = 1.0f;
		buffer[(poly*3)+1].nz = 0.0f;
		buffer[(poly*3)+2].nx = 0.0f;
		buffer[(poly*3)+2].ny = 1.0f;
		buffer[(poly*3)+2].nz = 0.0f;*/

		float d = radius*2;

		if(objectPos.x > buffer[(poly*3)+0].x)
		   buffer[(poly*3)+0].tx0.tu = (-objectPos.x+radius+buffer[(poly*3)+0].x)/d;
	    else
		   buffer[(poly*3)+0].tx0.tu = (-objectPos.x-radius+buffer[(poly*3)+0].x)/d+1.0f;

	    if(objectPos.z > buffer[(poly*3)+0].z)
		   buffer[(poly*3)+0].tx0.tv = (-objectPos.z+radius+buffer[(poly*3)+0].z)/d;
	    else
		   buffer[(poly*3)+0].tx0.tv = (-objectPos.z-radius+buffer[(poly*3)+0].z)/d+1.0f;
		//-------------
		if(objectPos.x > buffer[(poly*3)+1].x)
		   buffer[(poly*3)+1].tx0.tu = (-objectPos.x+radius+buffer[(poly*3)+1].x)/d;
	    else
		   buffer[(poly*3)+1].tx0.tu = (-objectPos.x-radius+buffer[(poly*3)+1].x)/d+1.0f;

	    if(objectPos.z > buffer[(poly*3)+1].z)
		   buffer[(poly*3)+1].tx0.tv = (-objectPos.z+radius+buffer[(poly*3)+1].z)/d;
	    else
		   buffer[(poly*3)+1].tx0.tv = (-objectPos.z-radius+buffer[(poly*3)+1].z)/d+1.0f;
		//------------
		if(objectPos.x > buffer[(poly*3)+2].x)
		   buffer[(poly*3)+2].tx0.tu = (-objectPos.x+radius+buffer[(poly*3)+2].x)/d;
	    else
		   buffer[(poly*3)+2].tx0.tu = (-objectPos.x-radius+buffer[(poly*3)+2].x)/d+1.0f;

	    if(objectPos.z > buffer[(poly*3)+2].z)
		   buffer[(poly*3)+2].tx0.tv = (-objectPos.z+radius+buffer[(poly*3)+2].z)/d;
	    else
		   buffer[(poly*3)+2].tx0.tv = (-objectPos.z-radius+buffer[(poly*3)+2].z)/d+1.0f;

		/*buffer[(poly*3)+0].tx0.tu = 0.0f;
	    buffer[(poly*3)+0].tx0.tv = 0.0f;
		buffer[(poly*3)+1].tx0.tu = 0.0f;
	    buffer[(poly*3)+1].tx0.tv = 0.0f;
		buffer[(poly*3)+2].tx0.tu = 0.0f;
	    buffer[(poly*3)+2].tx0.tv = 0.0f;*/

		//m_normals[poly] = Normals[poly];

        /*for(int line=0; line < 3; line++) {
			
			//vtx = &buffer[(poly*3)+line];

			
			//y offset to help correct z-errors
			//buffer[(poly*3)+line].y += .2f;

			float d = radius*2;
			//calculate texture coordinates

			if(objectPos.x > buffer[(poly*3)+line].x)
				buffer[(poly*3)+line].tx0.tu = (-objectPos.x+radius+buffer[(poly*3)+line].x)/d;
			else
				buffer[(poly*3)+line].tx0.tu = (-objectPos.x-radius+buffer[(poly*3)+line].x)/d+1.0f;

			if(objectPos.z > buffer[(poly*3)+line].z)
				buffer[(poly*3)+line].tx0.tv = (-objectPos.z+radius+buffer[(poly*3)+line].z)/d;
			else
				buffer[(poly*3)+line].tx0.tv = (-objectPos.z-radius+buffer[(poly*3)+line].z)/d+1.0f;
		}*/			

	}

	//de-allocate memory
	if(PolyList)  delete [] PolyList;
	if(Normals) delete [] Normals;
	
	//send out number of polygons
	//m_numvertices = cPolyList*3;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//

//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
BOOL EngineXWin::DuplicateTri(CPolyRetStruct *Polys, int cPolys, CPolyRetStruct TestPoly)
{
	BOOL ret=FALSE;

	for(int p=0; p < cPolys; p++) 
	{
		if(	Polys[p].polygon[0] == TestPoly.polygon[0] &&
			Polys[p].polygon[1] == TestPoly.polygon[1] &&
			Polys[p].polygon[2] == TestPoly.polygon[2] )
		{
			ret = TRUE;
			break;
		}
		
	}

	return ret;
}
//-----------------------------------------------------------------//
void EngineXWin::LoadRNEFile()
{
   //LB_GETSELITEMS 
	 int groupIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box,LB_GETCURSEL ,0,0);
     if(groupIndex == -1){
	    groupIndex=0;
	 }
     else{
        POSITION wldGroupPos = m_gStatus->m_worldGroupList->FindIndex(groupIndex);
        groupIndex=0;
        if(wldGroupPos){
           CWldGroupObj * wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetAt(wldGroupPos);
           groupIndex = wldPtr->List_groupInt;
		}
	 }


   CReferenceImpExpList * rList = new CReferenceImpExpList();
   //use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
   //end use mapsmodels folder for organization
   static char BASED_CODE filter[] = "RNE Files|*.RNE|RNE Files|*.RNE|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   rList->InitializeFromFile(opendialog.GetPathName());
	 }
    SetCurrentDirectory(MainDir);	
	//populate lists
	for(POSITION posLoc = rList->GetHeadPosition();posLoc != NULL;)
	{
        CReferenceImpExpObj * rImp = (CReferenceImpExpObj *) rList->GetNext(posLoc);
		if(rImp->m_type == 0)
		{//reference 
		  int index = m_gStatus->m_libraryReferenceDB->GetReferenceIndexByName(rImp->m_idName);
		  if(index > -1)
		    AddNodeByData(groupIndex,index,NULL,&rImp->m_matrix);
		  else
		  {
			AfxMessageBox("Cannot find reference::"+rImp->m_idName);
			break;
		  }
		}
		else if(rImp->m_type == 1)
		{//light
          CWldObject * ptrLoc = new CWldObject((m_gStatus->GlobalTraceNumber++));  
		  ptrLoc->m_lightObject = new CRTLightObj();
		  ptrLoc->m_lightObject->m_material = new CMaterialObject();
		  ptrLoc->List_groupNumber = groupIndex;
		  //load imediate mode mesh data
		     ptrLoc->m_fileNameRef = rImp->m_idName;
             float sightRange = rImp->m_sightRange;
			 ptrLoc->List_popoutRadius = rImp->m_endAttenuation;


			 ptrLoc->m_lightObject->m_wldspacePos.x = rImp->m_matrix._41;
			 ptrLoc->m_lightObject->m_wldspacePos.y = rImp->m_matrix._42;
			 ptrLoc->m_lightObject->m_wldspacePos.z = rImp->m_matrix._43;

			 ptrLoc->m_lightObject->m_attenuation0 = rImp->m_atten0;
			 ptrLoc->m_lightObject->m_attenuation1 = rImp->m_atten1;
			 ptrLoc->m_lightObject->m_attenuation2 = rImp->m_atten2;

			 ptrLoc->m_lightObject->m_range = rImp->m_endAttenuation*1.5;

			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Diffuse.r = rImp->m_redValue;
			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Diffuse.g = rImp->m_greenValue;
			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Diffuse.b = rImp->m_blueValue;
			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Diffuse.a = 1.0f;

			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Ambient.r = 0.0f;
			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Ambient.g = 0.0f;
			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Ambient.b = 0.0f;
			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Ambient.a = 1.0f;

			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Specular.r = 0.0f;
			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Specular.g = 0.0f;
			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Specular.b = 0.0f;
			 ptrLoc->m_lightObject->m_material->m_baseMaterial.Specular.a = 1.0f;


             //DPD 
			 //Light Type
			 ptrLoc->m_lightObject->m_type = rImp->m_lightType;
			 //Direction
			 memcpy(&ptrLoc->m_lightObject->m_direction, &rImp->m_direction, sizeof(D3DVECTOR));
			 //Theta, Phi, & Falloff
			 ptrLoc->m_lightObject->m_theta   = rImp->m_theta;
			 ptrLoc->m_lightObject->m_phi     = rImp->m_phi;
			 ptrLoc->m_lightObject->m_falloff = rImp->m_falloff;

			 ptrLoc->m_lightObject->m_material->CopyMaterial(ptrLoc->m_lightObject->m_material->m_baseMaterial,&ptrLoc->m_lightObject->m_material->m_useMaterial);

             if(rImp->m_optionalBox == FALSE)
			 {
				 ptrLoc->List_popoutBox.max.x =  ((sightRange+1)+ptrLoc->m_lightObject->m_wldspacePos.x);
				 ptrLoc->List_popoutBox.max.y =  ((sightRange+2)+ptrLoc->m_lightObject->m_wldspacePos.y);
				 ptrLoc->List_popoutBox.max.z =  ((sightRange+3)+ptrLoc->m_lightObject->m_wldspacePos.z);
				 ptrLoc->List_popoutBox.min.x = -(sightRange+4)+ptrLoc->m_lightObject->m_wldspacePos.x;
				 ptrLoc->List_popoutBox.min.y = -(sightRange+5)+ptrLoc->m_lightObject->m_wldspacePos.y;
				 ptrLoc->List_popoutBox.min.z = -(sightRange+6)+ptrLoc->m_lightObject->m_wldspacePos.z;
			 }
			 else
			 {
                 ptrLoc->List_popoutBox.max.x =  rImp->m_maxX;
				 ptrLoc->List_popoutBox.max.y =  rImp->m_maxY;
				 ptrLoc->List_popoutBox.max.z =  rImp->m_maxZ;
				 ptrLoc->List_popoutBox.min.x =  rImp->m_minX;
				 ptrLoc->List_popoutBox.min.y =  rImp->m_minY;
				 ptrLoc->List_popoutBox.min.z =  rImp->m_minZ;
			 }

			 m_gStatus->m_wldObjList->AddTail(ptrLoc);
         
			 HTREEITEM Root = AddTreeRoot(operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity))),IDC_OBJECT_TREE,&m_dlgBarObject);   
	         AddTreeChild(ptrLoc->m_fileNameRef,IDC_OBJECT_TREE,&m_dlgBarObject,Root);  
		}//end light
	}

	rList->SafeDelete();
	delete rList;
	rList = 0;
}

//------------------------------------------------------------------------------------//
/*
void EngineXWin::OnBtnUpdateAllTextures()
{
     m_gStatus->m_dynamicTextureDataBase->DirectoryUpdateAllTextures();
	 AfxMessageBox("Completed");
}
*/
//------------------------------------------------------------------------------------//
void EngineXWin::RenderCGMenuModCallback(long currentTime)
{
/*	if(m_gStatus->m_renderMenuClickZoneInfo)
    for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
	{//loop through menus
        CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
		if(menu->m_cursor)
		{
		 RenderClickZone(menu,-1);
         //editor help
		 if(m_userControls->m_input->m_keyboardKeys[DIK_F4].IsDown())
	     {//set position of visual
          int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	      if(menuIndex == -1)
	       continue;
          int visIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_UnitList,LB_GETCURSEL ,0,0);
	      if(visIndex == -1)
		   continue;
          POSITION posM = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	      if(!posM)
		   continue;
          CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posM);
	      if(menu->m_menuAccessID == menuObj->m_menuAccessID)
		  {
		  
		  posM = menuObj->m_visualItems->FindIndex(visIndex);
	      if(!posM)
		   continue;
          CMenuVisObj * visObj = (CMenuVisObj *)menuObj->m_visualItems->GetAt(posM);

          
             posM = menu->m_visualItems->FindIndex(visIndex);
	         if(!posM)
		         continue;
             CMenuVisObj * visObjLive = (CMenuVisObj *)menu->m_visualItems->GetAt(posM);
             visObjLive->m_startLocation = menu->m_cursor->m_position;
		     visObjLive->m_endLocation = menu->m_cursor->m_position;

		  

          visObj->m_startLocation.x = menu->m_cursor->m_position.x - menu->m_offsetMenu.x;
		  visObj->m_startLocation.y = menu->m_cursor->m_position.y - menu->m_offsetMenu.y;
		  //visObj->m_startLocation.z = menu->m_cursor->m_position.z - menu->m_offsetMenu.z;

		  CopyMemory(&visObj->m_endLocation,&visObj->m_startLocation,sizeof(D3DVECTOR));
		  if(visObj->m_menuBarObj)
		  { 
		     visObj->m_menuBarObj->m_xRoot = visObj->m_startLocation.x;
			 visObj->m_menuBarObj->m_yRoot = visObj->m_startLocation.y;
			 
			 visObjLive->m_menuBarObj->m_xRoot = menu->m_cursor->m_position.x;
			 visObjLive->m_menuBarObj->m_yRoot = menu->m_cursor->m_position.y;
		  }
		  m_gStatus->m_systemReadout->AddLine("NewPositionSet");
		  }
		 }//end set position of visual
		 if(m_userControls->m_input->m_keyboardKeys[DIK_F6].IsDown())
	     {//Set Max Click Zone Point
            int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	        if(menuIndex == -1)continue;
            int clIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_GETCURSEL ,0,0);
	        if(clIndex == -1)continue;
	        POSITION posM = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	        if(!posM)continue;
            CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posM);
		    if(menu->m_menuAccessID != menuObj->m_menuAccessID)continue;
			posM = menuObj->m_clickActions->FindIndex(clIndex);
		    if(!posM)continue;
	        CCLickZoneObj * cPtr = ( CCLickZoneObj* )menuObj->m_clickActions->GetAt(posM);

			posM = menu->m_clickActions->FindIndex(clIndex);
	        if(!posM)continue;
            CCLickZoneObj * cPtrLive = ( CCLickZoneObj* )menu->m_clickActions->GetAt(posM);
             
			cPtrLive->m_rectMaxX = menu->m_cursor->m_position.x;
		    cPtrLive->m_rectMaxY = menu->m_cursor->m_position.y;
			
            cPtr->m_rectMaxX = menu->m_cursor->m_position.x - menu->m_offsetMenu.x;
		    cPtr->m_rectMaxY = menu->m_cursor->m_position.y - menu->m_offsetMenu.y;
		    

		 }//end set max click zone point
		 if(m_userControls->m_input->m_keyboardKeys[DIK_F8].IsDown())
	     {//Set Min Click Zone Point
            int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	        if(menuIndex == -1)continue;
            int clIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_GETCURSEL ,0,0);
	        if(clIndex == -1)continue;
	        POSITION posM = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	        if(!posM)continue;
            CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posM);
		    if(menu->m_menuAccessID != menuObj->m_menuAccessID)continue;
			posM = menuObj->m_clickActions->FindIndex(clIndex);
		    if(!posM)continue;
	        CCLickZoneObj * cPtr = ( CCLickZoneObj* )menuObj->m_clickActions->GetAt(posM);

			posM = menu->m_clickActions->FindIndex(clIndex);
	        if(!posM)continue;
            CCLickZoneObj * cPtrLive = ( CCLickZoneObj* )menu->m_clickActions->GetAt(posM);
             
			cPtrLive->m_rectMinX = menu->m_cursor->m_position.x;
		    cPtrLive->m_rectMinY = menu->m_cursor->m_position.y;
			
            cPtr->m_rectMinX = menu->m_cursor->m_position.x - menu->m_offsetMenu.x;
		    cPtr->m_rectMinY = menu->m_cursor->m_position.y - menu->m_offsetMenu.y;
		    

		 }//end set min click zone point
		
		}
	}
*/}
//------------------------------------------------------------------------------------//
void EngineXWin::RenderClickZone(CMenu3DObj* menu,int index)
{
	D3DVECTOR offset;
    offset.x = 0.0f;
	offset.y = 0.0f;
	offset.z = 0.0f;
   
	if(menu->m_cursor)
	{
		D3DVECTOR cursorPosition;
		cursorPosition.x = menu->m_cursor->m_position.x;
		cursorPosition.y = menu->m_cursor->m_position.y;
		cursorPosition.z = menu->m_cursor->m_position.z;
		m_gStatus->m_systemReadout->AddLine("Cursor Position: ", cursorPosition);
	}

	if(menu->m_clickActions)
    for(POSITION clickPos = menu->m_clickActions->GetHeadPosition();clickPos != NULL;)
	{
        CCLickZoneObj * clickZone = (CCLickZoneObj *)menu->m_clickActions->GetNext(clickPos);
        //define
		D3DVECTOR upperLeft,upperRight,lowerLeft,lowerRight;
        upperLeft.x  = clickZone->m_rectMinX;
		upperLeft.y  = clickZone->m_rectMaxY;
		upperLeft.z  = 3.0f;
		lowerLeft.x  = clickZone->m_rectMinX;
		lowerLeft.y  = clickZone->m_rectMinY;
		lowerLeft.z  = 3.0f;
        upperRight.x = clickZone->m_rectMaxX;
		upperRight.y = clickZone->m_rectMaxY;
		upperRight.z = 3.0f;
        lowerRight.x = clickZone->m_rectMaxX;
		lowerRight.y = clickZone->m_rectMinY;
		lowerRight.z = 3.0f;
 
		//render
		/*DrawSegment(m_gStatus->m_dxRenderer->m_pd3dDevice,upperLeft,upperRight,offset);
		DrawSegment(m_gStatus->m_dxRenderer->m_pd3dDevice,upperLeft,lowerLeft,offset);
		DrawSegment(m_gStatus->m_dxRenderer->m_pd3dDevice,lowerLeft,lowerRight,offset);
		DrawSegment(m_gStatus->m_dxRenderer->m_pd3dDevice,lowerRight,upperRight,offset);*/
	}
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBtnPatchsys() 
{
	
}

/*void EngineXWin::OnSelchangeLISTAItree()
{
    int treeIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_GETCURSEL ,0,0);
	if(treeIndex == -1)
		return;
	POSITION posIndex = m_gStatus->GL_aiWebDatabase->FindIndex(treeIndex);
	if(!posIndex)
	    return;
    CAIWebObj * webObj = (CAIWebObj *)m_gStatus->GL_aiWebDatabase->GetAt(posIndex);
	if(!webObj->m_web)
		return;

	m_gStatus->m_dVectors.Clear();

	for(POSITION posLoc = webObj->m_web->GetHeadPosition();posLoc != NULL;)
	{
        CAIWebNode * wPtr = (CAIWebNode *)webObj->m_web->GetNext(posLoc);
		if(wPtr->m_directLinks)
		for(int iLoop=0;iLoop<wPtr->m_directLinkCount;iLoop++)
		{
			CAIWebNode * wPtrTarget = webObj->m_web->GetByIndex(wPtr->m_directLinks[iLoop]);
			if(wPtrTarget)
			   m_gStatus->m_dVectors.AddLineWLD(wPtr->m_position,wPtrTarget->m_position);
		}
	}
}
*/
//--------------------------------------------------//
void EngineXWin::OnCHECKWireMode()
{
    if(m_gStatus->m_renderStateObject->m_forceWireframe)
	   m_gStatus->m_renderStateObject->m_forceWireframe = 0;
	else
	   m_gStatus->m_renderStateObject->m_forceWireframe = 1;

	m_gStatus->m_renderStateObject->SetGlobalWireFrameMode(m_gStatus->m_renderStateObject->m_forceWireframe,m_gStatus->m_dxRenderer->m_pd3dDevice);

}
//--------------------------------------------------//
void EngineXWin::OnCHECKShadeMode()
{
    if(m_gStatus->m_renderStateObject->m_forceShadeMode)
	   m_gStatus->m_renderStateObject->m_forceShadeMode = 0;
	else
	   m_gStatus->m_renderStateObject->m_forceShadeMode = 1;
}
//--------------------------------------------------//
void EngineXWin::OnBTNENTAddLight() 
{
    CString conversionX,conversionY,conversionZ;
	m_entityDlgBar.GetDlgItemText( IDC_EDIT_quickMoveX,conversionX);
	m_entityDlgBar.GetDlgItemText( IDC_EDIT_quickMoveY,conversionY);
	m_entityDlgBar.GetDlgItemText( IDC_EDIT_quickMoveZ,conversionZ);

	int runtimeIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	   if(runtimeIndex == -1){
        MessageBox("Must select an entity object");
		return;
	   }
	   POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(runtimeIndex);
	   if(!posLoc)
		   return;
	       CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);


		   ptrLoc->List_baseFrame->SetPosition2(NULL,atof(conversionX),atof(conversionY),atof(conversionZ));
		   ptrLoc->List_baseFrame->GetPosition(NULL,&ptrLoc->List_movementDB->List_lastPosition);
		   ptrLoc->m_collisionRaySys->InitOnSpawn(ptrLoc->List_baseFrame,ptrLoc->List_skeletalObject,&m_gStatus->m_matLib);

	return;	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnChangeEDITForceLodGlobal() 
{
	
	CString buffer;
	m_wndDlgBar.GetDlgItemText( IDC_EDIT_forceLODGlobal,buffer);
    if(buffer.GetLength() < 1)
	{//disabled
        m_gStatus->m_lodOverride = -1;

		return;
	}

    m_gStatus->m_lodOverride = atoi(buffer);

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNEXPTest()
{
   int runtimeIndex = m_explosionDlgBar.SendDlgItemMessage(IDC_LIST_ExplosionList,LB_GETCURSEL ,0,0);
   if(runtimeIndex == -1){
      MessageBox("Must select an explosion");
	  return;
   }
   
   D3DVECTOR orientationDir;
   ZeroMemory(&orientationDir,sizeof(D3DVECTOR));
   orientationDir.y = 1.0f;

   POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);

   if(!entPosition)
   {
		AfxMessageBox("entity must be spawned first-uses first one in list");
		return;
   }
	
		
   CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
	
   D3DVECTOR intersect;
   ptrLoc->List_baseFrame->GetPosition(NULL,&intersect);

   /*m_gStatus->m_explosionDBList->CreateExplosion(runtimeIndex,
										         intersect,
										         m_gStatus->m_explosionRuntimeList,
										         orientationDir,
												 -1,
												 m_gStatus->m_dxRenderer->m_pd3dDevice,
												 0,
											     -1
										         ); */

   /*CExplosionObj * expObj = m_gStatus->m_explosionDBList->GetPtr(runtimeIndex);
   if(expObj)
	  m_gStatus->DoExpEffect(runtimeIndex,0,m_gStatus->m_explosionRuntimeList,expObj->m_pushPower,expObj->m_damageRadius);



}*/

//------------------------------------------------------------------------------------//
// Display > Capture All Zone Maps: open every zone in GameFiles (like the Open button),
// let it settle, capture it, write Maps\capture_log.txt
//------------------------------------------------------------------------------------//
void EngineXWin::MapBatchStep()
{
#ifndef _ClientOutput
	if(!m_gStatus)
		return;
	if(m_gStatus->m_mapCaptureAllZonesRequest)
	{
		m_gStatus->m_mapCaptureAllZonesRequest = FALSE;
		if(m_mapBatchIndex < 0)
		{
			m_mapBatchZones.RemoveAll();
			WIN32_FIND_DATAA fd;
			CString pattern = m_gStatus->GL_workingDirectory + "\\GameFiles\\*.exg";
			HANDLE h = FindFirstFileA(pattern, &fd);
			if(h != INVALID_HANDLE_VALUE)
			{
				do
				{
					if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						continue;
					CString name = fd.cFileName;
					CString upper = name;
					upper.MakeUpper();
					if(upper.Find("_AI") >= 0 || upper == "ENDLESS_CHARSEL.EXG")
						continue;   // monster pathing files / character select screen
					int at = (int)m_mapBatchZones.GetSize();
					while(at > 0 && m_mapBatchZones[at - 1].CompareNoCase(name) > 0)
						at--;
					m_mapBatchZones.InsertAt(at, name);
				} while(FindNextFileA(h, &fd));
				FindClose(h);
			}
			if(m_mapBatchZones.GetSize() == 0)
			{
				MessageBox("No zone files (*.exg) found in GameFiles.", "Capture All Zone Maps", MB_OK | MB_ICONWARNING);
				return;
			}
			m_mapBatchIndex = 0;
			m_mapBatchPhase = 0;
			m_mapBatchOk = m_mapBatchFail = 0;
			m_mapBatchLog = "Capture All Zone Maps\r\n\r\n";
			m_gStatus->m_mapCaptureBatch = TRUE;
		}
	}
	if(m_mapBatchIndex < 0)
		return;

	DWORD now = GetTickCount();
	if(m_mapBatchPhase == 0)
	{// open the next zone
		if(m_mapBatchIndex >= m_mapBatchZones.GetSize())
		{// all done
			m_gStatus->m_mapCaptureBatch = FALSE;
			m_mapBatchIndex = -1;
			CString dir = m_gStatus->GL_workingDirectory + "\\Maps";
			CreateDirectoryA(dir, NULL);
			CString summary;
			summary.Format("\r\n%d zone maps saved, %d failed.\r\n", m_mapBatchOk, m_mapBatchFail);
			m_mapBatchLog += summary;
			FILE * f = NULL;
			if(fopen_s(&f, dir + "\\capture_log.txt", "wb") == 0 && f)
			{
				fwrite((const char*)m_mapBatchLog, 1, m_mapBatchLog.GetLength(), f);
				fclose(f);
			}
			CString msg;
			msg.Format("Done: %d zone maps saved, %d failed.\n\nPictures are in %s\nDetails: Maps\\capture_log.txt",
			           m_mapBatchOk, m_mapBatchFail, (const char*)dir);
			MessageBox(msg, "Capture All Zone Maps", MB_OK | MB_ICONINFORMATION);
			return;
		}
		CString name = m_mapBatchZones[m_mapBatchIndex];
		m_gStatus->LoadNewWorld = TRUE;
		m_gStatus->NewWorld = name;
		m_gStatus->m_currentWorkingFile = name;
		m_wndDlgBar.SetDlgItemText(IDC_EDIT_FILENAME, name);
		m_refreshInterface = TRUE;
		m_mapBatchStamp = now;
		m_mapBatchPhase = 1;
		if(m_gStatus->m_systemReadout)
		{
			CString line;
			line.Format("Map %d of %d: %s", m_mapBatchIndex + 1, (int)m_mapBatchZones.GetSize(), (const char*)name);
			m_gStatus->m_systemReadout->AddLine(line);
		}
	}
	else if(m_mapBatchPhase == 1)
	{// wait for the load to run, then 3 seconds for the zone to settle
		if(m_gStatus->LoadNewWorld)
		{
			m_mapBatchStamp = now;
			return;
		}
		if(now - m_mapBatchStamp < 3000)
			return;
		m_gStatus->m_mapCaptureResult = "capture did not start";
		m_gStatus->m_mapCaptureRequest = TRUE;
		m_mapBatchPhase = 2;
	}
	else
	{// wait for the capture to finish
		if(m_gStatus->m_mapCaptureRequest || m_gStatus->m_mapCaptureBusy)
			return;
		bool ok = (m_gStatus->m_mapCaptureResult == "saved");
		if(ok) m_mapBatchOk++; else m_mapBatchFail++;
		CString line;
		if(ok && m_gStatus->m_mapCaptureFloors >= 2)
			line.Format("%-36s saved, %d floors\r\n", (const char*)m_mapBatchZones[m_mapBatchIndex], m_gStatus->m_mapCaptureFloors);
		else
			line.Format("%-36s %s\r\n", (const char*)m_mapBatchZones[m_mapBatchIndex], (const char*)m_gStatus->m_mapCaptureResult);
		m_mapBatchLog += line;
		m_mapBatchIndex++;
		m_mapBatchPhase = 0;
	}
#endif
}

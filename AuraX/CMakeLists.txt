# AuraX core engine library (generated; only files that exist on disk)
set(AURAX_SOURCES
    ../ABWebComm/DirectoryRegistryWrapper.cpp
    ../ABWebComm/HttpFileDownloader.cpp
    ../AXEncryptDLL/AXBase64EncoderClass.cpp
    ../AXEncryptDLL/aes.cpp
    ../AXEncryptDLL/sha256.cpp
    AXEncryptionLib.cpp
    AXLanguageFilter.cpp
    AXTextFormatter.cpp
    ActionZones.cpp
    AuraXDynamic.cpp
    AssimpSceneConverter.cpp   # Assimp -> engine mesh conversion (MFC-free, unit-testable)
    CAssimpModelImporter.cpp   # EngineX modern model import front end (FBX/OBJ/glTF/...)
    C2DVectorDrawClass.cpp
    CABFunctionLib.cpp
    CAIRaidClass.cpp
    CAIScriptClass.cpp
    CAITraceClass.cpp
    CAITreeSearchClass.cpp
    CAIWebClass.cpp
    CAIclass.cpp
    CAdvancedVertexClass.cpp
    CArmedInventoryClass.cpp
    CAttributeClass.cpp
    CBackgroundClass.cpp
    CBonusClass.cpp
    CBankClass.cpp
    CBattleSchedulerClass.cpp
    CBlockIPListClass.cpp
    CBoneAnimationClass.cpp
    CBoneClass.cpp
    CBoneCursorClass.cpp
    CBountySystem.cpp
    CCameraClass.cpp
    CCfgClass.cpp
    CCharacterDefinitionsClass.cpp
    CCharonaClass.cpp
    CCollisionClass.cpp
    CCommGroupClass.cpp
    CCommerceClass.cpp
    CCommonCtrObj.cpp
    CContainmentClass.cpp
    CCurrencyClass.cpp
    CDamagePathClass.cpp
    CDayStateClass.cpp
    CDotClass.cpp
    CDeformableMeshClass.cpp
    CDynamicOverlay.cpp
    CEXMeshClass.cpp
    CEXScriptClass.cpp
    CElementStatusClass.cpp
    CElementsClass.cpp
    CEnvironmentClass.cpp
    CExplosionClass.cpp
    CFilterSpeechClass.cpp
    CFireRangeClass.cpp
    CFloraGenClass.cpp
    CFrameObj.cpp
    CGFontClass.cpp
    CGlobalMaterialShaderLib.cpp
    CGmPageClass.cpp
    CGroupClass.cpp
    CHiarcleMeshClass.cpp
    CHousePlacementClass.cpp
    CHousingClass.cpp
    CHudClass.cpp
    CIconSystemClass.cpp
    CInventoryClass.cpp
    CItemClass.cpp
    CLaserClass.cpp
    CLightTrackerClass.cpp
    CLightningEffectClass.cpp
    CMaterialClass.cpp
    CMenu3DClass.cpp
    CMenuBarClass.cpp
    CMenuSliderClass.cpp
    CMeshLodClass.cpp
    CMissileClass.cpp
    CMovementModel.cpp
    CMovieRuntimeClass.cpp
    CMusicClass.cpp
    CNetMonSysClass.cpp
    CNetworkRecordClass.cpp
    CNodeObject.cpp
    CNoterietyClass.cpp
    CObjectLibraryClass.cpp
    CParticleEmitRange.cpp
    CPendingCommandsClass.cpp
    CPersistClass.cpp
    CPhysicsEmuClass.cpp
    CPluginTransfer.cpp
    CPolyObj.cpp
    CPortalSysClass.cpp
    CQuestJournal.cpp
    CRCgenerator.cpp
    CRTLightClass.cpp
    CReferenceImporterExporter.cpp
    CReferenceLibrary.cpp
    CRenderContainer.cpp
    CResourceClass.cpp
    CSSoundRange.cpp
    CSafeZones.cpp
    CScreenFlashClass.cpp
    CServerEventLogClass.cpp
    CSessionClass.cpp
    CShaderLibClass.cpp
    CShadowClass.cpp
    CSkeletalLODClass.cpp
    CSkeletonClass.cpp
    CSkillObject.cpp
    CSoundManagerClass.cpp
    CSoundTrackClass.cpp
    CSpawnClass.cpp
    CSphereCollisionClass.cpp
    CStatClass.cpp
    CStateManagementClass.cpp
    CStormClass.cpp
    CTextClass.cpp
    CTextRenderClass.cpp
    CTextureDBClass.cpp
    CTextureSwapClass.cpp
    CTimeSystem.cpp
    CTranslucentPolyCache.cpp
    CTriggerClass.cpp
    CVertexAnimationModule.cpp
    CWaterZoneClass.cpp
    CWldObject.cpp
    CWorldAvailableClass.cpp
    CWorldChannelRules.cpp
    CWorldGroupClass.cpp
    CitemExpanseObj.cpp
    Collision_RaySystem.cpp
    ControlsClass.cpp
    CtextReadoutClass.cpp
    DBConnectionSystem.cpp
    EnvParticleSystem.cpp
    MatrixArb.cpp
    PathTracker.cpp
    PostProcessModule.cpp
    TextureObj.cpp
    CInput.cpp
    ViewportClass.cpp
    stdafx.cpp
)

add_library(AuraX SHARED ${AURAX_SOURCES})
set_target_properties(AuraX PROPERTIES
    OUTPUT_NAME "AuraXDynamic"
    # The original AuraXDynamic.def export list is missing, so the DLL exported
    # nothing. Auto-export all symbols (including the linked-in EXGameCore static
    # lib) so NetworkExtension and the client can link against the engine DLL.
    WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_compile_definitions(AuraX PRIVATE WIN32 _WINDOWS _AFXEXT _AXDYNAMIC _CRT_SECURE_NO_WARNINGS)
target_link_libraries(AuraX PRIVATE
    EXGameCore
    Framedyn Version Wininet winmm
    d3d9 dxguid dinput8 dsound
    d3dx9 d3dxof   # legacy DirectX SDK - transitional
    assimp::assimp   # modern 3D model import (FBX, OBJ, glTF, ...) - static, linked into this DLL
)

target_include_directories(AuraX PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/ABWebComm
    ${CMAKE_SOURCE_DIR}/AXEncryptDLL
    ${CMAKE_SOURCE_DIR}/thirdparty/directplay
    ${CMAKE_SOURCE_DIR}/HeaderFiles/ClientBaseHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/CoreStatic
    ${CMAKE_SOURCE_DIR}/HeaderFiles/InterfaceHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/NetworkHeaders
    ${CMAKE_SOURCE_DIR}/HeaderFiles/RenderModules
    ${CMAKE_SOURCE_DIR}/HeaderFiles/Zpingheaders
)

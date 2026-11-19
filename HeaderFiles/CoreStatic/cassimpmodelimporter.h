#ifndef CASSIMPMODELIMPORTER_H
#define CASSIMPMODELIMPORTER_H

// ---------------------------------------------------------------------------
// CAssimpModelImporter
//
// EngineX front end for importing modern 3D model formats through Assimp
// (Open Asset Import Library): FBX, OBJ, glTF/glb, Collada, STL, PLY, 3DS,
// Blender, ASE, LightWave and DXF files.
//
// CEXMeshObj::LoadGeometryFromPlugin() dispatches to the Assimp path
// (CEXMeshObj::LoadGeometryFromAssimp, implemented in CAssimpModelImporter.cpp)
// whenever the file extension is one of the supported modern formats, so every
// editor tool that loads a model (object library, LODs, missiles, portals,
// water zones, ...) accepts the new formats without further changes. The
// engine's native serialized formats (.arw and the legacy .x archives) are
// NOT handled here and keep going through the native loader.
//
// See Documentation/AssimpModelImport.md for usage notes and limitations.
// ---------------------------------------------------------------------------

class CEXMeshObj;
class CEXMeshObjList;

class CAssimpModelImporter
{
public:
    // TRUE when fileName's extension is a modern format Assimp handles
    // (.fbx .obj .gltf .glb .dae .stl .ply .3ds .blend .ase .lwo .dxf).
    static BOOL IsSupportedFile(CString fileName);

    // Read-only import (used by CEXMeshObj::GetGeometryFromPlugin): fills raw
    // vertex/index arrays and the texture-name slots. The caller owns the
    // returned arrays and releases them with delete[].
    static BOOL ImportToArrays(CString fileName,
                               WORD * vertexCount,
                               ABVERTEX ** vertexArray,
                               WORD * indecieCount,
                               WORD ** indecieArray,
                               CString texture[10]);

    // Multi-material / auto-chunked import into a CEXMeshObjList (used by
    // Reference Library / Visual Manager so multi-material FBX/OBJ/glTF
    // models create a separate CEXMeshObj + CMaterialObject per material).
    static BOOL ImportToMeshList(CString fileName,
                                 LPDIRECT3DDEVICE9 g_pd3dDevice,
                                 CEXMeshObjList * outMeshList);

    // Export a single CEXMeshObj or a CEXMeshObjList (including legacy .arw
    // and .rui models) to a standard right-handed Wavefront .obj + .mtl file.
    static BOOL ExportMeshToObj(CEXMeshObj * mesh, CString fileName);
    static BOOL ExportMeshListToObj(CEXMeshObjList * meshList, CString fileName);

    // Human-readable text of the most recent failed import (for message boxes).
    static CString GetLastError();

    // Records the most recent failure (called by the import paths).
    static void SetLastError(const CString & message);
};

#endif

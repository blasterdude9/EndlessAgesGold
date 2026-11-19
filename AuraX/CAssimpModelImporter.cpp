#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CEXMeshClass.h"
#include "CAssimpModelImporter.h"
#include "assimpsceneconverter.h"

#include <string>
#include <vector>
#include "CSkeletalClass.h"
#include <d3dx9.h>
#include <cmath>
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#pragma pop_macro("max")
#pragma pop_macro("min")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------//
// Local helpers
//-----------------------------------------------------------------------------//

// Most recent import failure (shared by SetLastError/GetLastError).
static CString g_assimpLastError;

// Lowercase file extension without the dot ("FBX" -> "fbx"). "" when none.
static CString GetExtensionLower(CString fileName)
{
    CString ext;
    int length = fileName.GetLength();
    if(length == 0)
        return ext;

    while(length > 0)
    {
        length--;
        TCHAR chr = fileName.GetAt(length);
        if(chr == '.')
            break;
        ext.Insert(0, chr);
    }
    ext.MakeLower();
    return ext;
}

// The engine resolves texture names relative to its models path
// (GL_pathMapModelsRef + name), so store a relative name: forward slashes
// become backslashes, absolute paths are reduced to their file name and a
// leading ".\" is stripped.
static CString NormalizeTextureName(const std::string & rawPath)
{
    CString out(rawPath.c_str());
    out.Replace('/', '\\');

    BOOL absolute = FALSE;
    if(out.Find(':') >= 0)
        absolute = TRUE;                       // C:\... or http://...
    else if(out.GetLength() > 0 && out.GetAt(0) == '\\')
        absolute = TRUE;                       // \\server\... or \dir\...
    if(absolute)
    {
        int slash = out.ReverseFind('\\');
        if(slash >= 0)
            out = out.Mid(slash + 1);
    }

    if(out.GetLength() > 2 && out.GetAt(0) == '.' && out.GetAt(1) == '\\')
        out = out.Mid(2);

    return out;
}

static BOOL PopulateEngineMeshFromConverted(CEXMeshObj * mesh,
                                            const AssimpConvertedMesh & converted,
                                            float * sightRange,
                                            LPDIRECT3DDEVICE9 g_pd3dDevice,
                                            BNDBOX * boxOption)
{
    if(!mesh || converted.vertices.empty() || converted.indices.empty())
        return FALSE;

    int vCount = (int)converted.vertices.size();
    ABVERTEX * abVertex = new ABVERTEX[vCount];
    memset(abVertex, 0, sizeof(ABVERTEX) * vCount);
    for(int loop = 0; loop < vCount; loop++)
    {
        const AssimpConvertedVertex & src = converted.vertices[loop];
        ABVERTEX & dst = abVertex[loop];
        dst.x  = src.x;
        dst.y  = src.y;
        dst.z  = src.z;
        dst.nx = src.nx;
        dst.ny = src.ny;
        dst.nz = src.nz;

        TXTUV * uvSet[8];
        uvSet[0] = &dst.tx0;
        uvSet[1] = &dst.tx1;
        uvSet[2] = &dst.tx2;
        uvSet[3] = &dst.tx3;
        uvSet[4] = &dst.tx4;
        uvSet[5] = &dst.tx5;
        uvSet[6] = &dst.tx6;
        uvSet[7] = &dst.tx7;
        for(int uvLoop = 0; uvLoop < 8; uvLoop++)
        {
            uvSet[uvLoop]->tu = src.tu[uvLoop];
            uvSet[uvLoop]->tv = src.tv[uvLoop];
        }
    }

    if(mesh->m_abVertexArray.m_indecieArray)
    {
        delete [] mesh->m_abVertexArray.m_indecieArray;
        mesh->m_abVertexArray.m_indecieArray = 0;
    }
    mesh->m_abVertexArray.m_vertexCount  = (WORD)vCount;
    mesh->m_abVertexArray.m_indecieCount = (WORD)converted.indices.size();
    mesh->m_abVertexArray.m_indecieArray = new WORD[mesh->m_abVertexArray.m_indecieCount];
    for(size_t loop = 0; loop < converted.indices.size(); loop++)
        mesh->m_abVertexArray.m_indecieArray[loop] = converted.indices[loop];

    //Texture slots: the engine counts contiguous non-"NONE" slots from 0.
    for(int loop = 0; loop < 10; loop++)
        mesh->List_storedTextureFromFile[loop] = "NONE";
    if(converted.diffuseTexture.length() > 0)
        mesh->List_storedTextureFromFile[0] = NormalizeTextureName(converted.diffuseTexture);

    //Carry the source material's diffuse color into the engine material.
    if(!mesh->List_materialObject)
        mesh->List_materialObject = new CMaterialObject();   // InitBuffer needs one
    if(mesh->List_materialObject)
    {
        mesh->List_materialObject->m_baseMaterial.Diffuse.r = converted.diffuseColor[0];
        mesh->List_materialObject->m_baseMaterial.Diffuse.g = converted.diffuseColor[1];
        mesh->List_materialObject->m_baseMaterial.Diffuse.b = converted.diffuseColor[2];
        mesh->List_materialObject->m_baseMaterial.Diffuse.a = converted.diffuseColor[3];
        mesh->List_materialObject->m_baseMaterial.Ambient.r = converted.diffuseColor[0];
        mesh->List_materialObject->m_baseMaterial.Ambient.g = converted.diffuseColor[1];
        mesh->List_materialObject->m_baseMaterial.Ambient.b = converted.diffuseColor[2];
        mesh->List_materialObject->m_baseMaterial.Ambient.a = converted.diffuseColor[3];
        mesh->List_materialObject->CopyMaterial(mesh->List_materialObject->m_baseMaterial,
                                                &mesh->List_materialObject->m_useMaterial);
    }

    //Assimp already generated smooth normals; 3.0f is the engine's
    //"minimal smoothing" shade angle (keeps hard edges hard).
    return mesh->FinalizeLoadedGeometry(abVertex,
                                        converted.boundingRadius,
                                        FALSE,
                                        3.0f,
                                        g_pd3dDevice,
                                        boxOption,
                                        FALSE,
                                        sightRange);
}

static BOOL ExtractConvertedFromEngineMesh(CEXMeshObj * mesh,
                                           AssimpConvertedMesh & outConverted)
{
    if(!mesh)
        return FALSE;

    if(mesh->m_abVertexArray.m_vertexCount == 0 ||
       mesh->m_abVertexArray.m_indecieCount < 3 ||
       !mesh->m_abVertexArray.m_indecieArray)
    {
        return FALSE;
    }

    ABVERTEX * abVertex = NULL;
    int vCount = 0;
    mesh->m_abVertexArray.GetABVERTEXStyle(&abVertex, &vCount);
    if(!abVertex || vCount <= 0)
        return FALSE;

    outConverted = AssimpConvertedMesh();
    outConverted.vertices.resize(vCount);
    for(int i = 0; i < vCount; i++)
    {
        const ABVERTEX & src = abVertex[i];
        AssimpConvertedVertex & dst = outConverted.vertices[i];
        memset(&dst, 0, sizeof(dst));
        dst.x  = src.x;
        dst.y  = src.y;
        dst.z  = src.z;
        dst.nx = src.nx;
        dst.ny = src.ny;
        dst.nz = src.nz;

        dst.tu[0] = src.tx0.tu; dst.tv[0] = src.tx0.tv;
        dst.tu[1] = src.tx1.tu; dst.tv[1] = src.tx1.tv;
        dst.tu[2] = src.tx2.tu; dst.tv[2] = src.tx2.tv;
        dst.tu[3] = src.tx3.tu; dst.tv[3] = src.tx3.tv;
        dst.tu[4] = src.tx4.tu; dst.tv[4] = src.tx4.tv;
        dst.tu[5] = src.tx5.tu; dst.tv[5] = src.tx5.tv;
        dst.tu[6] = src.tx6.tu; dst.tv[6] = src.tx6.tv;
        dst.tu[7] = src.tx7.tu; dst.tv[7] = src.tx7.tv;
    }
    delete [] abVertex;

    const int iCount = (int)mesh->m_abVertexArray.m_indecieCount;
    outConverted.indices.resize(iCount);
    for(int i = 0; i < iCount; i++)
        outConverted.indices[i] = mesh->m_abVertexArray.m_indecieArray[i];

    outConverted.uvChannelCount = mesh->m_abVertexArray.m_uvCount;
    outConverted.boundingRadius = mesh->m_meshBoundingSphereRadius;

    if(mesh->List_storedTextureFromFile[0].GetLength() > 0 &&
       mesh->List_storedTextureFromFile[0].CompareNoCase("NONE") != 0)
    {
        outConverted.diffuseTexture = (LPCTSTR)mesh->List_storedTextureFromFile[0];
    }

    if(mesh->List_materialObject)
    {
        outConverted.diffuseColor[0] = mesh->List_materialObject->m_baseMaterial.Diffuse.r;
        outConverted.diffuseColor[1] = mesh->List_materialObject->m_baseMaterial.Diffuse.g;
        outConverted.diffuseColor[2] = mesh->List_materialObject->m_baseMaterial.Diffuse.b;
        outConverted.diffuseColor[3] = mesh->List_materialObject->m_baseMaterial.Diffuse.a;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------//
// CAssimpModelImporter
//-----------------------------------------------------------------------------//

BOOL CAssimpModelImporter::IsSupportedFile(CString fileName)
{
    CString ext = GetExtensionLower(fileName);
    if(ext.GetLength() == 0)
        return FALSE;
    return AssimpSceneConverter::IsSupportedExtension(std::string((LPCTSTR)ext)) ? TRUE : FALSE;
}

void CAssimpModelImporter::SetLastError(const CString & message)
{
    g_assimpLastError = message;
}

CString CAssimpModelImporter::GetLastError()
{
    return g_assimpLastError;
}

BOOL CAssimpModelImporter::ImportToArrays(CString fileName,
                                          WORD * vertexCount,
                                          ABVERTEX ** vertexArray,
                                          WORD * indecieCount,
                                          WORD ** indecieArray,
                                          CString texture[10])
{
    if(!vertexCount || !vertexArray || !indecieCount || !indecieArray || !texture)
        return FALSE;

    *vertexCount  = 0;
    *vertexArray  = NULL;
    *indecieCount = 0;
    *indecieArray = NULL;
    for(int loop = 0; loop < 10; loop++)
        texture[loop] = "NONE";

    AssimpConvertedMesh converted;
    std::string errorMsg;
    if(!AssimpSceneConverter::ConvertFile(std::string((LPCTSTR)fileName), converted, errorMsg))
    {
        SetLastError(operator+(operator+("Failed to import model:\n", fileName),
                               operator+("\n\n", CString(errorMsg.c_str()))));
        return FALSE;
    }
    if(converted.vertices.empty() || converted.indices.empty())
    {
        SetLastError(operator+("Failed to import model (no triangles):\n", fileName));
        return FALSE;
    }

    int vCount = (int)converted.vertices.size();
    ABVERTEX * abVertex = new ABVERTEX[vCount];
    memset(abVertex, 0, sizeof(ABVERTEX) * vCount);
    for(int loop = 0; loop < vCount; loop++)
    {
        const AssimpConvertedVertex & src = converted.vertices[loop];
        ABVERTEX & dst = abVertex[loop];
        dst.x  = src.x;
        dst.y  = src.y;
        dst.z  = src.z;
        dst.nx = src.nx;
        dst.ny = src.ny;
        dst.nz = src.nz;

        TXTUV * uvSet[8];
        uvSet[0] = &dst.tx0;
        uvSet[1] = &dst.tx1;
        uvSet[2] = &dst.tx2;
        uvSet[3] = &dst.tx3;
        uvSet[4] = &dst.tx4;
        uvSet[5] = &dst.tx5;
        uvSet[6] = &dst.tx6;
        uvSet[7] = &dst.tx7;
        for(int uvLoop = 0; uvLoop < 8; uvLoop++)
        {
            uvSet[uvLoop]->tu = src.tu[uvLoop];
            uvSet[uvLoop]->tv = src.tv[uvLoop];
        }
    }

    int iCount = (int)converted.indices.size();
    WORD * indexArray = new WORD[iCount];
    for(int loop = 0; loop < iCount; loop++)
        indexArray[loop] = converted.indices[loop];

    if(converted.diffuseTexture.length() > 0)
        texture[0] = NormalizeTextureName(converted.diffuseTexture);

    *vertexCount  = (WORD)vCount;
    *vertexArray  = abVertex;
    *indecieCount = (WORD)iCount;
    *indecieArray = indexArray;
    return TRUE;
}

BOOL CAssimpModelImporter::ImportToMeshList(CString fileName,
                                            LPDIRECT3DDEVICE9 g_pd3dDevice,
                                            CEXMeshObjList * outMeshList)
{
    if(!outMeshList)
        return FALSE;

    std::vector<AssimpConvertedMesh> subMeshes;
    std::string errorMsg;
    if(!AssimpSceneConverter::ConvertFileToSubMeshes(std::string((LPCTSTR)fileName), subMeshes, errorMsg))
    {
        CString full = operator+(operator+("Failed to import model:\n", fileName),
                                 operator+("\n\n", CString(errorMsg.c_str())));
        SetLastError(full);
        AfxMessageBox(full);
        return FALSE;
    }

    int addedCount = 0;
    for(size_t m = 0; m < subMeshes.size(); m++)
    {
        CEXMeshObj * meshObj = new CEXMeshObj();
        meshObj->List_materialObject = new CMaterialObject();
        float sightRange = 0.0f;
        if(PopulateEngineMeshFromConverted(meshObj, subMeshes[m], &sightRange, g_pd3dDevice, NULL))
        {
            outMeshList->AddTail(meshObj);
            addedCount++;
        }
        else
        {
            meshObj->SafeDelete();
            delete meshObj;
        }
    }

    return (addedCount > 0) ? TRUE : FALSE;
}

BOOL CAssimpModelImporter::ExportMeshToObj(CEXMeshObj * mesh, CString fileName)
{
    AssimpConvertedMesh converted;
    if(!ExtractConvertedFromEngineMesh(mesh, converted))
    {
        SetLastError(operator+("Cannot export mesh (no triangle geometry):\n", fileName));
        return FALSE;
    }

    std::string errorMsg;
    if(!AssimpSceneConverter::ExportToObjFile(std::string((LPCTSTR)fileName), converted, errorMsg))
    {
        SetLastError(operator+(operator+("Failed to export OBJ:\n", fileName),
                               operator+("\n\n", CString(errorMsg.c_str()))));
        return FALSE;
    }

    return TRUE;
}

BOOL CAssimpModelImporter::ExportMeshListToObj(CEXMeshObjList * meshList, CString fileName)
{
    if(!meshList || meshList->GetCount() == 0)
    {
        SetLastError(operator+("Cannot export empty mesh list:\n", fileName));
        return FALSE;
    }

    std::vector<AssimpConvertedMesh> subMeshes;
    for(POSITION pos = meshList->GetHeadPosition(); pos != NULL;)
    {
        CEXMeshObj * mesh = (CEXMeshObj *)meshList->GetNext(pos);
        AssimpConvertedMesh converted;
        if(ExtractConvertedFromEngineMesh(mesh, converted))
            subMeshes.push_back(converted);
    }

    if(subMeshes.empty())
    {
        SetLastError(operator+("Cannot export mesh list (no triangle geometry):\n", fileName));
        return FALSE;
    }

    std::string errorMsg;
    if(!AssimpSceneConverter::ExportToObjFile(std::string((LPCTSTR)fileName), subMeshes, errorMsg))
    {
        SetLastError(operator+(operator+("Failed to export OBJ:\n", fileName),
                               operator+("\n\n", CString(errorMsg.c_str()))));
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------//
// CEXMeshObj::LoadGeometryFromAssimp - the Assimp import path.
// Fills this mesh from a modern model file (FBX, OBJ, glTF, ...) and runs the
// same finalization the native loader uses (buffers, normals, bounds, popout
// box), so the result is indistinguishable from a natively loaded mesh.
//-----------------------------------------------------------------------------//

BOOL CEXMeshObj::LoadGeometryFromAssimp(CString fileName,
                                        float * sightRange,
                                        LPDIRECT3DDEVICE9 g_pd3dDevice,
                                        BNDBOX * boxOption)
{
    AssimpConvertedMesh converted;
    std::string errorMsg;
    if(!AssimpSceneConverter::ConvertFile(std::string((LPCTSTR)fileName), converted, errorMsg))
    {
        CString full = operator+(operator+("Failed to import model:\n", fileName),
                                 operator+("\n\n", CString(errorMsg.c_str())));
        CAssimpModelImporter::SetLastError(full);
        AfxMessageBox(full);
        return FALSE;
    }
    if(converted.vertices.empty() || converted.indices.empty())
    {
        CString full = operator+("Failed to import model (no triangles):\n", fileName);
        CAssimpModelImporter::SetLastError(full);
        AfxMessageBox(full);
        return FALSE;
    }

    return PopulateEngineMeshFromConverted(this, converted, sightRange, g_pd3dDevice, boxOption);
}

BOOL CEXMeshObj::ExportToObj(CString fileName)
{
    return CAssimpModelImporter::ExportMeshToObj(this, fileName);
}

BOOL CEXMeshObjList::ExportToObj(CString fileName)
{
    return CAssimpModelImporter::ExportMeshListToObj(this, fileName);
}

//------------------------------------------------------------------------------------//
// CSkeletonObject::ImportFromGltf - characters back in from Blender (easy-editor)
//
// Reads a .gltf/.glb (or .fbx) made from Quick > Export Character For Blender and
// puts the body parts back into this character, skinned to its OWN skeleton:
//   - meshes named part_0, part_1 ... go back into the same body-part slot
//     (others fill the slots in order)
//   - every vertex keeps up to 4 bones + weights from Blender; bones are matched
//     by name; a bone Blender added that the game does not have hands its
//     weights to the closest parent bone the game does have
//   - the game's existing animations keep playing on the new mesh
// Only the editor's copy changes - "Save Single Entity" / "Save Database" keeps it.
//------------------------------------------------------------------------------------//
// Builds the part's mesh from the file exactly as given: same vertex order and count, so the
// skin (vertex -> bones) built right after it lines up. The normal import path runs
// OptimizeMesh, which welds and reorders vertices (and miscounts on big meshes -> heap damage).
static BOOL BuildSkinnedMeshKeepOrder(CEXMeshObj * mesh, const AssimpConvertedMesh & conv, LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	const int vCount = (int)conv.vertices.size();
	if(!mesh || vCount <= 0 || vCount > 32000 || conv.indices.size() < 3) return FALSE;
	ABVERTEX * abVertex = new ABVERTEX[vCount];
	memset(abVertex, 0, sizeof(ABVERTEX) * vCount);
	for(int i = 0; i < vCount; i++)
	{
		const AssimpConvertedVertex & s = conv.vertices[i];
		abVertex[i].x = s.x; abVertex[i].y = s.y; abVertex[i].z = s.z;
		abVertex[i].nx = s.nx; abVertex[i].ny = s.ny; abVertex[i].nz = s.nz;
		abVertex[i].tx0.tu = s.tu[0]; abVertex[i].tx0.tv = s.tv[0];
	}
	if(mesh->m_abVertexArray.m_indecieArray) { delete [] mesh->m_abVertexArray.m_indecieArray; mesh->m_abVertexArray.m_indecieArray = 0; }
	const int iCount = (int)(conv.indices.size() / 3) * 3;
	mesh->m_abVertexArray.m_vertexCount  = (WORD)vCount;
	mesh->m_abVertexArray.m_indecieCount = (WORD)iCount;
	mesh->m_abVertexArray.m_indecieArray = new WORD[iCount];
	for(int i = 0; i < iCount; i++) mesh->m_abVertexArray.m_indecieArray[i] = (WORD)conv.indices[i];
	// Keep all engine texture names, including slots beyond diffuse. glTF image URIs
	// are relative to the imported file and often live outside MapsModels; replacing
	// these names makes otherwise valid armor load as white in the game.
	if(!mesh->List_materialObject) mesh->List_materialObject = new CMaterialObject();
	mesh->GetUVlayerCountString();
	mesh->m_abVertexArray.SetABVERTEXStyle(g_pd3dDevice, abVertex, vCount);
	delete [] abVertex;
	mesh->m_abVertexArray.NormalizeOnly();
	mesh->GetBox();
	mesh->GetCenterRadius();
	if(mesh->m_abVertexArray.m_indicieBuffer) { mesh->m_abVertexArray.m_indicieBuffer->Release(); mesh->m_abVertexArray.m_indicieBuffer = 0; }
	if(g_pd3dDevice) mesh->InitBuffer(g_pd3dDevice, NULL);
	return TRUE;
}
BOOL CSkeletonObject::ImportFromGltf(CString fileName, LPDIRECT3DDEVICE9 g_pd3dDevice, CString & report)
{
	report.Empty();
	if(!m_bonesList || m_bonesList->GetCount() == 0)
	{
		report = "This character has no skeleton.";
		return FALSE;
	}
	m_bonesList->CompileDirectAccess();
	const int boneCount = m_bonesList->m_directAccessArrayCount;
	std::vector<D3DXMATRIX> invBind(boneCount);
	std::vector<CString> boneNames(boneCount);
	for(int b = 0; b < boneCount; b++)
	{
		D3DXMatrixIdentity(&invBind[b]);
		CBoneObject * bp = m_bonesList->m_directAccessArray[b].object;
		if(!bp) continue;
		boneNames[b] = bp->m_boneName;
		D3DXMATRIX m((const float *)&bp->m_startPosition.m[0][0]);
		if(!D3DXMatrixInverse(&invBind[b], NULL, &m)) D3DXMatrixIdentity(&invBind[b]);
	}

	// the body-part slots, in the same order the export numbered them
	std::vector<CDeformableVisObj *> slots;
	auto addVis = [&](CDeformableVisObj * vo) {
		if(!vo || !vo->m_mesh || vo->m_mesh->m_abVertexArray.m_vertexCount == 0 || vo->m_mesh->m_abVertexArray.m_indecieCount < 3) return;
		for(size_t i = 0; i < slots.size(); i++) if(slots[i]->m_mesh == vo->m_mesh) return;
		slots.push_back(vo);
	};
	if(m_alternateConfigurations)
	{
		int slotIdx = 0;
		for(POSITION altPos = m_alternateConfigurations->GetHeadPosition(); altPos != NULL; ++slotIdx)
		{
			CAlterUnitDBObject * altUnit = (CAlterUnitDBObject *)m_alternateConfigurations->GetNext(altPos);
			if(!altUnit || !altUnit->m_configurations || altUnit->m_configurations->IsEmpty()) continue;
			int cfgIdx = (m_spawnCfg && slotIdx < m_deformableMeshArrayCount && m_spawnCfg[slotIdx] >= 0) ? m_spawnCfg[slotIdx] : 0;
			POSITION cfgPos = altUnit->m_configurations->FindIndex(cfgIdx);
			if(!cfgPos) cfgPos = altUnit->m_configurations->GetHeadPosition();
			CDeformableMesh * dm = cfgPos ? (CDeformableMesh *)altUnit->m_configurations->GetAt(cfgPos) : NULL;
			if(dm && dm->m_lodChain && !dm->m_lodChain->IsEmpty())
				addVis((CDeformableVisObj *)dm->m_lodChain->GetHead());
		}
	}
	if(m_deformableMeshArrayV2)
		for(int i = 0; i < m_deformableMeshArrayCount; i++)
		{
			CDeformableMesh * dm = m_deformableMeshArrayV2[i].m_defMeshVisualUnit;
			if(dm && dm->m_lodChain && !dm->m_lodChain->IsEmpty())
				addVis((CDeformableVisObj *)dm->m_lodChain->GetHead());
		}
	if(slots.empty())
	{
		report = "This character has no body parts to replace.";
		return FALSE;
	}
	// outfits by name (slot<S>_cfg<C>, from "Export Character + All Outfits"); ones not worn now get their own entry
	const size_t bodySlots = slots.size();
	std::vector<std::pair<CString, size_t> > outfitKeys;
	if(m_alternateConfigurations)
	{
		int slotIdx = 0;
		for(POSITION altPos = m_alternateConfigurations->GetHeadPosition(); altPos != NULL; ++slotIdx)
		{
			CAlterUnitDBObject * altUnit = (CAlterUnitDBObject *)m_alternateConfigurations->GetNext(altPos);
			if(!altUnit || !altUnit->m_configurations) continue;
			int c = 0;
			for(POSITION cp = altUnit->m_configurations->GetHeadPosition(); cp != NULL; ++c)
			{
				CDeformableMesh * dmc = (CDeformableMesh *)altUnit->m_configurations->GetNext(cp);
				if(!dmc || !dmc->m_lodChain || dmc->m_lodChain->IsEmpty()) continue;
				CDeformableVisObj * vo = (CDeformableVisObj *)dmc->m_lodChain->GetHead();
				if(!vo || !vo->m_mesh || vo->m_mesh->m_abVertexArray.m_vertexCount == 0) continue;
				size_t at = slots.size();
				for(size_t i = 0; i < slots.size(); i++) if(slots[i]->m_mesh == vo->m_mesh) { at = i; break; }
				CString key; key.Format("slot%d_cfg%d", slotIdx, c);
				if(at == slots.size()) slots.push_back(vo);
				outfitKeys.push_back(std::make_pair(key, at));
			}
		}
	}

	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);
	const aiScene * scene = importer.ReadFile((const char *)fileName,
		aiProcess_MakeLeftHanded | aiProcess_FlipUVs | aiProcess_FlipWindingOrder |
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights |
		aiProcess_GenSmoothNormals);
	if(!scene || !scene->mRootNode || scene->mNumMeshes == 0)
	{
		report = CString("Could not read the file:\n") + importer.GetErrorString();
		return FALSE;
	}

	// a file bone -> the game's bone (same name, or the closest parent the game has)
	auto mapBone = [&](const aiString & nm) -> int {
		const aiNode * node = scene->mRootNode->FindNode(nm);
		CString name = nm.C_Str();
		for(int guard = 0; guard < 64; guard++)
		{
			for(int b = 0; b < boneCount; b++)
				if(boneNames[b].CompareNoCase(name) == 0) return b;
			if(!node || !node->mParent) break;
			node = node->mParent;
			name = node->mName.C_Str();
		}
		return 0;
	};

	// group the file's meshes by part number (part_3 -> slot 3; unnamed -> next free slot)
	std::vector<std::vector<unsigned int> > groups(slots.size());
	std::vector<unsigned int> loose;
	for(unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		const aiMesh * am = scene->mMeshes[m];
		if(!am->HasPositions() || am->mNumFaces == 0) continue;
		int slot = -1;
		CString nm = am->mName.C_Str();
		nm.MakeLower();
		int so = nm.Find("slot"), co = nm.Find("_cfg");
		if(so >= 0 && co > so)
		{
			CString key; key.Format("slot%d_cfg%d", atoi((const char *)nm.Mid(so + 4)), atoi((const char *)nm.Mid(co + 4)));
			for(size_t k = 0; k < outfitKeys.size(); k++) if(outfitKeys[k].first == key) { slot = (int)outfitKeys[k].second; break; }
			if(slot < 0) continue;   // an outfit this character does not have
		}
		else
		{
			int at = nm.Find("part_");
			if(at >= 0) slot = atoi((const char *)nm.Mid(at + 5));
			if(slot >= (int)bodySlots) slot = -1;
		}
		if(slot >= 0 && slot < (int)slots.size()) groups[slot].push_back(m);
		else loose.push_back(m);
	}
	for(size_t i = 0, s = 0; i < loose.size(); i++)
	{
		while(s < bodySlots && !groups[s].empty()) s++;
		if(s >= bodySlots) break;
		groups[s].push_back(loose[i]);
	}

	int replaced = 0;
	CString details;
	for(size_t s = 0; s < slots.size(); s++)
	{
		if(groups[s].empty()) continue;
		CDeformableVisObj * vo = slots[s];

		AssimpConvertedMesh conv;
		std::vector<std::vector<std::pair<int, float> > > vw;
		bool tooBig = false;
		for(size_t g = 0; g < groups[s].size(); g++)
		{
			const aiMesh * am = scene->mMeshes[groups[s][g]];
			size_t base = conv.vertices.size();
			if(base + am->mNumVertices > 32000) { tooBig = true; break; }
			conv.vertices.resize(base + am->mNumVertices);
			vw.resize(base + am->mNumVertices);
			for(unsigned int v = 0; v < am->mNumVertices; v++)
			{
				AssimpConvertedVertex & d = conv.vertices[base + v];
				memset(&d, 0, sizeof(d));
				d.x = am->mVertices[v].x; d.y = am->mVertices[v].y; d.z = am->mVertices[v].z;
				if(am->HasNormals()) { d.nx = am->mNormals[v].x; d.ny = am->mNormals[v].y; d.nz = am->mNormals[v].z; }
				if(am->HasTextureCoords(0)) { d.tu[0] = am->mTextureCoords[0][v].x; d.tv[0] = am->mTextureCoords[0][v].y; }
			}
			for(unsigned int f = 0; f < am->mNumFaces; f++)
			{
				const aiFace & face = am->mFaces[f];
				if(face.mNumIndices != 3) continue;
				for(int k = 0; k < 3; k++) conv.indices.push_back((unsigned short)(base + face.mIndices[k]));
			}
			for(unsigned int b = 0; b < am->mNumBones; b++)
			{
				const aiBone * ab = am->mBones[b];
				int gameBone = mapBone(ab->mName);
				for(unsigned int w = 0; w < ab->mNumWeights; w++)
				{
					unsigned int vid = ab->mWeights[w].mVertexId;
					if(vid >= am->mNumVertices || ab->mWeights[w].mWeight <= 0.0f) continue;
					std::vector<std::pair<int, float> > & list = vw[base + vid];
					bool merged = false;
					for(size_t q = 0; q < list.size(); q++)
						if(list[q].first == gameBone) { list[q].second += ab->mWeights[w].mWeight; merged = true; break; }
					if(!merged) list.push_back(std::make_pair(gameBone, ab->mWeights[w].mWeight));
				}
			}
		}
		CString partLine;
		if(tooBig || conv.vertices.empty() || conv.indices.size() < 3)
		{
			partLine.Format("\n  part_%d: skipped (%s)", (int)s, tooBig ? "more than 32000 vertices" : "no triangles");
			details += partLine;
			continue;
		}

		// Retain the character's existing MapsModels texture references.
		CEXMeshObj * mesh = vo->m_mesh;
		conv.uvChannelCount = 1;
		float radius = 0.0f;
		for(size_t v = 0; v < conv.vertices.size(); v++)
		{
			float r = sqrtf(conv.vertices[v].x * conv.vertices[v].x + conv.vertices[v].y * conv.vertices[v].y + conv.vertices[v].z * conv.vertices[v].z);
			if(r > radius) radius = r;
		}
		conv.boundingRadius = radius;

		float sightRange = 0.0f;
		if(!BuildSkinnedMeshKeepOrder(mesh, conv, g_pd3dDevice) ||
		   mesh->m_abVertexArray.m_vertexCount != (WORD)conv.vertices.size())
		{
			partLine.Format("\n  part_%d: FAILED to build the mesh", (int)s);
			details += partLine;
			continue;
		}

		// new skin: per vertex, offsets in each bone's space (the game's own blend)
		const int vc = (int)conv.vertices.size();
		VertexAssignmentBlended * va = new VertexAssignmentBlended[vc];
		ABVERTEX0L * orig = new ABVERTEX0L[vc];
		for(int v = 0; v < vc; v++)
		{
			const AssimpConvertedVertex & cv = conv.vertices[v];
			orig[v].x = cv.x; orig[v].y = cv.y; orig[v].z = cv.z;
			orig[v].nx = cv.nx; orig[v].ny = cv.ny; orig[v].nz = cv.nz;
			std::vector<std::pair<int, float> > & list = vw[v];
			if(list.empty()) list.push_back(std::make_pair(0, 1.0f));
			float sum = 0.0f;
			for(size_t q = 0; q < list.size(); q++) sum += list[q].second;
			if(sum <= 0.0f) sum = 1.0f;
			int nb = (int)list.size();
			VertexAssignmentBlended & a = va[v];
			a.boneIndexesCount = (byte)nb;
			a.boneIndexes = new byte[nb];
			a.weightsToBone = new float[nb];
			a.offsetVects = new ABVERTEX0L[nb];
			a.vertexIndex = (short)v;
			a.vertexesCarried = NULL;
			a.carriedCount = 0;
			for(int q = 0; q < nb; q++)
			{
				int b = list[q].first;
				const D3DXMATRIX & inv = invBind[b];
				a.boneIndexes[q] = (byte)b;
				a.weightsToBone[q] = list[q].second / sum;
				a.offsetVects[q].x  = cv.x * inv._11 + cv.y * inv._21 + cv.z * inv._31 + inv._41;
				a.offsetVects[q].y  = cv.x * inv._12 + cv.y * inv._22 + cv.z * inv._32 + inv._42;
				a.offsetVects[q].z  = cv.x * inv._13 + cv.y * inv._23 + cv.z * inv._33 + inv._43;
				a.offsetVects[q].nx = cv.nx * inv._11 + cv.ny * inv._21 + cv.nz * inv._31;
				a.offsetVects[q].ny = cv.nx * inv._12 + cv.ny * inv._22 + cv.nz * inv._32;
				a.offsetVects[q].nz = cv.nx * inv._13 + cv.ny * inv._23 + cv.nz * inv._33;
			}
		}
		// the old skin arrays are left alone (another copy may still point at them);
		// the part now uses the blended skin built above
		vo->m_vertexAssignments = NULL;
		vo->m_blendedVertexAssignmentsWt = NULL;
		vo->m_blendedVertexAssignments = va;
		vo->m_originalArrayVector = orig;
		vo->m_assignmentCount = vc;
		vo->m_blendedType = 1;
		replaced++;
		partLine.Format("\n  part_%d: %d vertices, %d triangles", (int)s, vc, (int)(conv.indices.size() / 3));
		details += partLine;
	}

	if(replaced == 0)
	{
		report = "Nothing was replaced." + details;
		return FALSE;
	}
	report.Format("Replaced %d body part(s):", replaced);
	report += details;
	report += "\n\nExisting in-game texture names were kept.";
	return TRUE;
}


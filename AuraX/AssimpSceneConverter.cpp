// ---------------------------------------------------------------------------
// AssimpSceneConverter - see assimpsceneconverter.h for the overview.
// This file is deliberately MFC-free: it only talks to Assimp and STL so the
// conversion logic can be built and tested outside the Windows/MFC engine.
// ---------------------------------------------------------------------------

// Guard against <windows.h> min/max macros pulled in by /FIwinsock2.h.
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include "assimpsceneconverter.h"
#include "AssimpEmbeddedTextureUtils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>

namespace
{
// Keep in sync with the ASSIMP_BUILD_*_IMPORTER options in the root
// CMakeLists.txt and with the file-dialog filters in the editor dialogs.
// The engine's native serialized formats (.arw and the legacy .x archives)
// are intentionally NOT listed - those keep going through the native loader.
const char * const kSupportedExtensions[] =
{
    "fbx",   // Autodesk FBX
    "obj",   // Wavefront OBJ
    "gltf",  // glTF 2.0 (JSON)
    "glb",   // glTF 2.0 (binary container)
    "dae",   // Collada
    "stl",   // STL (ASCII + binary)
    "ply",   // Stanford PLY
    "3ds",   // 3ds Max (legacy conv3ds path is superseded by Assimp)
    "blend", // Blender
    "ase",   // 3ds Max ASE
    "lwo",   // LightWave
    "dxf",   // AutoCAD DXF
    NULL
};

struct SceneMeshInstance
{
    unsigned int meshIndex;
    aiMatrix4x4  worldTransform;
};

void TraverseNodeHierarchy(const aiScene * scene,
                           const aiNode * node,
                           const aiMatrix4x4 & parentTransform,
                           std::vector<SceneMeshInstance> & outInstances,
                           std::vector<bool> & referencedMeshes)
{
    if(!node)
        return;

    const aiMatrix4x4 worldTransform = parentTransform * node->mTransformation;
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        const unsigned int m = node->mMeshes[i];
        if(m < scene->mNumMeshes && scene->mMeshes[m])
        {
            SceneMeshInstance inst;
            inst.meshIndex      = m;
            inst.worldTransform = worldTransform;
            outInstances.push_back(inst);
            referencedMeshes[m] = true;
        }
    }

    for(unsigned int c = 0; c < node->mNumChildren; c++)
        TraverseNodeHierarchy(scene, node->mChildren[c], worldTransform, outInstances, referencedMeshes);
}

// Collects all triangle-bearing mesh instances from the scene graph, plus any
// unreferenced meshes in scene->mMeshes (e.g. synthetic unit-test scenes that
// populate mMeshes directly without wiring mRootNode).
void CollectSceneMeshInstances(const aiScene * scene,
                               std::vector<SceneMeshInstance> & outInstances)
{
    outInstances.clear();
    if(!scene || scene->mNumMeshes == 0)
        return;

    std::vector<bool> referenced(scene->mNumMeshes, false);
    if(scene->mRootNode)
    {
        aiMatrix4x4 identity;
        TraverseNodeHierarchy(scene, scene->mRootNode, identity, outInstances, referenced);
    }

    for(unsigned int m = 0; m < scene->mNumMeshes; m++)
    {
        if(!referenced[m] && scene->mMeshes[m])
        {
            SceneMeshInstance inst;
            inst.meshIndex      = m;
            inst.worldTransform = aiMatrix4x4();
            outInstances.push_back(inst);
        }
    }
}

void ComputeMeshBoundingRadius(AssimpConvertedMesh & mesh)
{
    if(mesh.vertices.empty())
    {
        mesh.boundingRadius = 0.0f;
        return;
    }

    float minX = mesh.vertices[0].x, maxX = minX;
    float minY = mesh.vertices[0].y, maxY = minY;
    float minZ = mesh.vertices[0].z, maxZ = minZ;
    for(size_t v = 1; v < mesh.vertices.size(); v++)
    {
        const AssimpConvertedVertex & vert = mesh.vertices[v];
        if(vert.x < minX) minX = vert.x;
        if(vert.x > maxX) maxX = vert.x;
        if(vert.y < minY) minY = vert.y;
        if(vert.y > maxY) maxY = vert.y;
        if(vert.z < minZ) minZ = vert.z;
        if(vert.z > maxZ) maxZ = vert.z;
    }
    const float cx = (minX + maxX) * 0.5f;
    const float cy = (minY + maxY) * 0.5f;
    const float cz = (minZ + maxZ) * 0.5f;

    float radius = 0.0f;
    for(size_t v = 0; v < mesh.vertices.size(); v++)
    {
        const AssimpConvertedVertex & vert = mesh.vertices[v];
        const float dx = vert.x - cx;
        const float dy = vert.y - cy;
        const float dz = vert.z - cz;
        const float dist = sqrtf(dx * dx + dy * dy + dz * dz);
        if(dist > radius)
            radius = dist;
    }
    mesh.boundingRadius = radius;
}

void ExtractMaterialIntoMesh(const aiScene * scene,
                             unsigned int materialIndex,
                             AssimpConvertedMesh & outMesh)
{
    if(!scene || !scene->mMaterials || materialIndex >= scene->mNumMaterials)
        return;

    const aiMaterial * mat = scene->mMaterials[materialIndex];
    if(!mat)
        return;

    // OBJ/FBX store the diffuse map under aiTextureType_DIFFUSE;
    // the glTF2 importer stores its base color map as BASE_COLOR.
    aiString texPath;
    if(mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS
       && texPath.length > 0)
    {
        outMesh.diffuseTexture = texPath.C_Str();
    }
    else if(mat->GetTexture(aiTextureType_BASE_COLOR, 0, &texPath) == AI_SUCCESS
            && texPath.length > 0)
    {
        outMesh.diffuseTexture = texPath.C_Str();
    }
    else if(mat->GetTexture(aiTextureType_EMISSIVE, 0, &texPath) == AI_SUCCESS
            && texPath.length > 0)
    {
        outMesh.diffuseTexture = texPath.C_Str();
    }
    else if(mat->GetTexture(aiTextureType_UNKNOWN, 0, &texPath) == AI_SUCCESS
            && texPath.length > 0)
    {
        outMesh.diffuseTexture = texPath.C_Str();
    }

    aiColor4D color;
    if(mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS
       || mat->Get(AI_MATKEY_BASE_COLOR, color) == AI_SUCCESS)
    {
        outMesh.diffuseColor[0] = color.r;
        outMesh.diffuseColor[1] = color.g;
        outMesh.diffuseColor[2] = color.b;
        outMesh.diffuseColor[3] = color.a;
    }
}

void AppendMeshInstanceGeometry(const aiMesh * mesh,
                                const aiMatrix4x4 & worldTransform,
                                AssimpConvertedMesh & dstMesh)
{
    if(!mesh)
        return;

    int uvChannels = 0;
    for(int c = 0; c < 8; c++)
        if(mesh->HasTextureCoords(c))
            uvChannels = c + 1;
    if(uvChannels > dstMesh.uvChannelCount)
        dstMesh.uvChannelCount = uvChannels;

    const bool isIdentity = worldTransform.IsIdentity();
    aiMatrix3x3 normalMatrix(worldTransform);
    bool flipWindingForNegativeScale = false;
    if(!isIdentity)
    {
        const float det = normalMatrix.Determinant();
        if(det < 0.0f)
            flipWindingForNegativeScale = true;
        if(fabsf(det) > 1e-12f)
        {
            normalMatrix.Inverse();
            normalMatrix.Transpose();
        }
    }

    const size_t vertexOffset = dstMesh.vertices.size();
    const bool hasNormals = mesh->HasNormals() ? true : false;
    for(unsigned int v = 0; v < mesh->mNumVertices; v++)
    {
        AssimpConvertedVertex vert;
        memset(&vert, 0, sizeof(vert));

        aiVector3D pos = mesh->mVertices[v];
        if(!isIdentity)
            pos = worldTransform * pos;
        vert.x = pos.x;
        vert.y = pos.y;
        vert.z = pos.z;

        if(hasNormals)
        {
            aiVector3D norm = mesh->mNormals[v];
            if(!isIdentity)
            {
                norm = normalMatrix * norm;
                const float len = sqrtf(norm.x * norm.x + norm.y * norm.y + norm.z * norm.z);
                if(len > 1e-8f)
                {
                    norm.x /= len;
                    norm.y /= len;
                    norm.z /= len;
                }
            }
            vert.nx = norm.x;
            vert.ny = norm.y;
            vert.nz = norm.z;
        }
        else
        {
            vert.nx = 0.0f;
            vert.ny = 0.0f;
            vert.nz = 1.0f;
        }

        for(int c = 0; c < 8; c++)
        {
            if(mesh->HasTextureCoords(c))
            {
                vert.tu[c] = mesh->mTextureCoords[c][v].x;
                vert.tv[c] = mesh->mTextureCoords[c][v].y;
            }
        }

        dstMesh.vertices.push_back(vert);
    }

    for(unsigned int f = 0; f < mesh->mNumFaces; f++)
    {
        const aiFace & face = mesh->mFaces[f];
        if(face.mNumIndices < 3)
            continue;
        if(face.mNumIndices == 3)
        {
            const unsigned short i0 = (unsigned short)(vertexOffset + face.mIndices[0]);
            const unsigned short i1 = (unsigned short)(vertexOffset + face.mIndices[1]);
            const unsigned short i2 = (unsigned short)(vertexOffset + face.mIndices[2]);
            if(flipWindingForNegativeScale)
            {
                dstMesh.indices.push_back(i2);
                dstMesh.indices.push_back(i1);
                dstMesh.indices.push_back(i0);
            }
            else
            {
                dstMesh.indices.push_back(i0);
                dstMesh.indices.push_back(i1);
                dstMesh.indices.push_back(i2);
            }
        }
        else
        {
            // Fan-triangulate anything that is not a triangle
            // (aiProcess_Triangulate normally guarantees triangles,
            // this is just insurance).
            for(unsigned int k = 1; k + 1 < face.mNumIndices; k++)
            {
                const unsigned short i0 = (unsigned short)(vertexOffset + face.mIndices[0]);
                const unsigned short i1 = (unsigned short)(vertexOffset + face.mIndices[k]);
                const unsigned short i2 = (unsigned short)(vertexOffset + face.mIndices[k + 1]);
                if(flipWindingForNegativeScale)
                {
                    dstMesh.indices.push_back(i2);
                    dstMesh.indices.push_back(i1);
                    dstMesh.indices.push_back(i0);
                }
                else
                {
                    dstMesh.indices.push_back(i0);
                    dstMesh.indices.push_back(i1);
                    dstMesh.indices.push_back(i2);
                }
            }
        }
    }
}

size_t CountMeshTriangleIndices(const aiMesh * mesh)
{
    if(!mesh || !mesh->mFaces)
        return 0;
    size_t count = 0;
    for(unsigned int f = 0; f < mesh->mNumFaces; f++)
    {
        const aiFace & face = mesh->mFaces[f];
        if(face.mNumIndices >= 3)
            count += (size_t)(face.mNumIndices - 2) * 3;
    }
    return count;
}

bool ValidateTriangleMeshData(const aiMesh * mesh, std::string & errorMessage)
{
    if(!mesh || (mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) == 0)
        return true;
    if(mesh->mNumVertices == 0 || !mesh->mVertices)
    {
        errorMessage = "triangle mesh is missing vertex positions";
        return false;
    }
    if(mesh->mNumFaces > 0 && !mesh->mFaces)
    {
        errorMessage = "triangle mesh is missing its face list";
        return false;
    }

    for(unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
    {
        const aiFace & face = mesh->mFaces[faceIndex];
        if(face.mNumIndices < 3)
            continue;
        if(!face.mIndices)
        {
            errorMessage = "triangle face is missing its index list";
            return false;
        }
        for(unsigned int index = 0; index < face.mNumIndices; ++index)
        {
            if(face.mIndices[index] >= mesh->mNumVertices)
            {
                errorMessage = "triangle face contains an out-of-range vertex index";
                return false;
            }
        }
    }
    return true;
}
} // namespace

unsigned int AssimpSceneConverter::GetEnginePostProcessFlags()
{
    // Engine conventions (Direct3D 9):
    //  MakeLeftHanded + FlipWindingOrder - right-handed sources (glTF, OBJ,
    //      Collada, ...) come out as left-handed geometry with clockwise
    //      faces, ready for the engine's CCW culling.
    //  FlipUVs - OpenGL-style bottom-left V origin becomes the D3D top-left
    //      origin the engine's textures use.
    //  Triangulate - engine meshes are triangle lists.
    //  JoinIdenticalVertices - weld duplicates so the 16-bit index buffer
    //      covers as much of the model as possible.
    //  GenSmoothNormals - fill in normals for formats that lack them
    //      (STL, OBJ without vn lines, ...).
    //  ImproveCacheLocality - reorder triangles for the vertex cache.
    //  RemoveRedundantMaterials - dedupe materials of multi-submesh models.
    return aiProcess_MakeLeftHanded
         | aiProcess_FlipUVs
         | aiProcess_FlipWindingOrder
         | aiProcess_Triangulate
         | aiProcess_JoinIdenticalVertices
         | aiProcess_GenSmoothNormals
         | aiProcess_ImproveCacheLocality
         | aiProcess_RemoveRedundantMaterials;
}

bool WriteRawBytesToFile(const std::string & path, const void * data, size_t byteCount)
{
    if(path.empty() || !data || byteCount == 0)
        return false;

    FILE * fp = fopen(path.c_str(), "wb");
    if(!fp)
        return false;

    bool ok = fwrite(data, 1, byteCount, fp) == byteCount;
    if(fclose(fp) != 0)
        ok = false;
    if(!ok)
        remove(path.c_str());
    return ok;
}

bool WriteUncompressedAiTextureToBmp(const std::string & path, const aiTexture * tex)
{
    if(path.empty() || !tex || !tex->pcData)
        return false;

    AssimpBmpLayout layout;
    if(!AssimpComputeBmpLayout(static_cast<std::uint32_t>(tex->mWidth),
                               static_cast<std::uint32_t>(tex->mHeight),
                               &layout))
        return false;

    const std::uint32_t width = static_cast<std::uint32_t>(tex->mWidth);
    const std::uint32_t fileSize = layout.fileSize;
    unsigned char bmpFileHeader[14] = {
        'B', 'M',
        (unsigned char)(fileSize & 0xFF),
        (unsigned char)((fileSize >> 8) & 0xFF),
        (unsigned char)((fileSize >> 16) & 0xFF),
        (unsigned char)((fileSize >> 24) & 0xFF),
        0, 0, 0, 0,
        54, 0, 0, 0
    };
    unsigned char bmpInfoHeader[40];
    memset(bmpInfoHeader, 0, sizeof(bmpInfoHeader));
    bmpInfoHeader[0] = 40;
    memcpy(&bmpInfoHeader[4], &width, sizeof(width));
    memcpy(&bmpInfoHeader[8], &layout.topDownHeight, sizeof(layout.topDownHeight));
    bmpInfoHeader[12] = 1;
    bmpInfoHeader[14] = 32;
    memcpy(&bmpInfoHeader[20], &layout.pixelBytes, sizeof(layout.pixelBytes));

    FILE * fp = fopen(path.c_str(), "wb");
    if(!fp)
        return false;

    bool ok = fwrite(bmpFileHeader, 1, sizeof(bmpFileHeader), fp) == sizeof(bmpFileHeader);
    if(ok)
        ok = fwrite(bmpInfoHeader, 1, sizeof(bmpInfoHeader), fp) == sizeof(bmpInfoHeader);

    // Batch the channel-swizzled pixels instead of making one fwrite call for
    // every texel. Keep the temporary buffer fixed-size for large textures.
    unsigned char bgraChunk[256 * 4];
    for(std::uint64_t offset = 0; ok && offset < layout.pixelCount;)
    {
        const std::uint64_t remaining = layout.pixelCount - offset;
        const size_t pixelsInChunk = static_cast<size_t>(remaining > 256ULL ? 256ULL : remaining);
        for(size_t pixel = 0; pixel < pixelsInChunk; ++pixel)
        {
            const aiTexel & src = tex->pcData[static_cast<unsigned int>(offset + pixel)];
            const size_t dst = pixel * 4;
            bgraChunk[dst + 0] = src.b;
            bgraChunk[dst + 1] = src.g;
            bgraChunk[dst + 2] = src.r;
            bgraChunk[dst + 3] = src.a;
        }

        const size_t bytesInChunk = pixelsInChunk * 4;
        if(fwrite(bgraChunk, 1, bytesInChunk, fp) != bytesInChunk)
            ok = false;
        offset += pixelsInChunk;
    }

    if(fclose(fp) != 0)
        ok = false;
    if(!ok)
        remove(path.c_str());
    return ok;
}

void ResolveEmbeddedTextureForMesh(const aiScene * scene,
                                   const std::string & modelFilePath,
                                   size_t subMeshIdx,
                                   AssimpConvertedMesh & mesh)
{
    if(!scene || scene->mNumTextures == 0 || !scene->mTextures)
        return;

    const aiTexture * embTex = NULL;
    if(!mesh.diffuseTexture.empty())
        embTex = scene->GetEmbeddedTexture(mesh.diffuseTexture.c_str());
    if(!embTex && mesh.diffuseTexture.empty() && scene->mNumTextures == 1)
        embTex = scene->mTextures[0];
    if(!embTex || !embTex->pcData || embTex->mWidth == 0)
        return;

    // Extract directory and base stem from modelFilePath
    std::string dirPath;
    std::string stem = modelFilePath;
    size_t lastSlash = modelFilePath.find_last_of("\\/");
    if(lastSlash != std::string::npos)
    {
        dirPath = modelFilePath.substr(0, lastSlash + 1);
        stem = modelFilePath.substr(lastSlash + 1);
    }
    size_t lastDot = stem.find_last_of('.');
    if(lastDot != std::string::npos)
        stem = stem.substr(0, lastDot);
    if(stem.empty())
        stem = "model";

    char idxBuf[32];
    snprintf(idxBuf, sizeof(idxBuf), "_tex%u", (unsigned int)subMeshIdx);

    if(embTex->mHeight == 0)
    {
        const unsigned char * bytes = reinterpret_cast<const unsigned char *>(embTex->pcData);
        std::string ext = "png";
        if(embTex->mWidth >= 4 && bytes[0] == 0x89 && bytes[1] == 'P' && bytes[2] == 'N' && bytes[3] == 'G')
            ext = "png";
        else if(embTex->mWidth >= 3 && bytes[0] == 0xFF && bytes[1] == 0xD8 && bytes[2] == 0xFF)
            ext = "jpg";
        else if(embTex->mWidth >= 4 && bytes[0] == 'D' && bytes[1] == 'D' && bytes[2] == 'S' && bytes[3] == ' ')
            ext = "dds";
        else if(embTex->mWidth >= 2 && bytes[0] == 'B' && bytes[1] == 'M')
            ext = "bmp";
        else if(embTex->achFormatHint[0] != '\0')
            ext = embTex->achFormatHint;

        std::string outName = stem + idxBuf + "." + ext;
        std::string outFullPath = dirPath + outName;
        const bool copiedBesideModel = WriteRawBytesToFile(outFullPath, embTex->pcData, embTex->mWidth);
        const bool copiedToEngineModels = WriteRawBytesToFile(std::string("MapsModels\\") + outName,
                                                                embTex->pcData,
                                                                embTex->mWidth);
        if(copiedToEngineModels)
            mesh.diffuseTexture = outName; // resolved relative to GL_pathMapModelsRef
        else if(copiedBesideModel)
            mesh.diffuseTexture = outFullPath;
        else
            mesh.diffuseTexture.clear();
    }
    else
    {
        std::string outName = stem + idxBuf + ".bmp";
        std::string outFullPath = dirPath + outName;
        const bool copiedBesideModel = WriteUncompressedAiTextureToBmp(outFullPath, embTex);
        const bool copiedToEngineModels = WriteUncompressedAiTextureToBmp(std::string("MapsModels\\") + outName,
                                                                            embTex);
        if(copiedToEngineModels)
            mesh.diffuseTexture = outName; // resolved relative to GL_pathMapModelsRef
        else if(copiedBesideModel)
            mesh.diffuseTexture = outFullPath;
        else
            mesh.diffuseTexture.clear();
    }
}

bool AssimpSceneConverter::ConvertFile(const std::string & filePath,
                                       AssimpConvertedMesh & outMesh,
                                       std::string & errorMessage)
{
    errorMessage.clear();
    outMesh = AssimpConvertedMesh();

    Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile(filePath, GetEnginePostProcessFlags());
    if(!scene)
    {
        errorMessage = importer.GetErrorString();
        if(errorMessage.empty())
            errorMessage = "unknown Assimp error";
        return false;
    }

    if(!ConvertScene(scene, outMesh, errorMessage))
        return false;

    ResolveEmbeddedTextureForMesh(scene, filePath, 0, outMesh);
    return true;
}

bool AssimpSceneConverter::ConvertScene(const aiScene * scene,
                                        AssimpConvertedMesh & outMesh,
                                        std::string & errorMessage)
{
    errorMessage.clear();
    outMesh = AssimpConvertedMesh();

    if(!scene)
    {
        errorMessage = "null scene";
        return false;
    }
    if(!scene->HasMeshes() || !scene->mMeshes)
    {
        errorMessage = "file contains no meshes";
        return false;
    }

    std::vector<SceneMeshInstance> instances;
    CollectSceneMeshInstances(scene, instances);

    // Count the triangle geometry that will be merged (post-processing,
    // including JoinIdenticalVertices, has already run at this point).
    size_t totalVertices = 0;
    size_t totalIndices  = 0;
    unsigned int dominantMesh = 0;
    size_t dominantFaces = 0;
    for(size_t i = 0; i < instances.size(); i++)
    {
        const unsigned int m = instances[i].meshIndex;
        const aiMesh * mesh = scene->mMeshes[m];
        if(!mesh)
            continue;
        // mPrimitiveTypes is a bitmask (a mesh may also carry the
        // NGONEncodingFlag); keep any mesh that contains triangles.
        if((mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) == 0)
            continue;
        if(!ValidateTriangleMeshData(mesh, errorMessage))
            return false;
        // Enforce the engine's 16-bit limits while accumulating. Checking
        // before addition prevents size_t wraparound on the Win32 build when
        // a scene contains many repeated mesh instances.
        if(mesh->mNumVertices > ENGINE_MAX_VERTICES - totalVertices)
        {
            char buf[320];
            snprintf(buf, sizeof(buf),
                "model exceeds the engine's %u-vertex 16-bit limit after welding; "
                "reduce the vertex count (decimate or split the model) and import again.",
                (unsigned int)ENGINE_MAX_VERTICES);
            errorMessage = buf;
            return false;
        }
        const size_t meshIndices = CountMeshTriangleIndices(mesh);
        if(meshIndices > ENGINE_MAX_INDICES - totalIndices)
        {
            char buf[320];
            snprintf(buf, sizeof(buf),
                "model exceeds the engine's %u-index 16-bit limit; reduce the "
                "triangle count (decimate or split the model) and import again.",
                (unsigned int)ENGINE_MAX_INDICES);
            errorMessage = buf;
            return false;
        }
        totalVertices += mesh->mNumVertices;
        totalIndices  += meshIndices;
        if((size_t)mesh->mNumFaces > dominantFaces)
        {
            dominantFaces = mesh->mNumFaces;
            dominantMesh  = m;
        }
    }
    if(totalVertices == 0 || totalIndices == 0)
    {
        errorMessage = "file contains no triangle geometry";
        return false;
    }
    if(totalVertices > ENGINE_MAX_VERTICES)
    {
        char buf[320];
        snprintf(buf, sizeof(buf),
            "model has %u vertices after welding; the engine uses 16-bit "
            "indices (max %u). Reduce the vertex count (decimate or split "
            "the model) and import again.",
            (unsigned int)totalVertices, (unsigned int)ENGINE_MAX_VERTICES);
        errorMessage = buf;
        return false;
    }
    if(totalIndices > ENGINE_MAX_INDICES)
    {
        char buf[320];
        snprintf(buf, sizeof(buf),
            "model has %u triangles (%u indices); the engine's index buffer "
            "is 16-bit (max %u indices, about %u triangles). Reduce the "
            "triangle count (decimate or split the model) and import again.",
            (unsigned int)(totalIndices / 3), (unsigned int)totalIndices,
            (unsigned int)ENGINE_MAX_INDICES,
            (unsigned int)(ENGINE_MAX_INDICES / 3));
        errorMessage = buf;
        return false;
    }

    outMesh.vertices.reserve(totalVertices);
    outMesh.indices.reserve(totalIndices);

    // Merge every triangle sub-mesh into one engine mesh.
    for(size_t i = 0; i < instances.size(); i++)
    {
        const aiMesh * mesh = scene->mMeshes[instances[i].meshIndex];
        if(!mesh)
            continue;
        if((mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) == 0)
            continue;

        AppendMeshInstanceGeometry(mesh, instances[i].worldTransform, outMesh);
    }

    // Bounding sphere around the mesh center; drives the engine's sight range.
    ComputeMeshBoundingRadius(outMesh);

    // Material of the dominant (largest) sub-mesh -> texture + color.
    const aiMesh * domMesh = scene->mMeshes[dominantMesh];
    if(domMesh)
        ExtractMaterialIntoMesh(scene, domMesh->mMaterialIndex, outMesh);

    return true;
}

bool AssimpSceneConverter::ConvertFileToSubMeshes(const std::string & filePath,
                                                  std::vector<AssimpConvertedMesh> & outSubMeshes,
                                                  std::string & errorMessage)
{
    errorMessage.clear();
    outSubMeshes.clear();

    Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile(filePath, GetEnginePostProcessFlags());
    if(!scene)
    {
        errorMessage = importer.GetErrorString();
        if(errorMessage.empty())
            errorMessage = "unknown Assimp error";
        return false;
    }

    if(!ConvertSceneToSubMeshes(scene, outSubMeshes, errorMessage))
        return false;

    for(size_t i = 0; i < outSubMeshes.size(); ++i)
        ResolveEmbeddedTextureForMesh(scene, filePath, i, outSubMeshes[i]);

    return true;
}

bool AssimpSceneConverter::ConvertSceneToSubMeshes(const aiScene * scene,
                                                   std::vector<AssimpConvertedMesh> & outSubMeshes,
                                                   std::string & errorMessage)
{
    errorMessage.clear();
    outSubMeshes.clear();

    if(!scene)
    {
        errorMessage = "null scene";
        return false;
    }
    if(!scene->HasMeshes() || !scene->mMeshes)
    {
        errorMessage = "file contains no meshes";
        return false;
    }

    std::vector<SceneMeshInstance> instances;
    CollectSceneMeshInstances(scene, instances);

    // Discover unique material indices in order of first appearance.
    std::vector<unsigned int> materialOrder;
    for(size_t i = 0; i < instances.size(); i++)
    {
        const aiMesh * mesh = scene->mMeshes[instances[i].meshIndex];
        if(!mesh || (mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) == 0)
            continue;
        if(!ValidateTriangleMeshData(mesh, errorMessage))
        {
            outSubMeshes.clear();
            return false;
        }
        if(mesh->mNumVertices == 0 || CountMeshTriangleIndices(mesh) == 0)
            continue;

        const unsigned int matIdx = mesh->mMaterialIndex;
        bool seen = false;
        for(size_t k = 0; k < materialOrder.size(); k++)
        {
            if(materialOrder[k] == matIdx)
            {
                seen = true;
                break;
            }
        }
        if(!seen)
            materialOrder.push_back(matIdx);
    }

    if(materialOrder.empty())
    {
        errorMessage = "file contains no triangle geometry";
        return false;
    }

    for(size_t mIdx = 0; mIdx < materialOrder.size(); mIdx++)
    {
        const unsigned int targetMaterial = materialOrder[mIdx];
        AssimpConvertedMesh currentChunk;
        ExtractMaterialIntoMesh(scene, targetMaterial, currentChunk);

        for(size_t i = 0; i < instances.size(); i++)
        {
            const aiMesh * mesh = scene->mMeshes[instances[i].meshIndex];
            if(!mesh || (mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) == 0)
                continue;
            if(mesh->mMaterialIndex != targetMaterial)
                continue;

            const size_t meshVerts   = mesh->mNumVertices;
            const size_t meshIndices = CountMeshTriangleIndices(mesh);
            if(meshVerts == 0 || meshIndices == 0)
                continue;

            if(meshVerts > ENGINE_MAX_VERTICES || meshIndices > ENGINE_MAX_INDICES)
            {
                char buf[320];
                snprintf(buf, sizeof(buf),
                    "sub-mesh has %u vertices and %u indices; a single engine "
                    "sub-mesh uses 16-bit indices (max %u vertices / %u indices).",
                    (unsigned int)meshVerts, (unsigned int)meshIndices,
                    (unsigned int)ENGINE_MAX_VERTICES,
                    (unsigned int)ENGINE_MAX_INDICES);
                errorMessage = buf;
                outSubMeshes.clear();
                return false;
            }

            // If adding this mesh instance would overflow 16-bit vertex or
            // index counts on the current chunk, flush the current chunk first.
            if(!currentChunk.vertices.empty() &&
               (currentChunk.vertices.size() + meshVerts > ENGINE_MAX_VERTICES ||
                currentChunk.indices.size() + meshIndices > ENGINE_MAX_INDICES))
            {
                ComputeMeshBoundingRadius(currentChunk);
                outSubMeshes.push_back(currentChunk);
                currentChunk = AssimpConvertedMesh();
                ExtractMaterialIntoMesh(scene, targetMaterial, currentChunk);
            }

            AppendMeshInstanceGeometry(mesh, instances[i].worldTransform, currentChunk);
        }

        if(!currentChunk.vertices.empty() && !currentChunk.indices.empty())
        {
            ComputeMeshBoundingRadius(currentChunk);
            outSubMeshes.push_back(currentChunk);
        }
    }

    if(outSubMeshes.empty())
    {
        errorMessage = "file contains no triangle geometry";
        return false;
    }

    return true;
}

bool AssimpSceneConverter::ExportToObjFile(const std::string & objFilePath,
                                           const AssimpConvertedMesh & mesh,
                                           std::string & errorMessage)
{
    std::vector<AssimpConvertedMesh> list;
    list.push_back(mesh);
    return ExportToObjFile(objFilePath, list, errorMessage);
}

bool AssimpSceneConverter::ExportToObjFile(const std::string & objFilePath,
                                           const std::vector<AssimpConvertedMesh> & subMeshes,
                                           std::string & errorMessage)
{
    errorMessage.clear();
    if(objFilePath.empty())
    {
        errorMessage = "empty output path";
        return false;
    }
    if(subMeshes.empty())
    {
        errorMessage = "no meshes to export";
        return false;
    }

    size_t totalVerts   = 0;
    size_t totalIndices = 0;
    const size_t maxObjVertices = static_cast<size_t>(std::numeric_limits<unsigned int>::max());
    const size_t maxSize = std::numeric_limits<size_t>::max();
    for(size_t m = 0; m < subMeshes.size(); m++)
    {
        const AssimpConvertedMesh & sub = subMeshes[m];
        if(sub.vertices.empty() && sub.indices.empty())
            continue;
        if(sub.vertices.empty() || sub.indices.size() < 3 || sub.indices.size() % 3 != 0)
        {
            errorMessage = "sub-mesh " + std::to_string(m) + " has an incomplete triangle list";
            return false;
        }
        if(sub.vertices.size() > ENGINE_MAX_VERTICES || sub.indices.size() > ENGINE_MAX_INDICES)
        {
            errorMessage = "sub-mesh " + std::to_string(m) + " exceeds the engine's 16-bit mesh limits";
            return false;
        }
        for(size_t idx = 0; idx < sub.indices.size(); idx++)
        {
            if(static_cast<size_t>(sub.indices[idx]) >= sub.vertices.size())
            {
                errorMessage = "sub-mesh " + std::to_string(m) + " contains an out-of-range vertex index";
                return false;
            }
        }
        if(sub.vertices.size() > maxObjVertices - totalVerts ||
           sub.indices.size() > maxSize - totalIndices)
        {
            errorMessage = "combined mesh is too large to export safely";
            return false;
        }
        totalVerts   += sub.vertices.size();
        totalIndices += sub.indices.size();
    }
    if(totalVerts == 0 || totalIndices < 3)
    {
        errorMessage = "mesh has no triangle geometry to export";
        return false;
    }

    // Derive companion .mtl path and bare mtl filename.
    std::string mtlFilePath = objFilePath;
    const size_t lastSlash = mtlFilePath.find_last_of("/\\");
    const size_t lastDot   = mtlFilePath.find_last_of('.');
    if(lastDot != std::string::npos && (lastSlash == std::string::npos || lastDot > lastSlash))
        mtlFilePath = mtlFilePath.substr(0, lastDot) + ".mtl";
    else
        mtlFilePath += ".mtl";

    std::string mtlFileName = mtlFilePath;
    if(lastSlash != std::string::npos)
        mtlFileName = mtlFilePath.substr(lastSlash + 1);

    FILE * objFp = fopen(objFilePath.c_str(), "wb");
    if(!objFp)
    {
        errorMessage = "could not open .obj file for writing: " + objFilePath;
        return false;
    }

    FILE * mtlFp = fopen(mtlFilePath.c_str(), "wb");
    if(!mtlFp)
    {
        fclose(objFp);
        remove(objFilePath.c_str()); // don't leave an empty half-export behind
        errorMessage = "could not open .mtl file for writing: " + mtlFilePath;
        return false;
    }

    fprintf(objFp, "# Exported by EngineX (Endless Ages)\n");
    fprintf(objFp, "mtllib %s\n\n", mtlFileName.c_str());
    fprintf(mtlFp, "# Exported by EngineX (Endless Ages)\n\n");

    size_t globalVertexOffset = 0;
    for(size_t m = 0; m < subMeshes.size(); m++)
    {
        const AssimpConvertedMesh & sub = subMeshes[m];
        if(sub.vertices.empty() || sub.indices.size() < 3)
            continue;

        char matName[64];
        snprintf(matName, sizeof(matName), "material_%u", (unsigned int)m);

        // Write material definition to .mtl.
        fprintf(mtlFp, "newmtl %s\n", matName);
        fprintf(mtlFp, "Ka %.6f %.6f %.6f\n",
                sub.diffuseColor[0], sub.diffuseColor[1], sub.diffuseColor[2]);
        fprintf(mtlFp, "Kd %.6f %.6f %.6f\n",
                sub.diffuseColor[0], sub.diffuseColor[1], sub.diffuseColor[2]);
        fprintf(mtlFp, "Ks 0.000000 0.000000 0.000000\n");
        fprintf(mtlFp, "d %.6f\n", sub.diffuseColor[3]);
        if(!sub.diffuseTexture.empty() && sub.diffuseTexture != "NONE")
        {
            std::string texName = sub.diffuseTexture;
            const size_t slashPos = texName.find_last_of("/\\");
            if(slashPos != std::string::npos)
                texName = texName.substr(slashPos + 1);
            fprintf(mtlFp, "map_Kd %s\n", texName.c_str());
        }
        fprintf(mtlFp, "\n");

        // Write object group to .obj.
        // Inverse of MakeLeftHanded | FlipUVs | FlipWindingOrder:
        //   position: (x, y, -z)
        //   texcoord: (tu[0], 1.0f - tv[0])
        //   normal:   (nx, ny, -nz)
        //   face:     (i2, i1, i0)
        fprintf(objFp, "o mesh_%u\n", (unsigned int)m);
        for(size_t v = 0; v < sub.vertices.size(); v++)
        {
            const AssimpConvertedVertex & vert = sub.vertices[v];
            fprintf(objFp, "v %.6f %.6f %.6f\n", vert.x, vert.y, -vert.z);
        }
        for(size_t v = 0; v < sub.vertices.size(); v++)
        {
            const AssimpConvertedVertex & vert = sub.vertices[v];
            fprintf(objFp, "vt %.6f %.6f\n", vert.tu[0], 1.0f - vert.tv[0]);
        }
        for(size_t v = 0; v < sub.vertices.size(); v++)
        {
            const AssimpConvertedVertex & vert = sub.vertices[v];
            fprintf(objFp, "vn %.6f %.6f %.6f\n", vert.nx, vert.ny, -vert.nz);
        }

        fprintf(objFp, "usemtl %s\n", matName);
        for(size_t idx = 0; idx + 2 < sub.indices.size(); idx += 3)
        {
            const unsigned int i0 = (unsigned int)(globalVertexOffset + sub.indices[idx + 0] + 1);
            const unsigned int i1 = (unsigned int)(globalVertexOffset + sub.indices[idx + 1] + 1);
            const unsigned int i2 = (unsigned int)(globalVertexOffset + sub.indices[idx + 2] + 1);
            fprintf(objFp, "f %u/%u/%u %u/%u/%u %u/%u/%u\n",
                    i2, i2, i2,
                    i1, i1, i1,
                    i0, i0, i0);
        }
        fprintf(objFp, "\n");

        globalVertexOffset += sub.vertices.size();
    }

    const bool streamError = ferror(objFp) != 0 || ferror(mtlFp) != 0;
    const int mtlCloseResult = fclose(mtlFp);
    const int objCloseResult = fclose(objFp);
    if(streamError || mtlCloseResult != 0 || objCloseResult != 0)
    {
        remove(objFilePath.c_str());
        remove(mtlFilePath.c_str());
        errorMessage = "I/O error while writing the OBJ/MTL files; partial output was removed";
        return false;
    }
    return true;
}

const char * const * AssimpSceneConverter::GetSupportedExtensions()
{
    return kSupportedExtensions;
}

bool AssimpSceneConverter::IsSupportedExtension(const std::string & extensionLowerNoDot)
{
    for(const char * const * ext = kSupportedExtensions; *ext != NULL; ext++)
    {
        if(extensionLowerNoDot == *ext)
            return true;
    }
    return false;
}

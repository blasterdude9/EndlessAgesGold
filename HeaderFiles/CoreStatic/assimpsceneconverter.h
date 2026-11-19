#ifndef ASSIMPSCENECONVERTER_H
#define ASSIMPSCENECONVERTER_H

// ---------------------------------------------------------------------------
// AssimpSceneConverter
//
// MFC-free conversion layer between Assimp (Open Asset Import Library) and
// the engine's mesh data. Kept free of MFC/D3D types on purpose so the exact
// code that runs inside EngineX can be compiled and unit-tested standalone.
//
// AssimpSceneConverter::ConvertFile() reads a modern model file (FBX, OBJ,
// glTF/glb, Collada, STL, PLY, 3DS, ...) and produces one merged triangle
// mesh in the engine's conventions:
//   - left-handed geometry (Direct3D 9 world)
//   - top-left UV origin (D3D textures; source V is flipped)
//   - clockwise face order (D3D CCW culling)
//   - smooth normals generated when the source has none
//   - duplicate vertices welded so 16-bit indices go as far as possible
//   - 8 UV channels max (ABVERTEX tx0..tx7)
//   - the dominant material's diffuse texture path + color
//
// The engine indexes meshes with 16-bit WORDs, so a model that still has more
// than 65535 vertices after welding is rejected with an explanatory error.
// ---------------------------------------------------------------------------

// Guard against <windows.h> min/max macros pulled in by /FIwinsock2.h.
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <string>
#include <vector>

struct aiScene;

// One engine-ready vertex: position, normal and up to 8 UV channels.
struct AssimpConvertedVertex
{
    float x, y, z;
    float nx, ny, nz;
    float tu[8];
    float tv[8];
};

// Plain-old-data result of converting an aiScene.
struct AssimpConvertedMesh
{
    std::vector<AssimpConvertedVertex> vertices;     // merged, engine-ready
    std::vector<unsigned short>        indices;      // triangle list
    std::string                        diffuseTexture; // "" when untextured
    float                              diffuseColor[4]; // RGBA, 0..1
    float                              boundingRadius;  // around mesh center
    int                                uvChannelCount;  // 0..8

    AssimpConvertedMesh()
    {
        diffuseColor[0] = 1.0f;
        diffuseColor[1] = 1.0f;
        diffuseColor[2] = 1.0f;
        diffuseColor[3] = 1.0f;
        boundingRadius  = 0.0f;
        uvChannelCount  = 0;
    }
};

class AssimpSceneConverter
{
public:
    // The engine's CAdvancedVertexObj stores WORD (16-bit) vertex indices and
    // a WORD (16-bit) index count, so both the vertex count and the index
    // count of an imported model must stay below 65536.
    static constexpr unsigned int ENGINE_MAX_VERTICES = 65535;
    static constexpr unsigned int ENGINE_MAX_INDICES  = 65535;

    // Assimp post-process flags matching the engine's conventions.
    static unsigned int GetEnginePostProcessFlags();

    // Load a file with Assimp and convert the resulting scene.
    // Returns false on failure and fills errorMessage.
    static bool ConvertFile(const std::string & filePath,
                            AssimpConvertedMesh & outMesh,
                            std::string & errorMessage);

    // Convert an already-loaded scene (post-processing must have run).
    // Returns false on failure and fills errorMessage.
    static bool ConvertScene(const aiScene * scene,
                             AssimpConvertedMesh & outMesh,
                             std::string & errorMessage);

    // Multi-material / auto-chunked import: splits a model by material index
    // (and chunks any material group exceeding ENGINE_MAX_VERTICES or
    // ENGINE_MAX_INDICES into valid 16-bit sub-meshes). Used when importing
    // into a CEXMeshObjList (Reference Library / Visual Manager) so multi-
    // material FBX/OBJ/glTF assets keep each material and texture separate.
    static bool ConvertFileToSubMeshes(const std::string & filePath,
                                       std::vector<AssimpConvertedMesh> & outSubMeshes,
                                       std::string & errorMessage);

    static bool ConvertSceneToSubMeshes(const aiScene * scene,
                                        std::vector<AssimpConvertedMesh> & outSubMeshes,
                                        std::string & errorMessage);

    // Export one or more engine-space meshes (left-handed D3D9, top-left UV,
    // CW faces) to a standard right-handed Wavefront .obj + companion .mtl
    // file suitable for Blender/Maya/3ds Max round-tripping.
    static bool ExportToObjFile(const std::string & objFilePath,
                                const std::vector<AssimpConvertedMesh> & subMeshes,
                                std::string & errorMessage);

    static bool ExportToObjFile(const std::string & objFilePath,
                                const AssimpConvertedMesh & mesh,
                                std::string & errorMessage);

    // NULL-terminated list of supported extensions (lowercase, no dot).
    // Must match the importers enabled in the root CMakeLists.txt.
    static const char * const * GetSupportedExtensions();

    // True when extensionLowerNoDot ("fbx", "obj", "gltf", ...) is supported.
    static bool IsSupportedExtension(const std::string & extensionLowerNoDot);
};

#endif

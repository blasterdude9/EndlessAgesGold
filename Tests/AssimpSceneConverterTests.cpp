#include "assimpsceneconverter.h"

#include <assimp/scene.h>

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

namespace
{
int failures = 0;

void Check(bool passed, const char * description)
{
    if(passed)
        return;

    std::fprintf(stderr, "FAILED: %s\n", description);
    ++failures;
}

std::string ReadFile(const char * path)
{
    std::ifstream file(path, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

bool FileExists(const char * path)
{
    std::ifstream file(path, std::ios::binary);
    return file.good();
}

AssimpConvertedMesh MakeTriangle()
{
    AssimpConvertedMesh mesh;
    mesh.vertices.resize(3);
    mesh.vertices[0].x = 1.0f; mesh.vertices[0].y = 2.0f; mesh.vertices[0].z = 3.0f;
    mesh.vertices[0].nx = 0.0f; mesh.vertices[0].ny = 0.0f; mesh.vertices[0].nz = 1.0f;
    mesh.vertices[0].tu[0] = 0.25f; mesh.vertices[0].tv[0] = 0.25f;
    mesh.vertices[1].x = 4.0f; mesh.vertices[1].y = 5.0f; mesh.vertices[1].z = 6.0f;
    mesh.vertices[1].nx = 0.0f; mesh.vertices[1].ny = 0.0f; mesh.vertices[1].nz = 1.0f;
    mesh.vertices[1].tu[0] = 0.50f; mesh.vertices[1].tv[0] = 0.50f;
    mesh.vertices[2].x = 7.0f; mesh.vertices[2].y = 8.0f; mesh.vertices[2].z = 9.0f;
    mesh.vertices[2].nx = 0.0f; mesh.vertices[2].ny = 0.0f; mesh.vertices[2].nz = 1.0f;
    mesh.vertices[2].tu[0] = 0.75f; mesh.vertices[2].tv[0] = 0.75f;
    mesh.indices.push_back(0);
    mesh.indices.push_back(1);
    mesh.indices.push_back(2);
    mesh.diffuseTexture = "frog.png";
    return mesh;
}

std::unique_ptr<aiScene> MakeTransformedTriangleScene()
{
    std::unique_ptr<aiScene> scene(new aiScene());
    scene->mNumMeshes = 1;
    scene->mMeshes = new aiMesh *[1];
    scene->mMeshes[0] = new aiMesh();

    aiMesh * mesh = scene->mMeshes[0];
    mesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE;
    mesh->mNumVertices = 3;
    mesh->mVertices = new aiVector3D[3];
    mesh->mVertices[0] = aiVector3D(1.0f, 0.0f, 0.0f);
    mesh->mVertices[1] = aiVector3D(0.0f, 1.0f, 0.0f);
    mesh->mVertices[2] = aiVector3D(0.0f, 0.0f, 1.0f);
    mesh->mNormals = new aiVector3D[3];
    for(int i = 0; i < 3; ++i)
        mesh->mNormals[i] = aiVector3D(1.0f, 1.0f, 0.0f);
    mesh->mNumFaces = 1;
    mesh->mFaces = new aiFace[1];
    mesh->mFaces[0].mNumIndices = 3;
    mesh->mFaces[0].mIndices = new unsigned int[3];
    mesh->mFaces[0].mIndices[0] = 0;
    mesh->mFaces[0].mIndices[1] = 1;
    mesh->mFaces[0].mIndices[2] = 2;

    scene->mRootNode = new aiNode();
    scene->mRootNode->mTransformation.a4 = 1.0f;
    scene->mRootNode->mTransformation.b4 = 2.0f;
    scene->mRootNode->mTransformation.c4 = 3.0f;
    scene->mRootNode->mNumChildren = 1;
    scene->mRootNode->mChildren = new aiNode *[1];
    scene->mRootNode->mChildren[0] = new aiNode();
    aiNode * meshNode = scene->mRootNode->mChildren[0];
    meshNode->mParent = scene->mRootNode;
    meshNode->mTransformation.a1 = -2.0f;
    meshNode->mTransformation.b2 = 3.0f;
    meshNode->mTransformation.a4 = 5.0f;
    meshNode->mNumMeshes = 1;
    meshNode->mMeshes = new unsigned int[1];
    meshNode->mMeshes[0] = 0;
    return scene;
}

void CheckNear(float actual, float expected, const char * description)
{
    Check(std::fabs(actual - expected) < 0.0001f, description);
}

void TestSceneHierarchyTransform()
{
    std::unique_ptr<aiScene> scene = MakeTransformedTriangleScene();
    AssimpConvertedMesh converted;
    std::string error = "stale error";
    Check(AssimpSceneConverter::ConvertScene(scene.get(), converted, error),
          "scene converter accepts a valid hierarchical triangle scene");
    Check(error.empty(), "successful scene conversion clears an old error message");
    Check(converted.vertices.size() == 3, "hierarchical scene preserves all vertices");
    Check(converted.indices.size() == 3, "hierarchical scene preserves the triangle");
    if(converted.vertices.size() == 3 && converted.indices.size() == 3)
    {
        CheckNear(converted.vertices[0].x, 4.0f, "parent and child X transforms compose");
        CheckNear(converted.vertices[0].y, 2.0f, "parent Y translation is applied");
        CheckNear(converted.vertices[0].z, 3.0f, "parent Z translation is applied");
        CheckNear(converted.vertices[1].x, 6.0f, "reflected child transform applies to second vertex");
        CheckNear(converted.vertices[1].y, 5.0f, "non-uniform child scale applies to second vertex");
        CheckNear(converted.vertices[0].nx, -0.8320503f, "normal X uses the inverse-transpose transform");
        CheckNear(converted.vertices[0].ny, 0.5547002f, "normal is normalized after inverse-transpose transform");
        Check(converted.indices[0] == 2 && converted.indices[1] == 1 && converted.indices[2] == 0,
              "negative-determinant node transform reverses triangle winding");
    }
}

void TestMalformedSceneData()
{
    std::unique_ptr<aiScene> scene = MakeTransformedTriangleScene();
    scene->mMeshes[0]->mFaces[0].mIndices[2] = 3;

    AssimpConvertedMesh converted;
    std::string error;
    Check(!AssimpSceneConverter::ConvertScene(scene.get(), converted, error),
          "single-mesh conversion rejects out-of-range source indices");
    Check(!error.empty(), "invalid source indices produce an error message");

    std::vector<AssimpConvertedMesh> subMeshes(1, MakeTriangle());
    error.clear();
    Check(!AssimpSceneConverter::ConvertSceneToSubMeshes(scene.get(), subMeshes, error),
          "multi-mesh conversion rejects out-of-range source indices");
    Check(subMeshes.empty(), "failed multi-mesh conversion discards partial output");

    std::unique_ptr<aiScene> missingPositions = MakeTransformedTriangleScene();
    delete [] missingPositions->mMeshes[0]->mVertices;
    missingPositions->mMeshes[0]->mVertices = 0;
    error.clear();
    Check(!AssimpSceneConverter::ConvertScene(missingPositions.get(), converted, error),
          "single-mesh conversion rejects missing source positions");
    Check(!error.empty(), "missing source positions produce an error message");
}
}

int main()
{
    const char * objPath = "AssimpSceneConverterTests_output.obj";
    const char * mtlPath = "AssimpSceneConverterTests_output.mtl";
    std::remove(objPath);
    std::remove(mtlPath);

    AssimpConvertedMesh triangle = MakeTriangle();
    std::vector<AssimpConvertedMesh> meshes(1, triangle);
    std::string error = "stale error";

    Check(AssimpSceneConverter::ExportToObjFile(objPath, meshes, error),
          "valid triangle exports successfully");
    Check(error.empty(), "successful export clears an old error message");
    const std::string obj = ReadFile(objPath);
    const std::string mtl = ReadFile(mtlPath);
    Check(obj.find("v 1.000000 2.000000 -3.000000") != std::string::npos,
          "OBJ positions are converted back to right-handed space");
    Check(obj.find("vt 0.250000 0.750000") != std::string::npos,
          "OBJ texture coordinates invert the engine V origin");
    Check(obj.find("vn 0.000000 0.000000 -1.000000") != std::string::npos,
          "OBJ normals are converted back to right-handed space");
    Check(obj.find("f 3/3/3 2/2/2 1/1/1") != std::string::npos,
          "OBJ faces restore the source winding order");
    Check(mtl.find("map_Kd frog.png") != std::string::npos,
          "MTL keeps the source texture basename");

    std::remove(objPath);
    std::remove(mtlPath);

    error = "stale error";
    Check(!AssimpSceneConverter::ExportToObjFile("", meshes, error),
          "empty output paths are rejected");
    Check(!error.empty(), "empty output path reports an error");

    std::vector<AssimpConvertedMesh> badIndexMeshes(1, triangle);
    badIndexMeshes[0].indices[0] = 3;
    error = "stale error";
    Check(!AssimpSceneConverter::ExportToObjFile(objPath, badIndexMeshes, error),
          "out-of-range mesh indices are rejected");
    Check(!error.empty(), "invalid mesh reports a useful error");
    Check(!FileExists(objPath), "invalid mesh does not create a partial OBJ");
    Check(!FileExists(mtlPath), "invalid mesh does not create a partial MTL");

    std::vector<AssimpConvertedMesh> incompleteMeshes(1, triangle);
    incompleteMeshes[0].indices.resize(2);
    error.clear();
    Check(!AssimpSceneConverter::ExportToObjFile(objPath, incompleteMeshes, error),
          "incomplete triangle lists are rejected");

    std::vector<AssimpConvertedMesh> emptyMeshes;
    error.clear();
    Check(!AssimpSceneConverter::ExportToObjFile(objPath, emptyMeshes, error),
          "empty mesh lists are rejected");

    TestSceneHierarchyTransform();
    TestMalformedSceneData();

    if(failures != 0)
        std::fprintf(stderr, "%d scene-converter test(s) failed.\n", failures);
    else
        std::printf("All scene-converter tests passed.\n");

    std::remove(objPath);
    std::remove(mtlPath);
    return failures == 0 ? 0 : 1;
}

# Importing Modern 3D Models (Assimp)

EngineX can import modern 3D model formats directly, powered by
[Assimp](https://github.com/assimp/assimp) (Open Asset Import Library, BSD-3).
FBX, OBJ and glTF models - and a range of other formats - can be dropped into
any editor tool that loads a model, without a separate conversion step.

## Supported formats

| Format | Extensions | Notes |
|---|---|---|
| Autodesk FBX | `.fbx` | static geometry |
| Wavefront OBJ | `.obj` | static geometry, `usemtl`/`map_Kd` supported |
| glTF 2.0 | `.gltf`, `.glb` | static geometry; base color texture + factor |
| Collada | `.dae` | static geometry |
| STL | `.stl` | ASCII + binary |
| Stanford PLY | `.ply` | |
| 3ds Max | `.3ds` | supersedes the old `conv3ds` external-tool path |
| Blender | `.blend` | static geometry |
| 3ds Max ASE | `.ase` | |
| LightWave | `.lwo` | |
| AutoCAD DXF | `.dxf` | |

The engine's own serialized formats (`.arw`, and the legacy `.x` archives
produced by the old 3ds Max plugin / `conv3ds` pipeline) are **not** routed
through Assimp - they keep loading through the native loader.

## How to import

Every editor dialog that loads a model file (Object Library, Visual Manager,
LOD System, Missile base mesh, Portals, Water zones, Housing, Commerce, Bank
zones, Action zones, Explosions, Reference Library, Weapon system, halo /
particle / lens-flare meshes, ...) now lists the formats above in its file
picker ("Engine Models"). The import happens inside
`CEXMeshObj::LoadGeometryFromPlugin()`, so all of those tools accept the new
formats with no per-tool changes.

Typical flow:

1. Put the model file (and its textures) under the engine's models folder
   (e.g. `MapsModels\`).
2. In the editor, open the tool that places the model (e.g. Object Library ->
   Add Object) and pick the `.fbx` / `.obj` / `.gltf` / `.glb` file.
3. The mesh is converted to the engine's native mesh data on the spot and can
   be saved into the object library / world like any native model.

## What the importer does

`AuraX/CAssimpModelImporter.cpp` + `AuraX/AssimpSceneConverter.cpp` convert the
Assimp scene into the engine's `CEXMeshObj` geometry:

- **Coordinate system**: right-handed sources (glTF, OBJ, Collada, ...) are
  converted to the engine's left-handed Direct3D 9 world, with clockwise face
  order for the engine's CCW culling (`aiProcess_MakeLeftHanded |
  aiProcess_FlipWindingOrder`).
- **UVs**: V is flipped (`aiProcess_FlipUVs`) so OpenGL-style bottom-left-origin
  texture coordinates match the engine's top-left-origin textures. Up to 8 UV
  channels are imported (the engine's `ABVERTEX` has `tx0`..`tx7`).
- **Triangulation**: everything is converted to triangle lists
  (`aiProcess_Triangulate`).
- **Normals**: smooth normals are generated for formats that lack them
  (`aiProcess_GenSmoothNormals`, e.g. STL, OBJ without `vn` lines). Existing
  normals are kept; the engine's own minimal-smoothing pass (shade angle 3)
  runs afterwards, so hard edges stay hard.
- **Vertex welding**: duplicate vertices are merged
  (`aiProcess_JoinIdenticalVertices`) so the 16-bit index buffer covers as much
  of the model as possible.
- **Scene-graph node transforms**: `aiNode::mTransformation` hierarchies in
  multi-part FBX / glTF / Collada / 3DS / Blender scenes are traversed and
  applied to vertex positions and normals (with automatic winding correction for
  negative-determinant mirrored transforms).
- **Multi-material sub-mesh splitting**: when importing into the Reference
  Library (`Visual Manager -> Add Visual`), `CAssimpModelImporter::ImportToMeshList`
  splits multi-material models by material index (and auto-chunks sub-meshes
  that would exceed 65,535 vertices or indices) so each material gets its own
  `CEXMeshObj` and `CMaterialObject`. Single-mesh callers (`LoadGeometryFromPlugin`)
  merge all sub-meshes and apply the dominant sub-mesh's diffuse texture +
  material color.
- **Material color & embedded texture extraction**: the diffuse/base color is
  carried into the engine material's diffuse + ambient. Embedded textures inside
  binary `.glb` and `.fbx` files (`*0` or `scene->GetEmbeddedTexture(...)`) are
  automatically extracted next to the source model and into `MapsModels\` (both
  compressed PNG/JPG/DDS/BMP buffers and uncompressed RGBA8888 texels written
  as 32-bit `.bmp`) so the engine's texture database can load them seamlessly.
- **Bounds**: the sight range / bounding data is computed from the imported
  vertices, exactly like the native loader does.

## Exporting Engine Meshes & Skeletal Entities to Wavefront OBJ (`.obj` + `.mtl`)

Any loaded mesh, Reference Library unit, or Skeletal Entity (including legacy
`.arw`, `.rui`, `.wle`, and `.SIE` models) can be exported to a standard
right-handed Wavefront `.obj` + `.mtl` pair for editing in Blender, Maya or
3ds Max:

- **Object Editor -> `Export Object`**: choose **Wavefront OBJ (`*.obj`)** in
  the save dialog to export the selected world object's mesh (or its referenced
  Reference Library mesh list).
- **Reference Library -> `Export`**: choose **Wavefront OBJ (`*.obj`)** in the
  save dialog to export all sub-meshes and materials (both static `m_meshObject`
  and skeletal `m_skeletalOption` units).
- **Entity Editor -> `Save Entity`**: choose **Wavefront OBJ (`*.obj`)** in the
  save dialog to export all deformable skin meshes and rigid bone hierarchy
  meshes of the selected character/entity (`CSkeletonObject::ExportToObj`).

`AssimpSceneConverter::ExportToObjFile` applies the exact inverse of the import
conventions (`z -> -z`, `nz -> -nz`, `tv -> 1 - tv`, and reversed face winding),
so exporting to `.obj` and re-importing via Assimp is a 1:1 round-trip.

## Limitations

- **16-bit indices**: the engine indexes individual `CEXMeshObj` buffers with
  `WORD` (16-bit), so a single merged mesh must have **at most 65,535 vertices
  and 65,535 indices** after welding. Multi-part models imported through the
  Reference Library (`ImportToMeshList`) are automatically split by material and
  chunked at 65,535 vertices/indices.
- **Static geometry**: skeletal animations continue to use the engine's `.arb` /
  `.dim` skeletal pipeline.
- **No Draco**: Draco-compressed glTF meshes are not decoded (Assimp is built
  without Draco). Export uncompressed glTF from the DCC tool.
- **No unit conversion**: glTF is meters, OBJ/FBX are usually centimeters.
  Models import at their authored size - scale them in the editor
  (the engine's scale tools) if needed.

## Build integration

Assimp is fetched at CMake configure time and pinned by hash
(`v6.0.5`, see the root `CMakeLists.txt`). It is built **import-only**,
**static**, with only the importers listed above, and links straight into
`AuraXDynamic.dll` - no `assimp*.dll` has to be shipped beside `EngineX.exe` /
`EndlessAges.exe`.

To build fully offline, drop an Assimp source checkout into
`thirdparty/assimp/` - the root `CMakeLists.txt` picks it up automatically
instead of downloading.

The conversion layer (`AuraX/AssimpSceneConverter.cpp`) is deliberately free of
MFC/D3D types so it can be compiled and unit-tested standalone on any platform.

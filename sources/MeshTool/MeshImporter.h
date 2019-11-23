#pragma once

#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <Engine/Modules/Graphics/Resources/RawMesh.h>

struct MeshImportOptions {
    bool flipUV = false;
    bool joinIdenticalVertices = false;
    bool calculateTangents = false;
    bool glueByMaterials = false;
};

class MeshImporter
{
public:
    MeshImporter();

    std::unique_ptr<RawMesh> importFromFile(const std::string& path, const MeshImportOptions& options);

private:
    std::unique_ptr<RawMesh> convertSceneToMesh(const aiScene& scene, const MeshImportOptions& options);
};

#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <Engine/Modules/Graphics/Resources/RawMesh.h>

struct SkeletonImportOptions {
    int maxBonexPerVertex = 4;
};

class SkeletonImporter
{
public:
    SkeletonImporter();

    std::unique_ptr<RawSkeleton> importFromFile(const std::string& path, const SkeletonImportOptions& options);

private:
    std::unique_ptr<RawSkeleton> convertSceneToSkeleton(const aiScene& scene, const SkeletonImportOptions& options);
    std::unordered_map<std::string, const aiBone*> collectBones(const aiScene& scene) const;

    const aiNode* findRootBoneNode(const aiNode* sceneRootNode,
                                   const std::unordered_map<std::string, const aiBone*>& bonesList) const;

    void buildSkeleton(const aiNode* skeletonNode, const std::unordered_map<std::string, const aiBone*>& bonesList,
                       std::vector<RawBone>& rawBonesList, int parentBoneId) const;
};

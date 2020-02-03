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

struct ImportSceneBoneData {
    const aiBone* bone;
    aiMatrix4x4 sceneTransfromationMatrix;
};

class SkeletonImporter
{
public:
    SkeletonImporter();

    std::unique_ptr<RawSkeleton> importFromFile(const std::string& path, const SkeletonImportOptions& options);

private:
    std::unique_ptr<RawSkeleton> convertSceneToSkeleton(const aiScene& scene, const SkeletonImportOptions& options);
    std::unordered_map<std::string, const aiBone*> collectBones(const aiScene& scene) const;

    void traverseSkeletonHierarchy(const aiNode* sceneNode,
                                   const aiMatrix4x4& parentNodeTransform,
                                   const std::unordered_map<std::string, const aiBone*>& usedBones,
                                   std::unordered_map<std::string, ImportSceneBoneData>& bonesData);

    const aiNode* findRootBoneNode(const aiNode* sceneRootNode,
                                   const std::unordered_map<std::string, const aiBone*>& bonesList) const;

    void buildSkeleton(const aiNode* skeletonNode, const std::unordered_map<std::string, ImportSceneBoneData>& bonesList,
                       std::vector<RawBone>& rawBonesList, int parentBoneId) const;
};

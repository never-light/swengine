#pragma once

#include <memory>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <Engine/Modules/Graphics/Resources/RawMesh.h>

struct AnimationImportOptions {
    std::string clipName;
    int maxBonesPerVertex = 4;
};

class AnimationImporter
{
public:
    AnimationImporter();

    std::unique_ptr<RawSkeletalAnimation> importFromFile(const std::string& path, const AnimationImportOptions& options);

private:
    std::unique_ptr<RawSkeletalAnimation> convertSceneToAnimation(const aiScene& scene,
                                                const RawSkeleton* skeleton,
                                                const AnimationImportOptions& options);

    void collectMeshes(const aiScene& scene,
                       const aiNode& sceneNode,
                       std::unordered_map<std::string, const aiMesh*>& meshesList,
                       const aiMatrix4x4& parentNodeTransform) const;

   std::unique_ptr<RawSkeleton> getSkeleton(const std::string& path, const AnimationImportOptions& options) const;
   std::unordered_map<std::string, int> getBonesMap(const RawSkeleton& skeleton) const;

   const aiAnimation* findAnimation(const aiScene& scene, const std::string& animationName) const;
};

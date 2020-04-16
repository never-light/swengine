#include "AnimationImporter.h"

#include <spdlog/spdlog.h>

#include <string>
#include <bitset>
#include <Engine/Exceptions/EngineRuntimeException.h>
#include <Engine/swdebug.h>

#include "AssimpMeshLoader.h"
#include "SkeletonImporter.h"

AnimationImporter::AnimationImporter() {

}

std::unique_ptr<RawSkeletalAnimationClip> AnimationImporter::importFromFile(const std::string& path,
                                                                            const AnimationImportOptions& options) {
  spdlog::info("Load source mesh: {}", path);

  AssimpMeshLoadOptions assimpOptions;
  assimpOptions.maxBonexPerVertex = options.maxBonesPerVertex;

  std::unique_ptr<AssimpScene> scene = AssimpMeshLoader::loadScene(path, assimpOptions);

  spdlog::info("Source mesh is loaded");
  spdlog::info("Start mesh parsing");

  spdlog::info("Start to load mesh skeleton...");
  std::unique_ptr<RawSkeleton> skeleton = getSkeleton(path, options);
  spdlog::info("Mesh skeleton is loaded");

  std::unique_ptr<RawSkeletalAnimationClip>
      animation = convertSceneToAnimation(scene->getScene(), skeleton.get(), options);

  spdlog::info("Animation is parsed");

  return animation;
}

std::unique_ptr<RawSkeletalAnimationClip> AnimationImporter::convertSceneToAnimation(const aiScene& scene,
                                                                                     const RawSkeleton* skeleton,
                                                                                     const AnimationImportOptions& options) {
  SW_ASSERT(skeleton != nullptr);

  spdlog::info("Start to import animation, available animations: ");

  for (size_t animationIndex = 0; animationIndex < scene.mNumAnimations; animationIndex++) {
    const aiAnimation* animation = scene.mAnimations[animationIndex];

    spdlog::info("\t- {}", std::string(animation->mName.C_Str()));
  }

  std::unordered_map<std::string, int> bonesMap = getBonesMap(*skeleton);

  const aiAnimation* animationPtr = findAnimation(scene, options.clipName);

  if (animationPtr == nullptr) {
    ENGINE_RUNTIME_ERROR("Failed to find target animation");
  }

  std::unique_ptr<RawSkeletalAnimationClip> rawAnimation = std::make_unique<RawSkeletalAnimationClip>();
  strcpy_s(rawAnimation->header.name, animationPtr->mName.C_Str());

  rawAnimation->header.formatVersion = SKELETON_FORMAT_VERSION;
  rawAnimation->header.rate = static_cast<float>(animationPtr->mTicksPerSecond);
  rawAnimation->header.duration = static_cast<float>(animationPtr->mDuration);
  rawAnimation->header.skeletonBonesCount = skeleton->header.bonesCount;

  rawAnimation->bonesAnimationChannels.resize(skeleton->header.bonesCount);

  for (size_t channelIndex = 0; channelIndex < animationPtr->mNumChannels; channelIndex++) {
    const aiNodeAnim* animationChannel = animationPtr->mChannels[channelIndex];

    if (animationChannel->mNumScalingKeys > 0) {
      spdlog::warn("Animations with scaling keys are not supported yet, "
                   "scaling transforms will be skipped");
    }

    RawBoneAnimationChannel rawChannel;
    rawChannel.header.positionFramesCount = static_cast<uint16_t>(animationChannel->mNumPositionKeys);
    rawChannel.header.orientationFramesCount = static_cast<uint16_t>(animationChannel->mNumRotationKeys);

    rawChannel.positionFrames.resize(rawChannel.header.positionFramesCount);

    for (size_t positionKeyIndex = 0; positionKeyIndex < animationChannel->mNumPositionKeys; positionKeyIndex++) {
      const aiVectorKey& positionKey = animationChannel->mPositionKeys[positionKeyIndex];

      rawChannel.positionFrames[positionKeyIndex].time = static_cast<float>(positionKey.mTime);
      rawChannel.positionFrames[positionKeyIndex].position = {
          positionKey.mValue.x,
          positionKey.mValue.y,
          positionKey.mValue.z,
      };
    }

    rawChannel.orientationFrames.resize(rawChannel.header.orientationFramesCount);

    for (size_t orientationKeyIndex = 0; orientationKeyIndex < animationChannel->mNumRotationKeys;
         orientationKeyIndex++) {
      const aiQuatKey& orientationKey = animationChannel->mRotationKeys[orientationKeyIndex];

      rawChannel.orientationFrames[orientationKeyIndex].time = static_cast<float>(orientationKey.mTime);
      rawChannel.orientationFrames[orientationKeyIndex].orientation = {
          orientationKey.mValue.x,
          orientationKey.mValue.y,
          orientationKey.mValue.z,
          orientationKey.mValue.w,
      };
    }

    uint8_t boneId = static_cast<uint8_t>(bonesMap[std::string(animationChannel->mNodeName.C_Str())]);

    // Merge channels if a channel for current bone already exists
    size_t currentPositonFramesCount = rawAnimation->bonesAnimationChannels[boneId].positionFrames.size();
    size_t currentOrientationFramesCount = rawAnimation->bonesAnimationChannels[boneId].orientationFrames.size();

    if (currentPositonFramesCount == 1 && rawChannel.positionFrames.size() > 1) {
      rawAnimation->bonesAnimationChannels[boneId].positionFrames.resize(rawChannel.positionFrames.size());

      for (size_t frameIndex = 0; frameIndex < rawChannel.positionFrames.size(); frameIndex++) {
        rawAnimation->bonesAnimationChannels[boneId].positionFrames[frameIndex] =
            rawChannel.positionFrames[frameIndex];
      }

      spdlog::warn("Position frames for bone #{} {} were merged", boneId, std::string(skeleton->bones[boneId].name));
    }
    else if (currentOrientationFramesCount == 1 &&
        rawChannel.orientationFrames.size() > 1) {
      rawAnimation->bonesAnimationChannels[boneId].orientationFrames.resize(rawChannel.orientationFrames.size());

      for (size_t frameIndex = 0; frameIndex < rawChannel.orientationFrames.size(); frameIndex++) {
        rawAnimation->bonesAnimationChannels[boneId].orientationFrames[frameIndex] =
            rawChannel.orientationFrames[frameIndex];
      }

      spdlog::warn("Orientation frames for bone #{} {} were merged", boneId, std::string(skeleton->bones[boneId].name));
    }
    else {
      if (currentPositonFramesCount > 0) {
        if (rawChannel.positionFrames.size() > currentPositonFramesCount) {
          rawAnimation->bonesAnimationChannels[boneId].positionFrames = rawChannel.positionFrames;

          spdlog::warn("Position frames for bone #{} {} were replaced",
                       boneId,
                       std::string(skeleton->bones[boneId].name));
        }
        else {
          spdlog::warn("Position frames for bone #{} {} were skipped",
                       boneId,
                       std::string(skeleton->bones[boneId].name));
        }
      }
      else {
        rawAnimation->bonesAnimationChannels[boneId].positionFrames = rawChannel.positionFrames;
      }

      if (currentOrientationFramesCount > 0) {
        if (rawChannel.orientationFrames.size() > currentOrientationFramesCount) {
          rawAnimation->bonesAnimationChannels[boneId].orientationFrames = rawChannel.orientationFrames;

          spdlog::warn("Orientation frames for bone #{} {} were replaced",
                       boneId,
                       std::string(skeleton->bones[boneId].name));
        }
        else {
          spdlog::warn("Orientation frames for bone #{} {} were skipped",
                       boneId,
                       std::string(skeleton->bones[boneId].name));
        }
      }
      else {
        rawAnimation->bonesAnimationChannels[boneId].orientationFrames = rawChannel.orientationFrames;
      }
    }

    rawAnimation->bonesAnimationChannels[boneId].header.positionFramesCount =
        static_cast<uint16_t>(rawAnimation->bonesAnimationChannels[boneId].positionFrames.size());

    rawAnimation->bonesAnimationChannels[boneId].header.orientationFramesCount =
        static_cast<uint16_t>(rawAnimation->bonesAnimationChannels[boneId].orientationFrames.size());
  }

  // Swap (parent => child) transform to (child => parent)
//    for (size_t boneIndex = 0; boneIndex < skeleton->bones.size(); boneIndex++) {
//        for (RawBonePositionFrame& frame : rawAnimation->bonesAnimationChannels[boneIndex].positionFrames) {
//            frame.position = {-frame.position.x, -frame.position.y, -frame.position.z};
//        }

//        for (RawBoneOrientationFrame& frame : rawAnimation->bonesAnimationChannels[boneIndex].orientationFrames) {
//            glm::mat4 rotationMatrix = glm::mat4_cast(glm::quat{ frame.orientation.x, frame.orientation.y, frame.orientation.z, frame.orientation.w });
//            glm::quat inversedRotation = glm::quat_cast(glm::inverse(rotationMatrix));

//            frame.orientation = { inversedRotation.x, inversedRotation.y, inversedRotation.z, inversedRotation.w };
//        }
//    }

  return rawAnimation;
}

std::unique_ptr<RawSkeleton> AnimationImporter::getSkeleton(const std::string& path,
                                                            const AnimationImportOptions& options) const {
  SkeletonImporter importer;
  SkeletonImportOptions importOptions;
  importOptions.maxBonexPerVertex = options.maxBonesPerVertex;

  return importer.importFromFile(path, importOptions);
}

std::unordered_map<std::string, int> AnimationImporter::getBonesMap(const RawSkeleton& skeleton) const {
  std::unordered_map<std::string, int> bonesMap;

  for (size_t boneIndex = 0; boneIndex < skeleton.bones.size(); boneIndex++) {
    const RawBone& bone = skeleton.bones[boneIndex];
    bonesMap[std::string(bone.name)] = static_cast<int>(boneIndex);
  }

  return bonesMap;
}

const aiAnimation* AnimationImporter::findAnimation(const aiScene& scene, const std::string& animationName) const {
  for (size_t animationIndex = 0; animationIndex < scene.mNumAnimations; animationIndex++) {
    const aiAnimation* animation = scene.mAnimations[animationIndex];

    if (std::string(animation->mName.C_Str()) == animationName) {
      return animation;
    }
  }

  return nullptr;
}

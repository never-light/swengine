#include "AnimationLoader.h"

#include <fstream>
#include "ResourceLoadingException.h"

AnimationLoader::AnimationLoader() {

}

AnimationLoader::~AnimationLoader()
{
}

BaseResourceInstance * AnimationLoader::load(const std::string& path, std::optional<std::any> options)
{
	try {
		std::ifstream in(path, std::ios::binary | std::ios::in);

		if (!in.is_open())
			throw ResourceLoadingException(ResourceLoadingError::FileNotAvailable, path.c_str(), "", __FILE__, __LINE__, __FUNCTION__);

		HeaderData header;
		in.read((char*)&header, sizeof(header));

		AnimationDescription animationDescription;
		in.read((char*)&animationDescription, sizeof(animationDescription));

		if (animationDescription.affectedBonesCount == 0)
			throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), "Animation can't have 0 bones sub-animations", __FILE__, __LINE__, __FUNCTION__);

		std::vector<BoneAnimation> bonesAnimations;

		for (size_t affectedBoneIndex = 0; affectedBoneIndex < animationDescription.affectedBonesCount; affectedBoneIndex++) {
			BoneAnimationDescription boneAnimationDescription;
			in.read((char*)&boneAnimationDescription, sizeof(BoneAnimationDescription));

			std::vector<BonePositionKeyFrame> bonePositionKeyFrames(boneAnimationDescription.positionKeyFramesCount);
			in.read((char*)bonePositionKeyFrames.data(), sizeof(bonePositionKeyFrames[0]) * bonePositionKeyFrames.size());

			std::vector<BoneOrientationKeyFrame> boneOrientationKeyFrames(boneAnimationDescription.orientationKeyFramesCount);
			in.read((char*)boneOrientationKeyFrames.data(), sizeof(boneOrientationKeyFrames[0]) * boneOrientationKeyFrames.size());

			bonesAnimations.push_back(BoneAnimation(boneAnimationDescription.boneIndex, 
				bonePositionKeyFrames, 
				boneOrientationKeyFrames));
		}

		return new ResourceInstance<Animation>(new Animation(
			animationDescription.durationInTicks, 
			animationDescription.ticksPerSecond, bonesAnimations));
	}
	catch (const std::ifstream::failure& failture) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), failture.what(), __FILE__, __LINE__, __FUNCTION__);
	}
	catch (const std::length_error& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), exception.what(), __FILE__, __LINE__, __FUNCTION__);
	}
}
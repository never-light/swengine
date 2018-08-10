#pragma once

#include <Engine\Components\ResourceManager\ResourceLoader.h>
#include <Engine\Components\ResourceManager\ResourceManager.h>

#include "DefaultMaterial.h"

#define SOLID_MESH_LOADER_MAX_NAMES_LENGTH 256
#define SOLID_MESH_LOADER_MAX_PATH_LENGTH 256

class AnimationLoader : public ResourceLoader {
private:
	struct HeaderData {
		std::uint32_t version;
	};

	struct AnimationDescription {
		float durationInTicks;
		float ticksPerSecond;

		uint32 affectedBonesCount;
	};

	struct BoneAnimationDescription {
		size_t boneIndex;
		size_t positionKeyFramesCount;
		size_t orientationKeyFramesCount;
	};
public:
	AnimationLoader();
	virtual ~AnimationLoader();

	virtual Resource* load(const std::string& filename) override;
};
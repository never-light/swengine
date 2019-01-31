#pragma once

#include <Engine/Components/Graphics/Animation/Animation.h>
#include "ResourceLoader.h"

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

		std::uint32_t affectedBonesCount;
	};

	struct BoneAnimationDescription {
		size_t boneIndex;
		size_t positionKeyFramesCount;
		size_t orientationKeyFramesCount;
	};

public:
	AnimationLoader();
	virtual ~AnimationLoader();

	virtual BaseResourceInstance * load(const std::string & path, std::optional<std::any> options) override;
};
#include "AnimationExporter.h"

#include <fstream>
#include <spdlog/spdlog.h>

#include <Engine/swdebug.h>

AnimationExporter::AnimationExporter()
{

}

void AnimationExporter::exportToFile(const std::string& path,
                                     const RawSkeletalAnimation& animation,
                                     const AnimationExportOptions& options)
{
    ARG_UNUSED(options);

    // Check mesh state
    SW_ASSERT(animation.header.formatVersion == ANIMATION_FORMAT_VERSION);

    SW_ASSERT(animation.header.skeletonBonesCount > 0 && animation.header.skeletonBonesCount ==
              animation.bonesAnimationChannels.size());

    spdlog::info("Save animation clip to file: {}", path);

    std::ofstream out(path, std::ios::binary);

    const uint16_t affectedbonesCount = animation.header.skeletonBonesCount;

    out.write(reinterpret_cast<const char*>(&animation.header), sizeof(animation.header));

    for (size_t channelIndex = 0; channelIndex < affectedbonesCount; channelIndex++) {
        const RawBoneAnimationChannel& channel = animation.bonesAnimationChannels[channelIndex];

        out.write(reinterpret_cast<const char*>(&channel.header), sizeof(channel.header));

        if (channel.positionFrames.size() > 0) {
            out.write(reinterpret_cast<const char*>(channel.positionFrames.data()),
                      sizeof(*channel.positionFrames.begin()) * channel.header.positionFramesCount);
        }

        if (channel.orientationFrames.size() > 0) {
            out.write(reinterpret_cast<const char*>(channel.orientationFrames.data()),
                      sizeof(*channel.orientationFrames.begin()) * channel.header.orientationFramesCount);
        }
    }

    out.close();
}

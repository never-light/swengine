#include "precompiled.h"
#pragma hdrstop

#include <fstream>
#include <streambuf>
#include <bitset>
#include <algorithm>

#include "SkeletalAnimationResource.h"
#include "Exceptions/EngineRuntimeException.h"
#include "Utility/memory.h"

#include "RawMesh.h"

SkeletalAnimationResource::SkeletalAnimationResource()
{

}

SkeletalAnimationResource::~SkeletalAnimationResource()
{
    SW_ASSERT(m_clip.use_count() <= 1);
}

void SkeletalAnimationResource::load(const ResourceDeclaration& declaration, ResourceManager& resourceManager)
{
    ARG_UNUSED(resourceManager);

    SW_ASSERT(m_clip == nullptr);

    SkeletalAnimationResourceParameters parameters = declaration.getParameters<SkeletalAnimationResourceParameters>();

    if (auto sourceFile = std::get_if<ResourceSourceFile>(&declaration.source)) {
        m_clip = loadFromFile(sourceFile->path, parameters);
    }
    else {
        ENGINE_RUNTIME_ERROR("Trying to load skeletal animation resource from invalid source");
    }
}

void SkeletalAnimationResource::unload()
{
    SW_ASSERT(m_clip.use_count() == 1);

    m_clip.reset();
}

bool SkeletalAnimationResource::isBusy() const
{
    return m_clip.use_count() > 1;
}

std::shared_ptr<SkeletalAnimationClip> SkeletalAnimationResource::loadFromFile(const std::string& path,
                                                                  const SkeletalAnimationResourceParameters& parameters)
{
    ARG_UNUSED(parameters);

    // Read raw mesh
    std::ifstream clipFile(path, std::ios::binary);

    RawSkeletalAnimation rawClip;

    clipFile.read(reinterpret_cast<char*>(&rawClip.header), sizeof(rawClip.header));

    if (rawClip.header.formatVersion != ANIMATION_FORMAT_VERSION) {
        ENGINE_RUNTIME_ERROR("Trying to load animation clip with incompatible format version");
    }

    if (rawClip.header.skeletonBonesCount == 0) {
        ENGINE_RUNTIME_ERROR("Trying to load animation clip with zero bones count");
    }

    const uint16_t skeletonBonesCount = rawClip.header.skeletonBonesCount;
    rawClip.bonesAnimationChannels.resize(skeletonBonesCount);

    for (size_t channelIndex = 0; channelIndex < skeletonBonesCount; channelIndex++) {
        RawBoneAnimationChannel& channel = rawClip.bonesAnimationChannels[channelIndex];

        clipFile.read(reinterpret_cast<char*>(&channel.header), sizeof(channel.header));

        if (channel.header.positionFramesCount > 0) {
            channel.positionFrames.resize(channel.header.positionFramesCount);

            clipFile.read(reinterpret_cast<char*>(channel.positionFrames.data()),
                              sizeof(*channel.positionFrames.begin()) * channel.header.positionFramesCount);
        }

        if (channel.header.orientationFramesCount > 0) {
            channel.orientationFrames.resize(channel.header.orientationFramesCount);

            clipFile.read(reinterpret_cast<char*>(channel.orientationFrames.data()),
                              sizeof(*channel.orientationFrames.begin()) * channel.header.orientationFramesCount);
        }
    }

    clipFile.close();

    // Convert raw animation clip to internal animation clip object
    std::vector<BoneAnimationChannel> animationChannels;
    animationChannels.reserve(skeletonBonesCount);

    for (size_t channelIndex = 0; channelIndex < skeletonBonesCount; channelIndex++) {
        RawBoneAnimationChannel& channel = rawClip.bonesAnimationChannels[channelIndex];

        std::vector<BoneAnimationPositionFrame> positionFrames =
            MemoryUtils::createBinaryCompatibleVector<RawBonePositionFrame, BoneAnimationPositionFrame>(channel.positionFrames);

        std::vector<BoneAnimationOrientationFrame> orientationFrames =
            MemoryUtils::createBinaryCompatibleVector<RawBoneOrientationFrame, BoneAnimationOrientationFrame>(channel.orientationFrames);

        animationChannels.push_back(BoneAnimationChannel(positionFrames, orientationFrames));
    }

    std::string animationName = rawClip.header.name;
    float animationDuration = rawClip.header.duration;
    float animationRate = rawClip.header.rate;

    std::shared_ptr<SkeletalAnimationClip> animationClip = std::make_shared<SkeletalAnimationClip>(animationName,
        animationDuration, animationRate, animationChannels);

    return animationClip;
}

SkeletalAnimationResource::ParametersType SkeletalAnimationResource::buildDeclarationParameters(const pugi::xml_node& declarationNode,
                                                                const ParametersType& defaultParameters)
{
    ARG_UNUSED(declarationNode);

    ParametersType parameters = defaultParameters;

    return parameters;
}

std::shared_ptr<SkeletalAnimationClip> SkeletalAnimationResource::getClip() const
{
    return m_clip;
}

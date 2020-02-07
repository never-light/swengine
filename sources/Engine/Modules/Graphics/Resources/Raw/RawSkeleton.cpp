#include "precompiled.h"
#pragma hdrstop

#include "RawSkeleton.h"
#include "Exceptions/EngineRuntimeException.h"

RawSkeleton RawSkeleton::readFromFile(const std::string& path)
{
    std::ifstream skeletonFile(path, std::ios::binary);

    RawSkeleton rawSkeleton;

    skeletonFile.read(reinterpret_cast<char*>(&rawSkeleton.header), sizeof(rawSkeleton.header));

    if (rawSkeleton.header.formatVersion != SKELETON_FORMAT_VERSION) {
        ENGINE_RUNTIME_ERROR("Trying to load skeleton with incompatible format version");
    }

    if (rawSkeleton.header.bonesCount == 0) {
        ENGINE_RUNTIME_ERROR("Trying to load skeleton with zero bones count");
    }

    const uint16_t bonesCount = rawSkeleton.header.bonesCount;

    rawSkeleton.bones.resize(rawSkeleton.header.bonesCount);

    skeletonFile.read(reinterpret_cast<char*>(rawSkeleton.bones.data()),
                      sizeof(*rawSkeleton.bones.begin()) * bonesCount);

    skeletonFile.close();

    return rawSkeleton;
}

void RawSkeleton::writeToFile(const std::string& path, const RawSkeleton& rawSkeleton)
{
    SW_ASSERT(rawSkeleton.header.formatVersion == SKELETON_FORMAT_VERSION);

    SW_ASSERT(rawSkeleton.header.bonesCount > 0 && rawSkeleton.header.bonesCount == rawSkeleton.bones.size());

    std::ofstream out(path, std::ios::binary);

    const uint16_t bonesCount = rawSkeleton.header.bonesCount;

    out.write(reinterpret_cast<const char*>(&rawSkeleton.header), sizeof(rawSkeleton.header));

    out.write(reinterpret_cast<const char*>(rawSkeleton.bones.data()),
              sizeof(*rawSkeleton.bones.begin()) * bonesCount);

    out.close();
}

#include "SkeletonExporter.h"

#include <fstream>
#include <spdlog/spdlog.h>

#include <Engine/swdebug.h>

SkeletonExporter::SkeletonExporter()
{

}

void SkeletonExporter::exportToFile(const std::string& path,
                                    const RawSkeleton& skeleton,
                                    const SkeletonExportOptions& options)
{
    ARG_UNUSED(options);

    // Check mesh state
    SW_ASSERT(skeleton.header.formatVersion == SKELETON_FORMAT_VERSION);

    SW_ASSERT(skeleton.header.bonesCount > 0 && skeleton.header.bonesCount == skeleton.bones.size());

    spdlog::info("Save skeleton to file: {}", path);

    std::ofstream out(path, std::ios::binary);

    const uint16_t bonesCount = skeleton.header.bonesCount;

    out.write(reinterpret_cast<const char*>(&skeleton.header), sizeof(skeleton.header));

    out.write(reinterpret_cast<const char*>(skeleton.bones.data()),
              sizeof(*skeleton.bones.begin()) * bonesCount);

    out.close();
}

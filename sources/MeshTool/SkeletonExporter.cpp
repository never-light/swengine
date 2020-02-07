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

    spdlog::info("Save skeleton to file: {}", path);
    RawSkeleton::writeToFile(path, skeleton);
}

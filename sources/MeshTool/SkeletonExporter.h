#pragma once

#include <memory>
#include <Engine/Modules/Graphics/Resources/RawMesh.h>

struct SkeletonExportOptions {

};

class SkeletonExporter
{
public:
    SkeletonExporter();

    void exportToFile(const std::string& path,
                      const RawSkeleton& skeleton,
                      const SkeletonExportOptions& options);
};

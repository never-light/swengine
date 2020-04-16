#pragma once

#include <memory>
#include <string>
#include <Engine/Modules/Graphics/Resources/Raw/RawSkeleton.h>

struct SkeletonExportOptions {

};

class SkeletonExporter {
 public:
  SkeletonExporter();

  void exportToFile(const std::string& path,
                    const RawSkeleton& skeleton,
                    const SkeletonExportOptions& options);
};

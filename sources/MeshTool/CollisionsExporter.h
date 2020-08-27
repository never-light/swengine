#pragma once

#include <memory>
#include <string>
#include <Engine/Modules/Physics/Resources/Raw/RawMeshCollisionData.h>

struct CollisionsExportOptions {
  char unused_padding[1];
};

class CollisionsExporter {
 public:
  CollisionsExporter();

  void exportToFile(const std::string& path,
    const RawMeshCollisionData& collisionData,
    const CollisionsExportOptions& options);
};

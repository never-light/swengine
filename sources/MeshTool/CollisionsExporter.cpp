#include "CollisionsExporter.h"

#include <fstream>
#include <spdlog/spdlog.h>

#include <Engine/swdebug.h>

CollisionsExporter::CollisionsExporter()
{

}

void CollisionsExporter::exportToFile(const std::string& path,
  const RawMeshCollisionData& collisionData,
  const CollisionsExportOptions& options)
{
  ARG_UNUSED(options);

  spdlog::info("Save collision data to file: {}", path);
  RawMeshCollisionData::writeToFile(path, collisionData);
}

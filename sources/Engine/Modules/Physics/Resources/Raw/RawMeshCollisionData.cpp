#include "precompiled.h"

#pragma hdrstop

#include "RawMeshCollisionData.h"
#include "Exceptions/exceptions.h"

#include "Utility/files.h"

RawMeshCollisionData RawMeshCollisionData::readFromFile(const std::string& path)
{
  if (!FileUtils::isFileExists(path)) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to read not existing mesh collision data file " + path);
  }

  RawMeshCollisionData rawCollisionData;

  std::ifstream collisionDataFile(path, std::ios::binary);
  collisionDataFile.read(reinterpret_cast<char*>(&rawCollisionData.header), sizeof(rawCollisionData.header));

  if (rawCollisionData.header.formatVersion != MESH_COLLISION_DATA_FORMAT_VERSION) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load mesh with incompatible format version");
  }

  if (rawCollisionData.header.collisionShapesCount == 0) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load mesh collision data "
                                            "with zero collision shapes count");
  }

  const uint16_t collisionShapesCount = rawCollisionData.header.collisionShapesCount;

  rawCollisionData.collisionShapes.resize(collisionShapesCount);

  for (auto& shape : rawCollisionData.collisionShapes) {
    collisionDataFile.read(reinterpret_cast<char*>(&shape.type),sizeof(shape.type));

    switch (shape.type) {
      case RawMeshCollisionShapeType::AABB:
        collisionDataFile.read(reinterpret_cast<char*>(&shape.aabb),sizeof(shape.aabb));
        break;
      case RawMeshCollisionShapeType::Sphere:
        collisionDataFile.read(reinterpret_cast<char*>(&shape.sphere),sizeof(shape.sphere));
        break;
      case RawMeshCollisionShapeType::TriangleMesh:
        collisionDataFile.read(reinterpret_cast<char*>(&shape.triangleMesh.header),sizeof(shape.triangleMesh.header));

        shape.triangleMesh.vertices.resize(shape.triangleMesh.header.verticesCount);

        collisionDataFile.read(reinterpret_cast<char*>(shape.triangleMesh.vertices.data()),
          sizeof(*shape.triangleMesh.vertices.begin()) * shape.triangleMesh.header.verticesCount);
        break;
      default:
        SW_ASSERT(0);
        break;
    }
  }

  collisionDataFile.close();

  return rawCollisionData;
}

void RawMeshCollisionData::writeToFile(const std::string& path, const RawMeshCollisionData& rawCollisionData)
{
  SW_ASSERT(rawCollisionData.header.formatVersion == MESH_COLLISION_DATA_FORMAT_VERSION);

  SW_ASSERT(rawCollisionData.header.collisionShapesCount > 0);

  std::ofstream collisionDataFile(path, std::ios::binary);

  const uint16_t collisionShapesCount = rawCollisionData.header.collisionShapesCount;

  collisionDataFile.write(reinterpret_cast<const char*>(&rawCollisionData.header), sizeof(rawCollisionData.header));

  for (auto& shape : rawCollisionData.collisionShapes) {
    collisionDataFile.write(reinterpret_cast<const char*>(&shape.type), sizeof(shape.type));

    switch (shape.type) {
      case RawMeshCollisionShapeType::AABB:
        collisionDataFile.write(reinterpret_cast<const char*>(&shape.aabb),sizeof(shape.aabb));
        break;
      case RawMeshCollisionShapeType::Sphere:
        collisionDataFile.write(reinterpret_cast<const char*>(&shape.sphere),sizeof(shape.sphere));
        break;
      case RawMeshCollisionShapeType::TriangleMesh:
        collisionDataFile.write(reinterpret_cast<const char*>(&shape.triangleMesh.header),sizeof(shape.triangleMesh.header));

        collisionDataFile.write(reinterpret_cast<const char*>(shape.triangleMesh.vertices.data()),
          sizeof(*shape.triangleMesh.vertices.begin()) * shape.triangleMesh.header.verticesCount);
        break;
      default:
        SW_ASSERT(0);
        break;
    }
  }

  collisionDataFile.write(reinterpret_cast<const char*>(rawCollisionData.collisionShapes.data()),
    sizeof(*rawCollisionData.collisionShapes.begin()) * collisionShapesCount);

  collisionDataFile.close();
}

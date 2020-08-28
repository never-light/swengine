#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <string>

#include "Modules/ResourceManagement/RawDataStructures.h"
#include "Modules/Math/geometry.h"

constexpr uint16_t MESH_COLLISION_DATA_FORMAT_VERSION = 112;

enum class RawMeshCollisionShapeType {
  Sphere,
  AABB,
  TriangleMesh
};

struct RawMeshCollisionShapeSphere {
  RawVector3 origin;
  float radius;
};

struct RawMeshCollisionShapeAABB {
  RawVector3 min;
  RawVector3 max;
};

struct RawMeshCollisionShapeTriangleMeshHeader {
  uint16_t verticesCount = 0;
};

struct RawMeshCollisionShapeTriangleMesh {
  RawMeshCollisionShapeTriangleMeshHeader header;
  std::vector<RawVector3> vertices;
};

struct RawMeshCollisionShape {
  RawMeshCollisionShapeType type{};

  RawMeshCollisionShapeSphere sphere{};
  RawMeshCollisionShapeAABB aabb{};
  RawMeshCollisionShapeTriangleMesh triangleMesh;
};

struct RawMeshCollisionDataHeader {
  uint16_t formatVersion;
  uint16_t collisionShapesCount;
};

struct RawMeshCollisionData {
  RawMeshCollisionDataHeader header;

  std::vector<RawMeshCollisionShape> collisionShapes;

  static RawMeshCollisionData readFromFile(const std::string& path);
  static void writeToFile(const std::string& path, const RawMeshCollisionData& rawCollisionData);
};

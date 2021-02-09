#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <string>

#include "Modules/ResourceManagement/RawDataStructures.h"
#include "Modules/Math/geometry.h"

// TODO: assume that there could be migrations from previous meshes formats,
//  try to avoid manual meshes reimporting.
constexpr uint16_t MESH_FORMAT_VERSION = 115;

enum class RawMeshAttributes {
  Empty = 0,
  Positions = 1,
  Normals = 2,
  UV = 4,
  Tangents = 8,
  BonesIDs = 16,
  BonesWeights = 32,
};

inline RawMeshAttributes operator|(RawMeshAttributes a, RawMeshAttributes b)
{
  return static_cast<RawMeshAttributes>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

inline RawMeshAttributes operator&(RawMeshAttributes a, RawMeshAttributes b)
{
  return static_cast<RawMeshAttributes>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

inline RawMeshAttributes operator^(RawMeshAttributes a, RawMeshAttributes b)
{
  return static_cast<RawMeshAttributes>(static_cast<unsigned int>(a) ^ static_cast<unsigned int>(b));
}

/**
 * @brief Mesh format is intended to store meshes geometry in single vertex buffer, so
 * all submeshes should have the same vertex format.
 *
 * Raw mesh could contain one or more submeshes, each submesh is represented by
 * indices count and indices values.
 *
 * Indices are intended to be 2-bytes integers, so mesh should contain not more than
 * 65535 vertices, but indices count can be bigger.
 */
struct RawMeshHeader {
  uint16_t formatVersion;
  uint16_t verticesCount;

  bitmask64 storedAttributesMask;
  uint16_t subMeshesCount;
};

struct RawSubMeshDescription {
  uint32_t indicesCount;
  std::vector<uint16_t> indices;
};

struct RawMesh {
  RawMeshHeader header;

  std::vector<RawVector3> positions;
  std::vector<RawVector3> normals;
  std::vector<RawVector3> tangents;
  std::vector<RawVector2> uv;
  std::vector<RawU8Vector4> bonesIds;
  std::vector<RawU8Vector4> bonesWeights;

  std::vector<RawSubMeshDescription> subMeshesDescriptions;

  /**
   * @brief Axis-aligned bounding box for the whole mesh
   */
  AABB aabb;

  static RawMesh readFromFile(const std::string& path);
  static void writeToFile(const std::string& path, const RawMesh& rawMesh);
};

#pragma once

#include <cstdint>
#include <vector>

#include "Modules/ResourceManagement/RawDataStructures.h"
#include "Modules/Math/geometry.h"

constexpr uint16_t MESH_FORMAT_VERSION = 111;

enum class RawMeshAttributes {
    Positions = 0,
    Normals = 1,
    UV = 2
};

struct RawMeshHeader {
    uint16_t formatVersion;
    uint16_t verticesCount;
    uint16_t indicesCount;

    uint16_t subMeshesIndicesOffsetsCount;
    bitmask64 storedAttributesMask;
};

struct RawMesh {
    RawMeshHeader header;

    std::vector<RawVector3> positions;
    std::vector<RawVector3> normals;
    std::vector<RawVector2> uv;

    std::vector<uint16_t> indices;
    std::vector<uint16_t> subMeshesIndicesOffsets;

    AABB aabb;
};

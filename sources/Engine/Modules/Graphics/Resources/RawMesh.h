#pragma once

#include <cstdint>
#include <vector>

#include "Modules/ResourceManagement/RawDataStructures.h"
#include "Modules/Math/geometry.h"

constexpr uint16_t MESH_FORMAT_VERSION = 112;
constexpr uint16_t SKELETON_FORMAT_VERSION = 112;
constexpr size_t MAX_BONE_NAME_LENGTH = 64;

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
    return static_cast<RawMeshAttributes>(static_cast<int>(a) | static_cast<int>(b));
}

inline RawMeshAttributes operator&(RawMeshAttributes a, RawMeshAttributes b)
{
    return static_cast<RawMeshAttributes>(static_cast<int>(a) & static_cast<int>(b));
}

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
    std::vector<RawVector3> tangents;
    std::vector<RawVector2> uv;
    std::vector<RawU8Vector4> bonesIds;
    std::vector<RawU8Vector4> bonesWeights;

    std::vector<uint16_t> indices;
    std::vector<uint16_t> subMeshesIndicesOffsets;

    AABB aabb;
};

struct RawSkeletonHeader {
    uint16_t formatVersion;
    uint16_t bonesCount;
};

struct RawBone {
    char name[MAX_BONE_NAME_LENGTH];

    int16_t parentId;
    RawMatrix4 inverseBindPoseMatrix;
};

struct RawSkeleton {
    RawSkeletonHeader header;

    std::vector<RawBone> bones;
};

#include "MeshExporter.h"

#include <fstream>
#include <bitset>
#include <spdlog/spdlog.h>

#include <Engine/Modules/Graphics/Resources/RawMesh.h>
#include <Engine/swdebug.h>
#include <Engine/Exceptions/EngineRuntimeException.h>

MeshExporter::MeshExporter()
{

}

void MeshExporter::exportToFile(const std::string& path, const RawMesh& mesh, const MeshExportOptions& options)
{
    ARG_UNUSED(options);

    // Mesh state preconditions
    SW_ASSERT(mesh.header.formatVersion == MESH_FORMAT_VERSION);

    SW_ASSERT(mesh.header.indicesCount == mesh.indices.size() &&
              mesh.header.subMeshesIndicesOffsetsCount == mesh.subMeshesIndicesOffsets.size());

    // Check mesh and format compatibility
    RawMeshAttributes meshAttributes = static_cast<RawMeshAttributes>(mesh.header.storedAttributesMask);
    RawMeshAttributes targetAttributesMask = RawMeshAttributes::Empty;

    switch (options.format) {
    case Pos3Norm3UV:
        targetAttributesMask = RawMeshAttributes::Positions | RawMeshAttributes::Normals
                | RawMeshAttributes::UV;
        break;

    case Pos3Norm3UVSkinned:
        targetAttributesMask = RawMeshAttributes::Positions | RawMeshAttributes::Normals
                | RawMeshAttributes::UV | RawMeshAttributes::BonesIDs
                | RawMeshAttributes::BonesWeights;
        break;

    case Pos3Norm3Tan3UV:
        targetAttributesMask = RawMeshAttributes::Positions | RawMeshAttributes::Normals
                | RawMeshAttributes::UV | RawMeshAttributes::Tangents;
        break;

    case Pos3Norm3Tan3UVSkinned:
        targetAttributesMask = RawMeshAttributes::Positions | RawMeshAttributes::Normals
                | RawMeshAttributes::UV | RawMeshAttributes::Tangents
                | RawMeshAttributes::BonesIDs | RawMeshAttributes::BonesWeights;
        break;

    default:
        ENGINE_RUNTIME_ERROR("The target format is not supported");
    }

    if ((meshAttributes & targetAttributesMask) != targetAttributesMask)
    {
        ENGINE_RUNTIME_ERROR("The mesh is incompatible with the target format");
    }

    // Update stored attributes mask according to target format
    RawMeshHeader targetRawMeshHeader = mesh.header;
    targetRawMeshHeader.storedAttributesMask = static_cast<bitmask64>(targetAttributesMask);

    spdlog::info("Save mesh to file: {}", path);

    std::ofstream out(path, std::ios::binary);

    const uint16_t verticesCount = mesh.header.verticesCount;

    out.write(reinterpret_cast<const char*>(&targetRawMeshHeader), sizeof(targetRawMeshHeader));

    if ((targetAttributesMask & RawMeshAttributes::Positions) != RawMeshAttributes::Empty) {
        SW_ASSERT(mesh.positions.size() == mesh.header.verticesCount);

        out.write(reinterpret_cast<const char*>(mesh.positions.data()),
                  sizeof(*mesh.positions.begin()) * verticesCount);
    }

    if ((targetAttributesMask & RawMeshAttributes::Normals) != RawMeshAttributes::Empty) {
        SW_ASSERT(mesh.normals.size() == mesh.header.verticesCount);

        out.write(reinterpret_cast<const char*>(mesh.normals.data()),
                  sizeof(*mesh.normals.begin()) * verticesCount);
    }

    if ((targetAttributesMask & RawMeshAttributes::Tangents) != RawMeshAttributes::Empty) {
        SW_ASSERT(mesh.tangents.size() == mesh.header.verticesCount);

        out.write(reinterpret_cast<const char*>(mesh.tangents.data()),
                  sizeof(*mesh.tangents.begin()) * verticesCount);
    }

    if ((targetAttributesMask & RawMeshAttributes::UV) != RawMeshAttributes::Empty) {
        SW_ASSERT(mesh.uv.size() == mesh.header.verticesCount);

        out.write(reinterpret_cast<const char*>(mesh.uv.data()),
                  sizeof(*mesh.uv.begin()) * verticesCount);
    }

    if ((targetAttributesMask & RawMeshAttributes::BonesIDs) != RawMeshAttributes::Empty) {
        SW_ASSERT(mesh.bonesIds.size() == mesh.header.verticesCount);

        out.write(reinterpret_cast<const char*>(mesh.bonesIds.data()),
                  sizeof(*mesh.bonesIds.begin()) * verticesCount);
    }

    if ((targetAttributesMask & RawMeshAttributes::BonesWeights) != RawMeshAttributes::Empty) {
        SW_ASSERT(mesh.bonesWeights.size() == mesh.header.verticesCount);

        out.write(reinterpret_cast<const char*>(mesh.bonesWeights.data()),
                  sizeof(*mesh.bonesWeights.begin()) * verticesCount);
    }

    const uint16_t indicesCount = mesh.header.indicesCount;

    out.write(reinterpret_cast<const char*>(mesh.indices.data()),
              sizeof(*mesh.indices.begin()) * indicesCount);

    const uint16_t subMeshesIndicesOffsetsCount = mesh.header.subMeshesIndicesOffsetsCount;

    out.write(reinterpret_cast<const char*>(mesh.subMeshesIndicesOffsets.data()),
              sizeof(*mesh.subMeshesIndicesOffsets.begin()) * subMeshesIndicesOffsetsCount);

    out.write(reinterpret_cast<const char*>(&mesh.aabb), sizeof(mesh.aabb));

    out.close();
}

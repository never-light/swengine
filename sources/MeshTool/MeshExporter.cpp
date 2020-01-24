#include "MeshExporter.h"

#include <fstream>
#include <bitset>
#include <spdlog/spdlog.h>

#include <Engine/Modules/Graphics/Resources/RawMesh.h>
#include <Engine/swdebug.h>

MeshExporter::MeshExporter()
{

}

void MeshExporter::exportToFile(const std::string& path, const RawMesh& mesh, const MeshExportOptions& options)
{
    ARG_UNUSED(options);

    // Check mesh state
    SW_ASSERT(mesh.header.formatVersion == MESH_FORMAT_VERSION);

    SW_ASSERT(mesh.header.verticesCount == mesh.positions.size() &&
              mesh.header.verticesCount == mesh.normals.size() &&
              mesh.header.verticesCount == mesh.uv.size());

    SW_ASSERT(mesh.header.indicesCount == mesh.indices.size() &&
              mesh.header.subMeshesIndicesOffsetsCount == mesh.subMeshesIndicesOffsets.size());

    spdlog::info("Save mesh to file: {}", path);

    std::ofstream out(path, std::ios::binary);

    const uint16_t verticesCount = mesh.header.verticesCount;

    out.write(reinterpret_cast<const char*>(&mesh.header), sizeof(mesh.header));

    out.write(reinterpret_cast<const char*>(mesh.positions.data()),
              sizeof(*mesh.positions.begin()) * verticesCount);

    out.write(reinterpret_cast<const char*>(mesh.normals.data()),
              sizeof(*mesh.normals.begin()) * verticesCount);

    out.write(reinterpret_cast<const char*>(mesh.uv.data()),
              sizeof(*mesh.uv.begin()) * verticesCount);

    const uint16_t indicesCount = mesh.header.indicesCount;

    out.write(reinterpret_cast<const char*>(mesh.indices.data()),
              sizeof(*mesh.indices.begin()) * indicesCount);

    const uint16_t subMeshesIndicesOffsetsCount = mesh.header.subMeshesIndicesOffsetsCount;

    out.write(reinterpret_cast<const char*>(mesh.subMeshesIndicesOffsets.data()),
              sizeof(*mesh.subMeshesIndicesOffsets.begin()) * subMeshesIndicesOffsetsCount);

    out.write(reinterpret_cast<const char*>(&mesh.aabb), sizeof(mesh.aabb));

    out.close();
}

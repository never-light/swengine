#include "MeshExporter.h"

#include <fstream>
#include <bitset>
#include <spdlog/spdlog.h>

#include <Engine/swdebug.h>
#include <Engine/Exceptions/exceptions.h>

MeshExporter::MeshExporter()
{

}

void MeshExporter::exportToFile(const std::string& path, const RawMesh& mesh, const MeshExportOptions& options)
{
  // Check mesh and format compatibility
  auto meshAttributes = static_cast<RawMeshAttributes>(mesh.header.storedAttributesMask);
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
      THROW_EXCEPTION(EngineRuntimeException, "The target format is not supported");
  }

  if ((meshAttributes & targetAttributesMask) != targetAttributesMask) {
    THROW_EXCEPTION(EngineRuntimeException, "The mesh is incompatible with the target format");
  }

  // Update stored attributes mask according to target format
  RawMesh exportedMesh = mesh;
  exportedMesh.header.storedAttributesMask = static_cast<bitmask64>(targetAttributesMask);

  spdlog::info("Save mesh to file: {}", path);

  RawMesh::writeToFile(path, exportedMesh);
}

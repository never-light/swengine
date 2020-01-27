#pragma once

#include <memory>
#include <Engine/Modules/Graphics/Resources/RawMesh.h>

// pos3_norm3_uv, pos3_norm3_uv_skinned, "pos3_norm3_tan3_uv, pos3_norm3_tan3_uv_skinned
enum MeshExportFormat {
    Pos3Norm3UV,
    Pos3Norm3UVSkinned,
    Pos3Norm3Tan3UV,
    Pos3Norm3Tan3UVSkinned
};

struct MeshExportOptions {
    MeshExportFormat format;
};

class MeshExporter
{
public:
    MeshExporter();

    void exportToFile(const std::string& path, const RawMesh& mesh, const MeshExportOptions& options);
};

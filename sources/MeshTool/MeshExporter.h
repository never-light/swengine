#pragma once

#include <memory>
#include <Engine/Modules/Graphics/Resources/RawMesh.h>

struct MeshExportOptions {

};

class MeshExporter
{
public:
    MeshExporter();

    void exportToFile(const std::string& path, const RawMesh& mesh, const MeshExportOptions& options);
};

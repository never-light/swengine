#include "MeshToolApplication.h"

#include <spdlog/spdlog.h>
#include <Engine/Exceptions/EngineRuntimeException.h>

#include "MeshImporter.h"
#include "MeshExporter.h"

MeshToolApplication::MeshToolApplication()
{

}

void MeshToolApplication::execute(int argc, char * argv[])
{
    cxxopts::Options options("SWEngine Mesh Tool", "StarWind mesh processing tool");
    options.add_options()
            ("i,input", "Input file", cxxopts::value<std::string>())
            ("o,output", "Output file", cxxopts::value<std::string>())
            ("a,action", "Action", cxxopts::value<std::string>());

    auto parsedArgs = options.parse(argc, argv);

    std::string action = parsedArgs["action"].as<std::string>();

    if (action == "import") {
        importMesh(parsedArgs);
    }
    else {
        ENGINE_RUNTIME_ERROR("Unknown action");
    }
}

void MeshToolApplication::importMesh(const cxxopts::ParseResult& options)
{
    spdlog::info("Convertation started");

    // Import mesh as raw mesh data
    MeshImportOptions importOptions;
    importOptions.flipUV = false;
    importOptions.glueByMaterials = true;
    importOptions.calculateTangents = true;
    importOptions.joinIdenticalVertices = true;

    MeshImporter importer;

    const std::string inputPath = options["input"].as<std::string>();
    std::unique_ptr<RawMesh> mesh = importer.importFromFile(inputPath, importOptions);

    // Save raw mesh data
    MeshExportOptions exportOptions;

    MeshExporter exporter;

    const std::string outputPath = options["output"].as<std::string>();
    exporter.exportToFile(outputPath, *mesh.get(), exportOptions);

    spdlog::info("Convertation finished");
}

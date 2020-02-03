#include "MeshToolApplication.h"

#include <spdlog/spdlog.h>
#include <Engine/Exceptions/EngineRuntimeException.h>
#include <Engine/swdebug.h>
#include <Engine/Utility/strings.h>

#include "MeshImporter.h"
#include "MeshExporter.h"

#include "SkeletonImporter.h"
#include "SkeletonExporter.h"

#include "AnimationImporter.h"
#include "AnimationExporter.h"

MeshToolApplication::MeshToolApplication()
{

}

void MeshToolApplication::execute(int argc, char * argv[])
{
    cxxopts::Options options("StarWind Mesh Tool", "StarWind mesh processing tool");
    options.add_options()
            ("h,help", "Help")
            ("i,input", "Input file", cxxopts::value<std::string>())
            ("o,output", "Output file", cxxopts::value<std::string>())
            ("a,action", "Action (import)", cxxopts::value<std::string>()->default_value("import"))
            ("t,type", "Import type (mesh, skeleton, animation)", cxxopts::value<std::string>()->default_value("mesh"))
            ("format", "Output mesh format (pos3_norm3_uv, pos3_norm3_uv_skinned,"
                       "pos3_norm3_tan3_uv, pos3_norm3_tan3_uv_skinned)",
                        cxxopts::value<std::string>()->default_value("pos3_norm3_uv"))
            ("clip-name", "Skeletal animation clip name to import", cxxopts::value<std::string>());

    auto parsedArgs = options.parse(argc, argv);

    if (parsedArgs.count("help") > 0) {
        std::cout << options.help() << std::endl << std::endl;
        std::cout << "Examples: " << std::endl;
        std::cout << "./MeshTool -i mesh.dae -o mesh.mesh -a import -t mesh --format pos3_norm3_uv" << std::endl;
        std::cout << "./MeshTool -i mesh.dae -o mesh.mesh -a import -t skeleton" << std::endl;
        std::cout << "./MeshTool -i mesh.dae -o mesh.mesh -a import -t animation --clip-name idle" << std::endl;

        return;
    }

    std::string action = parsedArgs["action"].as<std::string>();

    if (action == "import") {
        std::string importType = parsedArgs["type"].as<std::string>();

        if (importType == "mesh") {
            importMesh(parsedArgs);
        }
        else if (importType == "skeleton") {
            importSkeleton(parsedArgs);
        }
        else if (importType == "animation") {
            importAnimation(parsedArgs);
        }
        else {
            ENGINE_RUNTIME_ERROR("Unknown import type");
        }
    }
    else {
        ENGINE_RUNTIME_ERROR("Unknown action");
    }
}

void MeshToolApplication::importMesh(const cxxopts::ParseResult& options)
{
    spdlog::info("Convertation started");

    MeshExportFormat exportFormat = StringUtils::filterValue(options["format"].as<std::string>(), {
        { "pos3_norm3_uv", MeshExportFormat::Pos3Norm3UV },
        { "pos3_norm3_uv_skinned", MeshExportFormat::Pos3Norm3UVSkinned },
        { "pos3_norm3_tan3_uv", MeshExportFormat::Pos3Norm3Tan3UV },
        { "pos3_norm3_tan3_uv_skinned", MeshExportFormat::Pos3Norm3Tan3UVSkinned },
    }, MeshExportFormat::Pos3Norm3UV);

    // Import mesh as raw mesh data
    MeshImportOptions importOptions;
    importOptions.flipUV = false;
    importOptions.glueByMaterials = true;
    importOptions.calculateTangents = true;
    importOptions.joinIdenticalVertices = true;

    if (exportFormat == MeshExportFormat::Pos3Norm3UVSkinned || exportFormat == MeshExportFormat::Pos3Norm3Tan3UVSkinned) {
        importOptions.loadSkin = true;
    }

    MeshImporter importer;

    const std::string inputPath = options["input"].as<std::string>();
    std::unique_ptr<RawMesh> mesh = importer.importFromFile(inputPath, importOptions);

    // Save raw mesh data
    MeshExportOptions exportOptions;

    // Export format
    exportOptions.format = exportFormat;

    MeshExporter exporter;

    const std::string outputPath = options["output"].as<std::string>();
    exporter.exportToFile(outputPath, *mesh.get(), exportOptions);

    spdlog::info("Convertation finished");
}

void MeshToolApplication::importSkeleton(const cxxopts::ParseResult& options)
{
    spdlog::info("Convertation started");

    // Import mesh as raw mesh data
    SkeletonImportOptions importOptions;

    SkeletonImporter importer;

    const std::string inputPath = options["input"].as<std::string>();
    std::unique_ptr<RawSkeleton> skeleton = importer.importFromFile(inputPath, importOptions);

    // Save raw mesh data
    SkeletonExportOptions exportOptions;

    SkeletonExporter exporter;

    const std::string outputPath = options["output"].as<std::string>();
    exporter.exportToFile(outputPath, *skeleton.get(), exportOptions);

    spdlog::info("Convertation finished");
}

void MeshToolApplication::importAnimation(const cxxopts::ParseResult& options)
{
    ARG_UNUSED(options);

    spdlog::info("Convertation started");

    // Import animation clip as raw animation clip data
    AnimationImportOptions importOptions;
    importOptions.clipName = options["clip-name"].as<std::string>();

    AnimationImporter importer;

    const std::string inputPath = options["input"].as<std::string>();
    std::unique_ptr<RawSkeletalAnimation> animation = importer.importFromFile(inputPath, importOptions);

    // Save raw animation clip data
    AnimationExportOptions exportOptions;
    AnimationExporter exporter;

    const std::string outputPath = options["output"].as<std::string>();
    exporter.exportToFile(outputPath, *animation.get(), exportOptions);

    spdlog::info("Convertation finished");
}

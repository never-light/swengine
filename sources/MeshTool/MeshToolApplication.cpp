#include "MeshToolApplication.h"

#include <spdlog/spdlog.h>
#include <Engine/Exceptions/exceptions.h>
#include <Engine/swdebug.h>
#include <Engine/Utility/strings.h>

#include "MeshImporter.h"
#include "MeshExporter.h"

#include "SkeletonImporter.h"
#include "SkeletonExporter.h"

#include "AnimationImporter.h"
#include "AnimationExporter.h"

#include "CollisionsImporter.h"
#include "CollisionsExporter.h"

#include "SceneImporter.h"
#include "SceneExporter.h"

#include "AssetsDump.h"

MeshToolApplication::MeshToolApplication()
{
  spdlog::set_level(spdlog::level::debug);
}

void MeshToolApplication::execute(int argc, char* argv[])
{
  cxxopts::Options options("StarWind Mesh Tool", "StarWind mesh processing tool");
  options.add_options()
    ("h,help", "Help")
    ("i,input", "Input file", cxxopts::value<std::string>())
    ("o,output", "Output file", cxxopts::value<std::string>())
    ("a,action", "Action (import, dump)", cxxopts::value<std::string>()->default_value("import"))
    ("t,type", "Import type (mesh, skeleton, animation, collisions, scene)",
      cxxopts::value<std::string>()->default_value("mesh"))
    ("format", "Output mesh format (pos3_norm3_uv, pos3_norm3_uv_skinned,"
               "pos3_norm3_tan3_uv, pos3_norm3_tan3_uv_skinned)",
      cxxopts::value<std::string>()->default_value("pos3_norm3_uv"))
    ("clip-name", "Skeletal animation clip name to import", cxxopts::value<std::string>());

  auto parsedArgs = options.parse(argc, argv);

  if (parsedArgs.count("help") > 0) {
    std::cout << options.help() << std::endl << std::endl;
    std::cout << "Examples: " << std::endl;
    std::cout << "./MeshTool -i mesh.dae -o mesh.mesh -a import -t mesh --format pos3_norm3_uv" << std::endl;
    std::cout << "./MeshTool -i mesh.dae -o mesh.skeleton -a import -t skeleton" << std::endl;
    std::cout << "./MeshTool -i mesh.dae -o mesh.anim -a import -t animation --clip-name idle" << std::endl;
    std::cout << "./MeshTool -i mesh.dae -o mesh.collision -a import -t collisions" << std::endl;
    std::cout << "./MeshTool -i scene.gltf -o scene_dir/ -a import -t scene" << std::endl;
    std::cout << "./MeshTool -a dump -i teapot.mesh" << std::endl;

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
    else if (importType == "collisions") {
      importCollisions(parsedArgs);
    }
    else if (importType == "scene") {
      importScene(parsedArgs);
    }
    else {
      THROW_EXCEPTION(EngineRuntimeException, "Unknown import type");
    }
  }
  else if (action == "dump") {
    const std::string inputPath = parsedArgs["input"].as<std::string>();

    AssetsDump assetsDump;
    assetsDump.dumpAssetData(inputPath);
  }
  else {
    THROW_EXCEPTION(EngineRuntimeException, "Unknown action");
  }
}

void MeshToolApplication::importMesh(const cxxopts::ParseResult& options)
{
  spdlog::info("Conversion started");

  MeshExportFormat exportFormat = StringUtils::filterValue(options["format"].as<std::string>(), {
    {"pos3_norm3_uv", MeshExportFormat::Pos3Norm3UV},
    {"pos3_norm3_uv_skinned", MeshExportFormat::Pos3Norm3UVSkinned},
    {"pos3_norm3_tan3_uv", MeshExportFormat::Pos3Norm3Tan3UV},
    {"pos3_norm3_tan3_uv_skinned", MeshExportFormat::Pos3Norm3Tan3UVSkinned},
  }, MeshExportFormat::Pos3Norm3UV);

  // Import mesh as raw mesh data
  MeshImportOptions importOptions;
  importOptions.flipUV = false;
  importOptions.glueByMaterials = true;
  importOptions.calculateTangents = true;
  importOptions.joinIdenticalVertices = true;

  if (exportFormat == MeshExportFormat::Pos3Norm3UVSkinned
    || exportFormat == MeshExportFormat::Pos3Norm3Tan3UVSkinned) {
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

  spdlog::info("Conversion finished");
}

void MeshToolApplication::importSkeleton(const cxxopts::ParseResult& options)
{
  spdlog::info("Conversion started");

  // Import mesh as raw mesh data
  SkeletonImportOptions importOptions;

  SkeletonImporter importer;

  const std::string inputPath = options["input"].as<std::string>();
  std::unique_ptr<RawSkeleton> skeleton = importer.importFromFile(inputPath, importOptions);

  // Save raw mesh data
  SkeletonExportOptions exportOptions;

  SkeletonExporter exporter;

  const std::string outputPath = options["output"].as<std::string>();
  exporter.exportToFile(outputPath, *skeleton, exportOptions);

  spdlog::info("Conversion finished");
}

void MeshToolApplication::importAnimation(const cxxopts::ParseResult& options)
{
  ARG_UNUSED(options);

  spdlog::info("Conversion started");

  // Import animation clip as raw animation clip data
  AnimationImportOptions importOptions;
  importOptions.clipName = options["clip-name"].as<std::string>();

  AnimationImporter importer;

  const std::string inputPath = options["input"].as<std::string>();
  std::unique_ptr<RawSkeletalAnimationClip> animation = importer.importFromFile(inputPath, importOptions);

  // Save raw animation clip data
  AnimationExportOptions exportOptions;
  AnimationExporter exporter;

  const std::string outputPath = options["output"].as<std::string>();
  exporter.exportToFile(outputPath, *animation, exportOptions);

  spdlog::info("Conversion finished");
}

void MeshToolApplication::importCollisions(const cxxopts::ParseResult& options)
{
  ARG_UNUSED(options);

  spdlog::info("Conversion started");

  CollisionsImportOptions importOptions{};

  CollisionsImporter importer;

  const std::string inputPath = options["input"].as<std::string>();
  std::unique_ptr<RawMeshCollisionData> collisionData = importer.importFromFile(inputPath, importOptions);

  // Save raw animation clip data
  CollisionsExportOptions exportOptions{};
  CollisionsExporter exporter;

  const std::string outputPath = options["output"].as<std::string>();
  exporter.exportToFile(outputPath, *collisionData, exportOptions);

  spdlog::info("Conversion finished");
}

void MeshToolApplication::importScene(const cxxopts::ParseResult& options)
{
  ARG_UNUSED(options);

  spdlog::info("Conversion started");

  SceneImportOptions importOptions{};

  SceneImporter importer;

  const std::string inputPath = options["input"].as<std::string>();
  auto sceneData = importer.importFromFile(inputPath, importOptions);

  // Save raw scene data
  SceneExportOptions exportOptions{};
  SceneExporter exporter;

  const std::string outputPath = options["output"].as<std::string>();
  exporter.exportDataToDirectory(outputPath, *sceneData, exportOptions);

  spdlog::info("Conversion finished");
}
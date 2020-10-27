#include <catch2/catch.hpp>

#include <Engine/Modules/ResourceManagement/ResourceManagementModule.h>
#include <Engine/Modules/Graphics/Resources/ShaderResourceManager.h>
#include <Engine/Modules/Graphics/Resources/MaterialResourceManager.h>

#include <Engine/Modules/Math/MathUtils.h>

#include "utility/resourcesUtility.h"

TEST_CASE("resources_maps_loading", "[resources]")
{
  std::shared_ptr<ResourcesManager> manager = generateTestResourcesManager();

  manager->loadResourcesMap("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                            "<resources>\n"
                            "    <resource type=\"shader\" id=\"vertex\" source=\"../resources/shaders/debug_vertex_shader.glsl\">\n"
                            "        <type>vertex</type>\n"
                            "    </resource>\n"
                            "\n"
                            "    <resource type=\"shader\" id=\"fragment\" source=\"../resources/shaders/debug_fragment_shader.glsl\">\n"
                            "        <type>fragment</type>\n"
                            "    </resource>\n"
                            "\n"
                            "    <resource type=\"material\" id=\"material\">\n"
                            "        <shaders_pipeline>\n"
                            "            <vertex id=\"vertex\"/>\n"
                            "            <fragment id=\"fragment\"/>\n"
                            "        </shaders_pipeline>\n"
                            "\n"
                            "        <gpu_state>\n"
                            "            <blending mode=\"alpha_one_minus_alpha\"/>\n"
                            "            <depth write=\"enabled\" testMode=\"less\"/>\n"
                            "            <face_culling mode=\"back\"/>\n"
                            "            <polygon_filling mode=\"fill\"/>\n"
                            "        </gpu_state>\n"
                            "\n"
                            "        <params>\n"
                            "            <param shader=\"fragment\" name=\"paramName\" type=\"int\" value=\"50\"/>"
                            "        </params>\n"
                            "    </resource>\n"
                            "</resources>");

  const auto& shaderDeclaration = manager->getResourceConfig<GLShader, ShaderResourceConfig>("vertex");

  REQUIRE(shaderDeclaration->resourcePath == "../resources/shaders/debug_vertex_shader.glsl");
  REQUIRE(shaderDeclaration->shaderType == ShaderType::Vertex);

  const auto& materialDeclaration = manager->getResourceConfig<Material, MaterialResourceConfig>("material");

  REQUIRE(materialDeclaration->shadersPipeline.vertexShaderId == "vertex");
  REQUIRE(materialDeclaration->shadersPipeline.fragmentShaderId == "fragment");

  REQUIRE(materialDeclaration->gpuState.blendingMode == BlendingMode::Alpha_OneMinusAlpha);
  REQUIRE(materialDeclaration->gpuState.depthWritingMode == DepthWritingMode::Enabled);
  REQUIRE(materialDeclaration->gpuState.depthTestMode == DepthTestMode::Less);
  REQUIRE(materialDeclaration->gpuState.faceCullingMode == FaceCullingMode::Back);
  REQUIRE(materialDeclaration->gpuState.polygonFillingMode == PolygonFillingMode::Fill);

  REQUIRE(materialDeclaration->parameters[0].shaderType == ShaderType::Fragment);
  REQUIRE(materialDeclaration->parameters[0].name == "paramName");
  REQUIRE(materialDeclaration->parameters[0].type == MaterialResourceConfig::ShaderParamType::Int);
  REQUIRE(std::get<int>(materialDeclaration->parameters[0].value) == 50);
}


TEST_CASE("resource_inplace_creation", "[resources]")
{
  std::shared_ptr<ResourcesManager> manager = generateTestResourcesManager();

  ResourceHandle<CollisionShape> shape =
    manager->createResourceInPlace<CollisionShape>(CollisionShapeSphere(1.0f));

  REQUIRE(shape.get() != nullptr);
  REQUIRE(MathUtils::isEqual(std::get<CollisionShapeSphere>(*shape).getRadius(), 1.0f));
}

#include <catch2/catch.hpp>

#include <Engine/Modules/ResourceManagement/ResourceManagementModule.h>
#include <Engine/Modules/Graphics/Resources/ShaderResource.h>
#include <Engine/Modules/Graphics/Resources/MaterialResource.h>

TEST_CASE("resources_maps_loading", "[resources]")
{
  std::shared_ptr<ResourceManager> manager = std::make_shared<ResourceManager>();
  manager->declareResourceType<ShaderResource>("shader");
  manager->declareResourceType<MaterialResource>("material");

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

  const auto& shaderDeclaration = manager->getResourceDeclaration("vertex");
  auto shaderParams = shaderDeclaration.getParameters<ShaderResourceParameters>();

  REQUIRE(std::get<ResourceSourceFile>(shaderDeclaration.source).path == "../resources/shaders/debug_vertex_shader.glsl");
  REQUIRE(shaderParams.shaderType == GL_VERTEX_SHADER);

  const auto& materialDeclaration = manager->getResourceDeclaration("material");
  auto materialParams = materialDeclaration.getParameters<MaterialResourceParameters>();

  REQUIRE(std::get_if<ResourceSourceDeclaration>(&materialDeclaration.source));

  REQUIRE(materialParams.shadersPipeline.vertexShaderId == "vertex");
  REQUIRE(materialParams.shadersPipeline.fragmentShaderId == "fragment");

  REQUIRE(materialParams.gpuState.blendingMode == BlendingMode::Alpha_OneMinusAlpha);
  REQUIRE(materialParams.gpuState.depthWritingMode == DepthWritingMode::Enabled);
  REQUIRE(materialParams.gpuState.depthTestMode == DepthTestMode::Less);
  REQUIRE(materialParams.gpuState.faceCullingMode == FaceCullingMode::Back);
  REQUIRE(materialParams.gpuState.polygonFillingMode == PolygonFillingMode::Fill);

  REQUIRE(materialParams.parameters[0].shaderType == GL_FRAGMENT_SHADER);
  REQUIRE(materialParams.parameters[0].name == "paramName");
  REQUIRE(materialParams.parameters[0].type == MaterialResourceParameters::ShaderParamType::Int);
  REQUIRE(std::get<int>(materialParams.parameters[0].value) == 50);
}

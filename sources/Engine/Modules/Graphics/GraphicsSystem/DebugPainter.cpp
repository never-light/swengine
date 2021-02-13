#include "precompiled.h"

#pragma hdrstop

#include "DebugPainter.h"

#include <utility>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/GraphicsSystem/FrameStats.h"

ResourceHandle<Mesh> DebugPainter::s_sphere{};
ResourceHandle<Mesh> DebugPainter::s_box{};

std::shared_ptr<GLShadersPipeline> DebugPainter::s_debugShaderPipeline = nullptr;

std::vector<std::unique_ptr<GLMaterial>> DebugPainter::s_primitivesMaterials;

std::shared_ptr<GraphicsScene> DebugPainter::s_graphicsScene = nullptr;

std::vector<std::unique_ptr<Mesh>> DebugPainter::s_primitivesGeometry;

std::vector<glm::mat4> DebugPainter::s_primitivesTransforms;

std::vector<RenderTask> DebugPainter::s_debugRenderQueue;

void DebugPainter::initialize(std::shared_ptr<ResourcesManager> resourceManager,
  std::shared_ptr<GraphicsScene> graphicsScene)
{
  s_graphicsScene = std::move(graphicsScene);

  s_sphere = resourceManager->getResource<Mesh>("mesh_identity_sphere");
  s_box = resourceManager->getResource<Mesh>("mesh_identity_box");

  ResourceHandle<GLShader> vertexShader = resourceManager->
    getResource<GLShader>("debug_vertex_shader");

  ResourceHandle<GLShader> fragmentShader = resourceManager
    ->getResource<GLShader>("debug_fragment_shader");

  SW_ASSERT(fragmentShader->getType() == ShaderType::Fragment);

  s_debugShaderPipeline = std::make_shared<GLShadersPipeline>(vertexShader, fragmentShader,
    std::optional<ResourceHandle<GLShader>>());

  s_primitivesTransforms.reserve(65000);
  s_primitivesGeometry.reserve(65000);
  s_debugRenderQueue.reserve(65000);
}


void DebugPainter::deinitialize()
{
  s_sphere = {};
  s_box = {};

  s_debugShaderPipeline.reset();
  s_graphicsScene.reset();

  s_primitivesMaterials.clear();
  s_primitivesGeometry.clear();
  s_primitivesTransforms.clear();

  s_debugRenderQueue.clear();
}

void DebugPainter::renderSegment(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
{
  createRenderingTask({start, end}, glm::identity<glm::mat4>(),
    GL_LINES, color, false);
}

void DebugPainter::renderVector(const glm::vec3& origin, const glm::vec3& direction, const glm::vec4& color)
{
  glm::vec3 start = origin;
  glm::vec3 end = origin + direction;

  renderSegment(start, end, color);
  renderSphere(end, 0.02f, color, false);
}

void DebugPainter::renderBasis(const glm::vec3& origin, const glm::vec3& x, const glm::vec3& y, const glm::vec3& z)
{
  renderVector(origin, x, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  renderVector(origin, y, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  renderVector(origin, z, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
}

void DebugPainter::renderSphere(const glm::vec3& centerPosition, float radius, const glm::vec4& color, bool wireframe)
{
  glm::mat4 transformation = glm::translate(glm::identity<glm::mat4>(), centerPosition) *
    glm::scale(glm::identity<glm::mat4>(), glm::vec3(radius));

  createRenderingTask(s_sphere.get(), transformation,
    GL_TRIANGLES, color, wireframe);
}

void DebugPainter::renderSphere(const Sphere& sphere, const glm::vec4& color, bool wireframe)
{
  renderSphere(sphere.getOrigin(), sphere.getRadius(), color, wireframe);
}

void DebugPainter::renderBox(const glm::vec3& centerPosition,
  const glm::vec3& halfSize,
  const glm::quat& orientation,
  const glm::vec4& color,
  bool wireframe)
{
  glm::mat4 transformation = glm::translate(glm::identity<glm::mat4>(), centerPosition) *
    glm::toMat4(orientation) *
    glm::scale(glm::identity<glm::mat4>(), halfSize * 2.0f);

  createRenderingTask(s_box.get(), transformation,
    GL_TRIANGLES, color, wireframe);
}

void DebugPainter::renderFrustum(const glm::mat4x4& view, const glm::mat4x4& projection,
  const glm::vec4& color, bool wireframe)
{
  glm::mat4 inverseViewProjection =
    glm::inverse(projection * view) * glm::scale(glm::identity<glm::mat4>(), glm::vec3(2.0f));

  createRenderingTask(s_box.get(), inverseViewProjection,
    GL_TRIANGLES, color, wireframe);
}

void DebugPainter::renderFrustum(const Frustum& frustum, const glm::vec4& color)
{
  std::array<glm::vec3, 8> frustumCorners = frustum.getCorners();

  createRenderingTask({
      frustumCorners[0],
      frustumCorners[1],
      frustumCorners[1],
      frustumCorners[2],
      frustumCorners[2],
      frustumCorners[3],
      frustumCorners[3],
      frustumCorners[0],

      frustumCorners[4],
      frustumCorners[5],
      frustumCorners[5],
      frustumCorners[6],
      frustumCorners[6],
      frustumCorners[7],
      frustumCorners[7],
      frustumCorners[4],

      frustumCorners[0],
      frustumCorners[4],
      frustumCorners[1],
      frustumCorners[5],
      frustumCorners[2],
      frustumCorners[6],
      frustumCorners[3],
      frustumCorners[7]
    }, glm::identity<glm::mat4>(),
    GL_LINES, color, false);
}

void DebugPainter::renderAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color, bool wireframe)
{
  glm::vec3 origin = (max + min) * 0.5f;
  glm::vec3 scale = (max - min);

  glm::mat4 transformation = glm::translate(glm::identity<glm::mat4>(), origin) *
    glm::scale(glm::identity<glm::mat4>(), scale);

  createRenderingTask(s_box.get(), transformation,
    GL_TRIANGLES, color, wireframe);
}

void DebugPainter::renderAABB(const AABB& aabb, const glm::vec4& color, bool wireframe)
{
  renderAABB(aabb.getMin(), aabb.getMax(), color, wireframe);
}

void DebugPainter::flushRenderQueue(GLGraphicsContext* graphicsContext)
{
  for (const auto& queueItem : s_debugRenderQueue) {
    s_graphicsScene->getFrameStats().increaseSubMeshesCount(1);

    size_t primitivesCount = 0;

    if (queueItem.primitivesType == GL_TRIANGLES) {
      primitivesCount = queueItem.mesh->getSubMeshIndicesCount(0) / 3;
    }
    else if (queueItem.primitivesType == GL_LINES) {
      primitivesCount = queueItem.mesh->getSubMeshIndicesCount(0) / 2;
    }

    s_graphicsScene->getFrameStats().increasePrimitivesCount(primitivesCount);

    graphicsContext->scheduleRenderTask(queueItem);
  }
}

void DebugPainter::resetRenderQueue()
{
  s_debugRenderQueue.clear();
  s_primitivesGeometry.clear();
  s_primitivesTransforms.clear();
}

void DebugPainter::renderTriangle(const glm::vec3& v1,
  const glm::vec3& v2,
  const glm::vec3& v3,
  const glm::vec4& color,
  bool wireframe)
{
  createRenderingTask({v1, v2, v3}, glm::identity<glm::mat4>(),
    GL_TRIANGLES, color, wireframe);
}

void DebugPainter::createRenderingTask(
  const std::vector<glm::vec3>& points,
  const glm::mat4& transformationMatrix,
  GLenum primitivesType,
  const glm::vec4& color,
  bool wireframe)
{
  s_primitivesGeometry.push_back(std::make_unique<Mesh>());
  Mesh& mesh = **s_primitivesGeometry.rbegin();

  mesh.setVertices(points);
  mesh.setNormals(std::vector<glm::vec3>(points.size()));
  mesh.setUV(std::vector<glm::vec2>(points.size()));

  std::vector<uint16_t> indices(points.size());

  for (size_t pointIndex = 0; pointIndex < points.size(); pointIndex++) {
    indices[pointIndex] = static_cast<uint16_t>(pointIndex);
  }

  mesh.addSubMesh(indices);

  createRenderingTask(&mesh, transformationMatrix, primitivesType, color, wireframe);
}

void DebugPainter::createRenderingTask(Mesh* mesh,
  const glm::mat4& transformationMatrix,
  GLenum primitivesType,
  const glm::vec4& color,
  bool wireframe)
{
  GpuStateParameters gpuStateParameters;
  gpuStateParameters.setDepthTestMode(DepthTestMode::Less);
  gpuStateParameters.setBlendingMode(BlendingMode::Alpha_OneMinusAlpha);
  gpuStateParameters.setFaceCullingMode(FaceCullingMode::Disabled);
  gpuStateParameters.setDepthWritingMode(DepthWritingMode::Enabled);
  gpuStateParameters.setPolygonFillingMode(((wireframe) ? PolygonFillingMode::Wireframe : PolygonFillingMode::Fill));

  auto parametersSet = std::make_unique<ShadingParametersGenericSet>();
  parametersSet->setShaderParameter(ShaderType::Fragment, "color", color);

  s_primitivesMaterials.push_back(std::make_unique<GLMaterial>(
    RenderingStage::ForwardDebug,
    s_debugShaderPipeline,
    gpuStateParameters,
    std::move(parametersSet)));

  s_primitivesTransforms.push_back(transformationMatrix);

  s_debugRenderQueue.push_back(RenderTask{
    .material = s_primitivesMaterials.rbegin()->get(),
    .mesh = mesh,
    .subMeshIndex = 0,
    .transform = &*s_primitivesTransforms.rbegin(),
    .primitivesType = primitivesType
  });
}

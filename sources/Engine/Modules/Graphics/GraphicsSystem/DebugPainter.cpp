#include "precompiled.h"

#pragma hdrstop

#include "DebugPainter.h"

#include "Modules/Graphics/GraphicsSystem/SharedGraphicsState.h"

std::shared_ptr<Mesh> DebugPainter::s_sphere = nullptr;

std::shared_ptr<Mesh> DebugPainter::s_box = nullptr;

std::shared_ptr<GLShadersPipeline> DebugPainter::s_debugShaderPipeline = nullptr;

std::unique_ptr<GLMaterial> DebugPainter::s_debugMaterial = nullptr;

std::shared_ptr<SharedGraphicsState> DebugPainter::s_sharedGraphicsState = nullptr;

std::vector<std::unique_ptr<GLGeometryStore>> DebugPainter::s_primitivesGeometry;

std::vector<DebugRenderQueueItem> DebugPainter::s_debugRenderQueue;

void DebugPainter::initialize(std::shared_ptr<ResourceManager> resourceManager,
                              std::shared_ptr<SharedGraphicsState> sharedGraphicsState) {
  s_sharedGraphicsState = sharedGraphicsState;

  s_sphere = resourceManager->getResourceFromInstance<MeshResource>("mesh_identity_sphere")->getMesh();
  s_box = resourceManager->getResourceFromInstance<MeshResource>("mesh_identity_box")->getMesh();

  std::shared_ptr<GLShader> vertexShader = resourceManager->
      getResourceFromInstance<ShaderResource>("debug_vertex_shader")->getShader();

  std::shared_ptr<GLShader> fragmentShader = resourceManager
      ->getResourceFromInstance<ShaderResource>("debug_fragment_shader")->getShader();

  s_debugShaderPipeline = std::make_shared<GLShadersPipeline>(vertexShader, fragmentShader, nullptr);

  s_debugMaterial = std::make_unique<GLMaterial>();
  s_debugMaterial->setShadersPipeline(s_debugShaderPipeline);
  s_debugMaterial->setDepthTestMode(DepthTestMode::Less);
  s_debugMaterial->setBlendingMode(BlendingMode::Alpha_OneMinusAlpha);
  s_debugMaterial->setFaceCullingMode(FaceCullingMode::Disabled);
  s_debugMaterial->setDepthWritingMode(DepthWritingMode::Enabled);
}

void DebugPainter::renderSegment(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color) {
  s_primitivesGeometry.push_back(std::unique_ptr<GLGeometryStore>(createGeometryStore({start, end})));
  s_debugRenderQueue.push_back({(*s_primitivesGeometry.rbegin()).get(), glm::identity<glm::mat4>(),
                                   color, false, GL_LINES});
}

void DebugPainter::renderVector(const glm::vec3& origin, const glm::vec3& direction, const glm::vec4& color) {
  glm::vec3 start = origin;
  glm::vec3 end = origin + direction;

  renderSegment(start, end, color);
  renderSphere(end, 0.02f, color, false);
}

void DebugPainter::renderBasis(const glm::vec3& origin, const glm::vec3& x, const glm::vec3& y, const glm::vec3& z) {
  renderVector(origin, x, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  renderVector(origin, y, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  renderVector(origin, z, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
}

void DebugPainter::renderSphere(const glm::vec3& centerPosition, float radius, const glm::vec4& color, bool wireframe) {
  glm::mat4 transformation = glm::translate(glm::identity<glm::mat4>(), centerPosition) *
      glm::scale(glm::identity<glm::mat4>(), glm::vec3(radius));

  s_debugRenderQueue.push_back({s_sphere->getGeometryStore(), transformation, color, wireframe});
}

void DebugPainter::renderSphere(const Sphere& sphere, const glm::vec4& color, bool wireframe) {
  renderSphere(sphere.getOrigin(), sphere.getRadius(), color, wireframe);
}

void DebugPainter::renderBox(const glm::vec3& centerPosition,
                             const glm::vec3& halfSize,
                             const glm::quat& orientation,
                             const glm::vec4& color,
                             bool wireframe) {
  glm::mat4 transformation = glm::translate(glm::identity<glm::mat4>(), centerPosition) *
      glm::toMat4(orientation) *
      glm::scale(glm::identity<glm::mat4>(), halfSize * 2.0f);

  s_debugRenderQueue.push_back({s_box->getGeometryStore(), transformation, color, wireframe});
}

void DebugPainter::renderFrustum(const glm::mat4x4& view, const glm::mat4x4& projection,
                                 const glm::vec4& color, bool wireframe) {
  glm::mat4 inversedViewProjection =
      glm::inverse(projection * view) * glm::scale(glm::identity<glm::mat4>(), glm::vec3(2.0f));
  s_debugRenderQueue.push_back({s_box->getGeometryStore(), inversedViewProjection, color, wireframe});
}

void DebugPainter::renderFrustum(const Frustum& frustum, const glm::vec4& color) {
  std::array<glm::vec3, 8> frustumCorners = frustum.getCorners();

  s_primitivesGeometry.push_back(std::unique_ptr<GLGeometryStore>(createGeometryStore({
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
                                                                                     })));

  s_debugRenderQueue.push_back({(*s_primitivesGeometry.rbegin()).get(), glm::identity<glm::mat4>(),
                                   color, false, GL_LINES});

}

void DebugPainter::renderAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color, bool wireframe) {
  glm::vec3 origin = (max + min) * 0.5f;
  glm::vec3 scale = (max - min);

  glm::mat4 transformation = glm::translate(glm::identity<glm::mat4>(), origin) *
      glm::scale(glm::identity<glm::mat4>(), scale);

  s_debugRenderQueue.push_back({s_box->getGeometryStore(), transformation, color, wireframe});
}

void DebugPainter::renderAABB(const AABB& aabb, const glm::vec4& color, bool wireframe) {
  renderAABB(aabb.getMin(), aabb.getMax(), color, wireframe);
}

void DebugPainter::flushRenderQueue(GLGraphicsContext* graphicsContext) {
  for (const auto& queueItem : s_debugRenderQueue) {
    if (queueItem.isWireframe) {
      graphicsContext->setPolygonFillingMode(PolygonFillingMode::Wireframe);
    }
    else {
      graphicsContext->setPolygonFillingMode(PolygonFillingMode::Fill);
    }

    GLShader* vertexShader = s_debugShaderPipeline->getShader(GL_VERTEX_SHADER);
    vertexShader->setParameter("transform.localToWorld", queueItem.transformationMatrix);

    Camera* camera = s_sharedGraphicsState->getActiveCamera().get();
    vertexShader->setParameter("scene.worldToCamera", camera->getViewMatrix());
    vertexShader->setParameter("scene.cameraToProjection", camera->getProjectionMatrix());

    GLShader* fragmentShader = s_debugShaderPipeline->getShader(GL_FRAGMENT_SHADER);
    fragmentShader->setParameter("color", queueItem.color);

    s_sharedGraphicsState->getFrameStats().increaseSubMeshesCount(1);

    size_t primitivesCount = 0;

    if (queueItem.primitivesType == GL_TRIANGLES) {
      primitivesCount = queueItem.geometry->getIndicesCount() / 3;
    }
    else if (queueItem.primitivesType == GL_LINES) {
      primitivesCount = queueItem.geometry->getIndicesCount() / 2;
    }

    s_sharedGraphicsState->getFrameStats().increasePrimitivesCount(primitivesCount);

    graphicsContext->executeRenderTask({s_debugMaterial.get(), queueItem.geometry,
                                           0, queueItem.geometry->getIndicesCount(),
                                           queueItem.primitivesType});
  }

  s_debugRenderQueue.clear();
  s_primitivesGeometry.clear();
}

GLGeometryStore* DebugPainter::createGeometryStore(const std::vector<glm::vec3>& points) {
  std::vector<VertexPos3Norm3UV> vertices(points.size());
  std::vector<uint16_t> indices(points.size());

  for (size_t pointIndex = 0; pointIndex < points.size(); pointIndex++) {
    vertices[pointIndex].pos = points[pointIndex];
    indices[pointIndex] = static_cast<uint16_t>(pointIndex);
  }

  return new GLGeometryStore(vertices, indices);
}

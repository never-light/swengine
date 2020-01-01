#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/GraphicsSystem/Mesh.h"
#include "Modules/Graphics/GraphicsSystem/Camera.h"

#include "Modules/Graphics/Resources/MeshResource.h"
#include "Modules/Graphics/Resources/ShaderResource.h"

#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "Modules/Graphics/OpenGL/GLShadersPipeline.h"

#include "Modules/Math/geometry.h"

struct DebugRenderQueueItem {
    GLGeometryStore* geometry;
    glm::mat4x4 transformationMatrix;
    glm::vec4 color = glm::vec4(0.0f);
    bool isWireframe = false;
    GLenum primitivesType = GL_TRIANGLES;
};

class DebugPainter
{
public:
    static void initialize(std::shared_ptr<ResourceManager> resourceManager,
                           std::shared_ptr<SharedGraphicsState> sharedGraphicsState);

    static void renderSegment(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
    static void renderVector(const glm::vec3& origin, const glm::vec3& direction, const glm::vec4& color);
    static void renderBasis(const glm::vec3& origin, const glm::vec3& x, const glm::vec3& y, const glm::vec3& z);

    static void renderSphere(const glm::vec3& centerPosition, float radius, const glm::vec4& color = {}, bool wireframe = true);
    static void renderSphere(const Sphere& sphere, const glm::vec4& color = {}, bool wireframe = true);

    static void renderBox(const glm::vec3& centerPosition, const glm::vec3& halfSize,
                          const glm::quat& orientation, const glm::vec4& color = {}, bool wireframe = true);

    static void renderFrustum(const glm::mat4x4& view, const glm::mat4x4& projection,
                              const glm::vec4& color = {}, bool wireframe = true);

    static void renderFrustum(const Frustum& frustum,
                              const glm::vec4& color = {});

    static void flushRenderQueue(GLGraphicsContext* graphicsContext);

private:
    DebugPainter() = delete;

    static GLGeometryStore* createGeometryStore(const std::vector<glm::vec3>& points);

private:
    static std::shared_ptr<Mesh> s_sphere;
    static std::shared_ptr<Mesh> s_box;

    static std::unique_ptr<GLShadersPipeline> s_debugShaderPipeline;
    static std::shared_ptr<SharedGraphicsState> s_sharedGraphicsState;

    static std::vector<std::unique_ptr<GLGeometryStore>> s_primitivesGeomery;
    static std::vector<DebugRenderQueueItem> s_debugRenderQueue;
};


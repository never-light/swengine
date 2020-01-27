 #pragma once

#include <cstddef>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_precision.hpp>

#include "GL.h"
#include "GLDebug.h"

struct VertexPos3Norm3UV {
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 norm = { 0.0f, 0.0f, 0.0f };
    glm::vec2 uv = { 0.0f, 0.0f };
};

struct VertexPos3Norm3UVSkinned {
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 norm = { 0.0f, 0.0f, 0.0f };
    glm::vec2 uv = { 0.0f, 0.0f };
    glm::u8vec4 bonesIds = { 0, 0, 0, 0 };
    glm::u8vec4 bonesWeights = { 0, 0, 0, 0 };
};

struct VertexPos3Color4 {
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::vec4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
};

class GLGeometryStore
{
public:
    GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices);
    GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices, const std::vector<std::uint16_t>& indices);

    GLGeometryStore(const std::vector<VertexPos3Color4>& vertices);
    GLGeometryStore(const std::vector<VertexPos3Color4>& vertices, const std::vector<std::uint16_t>& indices);

    GLGeometryStore(const std::vector<VertexPos3Norm3UVSkinned>& vertices);
    GLGeometryStore(const std::vector<VertexPos3Norm3UVSkinned>& vertices, const std::vector<std::uint16_t>& indices);

    ~GLGeometryStore();

    size_t getVerticesCount() const;
    size_t getIndicesCount() const;

    bool isIndexed() const;

    void draw(GLenum primitivesType = GL_TRIANGLES) const;
    void drawRange(size_t start, size_t count, GLenum primitivesType = GL_TRIANGLES) const;

private:
    template <class T>
    void createBuffersAndVAO(const std::vector<T>& vertices, const std::vector<std::uint16_t>& indices);

    template <class T>
    void createVAOAndSetupAttributes();

private:
    GLuint m_vertexBuffer = 0;
    GLuint m_indexBuffer = 0;
    GLuint m_vertexArrayObject = 0;

    size_t m_verticesCount = 0;
    size_t m_indicesCount = 0;
};

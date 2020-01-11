 #pragma once

#include <cstddef>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "GL.h"
#include "GLDebug.h"

struct VertexPos3Norm3UV {
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 norm = { 0.0f, 0.0f, 0.0f };
    glm::vec2 uv = { 0.0f, 0.0f };
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
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_vertexArrayObject;

    size_t m_verticesCount;
    size_t m_indicesCount;
};

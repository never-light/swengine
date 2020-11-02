#pragma once

#include "GLGeometryStore.h"
#include "GLDebug.h"

template<class T>
void GLGeometryStore::setupVertexBuffers(const T& vertices)
{
  SW_ASSERT(false);
}


template<class T, class DescriptionType>
void GLGeometryStore::setupVAO(const T& vertices, const std::vector<std::uint16_t>& indices)
{
  // Create and fill vertex buffer
  GL_CALL_BLOCK_BEGIN();

  glCreateVertexArrays(1, &m_vertexArrayObject);

  setupVertexBuffers<T>(vertices);

  for (const VertexFormatAttributeSpec& attribData : DescriptionType::s_vertexFormatAttributes) {
    switch (attribData.type) {
      case GL_FLOAT:
        glVertexArrayAttribFormat(m_vertexArrayObject,
          attribData.attribIndex, attribData.size,
          attribData.type, attribData.normalized, attribData.relativeOffset);
        break;

      case GL_UNSIGNED_BYTE:
        glVertexArrayAttribIFormat(m_vertexArrayObject, attribData.attribIndex, attribData.size,
          attribData.type,
          attribData.relativeOffset);
        break;

      default:
        SW_ASSERT(false);
    }

    glVertexArrayAttribBinding(m_vertexArrayObject, attribData.attribIndex, attribData.bindingIndex);
    glEnableVertexArrayAttrib(m_vertexArrayObject, attribData.attribIndex);
  }

  GL_CALL_BLOCK_END();

  // Create and fill index buffer if is is needed
  if (!indices.empty()) {
    auto indicesMemorySize = static_cast<GLsizeiptr>(indices.size() * sizeof(indices[0]));

    GL_CALL_BLOCK_BEGIN();
    glCreateBuffers(1, &m_indexBuffer);
    glNamedBufferStorage(m_indexBuffer, indicesMemorySize, indices.data(), 0);

    m_indicesCount = indices.size();

    glVertexArrayElementBuffer(m_vertexArrayObject, m_indexBuffer);

    GL_CALL_BLOCK_END();
  }

}

template<>
void GLGeometryStore::setupVertexBuffers<>(const VerticesPos3Norm3UVSoA& vertices)
{
  m_verticesCount = vertices.positions->size();

  glCreateBuffers(3, &m_vertexBuffers[0]);

  glNamedBufferStorage(m_vertexBuffers[0],
    static_cast<GLsizeiptr>(vertices.positions->size() * sizeof(glm::vec3)),
    vertices.positions->data(), 0);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 0, m_vertexBuffers[0], 0, sizeof(glm::vec3));

  glNamedBufferStorage(m_vertexBuffers[1],
    static_cast<GLsizeiptr>(vertices.normals->size() * sizeof(glm::vec3)),
    vertices.normals->data(), 0);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 1, m_vertexBuffers[1], 0, sizeof(glm::vec3));

  glNamedBufferStorage(m_vertexBuffers[2],
    static_cast<GLsizeiptr>(vertices.uv->size() * sizeof(glm::vec2)),
    vertices.uv->data(), 0);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 2, m_vertexBuffers[2], 0, sizeof(glm::vec2));
}

template<>
void GLGeometryStore::setupVertexBuffers<>(const std::vector<VertexPos3Norm3UV>& vertices)
{
  m_verticesCount = vertices.size();

  glCreateBuffers(1, &m_vertexBuffers[0]);

  glNamedBufferStorage(m_vertexBuffers[0],
    static_cast<GLsizeiptr>(vertices.size() * sizeof(VertexPos3Norm3UV)),
    vertices.data(), 0);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 0, m_vertexBuffers[0], 0, sizeof(VertexPos3Norm3UV));
}

template<>
void GLGeometryStore::setupVertexBuffers<>(const VertexPos3Norm3UVSkinnedSoA& vertices)
{
  m_verticesCount = vertices.positions->size();

  glCreateBuffers(3, &m_vertexBuffers[0]);
  glCreateBuffers(2, &m_vertexBuffers[4]);

  glNamedBufferStorage(m_vertexBuffers[0],
    static_cast<GLsizeiptr>(vertices.positions->size() * sizeof(glm::vec3)),
    vertices.positions->data(), 0);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 0, m_vertexBuffers[0], 0, sizeof(glm::vec3));

  glNamedBufferStorage(m_vertexBuffers[1],
    static_cast<GLsizeiptr>(vertices.normals->size() * sizeof(glm::vec3)),
    vertices.normals->data(), 0);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 1, m_vertexBuffers[1], 0, sizeof(glm::vec3));

  glNamedBufferStorage(m_vertexBuffers[2],
    static_cast<GLsizeiptr>(vertices.uv->size() * sizeof(glm::vec2)),
    vertices.uv->data(), 0);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 2, m_vertexBuffers[2], 0, sizeof(glm::vec2));

  glNamedBufferStorage(m_vertexBuffers[4],
    static_cast<GLsizeiptr>(vertices.bonesIds->size() * sizeof(glm::u8vec4)),
    vertices.bonesIds->data(), 0);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 4, m_vertexBuffers[4], 0, sizeof(glm::u8vec4));

  glNamedBufferStorage(m_vertexBuffers[5],
    static_cast<GLsizeiptr>(vertices.bonesWeights->size() * sizeof(glm::u8vec4)),
    vertices.bonesWeights->data(), 0);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 5, m_vertexBuffers[5], 0, sizeof(glm::u8vec4));

}

template<>
void GLGeometryStore::setupVertexBuffers<>(const VertexPos3Color4SoA& vertices)
{
  m_verticesCount = vertices.positions->size();

  glCreateBuffers(2, &m_vertexBuffers[0]);

  glNamedBufferStorage(m_vertexBuffers[0],
    static_cast<GLsizeiptr>(vertices.positions->size() * sizeof(glm::vec3)),
    vertices.positions->data(), 0);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 0, m_vertexBuffers[0], 0, sizeof(glm::vec3));

  glNamedBufferStorage(m_vertexBuffers[1],
    static_cast<GLsizeiptr>(vertices.colors->size() * sizeof(glm::vec4)),
    vertices.colors->data(), 0);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 1, m_vertexBuffers[1], 0, sizeof(glm::vec4));
}

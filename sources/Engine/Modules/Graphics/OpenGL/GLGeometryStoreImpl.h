#pragma once

#include "GLGeometryStore.h"
#include "GLDebug.h"

template<class T>
void GLGeometryStore::setupVertexBuffers(const T& vertices, GLenum storageFlags, size_t minStorageCapacity)
{
  ARG_UNUSED(storageFlags);
  ARG_UNUSED(minStorageCapacity);

  SW_ASSERT(false);
}

template<class T, class DescriptionType>
void GLGeometryStore::setupVAO(const T& vertices,
  const std::vector<std::uint16_t>& indices,
  GLenum storageFlags,
  size_t minStorageCapacity)
{
  // Create and fill vertex buffer
  GL_CALL_BLOCK_BEGIN();

  glCreateVertexArrays(1, &m_vertexArrayObject);

  setupVertexBuffers<T>(vertices, storageFlags, minStorageCapacity);

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
    GL_CALL_BLOCK_BEGIN();
    glCreateBuffers(1, &m_indexBuffer);

    m_indicesStorageCapacity = std::max(indices.size(), minStorageCapacity);

    auto storageIndices = indices;

    if (storageIndices.size() < m_indicesStorageCapacity) {
      storageIndices.resize(m_indicesStorageCapacity, {});
    }

    glNamedBufferStorage(m_indexBuffer,
      static_cast<GLsizeiptr>(storageIndices.size() * sizeof(indices[0])),
      storageIndices.data(),
      storageFlags);

    m_indicesCount = indices.size();

    glVertexArrayElementBuffer(m_vertexArrayObject, m_indexBuffer);

    GL_CALL_BLOCK_END();
  }

}

template<>
void GLGeometryStore::setupVertexBuffers<>(const VerticesPos3Norm3UVSoA& vertices,
  GLenum storageFlags,
  size_t minStorageCapacity)
{
  m_verticesCount = vertices.positions->size();
  m_verticesStorageCapacity = std::max(vertices.positions->size(), minStorageCapacity);

  auto positions = *vertices.positions;
  auto normals = *vertices.normals;
  auto uv = *vertices.uv;

  if (positions.size() < m_verticesStorageCapacity) {
    positions.resize(m_verticesStorageCapacity, {});
  }

  if (normals.size() < m_verticesStorageCapacity) {
    normals.resize(m_verticesStorageCapacity, {});
  }

  if (uv.size() < m_verticesStorageCapacity) {
    uv.resize(m_verticesStorageCapacity, {});
  }

  glCreateBuffers(3, &m_vertexBuffers[0]);

  glNamedBufferStorage(m_vertexBuffers[0],
    static_cast<GLsizeiptr>(positions.size() * sizeof(glm::vec3)),
    positions.data(), storageFlags);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 0, m_vertexBuffers[0], 0, sizeof(glm::vec3));

  glNamedBufferStorage(m_vertexBuffers[1],
    static_cast<GLsizeiptr>(normals.size() * sizeof(glm::vec3)),
    normals.data(), storageFlags);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 1, m_vertexBuffers[1], 0, sizeof(glm::vec3));

  glNamedBufferStorage(m_vertexBuffers[2],
    static_cast<GLsizeiptr>(uv.size() * sizeof(glm::vec2)),
    uv.data(), storageFlags);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 2, m_vertexBuffers[2], 0, sizeof(glm::vec2));
}

template<>
void GLGeometryStore::setupVertexBuffers<>(const std::vector<VertexPos3Norm3UV>& vertices,
  GLenum storageFlags,
  size_t minStorageCapacity)
{
  m_verticesCount = vertices.size();
  m_verticesStorageCapacity = std::max(vertices.size(), minStorageCapacity);

  auto storageVertices = vertices;

  if (storageVertices.size() < m_verticesStorageCapacity) {
    storageVertices.resize(m_verticesStorageCapacity, VertexPos3Norm3UV{});
  }

  glCreateBuffers(1, &m_vertexBuffers[0]);

  glNamedBufferStorage(m_vertexBuffers[0],
    static_cast<GLsizeiptr>(storageVertices.size() * sizeof(VertexPos3Norm3UV)),
    storageVertices.data(), storageFlags);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 0, m_vertexBuffers[0], 0, sizeof(VertexPos3Norm3UV));
}

template<>
void GLGeometryStore::setupVertexBuffers<>(const VertexPos3Norm3UVSkinnedSoA& vertices,
  GLenum storageFlags,
  size_t minStorageCapacity)
{
  m_verticesCount = vertices.positions->size();
  m_verticesStorageCapacity = std::max(vertices.positions->size(), minStorageCapacity);

  auto positions = *vertices.positions;
  auto normals = *vertices.normals;
  auto uv = *vertices.uv;
  auto bonesIDs = *vertices.bonesIds;
  auto bonesWeights = *vertices.bonesWeights;

  if (positions.size() < m_verticesStorageCapacity) {
    positions.resize(m_verticesStorageCapacity, {});
  }

  if (normals.size() < m_verticesStorageCapacity) {
    normals.resize(m_verticesStorageCapacity, {});
  }

  if (uv.size() < m_verticesStorageCapacity) {
    uv.resize(m_verticesStorageCapacity, {});
  }

  if (bonesIDs.size() < m_verticesStorageCapacity) {
    bonesIDs.resize(m_verticesStorageCapacity, {});
  }

  if (bonesWeights.size() < m_verticesStorageCapacity) {
    bonesWeights.resize(m_verticesStorageCapacity, {});
  }

  glCreateBuffers(3, &m_vertexBuffers[0]);
  glCreateBuffers(2, &m_vertexBuffers[4]);

  glNamedBufferStorage(m_vertexBuffers[0],
    static_cast<GLsizeiptr>(positions.size() * sizeof(glm::vec3)),
    positions.data(), storageFlags);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 0, m_vertexBuffers[0], 0, sizeof(glm::vec3));

  glNamedBufferStorage(m_vertexBuffers[1],
    static_cast<GLsizeiptr>(normals.size() * sizeof(glm::vec3)),
    normals.data(), storageFlags);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 1, m_vertexBuffers[1], 0, sizeof(glm::vec3));

  glNamedBufferStorage(m_vertexBuffers[2],
    static_cast<GLsizeiptr>(uv.size() * sizeof(glm::vec2)),
    uv.data(), storageFlags);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 2, m_vertexBuffers[2], 0, sizeof(glm::vec2));

  glNamedBufferStorage(m_vertexBuffers[4],
    static_cast<GLsizeiptr>(bonesIDs.size() * sizeof(glm::u8vec4)),
    bonesIDs.data(), storageFlags);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 4, m_vertexBuffers[4], 0, sizeof(glm::u8vec4));

  glNamedBufferStorage(m_vertexBuffers[5],
    static_cast<GLsizeiptr>(bonesWeights.size() * sizeof(glm::u8vec4)),
    bonesWeights.data(), storageFlags);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 5, m_vertexBuffers[5], 0, sizeof(glm::u8vec4));

}

template<>
void GLGeometryStore::setupVertexBuffers<>(const VertexPos3Color4SoA& vertices,
  GLenum storageFlags,
  size_t minStorageCapacity)
{
  m_verticesCount = vertices.positions->size();
  m_verticesStorageCapacity = std::max(vertices.positions->size(), minStorageCapacity);

  auto positions = *vertices.positions;
  auto colors = *vertices.colors;

  if (positions.size() < m_verticesStorageCapacity) {
    positions.resize(m_verticesStorageCapacity, {});
  }

  if (colors.size() < m_verticesStorageCapacity) {
    colors.resize(m_verticesStorageCapacity, {});
  }

  glCreateBuffers(2, &m_vertexBuffers[0]);

  glNamedBufferStorage(m_vertexBuffers[0],
    static_cast<GLsizeiptr>(positions.size() * sizeof(glm::vec3)),
    positions.data(), storageFlags);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 0, m_vertexBuffers[0], 0, sizeof(glm::vec3));

  glNamedBufferStorage(m_vertexBuffers[1],
    static_cast<GLsizeiptr>(colors.size() * sizeof(glm::vec4)),
    colors.data(), storageFlags);
  glVertexArrayVertexBuffer(m_vertexArrayObject, 1, m_vertexBuffers[1], 0, sizeof(glm::vec4));
}

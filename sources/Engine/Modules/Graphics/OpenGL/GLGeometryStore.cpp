#include "precompiled.h"

#pragma hdrstop

#include "GLGeometryStore.h"
#include "GLGeometryStoreImpl.h"

// TODO: get rid of static initialization here as it could read to unhandled exceptions

std::vector<VertexFormatAttributeSpec> VertexPos3Norm3UV::s_vertexFormatAttributes = {
  VertexFormatAttributeSpec{
    .attribIndex = 0,
    .bindingIndex = 0,
    .size = 3,
    .type = GL_FLOAT,
    .normalized = GL_FALSE,
    .relativeOffset = static_cast<GLuint>(offsetof(VertexPos3Norm3UV, pos))
  },
  VertexFormatAttributeSpec{
    .attribIndex = 1,
    .bindingIndex = 0,
    .size = 3,
    .type = GL_FLOAT,
    .normalized = GL_FALSE,
    .relativeOffset = static_cast<GLuint>(offsetof(VertexPos3Norm3UV, norm))
  },
  VertexFormatAttributeSpec{
    .attribIndex = 2,
    .bindingIndex = 0,
    .size = 2,
    .type = GL_FLOAT,
    .normalized = GL_FALSE,
    .relativeOffset = static_cast<GLuint>(offsetof(VertexPos3Norm3UV, uv))
  },
};

std::vector<VertexFormatAttributeSpec> VerticesPos3Norm3UVSoA::s_vertexFormatAttributes = {
  VertexFormatAttributeSpec{
    .attribIndex = 0,
    .bindingIndex = 0,
    .size = 3,
    .type = GL_FLOAT,
    .normalized = GL_FALSE,
    .relativeOffset = 0
  },
  VertexFormatAttributeSpec{
    .attribIndex = 1,
    .bindingIndex = 1,
    .size = 3,
    .type = GL_FLOAT,
    .normalized = GL_FALSE,
    .relativeOffset = 0
  },
  VertexFormatAttributeSpec{
    .attribIndex = 2,
    .bindingIndex = 2,
    .size = 2,
    .type = GL_FLOAT,
    .normalized = GL_FALSE,
    .relativeOffset = 0
  },
};

std::vector<VertexFormatAttributeSpec> VertexPos3Norm3UVSkinnedSoA::s_vertexFormatAttributes = {
  VertexFormatAttributeSpec{
    .attribIndex = 0,
    .bindingIndex = 0,
    .size = 3,
    .type = GL_FLOAT,
    .normalized = GL_FALSE,
    .relativeOffset = 0
  },
  VertexFormatAttributeSpec{
    .attribIndex = 1,
    .bindingIndex = 1,
    .size = 3,
    .type = GL_FLOAT,
    .normalized = GL_FALSE,
    .relativeOffset = 0
  },
  VertexFormatAttributeSpec{
    .attribIndex = 2,
    .bindingIndex = 2,
    .size = 2,
    .type = GL_FLOAT,
    .normalized = GL_FALSE,
    .relativeOffset = 0
  },
  VertexFormatAttributeSpec{
    .attribIndex = 4,
    .bindingIndex = 4,
    .size = 4,
    .type = GL_UNSIGNED_BYTE,
    .normalized = GL_FALSE,
    .relativeOffset = 0
  },
  VertexFormatAttributeSpec{
    .attribIndex = 5,
    .bindingIndex = 5,
    .size = 4,
    .type = GL_UNSIGNED_BYTE,
    .normalized = GL_FALSE,
    .relativeOffset = 0
  },
};

std::vector<VertexFormatAttributeSpec> VertexPos3Color4SoA::s_vertexFormatAttributes = {
  VertexFormatAttributeSpec{
    .attribIndex = 0,
    .bindingIndex = 0,
    .size = 3,
    .type = GL_FLOAT,
    .normalized = GL_FALSE,
    .relativeOffset = 0
  },
  VertexFormatAttributeSpec{
    .attribIndex = 1,
    .bindingIndex = 1,
    .size = 4,
    .type = GL_FLOAT,
    .normalized = GL_FALSE,
    .relativeOffset = 0
  },
};

GLGeometryStore::GLGeometryStore(const VertexPos3Color4SoA& vertices, GLenum storageFlags,
  size_t minStorageCapacity)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.colors->size());

  setupVAO<VertexPos3Color4SoA, VertexPos3Color4SoA>(vertices,
    std::vector<uint16_t>(),
    storageFlags,
    minStorageCapacity);
}

GLGeometryStore::GLGeometryStore(const VertexPos3Color4SoA& vertices,
  const std::vector<uint16_t>& indices,
  GLenum storageFlags,
  size_t minStorageCapacity)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.colors->size() &&
    !indices.empty());

  setupVAO<VertexPos3Color4SoA, VertexPos3Color4SoA>(vertices, indices, storageFlags, minStorageCapacity);
}

GLGeometryStore::GLGeometryStore(const VertexPos3Norm3UVSkinnedSoA& vertices,
  GLenum storageFlags,
  size_t minStorageCapacity)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.normals->size() &&
    vertices.positions->size() == vertices.uv->size() && vertices.positions->size() == vertices.bonesIds->size() &&
    vertices.positions->size() == vertices.bonesWeights->size());

  setupVAO<VertexPos3Norm3UVSkinnedSoA, VertexPos3Norm3UVSkinnedSoA>(vertices,
    std::vector<uint16_t>(),
    storageFlags,
    minStorageCapacity);
}

GLGeometryStore::GLGeometryStore(const VertexPos3Norm3UVSkinnedSoA& vertices,
  const std::vector<uint16_t>& indices, GLenum storageFlags, size_t minStorageCapacity)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.normals->size() &&
    vertices.positions->size() == vertices.uv->size() && vertices.positions->size() == vertices.bonesIds->size() &&
    vertices.positions->size() == vertices.bonesWeights->size() && !indices.empty());

  setupVAO<VertexPos3Norm3UVSkinnedSoA, VertexPos3Norm3UVSkinnedSoA>(vertices,
    indices,
    storageFlags,
    minStorageCapacity);
}

GLGeometryStore::GLGeometryStore(const VerticesPos3Norm3UVSoA& vertices, GLenum storageFlags,
  size_t minStorageCapacity)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.normals->size() &&
    vertices.positions->size() == vertices.uv->size());

  setupVAO<VerticesPos3Norm3UVSoA, VerticesPos3Norm3UVSoA>(vertices, std::vector<uint16_t>(), storageFlags,
    minStorageCapacity);
}

GLGeometryStore::GLGeometryStore(const VerticesPos3Norm3UVSoA& vertices,
  const std::vector<std::uint16_t>& indices, GLenum storageFlags, size_t minStorageCapacity)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.normals->size() &&
    vertices.positions->size() == vertices.uv->size());

  setupVAO<VerticesPos3Norm3UVSoA, VerticesPos3Norm3UVSoA>(vertices, indices, storageFlags, minStorageCapacity);
}

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices,
  GLenum storageFlags,
  size_t minStorageCapacity)
{
  SW_ASSERT(!vertices.empty());

  setupVAO<std::vector<VertexPos3Norm3UV>, VertexPos3Norm3UV>(vertices,
    std::vector<uint16_t>(),
    storageFlags,
    minStorageCapacity);
}

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices,
  const std::vector<std::uint16_t>& indices, GLenum storageFlags, size_t minStorageCapacity)
{
  SW_ASSERT(!vertices.empty() && !indices.empty());

  setupVAO<std::vector<VertexPos3Norm3UV>, VertexPos3Norm3UV>(vertices, indices, storageFlags, minStorageCapacity);
}

GLGeometryStore::~GLGeometryStore()
{
  for (auto& vertexBuffer : m_vertexBuffers) {
    if (vertexBuffer != 0) {
      glDeleteBuffers(1, &vertexBuffer);
    }
  }

  if (m_indexBuffer != 0) {
    glDeleteBuffers(1, &m_indexBuffer);
  }

  if (m_vertexArrayObject != 0) {
    glDeleteVertexArrays(1, &m_vertexArrayObject);
  }
}

size_t GLGeometryStore::getVerticesCount() const
{
  return m_verticesCount;
}

size_t GLGeometryStore::getIndicesCount() const
{
  return m_indicesCount;
}

bool GLGeometryStore::isIndexed() const
{
  return m_indicesCount > 0;
}

void GLGeometryStore::draw(GLenum primitivesType) const
{
  glBindVertexArray(m_vertexArrayObject);

  if (isIndexed()) {
    glDrawElements(primitivesType, static_cast<GLsizei>(m_indicesCount), GL_UNSIGNED_SHORT, nullptr);
  }
  else {
    glDrawArrays(primitivesType, 0, static_cast<GLsizei>(m_verticesCount));
  }
}

void GLGeometryStore::drawRange(size_t start, size_t count, GLenum primitivesType) const
{
  glBindVertexArray(m_vertexArrayObject);

  if (isIndexed()) {
    glDrawElements(primitivesType,
      static_cast<GLsizei>(count),
      GL_UNSIGNED_SHORT,
      reinterpret_cast<GLvoid*>(start * sizeof(uint16_t)));
  }
  else {
    glDrawArrays(primitivesType, static_cast<GLint>(start), static_cast<GLsizei>(count));
  }
}

void GLGeometryStore::updateVertices(const std::vector<VertexPos3Norm3UV>& vertices)
{
  SW_ASSERT(vertices.size() <= m_verticesStorageCapacity && "Storage reallocation is forbidden");

  glNamedBufferSubData(m_vertexBuffers[0], 0, vertices.size() * sizeof(VertexPos3Norm3UV), vertices.data());
  m_verticesCount = vertices.size();
}

void GLGeometryStore::updateVertices(const VerticesPos3Norm3UVSoA& vertices)
{
  size_t verticesCount = 0;

  if (vertices.positions != nullptr) {
    glNamedBufferSubData(m_vertexBuffers[0],
      0,
      vertices.positions->size() * sizeof(glm::vec3),
      vertices.positions->data());

    verticesCount = vertices.positions->size();
  }

  if (vertices.normals != nullptr) {
    SW_ASSERT(verticesCount == 0 || verticesCount == vertices.normals->size());

    glNamedBufferSubData(m_vertexBuffers[1],
      0,
      vertices.normals->size() * sizeof(glm::vec3),
      vertices.normals->data());

    verticesCount = vertices.normals->size();
  }

  if (vertices.uv != nullptr) {
    SW_ASSERT(verticesCount == 0 || verticesCount == vertices.uv->size());

    glNamedBufferSubData(m_vertexBuffers[2],
      0,
      vertices.uv->size() * sizeof(glm::vec2),
      vertices.uv->data());

    verticesCount = vertices.uv->size();
  }

  SW_ASSERT(verticesCount <= m_verticesStorageCapacity && "Storage reallocation is forbidden");
  m_verticesCount = verticesCount;
}

void GLGeometryStore::updateVertices(const VertexPos3Color4SoA& vertices)
{
  size_t verticesCount = 0;

  if (vertices.positions != nullptr) {
    glNamedBufferSubData(m_vertexBuffers[0],
      0,
      vertices.positions->size() * sizeof(glm::vec3),
      vertices.positions->data());

    verticesCount = vertices.positions->size();
  }

  if (vertices.colors != nullptr) {
    SW_ASSERT(verticesCount == 0 || verticesCount == vertices.colors->size());

    glNamedBufferSubData(m_vertexBuffers[1],
      0,
      vertices.colors->size() * sizeof(glm::vec4),
      vertices.colors->data());

    verticesCount = vertices.colors->size();
  }

  SW_ASSERT(verticesCount <= m_verticesStorageCapacity && "Storage reallocation is forbidden");
  m_verticesCount = verticesCount;
}

void GLGeometryStore::updateVertices(const VertexPos3Norm3UVSkinnedSoA& vertices)
{
  size_t verticesCount = 0;

  if (vertices.positions != nullptr) {
    glNamedBufferSubData(m_vertexBuffers[0],
      0,
      vertices.positions->size() * sizeof(glm::vec3),
      vertices.positions->data());

    verticesCount = vertices.positions->size();
  }

  if (vertices.normals != nullptr) {
    SW_ASSERT(verticesCount == 0 || verticesCount == vertices.normals->size());

    glNamedBufferSubData(m_vertexBuffers[1],
      0,
      vertices.normals->size() * sizeof(glm::vec3),
      vertices.normals->data());

    verticesCount = vertices.normals->size();
  }

  if (vertices.uv != nullptr) {
    SW_ASSERT(verticesCount == 0 || verticesCount == vertices.uv->size());

    glNamedBufferSubData(m_vertexBuffers[2],
      0,
      vertices.uv->size() * sizeof(glm::vec2),
      vertices.uv->data());

    verticesCount = vertices.uv->size();
  }

  if (vertices.bonesIds != nullptr) {
    SW_ASSERT(verticesCount == 0 || verticesCount == vertices.bonesIds->size());

    glNamedBufferSubData(m_vertexBuffers[4],
      0,
      vertices.bonesIds->size() * sizeof(glm::u8vec4),
      vertices.bonesIds->data());

    verticesCount = vertices.bonesIds->size();
  }

  if (vertices.bonesWeights != nullptr) {
    SW_ASSERT(verticesCount == 0 || verticesCount == vertices.bonesWeights->size());

    glNamedBufferSubData(m_vertexBuffers[5],
      0,
      vertices.bonesWeights->size() * sizeof(glm::u8vec4),
      vertices.bonesWeights->data());

    verticesCount = vertices.bonesIds->size();
  }

  SW_ASSERT(verticesCount <= m_verticesStorageCapacity && "Storage reallocation is forbidden");
  m_verticesCount = verticesCount;
}

void GLGeometryStore::updateIndices(const std::vector<std::uint16_t>& indices)
{
  SW_ASSERT(indices.size() <= m_indicesStorageCapacity && "Storage reallocation is forbidden");

  glNamedBufferSubData(m_indexBuffer, 0, indices.size() * sizeof(std::uint16_t), indices.data());
  m_indicesCount = indices.size();
}

size_t GLGeometryStore::getVerticesCapacity() const
{
  return m_verticesStorageCapacity;
}

size_t GLGeometryStore::getIndicesCapacity() const
{
  return m_indicesStorageCapacity;
}

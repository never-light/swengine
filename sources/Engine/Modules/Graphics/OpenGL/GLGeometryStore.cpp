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

GLGeometryStore::GLGeometryStore(const VertexPos3Color4SoA& vertices)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.colors->size());

  setupVAO<VertexPos3Color4SoA, VertexPos3Color4SoA>(vertices, std::vector<uint16_t>());
}

GLGeometryStore::GLGeometryStore(const VertexPos3Color4SoA& vertices, const std::vector<uint16_t>& indices)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.colors->size() &&
    !indices.empty());

  setupVAO<VertexPos3Color4SoA, VertexPos3Color4SoA>(vertices, indices);
}

GLGeometryStore::GLGeometryStore(const VertexPos3Norm3UVSkinnedSoA& vertices)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.normals->size() &&
    vertices.positions->size() == vertices.uv->size() && vertices.positions->size() == vertices.bonesIds->size() &&
    vertices.positions->size() == vertices.bonesWeights->size());

  setupVAO<VertexPos3Norm3UVSkinnedSoA, VertexPos3Norm3UVSkinnedSoA>(vertices, std::vector<uint16_t>());
}

GLGeometryStore::GLGeometryStore(const VertexPos3Norm3UVSkinnedSoA& vertices,
  const std::vector<uint16_t>& indices)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.normals->size() &&
    vertices.positions->size() == vertices.uv->size() && vertices.positions->size() == vertices.bonesIds->size() &&
    vertices.positions->size() == vertices.bonesWeights->size() && !indices.empty());

  setupVAO<VertexPos3Norm3UVSkinnedSoA, VertexPos3Norm3UVSkinnedSoA>(vertices, indices);
}

GLGeometryStore::GLGeometryStore(const VerticesPos3Norm3UVSoA& vertices)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.normals->size() &&
    vertices.positions->size() == vertices.uv->size());

  setupVAO<VerticesPos3Norm3UVSoA, VerticesPos3Norm3UVSoA>(vertices, std::vector<uint16_t>());
}

GLGeometryStore::GLGeometryStore(const VerticesPos3Norm3UVSoA& vertices,
  const std::vector<std::uint16_t>& indices)
{
  SW_ASSERT(!vertices.positions->empty() && vertices.positions->size() == vertices.normals->size() &&
    vertices.positions->size() == vertices.uv->size());

  setupVAO<VerticesPos3Norm3UVSoA, VerticesPos3Norm3UVSoA>(vertices, indices);
}

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices)
{
  SW_ASSERT(!vertices.empty());

  setupVAO<std::vector<VertexPos3Norm3UV>, VertexPos3Norm3UV>(vertices, std::vector<uint16_t>());
}

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices,
  const std::vector<std::uint16_t>& indices)
{
  SW_ASSERT(!vertices.empty() && !indices.empty());

  setupVAO<std::vector<VertexPos3Norm3UV>, VertexPos3Norm3UV>(vertices, indices);
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


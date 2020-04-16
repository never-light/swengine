#include "precompiled.h"

#pragma hdrstop

#include "GLGeometryStore.h"
#include "GLGeometryStoreImpl.h"

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices) {
  SW_ASSERT(vertices.size() > 0);

  createBuffersAndVAO(vertices, std::vector<uint16_t>());
}

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices, const std::vector<uint16_t>& indices) {
  SW_ASSERT(vertices.size() > 0 && indices.size() > 0);

  createBuffersAndVAO(vertices, indices);
}

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Color4>& vertices) {
  SW_ASSERT(vertices.size() > 0);

  createBuffersAndVAO(vertices, std::vector<uint16_t>());
}

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Color4>& vertices, const std::vector<uint16_t>& indices) {
  SW_ASSERT(vertices.size() > 0 && indices.size() > 0);

  createBuffersAndVAO(vertices, indices);
}

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Norm3UVSkinned>& vertices) {
  SW_ASSERT(vertices.size() > 0);

  createBuffersAndVAO(vertices, std::vector<uint16_t>());
}

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Norm3UVSkinned>& vertices,
                                 const std::vector<uint16_t>& indices) {
  SW_ASSERT(vertices.size() > 0 && indices.size() > 0);

  createBuffersAndVAO(vertices, indices);
}

GLGeometryStore::~GLGeometryStore() {
  if (m_vertexBuffer != 0) {
    glDeleteBuffers(1, &m_vertexBuffer);
  }

  if (m_indexBuffer != 0) {
    glDeleteBuffers(1, &m_indexBuffer);
  }

  if (m_vertexArrayObject != 0) {
    glDeleteVertexArrays(1, &m_vertexArrayObject);
  }
}

size_t GLGeometryStore::getVerticesCount() const {
  return m_verticesCount;
}

size_t GLGeometryStore::getIndicesCount() const {
  return m_indicesCount;
}

bool GLGeometryStore::isIndexed() const {
  return m_indicesCount > 0;
}

void GLGeometryStore::draw(GLenum primitivesType) const {
  glBindVertexArray(m_vertexArrayObject);

  if (isIndexed()) {
    glDrawElements(primitivesType, static_cast<GLsizei>(m_indicesCount), GL_UNSIGNED_SHORT, nullptr);
  }
  else {
    glDrawArrays(primitivesType, 0, static_cast<GLsizei>(m_verticesCount));
  }
}

void GLGeometryStore::drawRange(size_t start, size_t count, GLenum primitivesType) const {
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

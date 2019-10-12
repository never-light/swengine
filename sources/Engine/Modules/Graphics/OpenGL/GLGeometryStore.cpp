#include "GLGeometryStore.h"
#include "GLGeometryStoreImpl.h"

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices)
    : m_vertexBuffer(0),
    m_indexBuffer(0),
    m_vertexArrayObject(0),
    m_verticesCount(0),
    m_indicesCount(0)

{
    SW_ASSERT(vertices.size() > 0);

    createBuffersAndVAO(vertices, std::vector<uint16_t>());
}

GLGeometryStore::GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices, const std::vector<uint16_t>& indices)
    : m_vertexBuffer(0),
      m_indexBuffer(0),
      m_vertexArrayObject(0),
      m_verticesCount(0),
      m_indicesCount(0)
{
    SW_ASSERT(vertices.size() > 0 && indices.size() > 0);

    createBuffersAndVAO(vertices, indices);
}

GLGeometryStore::~GLGeometryStore()
{
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

void GLGeometryStore::draw()
{
    glBindVertexArray(m_vertexArrayObject);

    if (isIndexed()) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indicesCount), GL_UNSIGNED_SHORT, nullptr);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_verticesCount));
    }

    glBindVertexArray(0);
}

#pragma once

#include "GLGeometryStore.h"
#include "GLDebug.h"

template<class T>
void GLGeometryStore::createBuffersAndVAO(const std::vector<T> &vertices, const std::vector<uint16_t> &indices)
{
    // Create and fill vertex buffer
    GL_CALL_BLOCK_BEGIN();

    glCreateBuffers(1, &m_vertexBuffer);
    glNamedBufferStorage(m_vertexBuffer,
        static_cast<GLsizeiptr>(vertices.size() * sizeof(T)),
        vertices.data(), 0);

    m_verticesCount = vertices.size();

    GL_CALL_BLOCK_END();

    // Create and fill index buffer if is is needed
    if (indices.size() > 0) {
        GLsizeiptr indicesMemorySize = static_cast<GLsizeiptr>(indices.size() * sizeof(indices[0]));

        GL_CALL_BLOCK_BEGIN();
        glCreateBuffers(1, &m_indexBuffer);
        glNamedBufferStorage(m_indexBuffer, indicesMemorySize, indices.data(), 0);

        m_indicesCount = indices.size();

        GL_CALL_BLOCK_END();
    }

    // Create and set up Vertex Array Object
    createVAOAndSetupAttributes<T>();
}

template<>
void GLGeometryStore::createVAOAndSetupAttributes<VertexPos3Norm3UV>()
{
    glCreateVertexArrays(1, &m_vertexArrayObject);

    glVertexArrayVertexBuffer(m_vertexArrayObject, 0, m_vertexBuffer, 0, sizeof(VertexPos3Norm3UV));

    if (isIndexed()) {
        glVertexArrayElementBuffer(m_vertexArrayObject, m_indexBuffer);
    }

    glEnableVertexArrayAttrib(m_vertexArrayObject, 0);
    glEnableVertexArrayAttrib(m_vertexArrayObject, 1);
    glEnableVertexArrayAttrib(m_vertexArrayObject, 2);

    glVertexArrayAttribFormat(m_vertexArrayObject, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPos3Norm3UV, pos));
    glVertexArrayAttribFormat(m_vertexArrayObject, 1, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPos3Norm3UV, norm));
    glVertexArrayAttribFormat(m_vertexArrayObject, 2, 2, GL_FLOAT, GL_FALSE, offsetof(VertexPos3Norm3UV, uv));

    glVertexArrayAttribBinding(m_vertexArrayObject, 0, 0);
    glVertexArrayAttribBinding(m_vertexArrayObject, 1, 0);
    glVertexArrayAttribBinding(m_vertexArrayObject, 2, 0);
}

template<>
void GLGeometryStore::createVAOAndSetupAttributes<VertexPos3Color4>()
{
    glCreateVertexArrays(1, &m_vertexArrayObject);

    glVertexArrayVertexBuffer(m_vertexArrayObject, 0, m_vertexBuffer, 0, sizeof(VertexPos3Color4));

    if (isIndexed()) {
        glVertexArrayElementBuffer(m_vertexArrayObject, m_indexBuffer);
    }

    glEnableVertexArrayAttrib(m_vertexArrayObject, 0);
    glEnableVertexArrayAttrib(m_vertexArrayObject, 1);

    glVertexArrayAttribFormat(m_vertexArrayObject, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPos3Color4, pos));
    glVertexArrayAttribFormat(m_vertexArrayObject, 1, 4, GL_FLOAT, GL_FALSE, offsetof(VertexPos3Color4, color));

    glVertexArrayAttribBinding(m_vertexArrayObject, 0, 0);
    glVertexArrayAttribBinding(m_vertexArrayObject, 1, 0);
}

template<>
void GLGeometryStore::createVAOAndSetupAttributes<VertexPos3Norm3UVSkinned>()
{
    glCreateVertexArrays(1, &m_vertexArrayObject);

    glVertexArrayVertexBuffer(m_vertexArrayObject, 0, m_vertexBuffer, 0, sizeof(VertexPos3Norm3UVSkinned));

    if (isIndexed()) {
        glVertexArrayElementBuffer(m_vertexArrayObject, m_indexBuffer);
    }

    // Position
    glEnableVertexArrayAttrib(m_vertexArrayObject, 0);
    // Normal
    glEnableVertexArrayAttrib(m_vertexArrayObject, 1);
    // UV
    glEnableVertexArrayAttrib(m_vertexArrayObject, 2);
    // Bones IDs
    glEnableVertexArrayAttrib(m_vertexArrayObject, 4);
    // Bones Weights
    glEnableVertexArrayAttrib(m_vertexArrayObject, 5);


    glVertexArrayAttribFormat(m_vertexArrayObject, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPos3Norm3UVSkinned, pos));
    glVertexArrayAttribFormat(m_vertexArrayObject, 1, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPos3Norm3UVSkinned, norm));
    glVertexArrayAttribFormat(m_vertexArrayObject, 2, 2, GL_FLOAT, GL_FALSE, offsetof(VertexPos3Norm3UVSkinned, uv));

    glVertexArrayAttribIFormat(m_vertexArrayObject, 4, 4, GL_UNSIGNED_BYTE,
                              offsetof(VertexPos3Norm3UVSkinned, bonesIds));
    glVertexArrayAttribIFormat(m_vertexArrayObject, 5, 4, GL_UNSIGNED_BYTE,
                              offsetof(VertexPos3Norm3UVSkinned, bonesWeights));

    glVertexArrayAttribBinding(m_vertexArrayObject, 0, 0);
    glVertexArrayAttribBinding(m_vertexArrayObject, 1, 0);
    glVertexArrayAttribBinding(m_vertexArrayObject, 2, 0);
    glVertexArrayAttribBinding(m_vertexArrayObject, 4, 0);
    glVertexArrayAttribBinding(m_vertexArrayObject, 5, 0);

}


#include "Mesh.h"
#include "Exceptions/EngineRuntimeException.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::setVertices(const std::vector<glm::vec3>& vertices)
{
    SW_ASSERT(m_vertices.size() == 0);

    m_vertices = vertices;
    m_needGeometryBufferUpdate = true;
}

size_t Mesh::addSubMesh(const std::vector<uint16_t> &indices)
{
    SW_ASSERT(m_vertices.size() > 0);

    m_needGeometryBufferUpdate = true;

    m_indices.push_back(indices);
    calculateSubMeshesOffsets();

    return m_indices.size() - 1;
}

void Mesh::setIndices(const std::vector<uint16_t> &indices, size_t subMeshIndex)
{
    SW_ASSERT(subMeshIndex < m_indices.size());
    SW_ASSERT(m_vertices.size() > 0);

    m_needGeometryBufferUpdate = true;
    calculateSubMeshesOffsets();

    m_indices[subMeshIndex] = indices;
}

void Mesh::setNormals(const std::vector<glm::vec3> &normals)
{
    SW_ASSERT(m_vertices.size() > 0 && normals.size() == m_vertices.size());

    m_needGeometryBufferUpdate = true;

    m_normals = normals;
}

void Mesh::setUV(const std::vector<glm::vec2> &uv)
{
    SW_ASSERT(m_vertices.size() > 0 && uv.size() == m_vertices.size());

    m_needGeometryBufferUpdate = true;

    m_uv = uv;
}

void Mesh::setSubMeshesIndices(const std::vector<uint16_t>& indices, const std::vector<uint16_t>& subMeshesOffsets)
{
    for (size_t subMeshIndex = 0; subMeshIndex < subMeshesOffsets.size(); subMeshIndex++) {
        auto startIt = indices.begin() + subMeshesOffsets[subMeshIndex];
        auto endIt = (subMeshIndex == subMeshesOffsets.size() - 1) ? indices.end() :
            indices.begin() + (subMeshesOffsets[subMeshIndex + 1] - 1);

        addSubMesh(std::vector<uint16_t>(startIt, endIt));
    }
}

size_t Mesh::getSubMeshesCount() const
{
    return m_indices.size();
}

size_t Mesh::getSubMeshIndicesOffset(size_t subMeshIndex) const
{
    SW_ASSERT(subMeshIndex < m_indices.size());

    return m_subMeshesOffsets[subMeshIndex];
}

size_t Mesh::getSubMeshIndicesCount(size_t subMeshIndex) const
{
    SW_ASSERT(subMeshIndex < m_indices.size());

    return m_indices[subMeshIndex].size();
}

GLGeometryStore* Mesh::getGeometryStore()
{
    updateGeometryBuffer();

    return m_geometryStore.get();
}

void Mesh::setAABB(const AABB& aabb)
{
    m_aabb = aabb;
}

const AABB& Mesh::getAABB() const
{
    return m_aabb;
}

void Mesh::calculateSubMeshesOffsets()
{
    m_subMeshesOffsets.clear();
    m_subMeshesOffsets.push_back(0);

    for (size_t subMeshIndex = 1; subMeshIndex < m_indices.size(); subMeshIndex++) {
        m_subMeshesOffsets.push_back(m_subMeshesOffsets[subMeshIndex - 1] + m_indices[subMeshIndex - 1].size());
    }
}


template<>
std::vector<VertexPos3Norm3UV> Mesh::constructVerticesList() const
{
    std::vector<VertexPos3Norm3UV> vertices(m_vertices.size());

    for (size_t vertexIndex = 0; vertexIndex < m_vertices.size(); vertexIndex++) {
        vertices[vertexIndex].pos = m_vertices[vertexIndex];
        vertices[vertexIndex].norm = m_normals[vertexIndex];
        vertices[vertexIndex].uv = m_uv[vertexIndex];
    }

    return vertices;
}

void Mesh::updateGeometryBuffer()
{
    if (!m_needGeometryBufferUpdate)
        return;

    std::vector<uint16_t> indices;

    for (const auto& subMeshIndices : m_indices)
        indices.insert(indices.end(), subMeshIndices.begin(), subMeshIndices.end());

    GLGeometryStore* geometryStore = nullptr;

    if (m_normals.size() > 0 && m_uv.size() > 0) {
        geometryStore = new GLGeometryStore(constructVerticesList<VertexPos3Norm3UV>(), indices);
    }

    if (geometryStore == nullptr) {
        ENGINE_RUNTIME_ERROR("Unsupported vertex buffer layout");
    }

    m_geometryStore.reset(geometryStore);
    m_needGeometryBufferUpdate = false;
}

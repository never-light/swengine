#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Modules/Graphics/OpenGL/GLGeometryStore.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void setVertices(const std::vector<glm::vec3>& vertices);

    size_t addSubMesh(const std::vector<uint16_t>& indices);
    void setIndices(const std::vector<uint16_t>& indices, size_t subMeshIndex);

    void setNormals(const std::vector<glm::vec3>& normals);
    void setUV(const std::vector<glm::vec2>& uv);

    void setSubMeshesIndices(const std::vector<uint16_t>& indices, const std::vector<uint16_t>& subMeshesOffsets);

    size_t getSubMeshesCount() const;
    size_t getSubMeshIndicesOffset(size_t subMeshIndex) const;
    size_t getSubMeshIndicesCount(size_t subMeshIndex) const;

    const GLGeometryStore* getGeometryStore();

private:
    void calculateSubMeshesOffsets();

    void updateGeometryBuffer();

    template<class T>
    std::vector<T> constructVerticesList() const;

private:
    std::unique_ptr<GLGeometryStore> m_geometryStore;

    std::vector<glm::vec3> m_vertices;

    std::vector<std::vector<uint16_t>> m_indices;
    std::vector<size_t> m_subMeshesOffsets;

    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_uv;

    bool m_needGeometryBufferUpdate = false;
};


#include "precompiled.h"
#pragma hdrstop

#include "MeshResource.h"
#include "Exceptions/EngineRuntimeException.h"

#include <fstream>
#include <streambuf>
#include <bitset>
#include <algorithm>

#include "RawMesh.h"

MeshResource::MeshResource()
{

}

MeshResource::~MeshResource()
{
    SW_ASSERT(m_mesh.use_count() <= 1);
}

void MeshResource::load(const ResourceDeclaration& declaration, ResourceManager& resourceManager)
{
    ARG_UNUSED(resourceManager);

    SW_ASSERT(m_mesh == nullptr);

    MeshResourceParameters parameters = declaration.getParameters<MeshResourceParameters>();

    if (auto sourceFile = std::get_if<ResourceSourceFile>(&declaration.source)) {
        m_mesh = loadFromFile(sourceFile->path, parameters);
    }
    else {
        ENGINE_RUNTIME_ERROR("Trying to load mesh resource from invalid source");
    }
}

void MeshResource::unload()
{
    SW_ASSERT(m_mesh.use_count() == 1);

    m_mesh.reset();
}

bool MeshResource::isBusy() const
{
    return m_mesh.use_count() > 1;
}

std::shared_ptr<Mesh> MeshResource::loadFromFile(const std::string& path, const MeshResourceParameters& parameters)
{
    ARG_UNUSED(parameters);

    // Read raw mesh
    std::ifstream meshFile(path, std::ios::binary);

    RawMesh rawMesh;

    meshFile.read(reinterpret_cast<char*>(&rawMesh.header), sizeof(rawMesh.header));

    if (rawMesh.header.formatVersion != MESH_FORMAT_VERSION) {
        ENGINE_RUNTIME_ERROR("Trying to load mesh with incompatible format version");
    }

    if (rawMesh.header.verticesCount == 0) {
        ENGINE_RUNTIME_ERROR("Trying to load mesh with zero vertices count");
    }

    const uint16_t verticesCount = rawMesh.header.verticesCount;
    const std::bitset<64> storedAttributesMask(rawMesh.header.storedAttributesMask);

    if (storedAttributesMask.test(static_cast<size_t>(RawMeshAttributes::Positions))) {
        rawMesh.positions.resize(rawMesh.header.verticesCount);
        meshFile.read(reinterpret_cast<char*>(rawMesh.positions.data()),
                      sizeof(*rawMesh.positions.begin()) * verticesCount);
    }

    if (storedAttributesMask.test(static_cast<size_t>(RawMeshAttributes::Normals))) {
        rawMesh.positions.resize(rawMesh.header.verticesCount);
        rawMesh.normals.resize(rawMesh.header.verticesCount);
        meshFile.read(reinterpret_cast<char*>(rawMesh.normals.data()),
                      sizeof(*rawMesh.normals.begin()) * verticesCount);
    }

    if (storedAttributesMask.test(static_cast<size_t>(RawMeshAttributes::UV))) {
        rawMesh.uv.resize(rawMesh.header.verticesCount);
        meshFile.read(reinterpret_cast<char*>(rawMesh.uv.data()),
                      sizeof(*rawMesh.uv.begin()) * verticesCount);
    }

    const uint16_t indicesCount = rawMesh.header.indicesCount;

    rawMesh.indices.resize(indicesCount);
    meshFile.read(reinterpret_cast<char*>(rawMesh.indices.data()),
                  sizeof(*rawMesh.indices.begin()) * indicesCount);

    const uint16_t subMeshesIndicesOffsetsCount = rawMesh.header.subMeshesIndicesOffsetsCount;

    rawMesh.subMeshesIndicesOffsets.resize(subMeshesIndicesOffsetsCount);
    meshFile.read(reinterpret_cast<char*>(rawMesh.subMeshesIndicesOffsets.data()),
                  sizeof(*rawMesh.subMeshesIndicesOffsets.begin()) * subMeshesIndicesOffsetsCount);

    meshFile.read(reinterpret_cast<char*>(&rawMesh.aabb), sizeof(rawMesh.aabb));

    meshFile.close();

    // Convert raw mesh to internal mesh object
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    if (rawMesh.positions.size() != 0) {
        std::vector<glm::vec3> positions;

        std::transform(rawMesh.positions.cbegin(), rawMesh.positions.cend(), std::back_inserter(positions),
                       [](const RawVector3& v) -> glm::vec3 { return { v.x, v.y, v.z }; }
        );

        mesh->setVertices(positions);
    }

    if (rawMesh.normals.size() != 0) {
        std::vector<glm::vec3> normals;

        std::transform(rawMesh.normals.cbegin(), rawMesh.normals.cend(), std::back_inserter(normals),
                       [](const RawVector3& v) -> glm::vec3 { return { v.x, v.y, v.z }; }
        );

        mesh->setNormals(normals);
    }

    if (rawMesh.uv.size() != 0) {
        std::vector<glm::vec2> uv;

        std::transform(rawMesh.uv.cbegin(), rawMesh.uv.cend(), std::back_inserter(uv),
                       [](const RawVector2& v) -> glm::vec2 { return { v.x, v.y }; }
        );

        mesh->setUV(uv);
    }

    mesh->setSubMeshesIndices(rawMesh.indices, rawMesh.subMeshesIndicesOffsets);
    mesh->setAABB(rawMesh.aabb);

    return mesh;
}

MeshResource::ParametersType MeshResource::buildDeclarationParameters(const pugi::xml_node& declarationNode,
                                                                const ParametersType& defaultParameters)
{
    ARG_UNUSED(declarationNode);

    ParametersType parameters = defaultParameters;

    return parameters;
}

std::shared_ptr<Mesh> MeshResource::getMesh() const
{
    return m_mesh;
}

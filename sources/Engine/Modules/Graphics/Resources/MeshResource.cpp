#include "precompiled.h"
#pragma hdrstop

#include "MeshResource.h"

#include <fstream>
#include <streambuf>
#include <bitset>
#include <algorithm>

#include "Modules/Graphics/GraphicsSystem/Animation/Skeleton.h"
#include "Modules/ResourceManagement/ResourceManager.h"
#include "Exceptions/EngineRuntimeException.h"
#include "Utility/memory.h"

#include "SkeletonResource.h"

#include "Modules/Graphics/Resources/Raw/RawMesh.h"

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

        if (parameters.skeletonResourceId.has_value()) {
            std::shared_ptr<Skeleton> skeleton = resourceManager.getResourceFromInstance<SkeletonResource>(
                        parameters.skeletonResourceId.value())->getSkeleton();

            m_mesh->setSkeleton(skeleton);
        }
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
    RawMesh rawMesh = RawMesh::readFromFile(path);

    // Convert raw mesh to internal mesh object
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    if (rawMesh.positions.size() != 0) {
        std::vector<glm::vec3> positions =
                MemoryUtils::createBinaryCompatibleVector<RawVector3, glm::vec3>(rawMesh.positions);

        mesh->setVertices(positions);
    }

    if (rawMesh.normals.size() != 0) {
        std::vector<glm::vec3> normals =
                MemoryUtils::createBinaryCompatibleVector<RawVector3, glm::vec3>(rawMesh.normals);

        mesh->setNormals(normals);
    }

    if (rawMesh.uv.size() != 0) {
        std::vector<glm::vec2> uv =
            MemoryUtils::createBinaryCompatibleVector<RawVector2, glm::vec2>(rawMesh.uv);

        mesh->setUV(uv);
    }

    if (rawMesh.tangents.size() != 0) {
        std::vector<glm::vec3> tangents =
            MemoryUtils::createBinaryCompatibleVector<RawVector3, glm::vec3>(rawMesh.tangents);

        mesh->setTangents(tangents);
    }

    if (rawMesh.bonesIds.size() != 0) {
        SW_ASSERT(rawMesh.bonesWeights.size() == rawMesh.bonesIds.size());

        std::vector<glm::u8vec4> bonesIDs =
            MemoryUtils::createBinaryCompatibleVector<RawU8Vector4, glm::u8vec4>(rawMesh.bonesIds);

        std::vector<glm::u8vec4> bonesWeights =
            MemoryUtils::createBinaryCompatibleVector<RawU8Vector4, glm::u8vec4>(rawMesh.bonesWeights);

        mesh->setSkinData(bonesIDs, bonesWeights);
    }

    mesh->setSubMeshesIndices(rawMesh.indices, rawMesh.subMeshesIndicesOffsets);
    mesh->setAABB(rawMesh.aabb);

    return mesh;
}

MeshResource::ParametersType MeshResource::buildDeclarationParameters(const pugi::xml_node& declarationNode,
                                                                const ParametersType& defaultParameters)
{
    ParametersType parameters = defaultParameters;

    pugi::xml_node skeletonNode = declarationNode.child("skeleton");

    if (skeletonNode) {
        pugi::xml_attribute skeletonId = skeletonNode.attribute("id");

        if (!skeletonId) {
            ENGINE_RUNTIME_ERROR("Skeleton attribute for a mesh is added, but resource id is not specified");
        }

        parameters.skeletonResourceId = skeletonId.as_string();
    }

    return parameters;
}

std::shared_ptr<Mesh> MeshResource::getMesh() const
{
    return m_mesh;
}

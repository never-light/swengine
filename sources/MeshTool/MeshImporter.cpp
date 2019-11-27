#include "MeshImporter.h"

#include <string>
#include <bitset>
#include <Engine/Exceptions/EngineRuntimeException.h>
#include <spdlog/spdlog.h>

MeshImporter::MeshImporter()
{

}

std::unique_ptr<RawMesh> MeshImporter::importFromFile(const std::string& path, const MeshImportOptions& options)
{    
    Assimp::Importer meshImporter;

    unsigned int importOptions = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_ImproveCacheLocality;

    if (options.flipUV)
        importOptions |= aiProcess_FlipUVs;

    if (options.joinIdenticalVertices)
        importOptions |= aiProcess_JoinIdenticalVertices;

    if (options.calculateTangents)
        importOptions |= aiProcess_CalcTangentSpace;

    if (options.glueByMaterials)
        importOptions = importOptions | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes;

    spdlog::info("Load source mesh: {}", path);

    const aiScene *scene = meshImporter.ReadFile(path, importOptions);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        ENGINE_RUNTIME_ERROR(meshImporter.GetErrorString());

    spdlog::info("Source mesh is loaded");
    spdlog::info("Start mesh parsing");

    std::unique_ptr<RawMesh> mesh = convertSceneToMesh(*scene, options);

    spdlog::info("Mesh is parsed ({} vertices, {} indices, {} submeshes)",
                 mesh->header.verticesCount, mesh->header.indicesCount, mesh->header.subMeshesIndicesOffsetsCount);

    return mesh;
}

std::unique_ptr<RawMesh> MeshImporter::convertSceneToMesh(const aiScene& scene, const MeshImportOptions& options)
{
    std::vector<RawVector3> positions;
    std::vector<RawVector3> normals;
    std::vector<RawVector2> uv;
    std::vector<std::vector<std::uint16_t>> subMeshesIndices;

    for (size_t meshIndex = 0; meshIndex < scene.mNumMeshes; meshIndex++) {
        const aiMesh& rawMesh = *scene.mMeshes[meshIndex];

        bool requiredAttributesFound = rawMesh.HasPositions() && rawMesh.HasNormals() && rawMesh.HasTextureCoords(0) && rawMesh.HasFaces();
        bool tangentsFound = rawMesh.HasTangentsAndBitangents();

        if (!requiredAttributesFound || (options.calculateTangents && !tangentsFound)) {
            spdlog::info("Submesh #{} ({}) is incomplete and was skipped", meshIndex, rawMesh.mName.C_Str());

            // Skip current mesh
            continue;
        }

        // Vertices
        size_t verticesAddIndex = positions.size();

        for (size_t vertexIndex = 0; vertexIndex < rawMesh.mNumVertices; vertexIndex++) {
            positions.push_back({ rawMesh.mVertices[vertexIndex].x,
                                  rawMesh.mVertices[vertexIndex].y,
                                  rawMesh.mVertices[vertexIndex].z });

            normals.push_back({ rawMesh.mNormals[vertexIndex].x,
                                rawMesh.mNormals[vertexIndex].y,
                                rawMesh.mNormals[vertexIndex].z });

            uv.push_back({ rawMesh.mTextureCoords[0][vertexIndex].x,
                           rawMesh.mTextureCoords[0][vertexIndex].y });

            // TODO: implement tangents, bitangents, bones import
        }

        // Indices
        bool nonTrianglePolygonFound = false;

        size_t indicesOffset = verticesAddIndex;

        std::vector<uint16_t> indices;

        for (size_t faceIndex = 0; faceIndex < rawMesh.mNumFaces; faceIndex++) {
            const aiFace& face = rawMesh.mFaces[faceIndex];

            if (face.mNumIndices != 3) {
                nonTrianglePolygonFound = true;
                break;
            }

            for (size_t indexNumber = 0; indexNumber < 3; indexNumber++) {
                indices.push_back(static_cast<uint16_t>(face.mIndices[indexNumber] + indicesOffset));
            }
        }

        if (nonTrianglePolygonFound) {
            spdlog::info("Submesh #{} ({}) has non-triangle polygon and was skipped", meshIndex, rawMesh.mName.C_Str());

            // Skip current mesh
            continue;
        }

        subMeshesIndices.push_back(std::move(indices));
    }

    // Mesh formation
    SW_ASSERT(positions.size() == normals.size() && positions.size() == uv.size());

    std::unique_ptr<RawMesh> mesh = std::make_unique<RawMesh>();

    mesh->positions = positions;
    mesh->normals = normals;
    mesh->uv = uv;

    for (const auto& subMeshIndices : subMeshesIndices) {
        mesh->subMeshesIndicesOffsets.push_back(static_cast<uint16_t>(mesh->indices.size()));
        mesh->indices.insert(mesh->indices.end(), subMeshIndices.begin(), subMeshIndices.end());
    }

    const uint16_t verticesCount = static_cast<uint16_t>(mesh->positions.size());
    const uint16_t indicesCount = static_cast<uint16_t>(mesh->indices.size());

    mesh->header.formatVersion = MESH_FORMAT_VERSION;
    mesh->header.verticesCount = verticesCount;
    mesh->header.indicesCount = indicesCount;
    mesh->header.subMeshesIndicesOffsetsCount = static_cast<uint16_t>(subMeshesIndices.size());

    std::bitset<64> storedAttributesMask;
    storedAttributesMask.set(static_cast<size_t>(RawMeshAttributes::Positions));
    storedAttributesMask.set(static_cast<size_t>(RawMeshAttributes::Normals));
    storedAttributesMask.set(static_cast<size_t>(RawMeshAttributes::UV));

    mesh->header.storedAttributesMask = storedAttributesMask.to_ullong();

    return mesh;
}

#include "AssimpMeshLoader.h"

#include <Engine/Exceptions/EngineRuntimeException.h>

AssimpScene::AssimpScene(const std::string& path, const AssimpMeshLoadOptions& options)
{
    unsigned int importOptions = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_ImproveCacheLocality;

    if (options.flipUV)
        importOptions |= aiProcess_FlipUVs;

    if (options.joinIdenticalVertices)
        importOptions |= aiProcess_JoinIdenticalVertices;

    if (options.calculateTangents)
        importOptions |= aiProcess_CalcTangentSpace;

    if (options.glueByMaterials)
        importOptions = importOptions | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes;

    importOptions |= aiProcess_LimitBoneWeights;
    importOptions |= aiProcess_ValidateDataStructure;
    //importOptions |= aiProcess_PreTransformVertices;

    m_sceneImporter.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, static_cast<int>(options.maxBonexPerVertex));

    if (importOptions & aiProcess_LimitBoneWeights) {
        m_sceneImporter.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, static_cast<int>(options.maxBonexPerVertex));
    }

    m_scene = m_sceneImporter.ReadFile(path, importOptions);

    if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode) {
        ENGINE_RUNTIME_ERROR(m_sceneImporter.GetErrorString());
    }
}

AssimpScene::~AssimpScene()
{

}

const aiScene& AssimpScene::getScene() const
{
    return *m_scene;
}

std::unique_ptr<AssimpScene> AssimpMeshLoader::loadScene(const std::string& path, const AssimpMeshLoadOptions& options)
{
    return std::make_unique<AssimpScene>(path, options);
}

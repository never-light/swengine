#pragma once

#include <optional>

#include "Modules/ResourceManagement/Resource.h"
#include "Modules/Graphics/GraphicsSystem/Mesh.h"

struct MeshResourceParameters : ResourceSourceParameters {
    std::optional<std::string> skeletonResourceId;
};

class MeshResource : public Resource
{
public:
    using ParametersType = MeshResourceParameters;

public:
    MeshResource();
    ~MeshResource() override;

    void load(const ResourceDeclaration& declaration, ResourceManager& resourceManager) override;
    void unload() override;

    bool isBusy() const override;

    static std::shared_ptr<Mesh> loadFromFile(const std::string& path,
                                              const MeshResourceParameters& parameters);

    static ParametersType buildDeclarationParameters(const pugi::xml_node& declarationNode,
                                                             const ParametersType& defaultParameters);

public:
    std::shared_ptr<Mesh> getMesh() const;

private:
    std::shared_ptr<Mesh> m_mesh;
};

#pragma once

#include "Modules/ResourceManagement/Resource.h"
#include "Modules/Graphics/GUI/BitmapFont.h"

struct BitmapFontResourceParameters : ResourceSourceParameters {
    BitmapFontResourceParameters() { }
};

class BitmapFontResource : public Resource
{
public:
    using ParametersType = BitmapFontResourceParameters;

public:
    BitmapFontResource();
    ~BitmapFontResource() override;

    void load(const ResourceDeclaration& declaration, ResourceManager& resourceManager) override;
    void unload() override;

    bool isBusy() const override;

    static std::shared_ptr<BitmapFont> loadFromFile(const std::string& path,
                                                    const BitmapFontResourceParameters& parameters);

    static ParametersType buildDeclarationParameters(const pugi::xml_node& declarationNode,
                                                                   const ParametersType& defaultParameters);

public:
    std::shared_ptr<BitmapFont> getFont() const;

private:
    std::shared_ptr<BitmapFont> m_font;
};

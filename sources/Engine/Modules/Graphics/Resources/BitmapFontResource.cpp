#include "precompiled.h"
#pragma hdrstop

#include "BitmapFontResource.h"
#include "Exceptions/EngineRuntimeException.h"

#include <fstream>
#include <streambuf>

#include <pugixml.hpp>

#include <glm/vec2.hpp>

#include "Utility/strings.h"
#include "TextureResource.h"

BitmapFontResource::BitmapFontResource()
{

}

BitmapFontResource::~BitmapFontResource()
{
    SW_ASSERT(m_font.use_count() <= 1);
}

void BitmapFontResource::load(const ResourceDeclaration& declaration, ResourceManager& resourceManager)
{
    ARG_UNUSED(resourceManager);

    SW_ASSERT(m_font == nullptr);

    BitmapFontResourceParameters parameters = declaration.getParameters<BitmapFontResourceParameters>();

    if (auto sourceFile = std::get_if<ResourceSourceFile>(&declaration.source)) {
        m_font = loadFromFile(sourceFile->path, parameters);
    }
    else {
        ENGINE_RUNTIME_ERROR("Trying to load shader resource from invalid source");
    }
}

void BitmapFontResource::unload()
{
    SW_ASSERT(m_font.use_count() == 1);

    m_font.reset();
}

bool BitmapFontResource::isBusy() const
{
    return m_font.use_count() > 1 || m_font->getBitmap().use_count() > 1;
}

std::shared_ptr<BitmapFont> BitmapFontResource::getFont() const
{
    return m_font;
}

std::shared_ptr<BitmapFont> BitmapFontResource::loadFromFile(const std::string& path, const BitmapFontResourceParameters& parameters)
{
    ARG_UNUSED(parameters);

    pugi::xml_document fontDescription;
    pugi::xml_parse_result result = fontDescription.load_file(path.c_str());

    if (!result) {
        ENGINE_RUNTIME_ERROR("Trying to load font resource from invalid source");
    }

    pugi::xml_node fontNode = fontDescription.child("font");

    std::array<BitmapCharacter, 256> fontCharacters;

    for (auto charNode : fontNode.children()) {
        std::vector<std::string> bitmapParts = StringUtils::split(charNode.attribute("rect").as_string(), ' ');

        Rect bitmapRect(std::stoi(bitmapParts[0]),
            std::stoi(bitmapParts[1]),
            std::stoi(bitmapParts[2]),
            std::stoi(bitmapParts[3]));

        std::vector<std::string> offsetParts = StringUtils::split(charNode.attribute("offset").as_string(), ' ');

        glm::ivec2 offset(std::stoi(offsetParts[0]), std::stoi(offsetParts[1]));

        int xAdvance = charNode.attribute("width").as_int();

        unsigned char character = static_cast<unsigned char>(charNode.attribute("code").as_string()[0]);
        size_t characterIndex = static_cast<size_t>(character);

        fontCharacters[characterIndex].bitmapArea = bitmapRect;
        fontCharacters[characterIndex].xOffset = offset.x;
        fontCharacters[characterIndex].yOffset = offset.y;
        fontCharacters[characterIndex].xAdvance = xAdvance;
        fontCharacters[characterIndex].isMapped = true;
    }

    std::string bitmapFilename = fontNode.attribute("bitmap").as_string();
    TextureResourceParameters bitmapParameters(GLTextureType::Texture2D, GLTextureInternalFormat::R8,
                                               false, GL_LINEAR, GL_LINEAR,
                                               GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    std::shared_ptr<GLTexture> bitmap = TextureResource::loadFromFile(bitmapFilename, bitmapParameters);

    unsigned int baseSize = fontNode.attribute("size").as_uint();
    unsigned int height = fontNode.attribute("height").as_uint();

    return std::make_shared<BitmapFont>(bitmap, fontCharacters, baseSize, height);
}

BitmapFontResource::ParametersType BitmapFontResource::buildDeclarationParameters(const pugi::xml_node& declarationNode,
                                                                            const ParametersType& defaultParameters)
{
    ARG_UNUSED(declarationNode);

    ParametersType parameters = defaultParameters;

    return parameters;
}


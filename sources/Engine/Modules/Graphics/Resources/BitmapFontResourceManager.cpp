#include "precompiled.h"

#pragma hdrstop

#include "BitmapFontResourceManager.h"
#include "Exceptions/exceptions.h"

#include <fstream>
#include <streambuf>

#include <pugixml.hpp>

#include <glm/vec2.hpp>

#include "Utility/strings.h"
#include "TextureResourceManager.h"

BitmapFontResourceManager::BitmapFontResourceManager(ResourcesManager* resourcesManager)
  : ResourceManager<BitmapFont, BitmapFontResourceConfig>(resourcesManager)
{

}

BitmapFontResourceManager::~BitmapFontResourceManager() = default;

void BitmapFontResourceManager::load(size_t resourceIndex)
{
  BitmapFontResourceConfig* config = getResourceConfig(resourceIndex);

  pugi::xml_document fontDescription;
  pugi::xml_parse_result result = fontDescription.load_file(config->resourcePath.c_str());

  if (!result) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load font resource from invalid source");
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
  }

  std::string bitmapResourceName = fontNode.attribute("bitmap").as_string();

  auto bitmapResource = getResourceManager()->getResource<GLTexture>(bitmapResourceName);

  unsigned int baseSize = fontNode.attribute("size").as_uint();
  unsigned int height = fontNode.attribute("height").as_uint();

  allocateResource<BitmapFont>(resourceIndex, bitmapResource, fontCharacters, baseSize, height);
}

void BitmapFontResourceManager::parseConfig(size_t resourceIndex, pugi::xml_node configNode)
{
  BitmapFontResourceConfig* resourceConfig = createResourceConfig(resourceIndex);
  resourceConfig->resourcePath = configNode.attribute("source").as_string();

  if (!FileUtils::isFileExists(resourceConfig->resourcePath)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Bitmap font resource refer to not existing file", resourceConfig->resourcePath));
  }
}

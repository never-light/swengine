#include "precompiled.h"

#pragma hdrstop

#include "TextureResourceManager.h"
#include "Exceptions/exceptions.h"

#include <fstream>
#include <streambuf>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Utility/strings.h"

TextureResourceManager::TextureResourceManager(ResourcesManager* resourcesManager)
  : ResourceManager<GLTexture, TextureResourceConfig>(resourcesManager)
{

}

TextureResourceManager::~TextureResourceManager() = default;

void TextureResourceManager::load(size_t resourceIndex)
{
  TextureResourceConfig* config = getResourceConfig(resourceIndex);

  int width, height;
  int nrChannels;
  auto* data = reinterpret_cast<std::byte*>(
    stbi_load(config->resourcePath.c_str(), &width, &height, &nrChannels, 0));

  if (data == nullptr) {
    THROW_EXCEPTION(EngineRuntimeException, std::string("Texture file has invalid format: ") +
      stbi_failure_reason());
  }

  GLenum pixelFormat;

  if (nrChannels == 1) {
    pixelFormat = GL_RED;
  }
  else if (nrChannels == 2) {
    pixelFormat = GL_RG;
  }
  else if (nrChannels == 3) {
    pixelFormat = GL_RGB;
  }
  else if (nrChannels == 4) {
    pixelFormat = GL_RGBA;
  }
  else {
    THROW_EXCEPTION(EngineRuntimeException, "Texture file has invalid format");
  }

  auto* texture = allocateResource<GLTexture>(resourceIndex, config->type, width, height,
    config->internalFormat);

  texture->setData(pixelFormat, GL_UNSIGNED_BYTE, data);

  if (config->autoGenerateMipmaps) {
    texture->generateMipMaps();
  }

  texture->setMinificationFilter(config->minificationFilter);
  texture->setMagnificationFilter(config->magnificationFilter);

  texture->setWrapModeU(config->wrapModeU);
  texture->setWrapModeV(config->wrapModeV);
  texture->setWrapModeW(config->wrapModeW);

  texture->enableAnisotropicFiltering(4.0f);
}

void TextureResourceManager::parseConfig(size_t resourceIndex, pugi::xml_node configNode)
{
  static std::unordered_map<std::string, GLTextureInternalFormat> internalFormatsMap = {
    {"r8", GLTextureInternalFormat::R8},
    {"rg8", GLTextureInternalFormat::RG8},
    {"rgb8", GLTextureInternalFormat::RGB8},
    {"rgba8", GLTextureInternalFormat::RGBA8},
    {"rgba16", GLTextureInternalFormat::RGBA16},
  };

  static std::unordered_map<std::string, GLint> filtersMap = {
    {"linear", GL_LINEAR},
    {"linear_mipmap_linear", GL_LINEAR_MIPMAP_LINEAR},
  };

  static std::unordered_map<std::string, GLint> wrapModesMap = {
    {"clamp_to_edge", GL_CLAMP_TO_EDGE},
    {"repeat", GL_REPEAT},
  };

  TextureResourceConfig* resourceConfig = createResourceConfig(resourceIndex);

  // Texture path
  resourceConfig->resourcePath = configNode.attribute("source").as_string();

  if (!FileUtils::isFileExists(resourceConfig->resourcePath)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Texture resource refer to not existing file", resourceConfig->resourcePath));
  }

  // Texture type
  if (configNode.child("type")) {
    GLTextureType textureType = ResourceDeclHelpers::getFilteredParameterValue(configNode, "type", {
      {"2d", GLTextureType::Texture2D},
    }, GLTextureType::Texture2D);

    resourceConfig->type = textureType;
  }

  // Internal format
  if (configNode.child("format")) {
    GLTextureInternalFormat internalFormat = ResourceDeclHelpers::getFilteredParameterValue(configNode,
      "format",
      internalFormatsMap,
      GLTextureInternalFormat::RGB8);

    resourceConfig->internalFormat = internalFormat;
  }

  // Auto generate mipmaps
  if (configNode.child("generate_mipmaps")) {
    bool autoGenerateMipmaps = ResourceDeclHelpers::getFilteredParameterValue(configNode, "generate_mipmaps", {
      {"true", true},
      {"false", false},
    }, true);

    resourceConfig->autoGenerateMipmaps = autoGenerateMipmaps;
  }

  // Minification filter
  if (configNode.child("min_filter")) {
    GLint minFilter = ResourceDeclHelpers::getFilteredParameterValue(configNode, "min_filter",
      filtersMap, GL_LINEAR_MIPMAP_LINEAR);

    resourceConfig->minificationFilter = minFilter;
  }

  // Magnification filter
  if (configNode.child("mag_filter")) {
    GLint magFilter = ResourceDeclHelpers::getFilteredParameterValue(configNode, "mag_filter",
      filtersMap, GL_LINEAR);

    resourceConfig->magnificationFilter = magFilter;
  }

  // Wrap
  if (configNode.child("wrap")) {
    if (configNode.child("wrap").attribute("u")) {
      GLint wrapModeU =
        ResourceDeclHelpers::getFilteredParameterValue(configNode.child("wrap").attribute("u").as_string(),
          "wrap_u", wrapModesMap, GL_CLAMP_TO_EDGE);

      resourceConfig->wrapModeU = wrapModeU;
    }

    if (configNode.child("wrap").attribute("v")) {
      GLint wrapModeV =
        ResourceDeclHelpers::getFilteredParameterValue(configNode.child("wrap").attribute("v").as_string(),
          "wrap_v", wrapModesMap, GL_CLAMP_TO_EDGE);

      resourceConfig->wrapModeV = wrapModeV;
    }

    if (configNode.child("wrap").attribute("w")) {
      GLint wrapModeW =
        ResourceDeclHelpers::getFilteredParameterValue(configNode.child("wrap").attribute("w").as_string(),
          "wrap_w", wrapModesMap, GL_CLAMP_TO_EDGE);

      resourceConfig->wrapModeW = wrapModeW;
    }
  }
}


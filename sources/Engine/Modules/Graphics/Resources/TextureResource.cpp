#include "precompiled.h"

#pragma hdrstop

#include "TextureResource.h"
#include "Exceptions/EngineRuntimeException.h"

#include <fstream>
#include <streambuf>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Utility/strings.h"

TextureResource::TextureResource() {

}

TextureResource::~TextureResource() {
  SW_ASSERT(m_texture.use_count() <= 1);
}

void TextureResource::load(const ResourceDeclaration& declaration, ResourceManager& resourceManager) {
  ARG_UNUSED(resourceManager);

  SW_ASSERT(m_texture == nullptr);

  TextureResourceParameters parameters = declaration.getParameters<TextureResourceParameters>();

  if (auto sourceFile = std::get_if<ResourceSourceFile>(&declaration.source)) {
    m_texture = loadFromFile(sourceFile->path, parameters);
  }
  else {
    ENGINE_RUNTIME_ERROR("Trying to load shader resource from invalid source");
  }
}

void TextureResource::unload() {
  SW_ASSERT(m_texture.use_count() == 1);

  m_texture.reset();
}

bool TextureResource::isBusy() const {
  return m_texture.use_count() > 1;
}

std::shared_ptr<GLTexture> TextureResource::loadFromFile(const std::string& path,
                                                         const TextureResourceParameters& parameters) {
  int width, height;
  int nrChannels;
  byte* data = reinterpret_cast<byte*>(stbi_load(path.c_str(), &width, &height, &nrChannels, 0));

  if (data == nullptr) {
    ENGINE_RUNTIME_ERROR(std::string("Texture file has invalid format: ") +
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
    ENGINE_RUNTIME_ERROR("Texture file has invalid format");
  }

  std::shared_ptr<GLTexture>
      texture = std::make_shared<GLTexture>(parameters.type, width, height, parameters.internalFormat);
  texture->setData(pixelFormat, GL_UNSIGNED_BYTE, data);

  if (parameters.autoGenerateMipmaps) {
    texture->generateMipMaps();
  }

  texture->setMinificationFilter(parameters.minificationFilter);
  texture->setMagnificationFilter(parameters.magnificationFilter);

  texture->setWrapModeU(parameters.wrapModeU);
  texture->setWrapModeV(parameters.wrapModeV);
  texture->setWrapModeW(parameters.wrapModeW);

  texture->enableAnisotropicFiltering(4.0f);

  return texture;
}

TextureResource::ParametersType TextureResource::buildDeclarationParameters(const pugi::xml_node& declarationNode,
                                                                            const ParametersType& defaultParameters) {
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

  ParametersType parameters = defaultParameters;

  // Texture type
  if (declarationNode.child("type")) {
    GLTextureType textureType = ResourceDeclHelpers::getFilteredParameterValue(declarationNode, "type", {
        {"2d", GLTextureType::Texture2D},
    }, GLTextureType::Texture2D);

    parameters.type = textureType;
  }

  // Internal format
  if (declarationNode.child("format")) {
    GLTextureInternalFormat internalFormat = ResourceDeclHelpers::getFilteredParameterValue(declarationNode,
                                                                                            "format",
                                                                                            internalFormatsMap,
                                                                                            GLTextureInternalFormat::RGB8);

    parameters.internalFormat = internalFormat;
  }

  // Auto generate mipmaps
  if (declarationNode.child("generate_mipmaps")) {
    bool autoGenerateMipmaps = ResourceDeclHelpers::getFilteredParameterValue(declarationNode, "generate_mipmaps", {
        {"true", true},
        {"false", false},
    }, true);

    parameters.autoGenerateMipmaps = autoGenerateMipmaps;
  }

  // Minification filter
  if (declarationNode.child("min_filter")) {
    GLint minFilter = ResourceDeclHelpers::getFilteredParameterValue(declarationNode, "min_filter",
                                                                     filtersMap, GL_LINEAR_MIPMAP_LINEAR);

    parameters.minificationFilter = minFilter;
  }

  // Magnification filter
  if (declarationNode.child("mag_filter")) {
    GLint magFilter = ResourceDeclHelpers::getFilteredParameterValue(declarationNode, "mag_filter",
                                                                     filtersMap, GL_LINEAR);

    parameters.magnificationFilter = magFilter;
  }

  // Wrap
  if (declarationNode.child("wrap")) {
    if (declarationNode.child("wrap").attribute("u")) {
      GLint wrapModeU =
          ResourceDeclHelpers::getFilteredParameterValue(declarationNode.child("wrap").attribute("u").as_string(),
                                                         "wrap_u", wrapModesMap, GL_CLAMP_TO_EDGE);

      parameters.wrapModeU = wrapModeU;
    }

    if (declarationNode.child("wrap").attribute("v")) {
      GLint wrapModeV =
          ResourceDeclHelpers::getFilteredParameterValue(declarationNode.child("wrap").attribute("v").as_string(),
                                                         "wrap_v", wrapModesMap, GL_CLAMP_TO_EDGE);

      parameters.wrapModeV = wrapModeV;
    }

    if (declarationNode.child("wrap").attribute("w")) {
      GLint wrapModeW =
          ResourceDeclHelpers::getFilteredParameterValue(declarationNode.child("wrap").attribute("w").as_string(),
                                                         "wrap_w", wrapModesMap, GL_CLAMP_TO_EDGE);

      parameters.wrapModeW = wrapModeW;
    }
  }

  return parameters;
}

std::shared_ptr<GLTexture> TextureResource::getTexture() const {
  return m_texture;
}


#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include <pugixml.hpp>

#include "Modules/ResourceManagement/Resource.h"
#include "Modules/Graphics/OpenGL/GLTexture.h"

struct TextureResourceParameters : ResourceSourceParameters {
  TextureResourceParameters() = default;

  TextureResourceParameters(GLTextureType type, GLTextureInternalFormat internalFormat, bool autoGenerateMipmaps,
    GLint minificationFilter, GLint magnificationFilter,
    GLint wrapModeU, GLint wrapModeV, GLint wrapModeW)
    : type(type),
      internalFormat(internalFormat),
      autoGenerateMipmaps(autoGenerateMipmaps),
      minificationFilter(minificationFilter),
      magnificationFilter(magnificationFilter),
      wrapModeU(wrapModeU),
      wrapModeV(wrapModeV),
      wrapModeW(wrapModeW)
  {}

  GLTextureType type = GLTextureType::Texture2D;
  GLTextureInternalFormat internalFormat = GLTextureInternalFormat::RGB8;
  bool autoGenerateMipmaps = false;
  GLint minificationFilter = GL_LINEAR;
  GLint magnificationFilter = GL_LINEAR;
  GLint wrapModeU = GL_CLAMP_TO_EDGE;
  GLint wrapModeV = GL_CLAMP_TO_EDGE;
  GLint wrapModeW = GL_CLAMP_TO_EDGE;
};

class TextureResource : public Resource {
 public:
  using ParametersType = TextureResourceParameters;

 public:
  TextureResource();
  ~TextureResource() override;

  void load(const ResourceDeclaration& declaration, ResourceManager& resourceManager) override;
  void unload() override;

  [[nodiscard]] bool isBusy() const override;

  static std::shared_ptr<GLTexture> loadFromFile(const std::string& path,
    const TextureResourceParameters& parameters);

  static ParametersType buildDeclarationParameters(const pugi::xml_node& declarationNode,
    const ParametersType& defaultParameters);

 public:
  [[nodiscard]] std::shared_ptr<GLTexture> getTexture() const;

 private:
  std::shared_ptr<GLTexture> m_texture;
};

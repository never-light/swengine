#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include <pugixml.hpp>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/OpenGL/GLTexture.h"

struct TextureResourceParameters {
  TextureResourceParameters() = default;

  std::string resourcePath;

  GLTextureType type = GLTextureType::Texture2D;
  GLTextureInternalFormat internalFormat = GLTextureInternalFormat::RGB8;
  bool autoGenerateMipmaps = false;
  GLint minificationFilter = GL_LINEAR;
  GLint magnificationFilter = GL_LINEAR;
  GLint wrapModeU = GL_CLAMP_TO_EDGE;
  GLint wrapModeV = GL_CLAMP_TO_EDGE;
  GLint wrapModeW = GL_CLAMP_TO_EDGE;
};


class TextureResource : public ResourceTypeManager<GLTexture, TextureResourceParameters> {
 public:
  explicit TextureResource(ResourcesManager* resourcesManager);
  ~TextureResource() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};

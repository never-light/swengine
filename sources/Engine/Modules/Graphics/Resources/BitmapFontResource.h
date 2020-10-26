#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/GUI/BitmapFont.h"

struct BitmapFontResourceParameters {
  BitmapFontResourceParameters() = default;

  std::string resourcePath;
};

class BitmapFontResource : public ResourceTypeManager<BitmapFont, BitmapFontResourceParameters> {
 public:
  explicit BitmapFontResource(ResourcesManager* resourcesManager);
  ~BitmapFontResource() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};

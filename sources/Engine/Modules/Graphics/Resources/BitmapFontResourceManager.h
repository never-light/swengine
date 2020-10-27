#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/GUI/BitmapFont.h"

struct BitmapFontResourceConfig {
  BitmapFontResourceConfig() = default;

  std::string resourcePath;
};

class BitmapFontResourceManager : public ResourceManager<BitmapFont, BitmapFontResourceConfig> {
 public:
  explicit BitmapFontResourceManager(ResourcesManager* resourcesManager);
  ~BitmapFontResourceManager() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};

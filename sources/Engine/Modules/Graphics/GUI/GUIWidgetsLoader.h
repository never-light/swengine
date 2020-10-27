#pragma once

#include <utility>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/Resources/TextureResourceManager.h"

#include "Utility/xml.h"

#include "GUIWidget.h"
#include "GUILayout.h"

class GUISystem;

struct WidgetClassLoadingData {
 public:
  using WidgetLoaderCallback = std::function<std::shared_ptr<GUIWidget>(const pugi::xml_node&)>;

  template<class WidgetType>
  static inline WidgetLoaderCallback genGenericWidgetLoader()
  {
    return [](const pugi::xml_node& widgetData) {
      ARG_UNUSED(widgetData);

      return std::make_shared<WidgetType>();
    };
  };

 public:
  WidgetLoaderCallback m_widgetLoader;
};

class GUIWidgetsLoader {
 public:
  using PropertyParser = std::function<GUIWidgetStylesheetProperty::Value(const pugi::xml_node&)>;
 public:
  explicit GUIWidgetsLoader(
    std::weak_ptr<GUISystem> guiSystem,
    std::shared_ptr<ResourcesManager> resourceManager);
  ~GUIWidgetsLoader();

  void registerWidgetLoader(
    const std::string& widgetClass,
    const WidgetClassLoadingData::WidgetLoaderCallback& loader);

  void registerPropertyTypeParser(
    const std::string& typeName,
    const PropertyParser& parser);

  [[nodiscard]] std::shared_ptr<GUILayout> loadScheme(const std::string& schemePath);
  [[nodiscard]] GUIWidgetStylesheet loadStylesheet(const std::string& stylesheetPath);

 private:
  [[nodiscard]] GUIWidgetStylesheet loadStylesheet(const pugi::xml_node& stylesheetNode);

  [[nodiscard]] std::shared_ptr<GUIWidget> loadSchemeWidget(std::shared_ptr<GUIWidget> parentWidget,
    const pugi::xml_node& widgetNode);

  [[nodiscard]] WidgetClassLoadingData::WidgetLoaderCallback getWidgetLoader(const std::string& widgetClass) const;
  [[nodiscard]] PropertyParser getPropertyTypeParser(const std::string& typeName) const;

 private:
  std::shared_ptr<GUIWidget> loadDropDownListWidget(const pugi::xml_node& widgetData);

 private:
  std::weak_ptr<GUISystem> m_guiSystem;
  std::shared_ptr<ResourcesManager> m_resourceManager;

  std::unordered_map<std::string, WidgetClassLoadingData> m_widgetsLoaders;
  std::unordered_map<std::string, PropertyParser> m_propertiesTypeParsers;
};

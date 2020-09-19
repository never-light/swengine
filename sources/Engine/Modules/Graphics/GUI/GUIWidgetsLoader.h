#pragma once

#include <utility>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/Resources/TextureResource.h"

#include "Utility/xml.h"

#include "GUIWidget.h"
#include "GUILayout.h"

class GUISystem;

template<class T>
struct GUIGenericPropertyParser {
  static inline T parse(const pugi::xml_node& propertyNode, ResourceManager& resourceManager)
  {
    ARG_UNUSED(propertyNode);
    ARG_UNUSED(resourceManager);

    SW_ASSERT(false && "Implement appropriate loader specialization for the specified type");
  }
};

template<>
struct GUIGenericPropertyParser<glm::vec4> {
  static inline glm::vec4 parse(const pugi::xml_node& propertyNode, ResourceManager& resourceManager)
  {
    ARG_UNUSED(resourceManager);

    return StringUtils::stringToVec4(propertyNode.attribute("value").as_string());
  }
};

template<>
struct GUIGenericPropertyParser<std::shared_ptr<GLTexture>> {
  static inline std::shared_ptr<GLTexture> parse(const pugi::xml_node& propertyNode,
    ResourceManager& resourceManager)
  {
    std::string imageResourceName = propertyNode.attribute("value").as_string();

    return resourceManager.getResourceFromInstance<TextureResource>(imageResourceName)->getTexture();
  }
};

struct WidgetClassLoadingData {
 public:

  using WidgetLoaderCallback = std::function<std::shared_ptr<GUIWidget>(const pugi::xml_node&)>;
  using PropertyLoaderCallback = std::function<void(GUIWidget&, GUIWidgetVisualState,
    const pugi::xml_node&, ResourceManager&)>;

  template<class WidgetType,
    class PropertyType,
    class PropertySetterPtr>
  static inline PropertyLoaderCallback genGenericPropertyLoader(PropertySetterPtr setterMethod)
  {
    return [setterMethod](GUIWidget& widget, GUIWidgetVisualState visualState,
      const pugi::xml_node& propertyNode, ResourceManager& resourceManager) {
      PropertyType property = GUIGenericPropertyParser<PropertyType>::parse(propertyNode, resourceManager);

      auto& widgetRef = dynamic_cast<WidgetType&>(widget);
      (widgetRef.*setterMethod)(property, visualState);
    };
  }

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
  std::unordered_map<std::string, PropertyLoaderCallback> m_propertiesLoaders;
};

template<class WidgetType, class PropertyType, class SetterPtr>
struct GUIGenericPropertySetter {
 public:
  std::string propertyName;
  SetterPtr propertySetter;

  inline WidgetClassLoadingData::PropertyLoaderCallback genLoaderCallback()
  {
    return WidgetClassLoadingData::genGenericPropertyLoader<WidgetType, PropertyType, SetterPtr>(propertySetter);
  }

};

class GUIWidgetsLoader {
 public:

 public:
  explicit GUIWidgetsLoader(
    std::weak_ptr<GUISystem> guiSystem,
    std::shared_ptr<ResourceManager> resourceManager);
  ~GUIWidgetsLoader();

  void registerWidgetLoader(
    const std::string& widgetClass,
    const WidgetClassLoadingData::WidgetLoaderCallback& loader);

  void registerPropertyLoader(
    const std::string& widgetClass,
    const std::string& propertyName,
    const WidgetClassLoadingData::PropertyLoaderCallback& loader);

  template<class WidgetType, class... PropertySettersTypes>
  void registerGenericWidgetLoader(const std::string& widgetName,
    PropertySettersTypes... setters)
  {
    registerWidgetLoader(widgetName, WidgetClassLoadingData::genGenericWidgetLoader<WidgetType>());
    registerGenericPropertiesLoaders<WidgetType>(widgetName, setters...);
  }

  template<class WidgetType, class PropertySetterType, class... PropertySettersTypes>
  void registerGenericPropertiesLoaders(
    const std::string& widgetName,
    PropertySetterType setter,
    PropertySettersTypes... setters)
  {
    registerGenericPropertiesLoaders<WidgetType>(widgetName, setter);
    registerGenericPropertiesLoaders<WidgetType>(widgetName, setters...);
  }

  template<class WidgetType, class PropertySetterType>
  void registerGenericPropertiesLoaders(
    const std::string& widgetName,
    PropertySetterType setter)
  {
    registerPropertyLoader(widgetName, setter.propertyName, setter.genLoaderCallback());
  }

  [[nodiscard]] std::shared_ptr<GUILayout> loadScheme(const std::string& schemePath);

 private:
  [[nodiscard]] std::shared_ptr<GUIWidget> loadSchemeWidget(std::shared_ptr<GUIWidget> parentWidget,
    const pugi::xml_node& widgetNode);

  [[nodiscard]] WidgetClassLoadingData::WidgetLoaderCallback getWidgetLoader(const std::string& widgetClass) const;
  [[nodiscard]] WidgetClassLoadingData::PropertyLoaderCallback getPropertyLoader(
    const std::string& widgetClass,
    const std::string& propertyName) const;

 private:
  std::weak_ptr<GUISystem> m_guiSystem;
  std::shared_ptr<ResourceManager> m_resourceManager;

  std::unordered_map<std::string, WidgetClassLoadingData> m_widgetsLoaders;
};

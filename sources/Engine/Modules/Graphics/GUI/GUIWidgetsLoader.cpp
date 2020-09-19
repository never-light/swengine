#include "precompiled.h"

#pragma hdrstop

#include "GUIWidgetsLoader.h"

#include <utility>

#include "Utility/files.h"

#include "Utility/xml.h"
#include "Exceptions/exceptions.h"

#include "GUISystem.h"
#include "GUILayout.h"
#include "GUIButton.h"
#include "GUITextBox.h"
#include "GUIText.h"
#include "GUIImage.h"

GUIWidgetsLoader::GUIWidgetsLoader(std::weak_ptr<GUISystem> guiSystem,
  std::shared_ptr<ResourceManager> resourceManager)
  : m_guiSystem(std::move(guiSystem)),
    m_resourceManager(std::move(resourceManager))
{
  registerGenericWidgetLoader<GUILayout>(
    "layout",
    GUIGenericPropertySetter<GUILayout, std::shared_ptr<GLTexture>, decltype(&GUILayout::setBackgroundImage)>
      {"background_image", &GUILayout::setBackgroundImage},
    GUIGenericPropertySetter<GUILayout, glm::vec4, decltype(&GUILayout::setBackgroundColor)>
      {"background_color", &GUILayout::setBackgroundColor}
  );

  registerGenericWidgetLoader<GUIButton>(
    "button",
    GUIGenericPropertySetter<GUIButton, std::shared_ptr<GLTexture>, decltype(&GUIButton::setBackgroundImage)>
      {"background_image", &GUIButton::setBackgroundImage},
    GUIGenericPropertySetter<GUIButton, glm::vec4, decltype(&GUIButton::setBackgroundColor)>
      {"background_color", &GUIButton::setBackgroundColor}
  );
}

GUIWidgetsLoader::~GUIWidgetsLoader() = default;

void GUIWidgetsLoader::registerWidgetLoader(const std::string& widgetClass,
  const WidgetClassLoadingData::WidgetLoaderCallback& loader)
{
  SW_ASSERT(!m_widgetsLoaders.contains(widgetClass));

  m_widgetsLoaders.insert({widgetClass, WidgetClassLoadingData{loader, {}}});
}

void GUIWidgetsLoader::registerPropertyLoader(const std::string& widgetClass,
  const std::string& propertyName,
  const WidgetClassLoadingData::PropertyLoaderCallback& loader)
{
  m_widgetsLoaders.at(widgetClass).m_propertiesLoaders.insert({propertyName, loader});
}

WidgetClassLoadingData::WidgetLoaderCallback GUIWidgetsLoader::getWidgetLoader(const std::string& widgetClass) const
{
  return m_widgetsLoaders.at(widgetClass).m_widgetLoader;
}

WidgetClassLoadingData::PropertyLoaderCallback GUIWidgetsLoader::getPropertyLoader(const std::string& widgetClass,
  const std::string& propertyName) const
{
  auto widgetLoadersDataIt = m_widgetsLoaders.find(widgetClass);

  if (widgetLoadersDataIt == m_widgetsLoaders.end()) {
    return nullptr;
  }

  auto propertyLoaderIt = widgetLoadersDataIt->second.m_propertiesLoaders.find(propertyName);

  if (propertyLoaderIt == widgetLoadersDataIt->second.m_propertiesLoaders.end()) {
    return nullptr;
  }

  return propertyLoaderIt->second;
}

std::shared_ptr<GUILayout> GUIWidgetsLoader::loadScheme(const std::string& schemePath)
{
  auto[schemeDescriptionDocument, schemeDescription] = XMLUtils::openDescriptionFile(schemePath,
    "widget");

  LOCAL_VALUE_UNUSED(schemeDescriptionDocument);

  return std::dynamic_pointer_cast<GUILayout>(
    loadSchemeWidget(m_guiSystem.lock()->getActiveLayout(), schemeDescription));
}

[[nodiscard]] std::shared_ptr<GUIWidget> GUIWidgetsLoader::loadSchemeWidget(std::shared_ptr<GUIWidget> parentWidget,
  const pugi::xml_node& widgetNode)
{
  std::string widgetType = widgetNode.attribute("class").as_string();

  auto widgetLoader = getWidgetLoader(widgetType);
  std::shared_ptr<GUIWidget> widget = widgetLoader(widgetNode);

  // Name attribute

  // TODO: check for widget name uniqueness

  auto widgetAttr = widgetNode.attribute("name");

  if (widgetAttr) {
    widget->setName(widgetAttr.as_string());
  }

  // Size attribute

  pugi::xml_attribute sizeAttr = widgetNode.attribute("size");

  if (sizeAttr) {
    widget->setSize(StringUtils::stringToIVec2(sizeAttr.as_string()));
  }
  else {
    widget->setSize(parentWidget->getSize());
  }

  // Origin attribute

  pugi::xml_attribute originAttr = widgetNode.attribute("origin");

  if (originAttr) {
    glm::ivec2 origin = {0, 0};

    auto parts = StringUtils::split(originAttr.as_string(), ' ');

    if (parts[0] == "center") {
      origin.x = parentWidget->getSize().x / 2;
    }
    else {
      origin.x = std::stoi(parts[0]);
    }

    if (parts[1] == "center") {
      origin.y = parentWidget->getSize().y / 2;
    }
    else {
      origin.y = std::stoi(parts[0]);
    }

    widget->setOrigin(origin);
  }
  else {
    widget->setOrigin({0, 0});
  }

  pugi::xml_attribute originOffsetAttr = widgetNode.attribute("origin_offset");

  if (originOffsetAttr) {
    widget->setOrigin(widget->getOrigin() + StringUtils::stringToIVec2(originOffsetAttr.as_string()));
  }

  // Visual parameters

  pugi::xml_node visualParameters = widgetNode.child("styles");

  if (visualParameters) {
    for (auto styleSheetNode : visualParameters.children("stylesheet")) {
      std::string styleSheetType = styleSheetNode.attribute("state").as_string("default");

      GUIWidgetVisualState visualState = GUIWidgetVisualState::Default;

      if (styleSheetType == "default") {
        visualState = GUIWidgetVisualState::Default;
      }
      else if (styleSheetType == "hover") {
        visualState = GUIWidgetVisualState::Hover;
      }
      else if (styleSheetType == "focus") {
        visualState = GUIWidgetVisualState::Focus;
      }
      else {
        THROW_EXCEPTION(EngineRuntimeException,
          fmt::format("GUI widget stylesheet state {} is invalid", styleSheetType));
      }

      for (auto propertyNode : styleSheetNode.children("property")) {
        std::string propertyName = propertyNode.attribute("name").as_string();

        auto propertyLoader = getPropertyLoader(widgetType, propertyName);

        if (propertyLoader == nullptr) {
          THROW_EXCEPTION(EngineRuntimeException,
            fmt::format("Widget {} has invalid property {}", widgetType, propertyName));
        }

        propertyLoader(*widget, visualState, propertyNode, *m_resourceManager);
      }
    }
  }

  // Children widgets

  pugi::xml_node childrenWidgetsNode = widgetNode.child("widgets");

  if (childrenWidgetsNode) {
    for (auto childWidgetNode : childrenWidgetsNode.children("widget")) {
      widget->addChildWidget(loadSchemeWidget(widget, childWidgetNode));
    }
  }

  return widget;
}

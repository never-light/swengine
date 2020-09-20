#include "precompiled.h"

#pragma hdrstop

#include "GUIWidgetsLoader.h"

#include <utility>

#include "Modules/Graphics/Resources/BitmapFontResource.h"

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
  // Image property
  registerPropertyTypeParser("image", [this](const pugi::xml_node& propertyNode) -> GUIWidgetStylesheetProperty::Value {
    std::string imageResourceName = propertyNode.attribute("value").as_string();

    return m_resourceManager->getResourceFromInstance<TextureResource>(imageResourceName)->getTexture();
  });

  // Font property
  registerPropertyTypeParser("font", [this](const pugi::xml_node& propertyNode) -> GUIWidgetStylesheetProperty::Value {
    std::string fontResourceName = propertyNode.attribute("value").as_string();

    return m_resourceManager->getResourceFromInstance<BitmapFontResource>(fontResourceName)->getFont();
  });

  // Color property
  registerPropertyTypeParser("color", [this](const pugi::xml_node& propertyNode) -> GUIWidgetStylesheetProperty::Value {
    return StringUtils::stringToVec4(propertyNode.attribute("value").as_string());
  });

  // Integer property
  registerPropertyTypeParser("integer", [this](const pugi::xml_node& propertyNode) -> GUIWidgetStylesheetProperty::Value {
    return propertyNode.attribute("value").as_int();
  });

  // Float property
  registerPropertyTypeParser("float", [this](const pugi::xml_node& propertyNode) -> GUIWidgetStylesheetProperty::Value {
    return propertyNode.attribute("value").as_float();
  });

  registerWidgetLoader("layout", WidgetClassLoadingData::genGenericWidgetLoader<GUILayout>());
  registerWidgetLoader("button", WidgetClassLoadingData::genGenericWidgetLoader<GUIButton>());
}

GUIWidgetsLoader::~GUIWidgetsLoader() = default;

void GUIWidgetsLoader::registerWidgetLoader(const std::string& widgetClass,
  const WidgetClassLoadingData::WidgetLoaderCallback& loader)
{
  SW_ASSERT(!m_widgetsLoaders.contains(widgetClass));

  m_widgetsLoaders.insert({widgetClass, WidgetClassLoadingData{loader}});
}

void GUIWidgetsLoader::registerPropertyTypeParser(const std::string& typeName,
  const GUIWidgetsLoader::PropertyParser& parser)
{
  m_propertiesTypeParsers.insert({typeName, parser});
}

WidgetClassLoadingData::WidgetLoaderCallback GUIWidgetsLoader::getWidgetLoader(const std::string& widgetClass) const
{
  return m_widgetsLoaders.at(widgetClass).m_widgetLoader;
}

GUIWidgetsLoader::PropertyParser GUIWidgetsLoader::getPropertyTypeParser(const std::string& typeName) const
{
  auto typeParserIt = m_propertiesTypeParsers.find(typeName);

  if (typeParserIt == m_propertiesTypeParsers.end()) {
    return nullptr;
  }

  return typeParserIt->second;
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

  pugi::xml_node stylesheetNode = widgetNode.child("stylesheet");

  if (stylesheetNode) {
    GUIWidgetStylesheet stylesheet = loadStylesheet(stylesheetNode);
    widget->applyStylesheet(stylesheet);
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

GUIWidgetStylesheet GUIWidgetsLoader::loadStylesheet(const pugi::xml_node& stylesheetNode)
{
  GUIWidgetStylesheet stylesheet;

  for (pugi::xml_node ruleNode : stylesheetNode.children("rule")) {
    std::string ruleVisualState = ruleNode.attribute("state").as_string("default");

    GUIWidgetVisualState visualState = GUIWidgetVisualState::Default;

    if (ruleVisualState == "default") {
      visualState = GUIWidgetVisualState::Default;
    }
    else if (ruleVisualState == "hover") {
      visualState = GUIWidgetVisualState::Hover;
    }
    else if (ruleVisualState == "focus") {
      visualState = GUIWidgetVisualState::Focus;
    }
    else {
      THROW_EXCEPTION(EngineRuntimeException,
        fmt::format("GUI widget stylesheet rule state {} is invalid", ruleVisualState));
    }

    std::vector<GUIWidgetStylesheetSelectorPart> selectorData;

    auto selectorRawParts = StringUtils::split(ruleNode.attribute("selector").as_string(), ' ');

    for (const auto& part : selectorRawParts) {
      auto selectorPartItems = StringUtils::split(part, '#');

      std::string classFilter = selectorPartItems[0];
      std::string nameFilter = selectorPartItems.size() >= 2 ? selectorPartItems[1] : "";

      selectorData.emplace_back(classFilter, nameFilter);
    }

    GUIWidgetStylesheetRule rule(visualState, selectorData);

    for (auto propertyNode : ruleNode.children("property")) {
      std::string propertyName = propertyNode.attribute("name").as_string();
      std::string propertyType = propertyNode.attribute("type").as_string();

      auto propertyTypeParser = getPropertyTypeParser(propertyType);

      if (propertyTypeParser == nullptr) {
        THROW_EXCEPTION(EngineRuntimeException,
          fmt::format("GUI widget stylesheet property {} has invalid type {}", propertyName, propertyType));
      }

      GUIWidgetStylesheetProperty property(propertyTypeParser(propertyNode));

      rule.setProperty(propertyName, property);
    }

    stylesheet.addRule(rule);
  }

  return stylesheet;
}

GUIWidgetStylesheet GUIWidgetsLoader::loadStylesheet(const std::string& stylesheetPath)
{
  auto[stylesheetDescriptionDocument, stylesheetDescription] = XMLUtils::openDescriptionFile(stylesheetPath,
    "stylesheet");

  LOCAL_VALUE_UNUSED(stylesheetDescriptionDocument);

  return loadStylesheet(stylesheetDescription);
}
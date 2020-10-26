#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <memory>
#include <functional>

#include <glm/vec4.hpp>

#include "Modules/Graphics/OpenGL/GLTexture.h"
#include "BitmapFont.h"

enum class GUIWidgetVisualState {
  Default,
  Hover,
  Focus
};

template<class... Ts>
struct GUIWidgetStylesheetPropertyVisitor : Ts ... { using Ts::operator()...; };
template<class... Ts> GUIWidgetStylesheetPropertyVisitor(Ts...) -> GUIWidgetStylesheetPropertyVisitor<Ts...>;

struct GUIWidgetStylesheetProperty {
 public:
  using Value = std::variant<
    std::string,
    ResourceHandle<GLTexture>,
    ResourceHandle<BitmapFont>,
    glm::vec4,
    glm::ivec2,
    int,
    float>;

 public:
  GUIWidgetStylesheetProperty() = default;
  explicit GUIWidgetStylesheetProperty(Value value);

  void setValue(const Value& value);
  [[nodiscard]] const Value& getValue() const;

 private:
  Value m_value;
};

struct GUIWidgetStylesheetSelectorPart {
 public:
  GUIWidgetStylesheetSelectorPart(std::string classFilter, std::string nameFilter)
    : m_classFilter(std::move(classFilter)),
      m_nameFilter(std::move(nameFilter))
  {

  }

  [[nodiscard]] const std::string& getClassFilter() const
  {
    return m_classFilter;
  }

  [[nodiscard]] const std::string& getNameFilter() const
  {
    return m_nameFilter;
  }

 private:
  std::string m_classFilter;
  std::string m_nameFilter;
};

class GUIWidgetStylesheetRule {
 public:
  using PropertiesVisitor = std::function<void(const std::string&,
    const GUIWidgetStylesheetProperty&,
    GUIWidgetVisualState)>;

 public:
  explicit GUIWidgetStylesheetRule(
    GUIWidgetVisualState visualState,
    std::vector<GUIWidgetStylesheetSelectorPart> selector);

  [[nodiscard]] GUIWidgetVisualState getVisualState() const;

  [[nodiscard]] const std::vector<GUIWidgetStylesheetSelectorPart>& getSelector() const;
  [[nodiscard]] const GUIWidgetStylesheetSelectorPart& getSelectorPart(size_t index) const;

  [[nodiscard]] bool isSelectionCompleted(size_t selectorPartIndex) const;

  void setProperty(const std::string& propertyName, const GUIWidgetStylesheetProperty& property);

  void setPropertyValue(const std::string& propertyName, const GUIWidgetStylesheetProperty::Value& value);
  [[nodiscard]] const GUIWidgetStylesheetProperty::Value& getPropertyValue(const std::string& propertyName) const;

  void visit(const PropertiesVisitor& visitor) const;

 private:
  GUIWidgetVisualState m_visualState;
  std::unordered_map<std::string, GUIWidgetStylesheetProperty> m_properties;
  std::vector<GUIWidgetStylesheetSelectorPart> m_selector;
};

class GUIWidgetStylesheet {
 public:
  GUIWidgetStylesheet() = default;

  void addRule(const GUIWidgetStylesheetRule& rule);

  [[nodiscard]] const std::vector<GUIWidgetStylesheetRule>& getRules() const;

 private:
  std::vector<GUIWidgetStylesheetRule> m_rules;
};

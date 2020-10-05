#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include <functional>

#include "Modules/Input/InputEvents.h"
#include "Modules/Graphics/OpenGL/GLTexture.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "GUIWidgetVisualParameters.h"
#include "GUIWidgetStylesheet.h"

struct GUIEvent {
};

struct GUIMouseButtonEvent : GUIEvent {
  MouseButtonEventType type{};
  uint8_t button{};

  bool isExclusive = false;
};

struct GUIMouseEnterEvent : GUIEvent {
};

struct GUIMouseLeaveEvent : GUIEvent {
};

struct GUIMouseMoveEvent : GUIEvent {
  int x;
  int y;

  int deltaX;
  int deltaY;
};

struct GUIKeyboardEvent : GUIEvent {
  KeyboardEventType type;
  SDL_Keycode keyCode;
  bool repeated;
  SDL_Keymod keyModifiers;
};

class GUISystem;

class GUIWidget : public std::enable_shared_from_this<GUIWidget> {
 public:
  template<class T>
  using EventCallback = std::function<void(const T&)>;

 public:
  explicit GUIWidget(std::string className);
  virtual ~GUIWidget();

  void setName(const std::string& name);
  [[nodiscard]] const std::string& getName() const;

  void setOrigin(const glm::ivec2& origin);
  [[nodiscard]] glm::ivec2 getOrigin() const;

  [[nodiscard]] glm::ivec2 getAbsoluteOrigin() const;

  virtual void setSize(const glm::ivec2& size);
  [[nodiscard]] virtual glm::ivec2 getSize() const;

  // TODO: remove this methods
  virtual void setWidth(int width);
  virtual void setHeight(int height);

  void addChildWidget(std::shared_ptr<GUIWidget> widget);
  void removeChildWidget(const std::shared_ptr<GUIWidget>& widget);

  std::shared_ptr<GUIWidget> findChildByName(const std::string& name) const;

  [[nodiscard]] const std::vector<std::shared_ptr<GUIWidget>>& getChildrenWidgets() const;

  void removeChildren();

  void show();
  void hide();

  [[nodiscard]] bool isShown() const;
  [[nodiscard]] bool isHovered() const;

  [[nodiscard]] bool hasFocus() const;

  [[nodiscard]] virtual bool canHaveFocus() const;

  virtual void update(float delta);
  virtual void render(GUISystem& guiSystem);

  void setZIndex(int zIndex);
  [[nodiscard]] int getZIndex() const;

  [[nodiscard]] const glm::mat4x4& getTransformationMatrix();

  void setMouseButtonCallback(EventCallback<GUIMouseButtonEvent> callback);
  void setMouseEnterCallback(EventCallback<GUIMouseEnterEvent> callback);
  void setMouseLeaveCallback(EventCallback<GUIMouseLeaveEvent> callback);
  void setKeyboardEventCallback(EventCallback<GUIKeyboardEvent> callback);

  [[nodiscard]] std::shared_ptr<GUIWidget> getParent() const;
  [[nodiscard]] RectI getRect() const;

  [[nodiscard]] bool isPointInside(const glm::ivec2& point) const;

  void applyStylesheetRuleWithSelector(const GUIWidgetStylesheetRule& stylesheetRule,
    std::vector<GUIWidgetStylesheetSelectorPart> currentPath);

  virtual void applyStylesheetRuleToChildren(const GUIWidgetStylesheetRule& stylesheetRule,
    const std::vector<GUIWidgetStylesheetSelectorPart>& currentPath);

  virtual void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule);

  void applyStylesheet(const GUIWidgetStylesheet& stylesheet);

 protected:
  virtual void onShow();
  virtual void onHide();

  void updateChildStyles(std::shared_ptr<GUIWidget> childWidget);

 protected:
  void resetTransformationCache();

  [[nodiscard]] virtual glm::mat4 updateTransformationMatrix();

 protected:
  const GUIWidgetVisualParameters& getVisualParameters(GUIWidgetVisualState state) const;
  GUIWidgetVisualParameters& getVisualParameters(GUIWidgetVisualState state);

  static bool isPathSatisfiesSelector(const std::vector<GUIWidgetStylesheetSelectorPart>& path,
    const std::vector<GUIWidgetStylesheetSelectorPart>& selector);

 protected:
  virtual void processKeyboardEvent(const GUIKeyboardEvent& event);
  virtual void processMouseButtonEvent(const GUIMouseButtonEvent& event);

  virtual void onSetFocus();
  virtual void onLostFocus();

 private:
  void triggerMouseButtonEvent(const GUIMouseButtonEvent& event, std::vector<std::function<void()>>& eventsQueue);
  void triggerMouseEnterEvent(const GUIMouseEnterEvent& event, std::vector<std::function<void()>>& eventsQueue);
  void triggerMouseLeaveEvent(const GUIMouseLeaveEvent& event, std::vector<std::function<void()>>& eventsQueue);

  void triggerKeyboardEvent(const GUIKeyboardEvent& event, std::vector<std::function<void()>>& eventsQueue);

  void setParent(std::weak_ptr<GUIWidget> parent);

  void setFocus();
  void resetFocus();

  void showChildren(GUIWidget* parent);
  void hideChildren(GUIWidget* parent);

  void orderChildrenByZIndex();

 private:
  std::string m_className;
  std::string m_name;

  glm::ivec2 m_origin = glm::ivec2(0);
  glm::ivec2 m_size = glm::ivec2(0);

  std::vector<GUIWidgetVisualParameters> m_visualParameters =
    std::vector<GUIWidgetVisualParameters>(3, GUIWidgetVisualParameters{});

  // TODO: set default widget state as not shown
  bool m_isShown = true;

  bool m_isHovered = false;
  bool m_hasFocus = false;

  int m_zIndex = 0;

  std::vector<std::shared_ptr<GUIWidget>> m_widgets;
  std::weak_ptr<GUIWidget> m_parent;

  EventCallback<GUIMouseButtonEvent> m_mouseButtonCallback;
  EventCallback<GUIMouseEnterEvent> m_mouseEnterCallback;
  EventCallback<GUIMouseLeaveEvent> m_mouseLeaveCallback;
  EventCallback<GUIKeyboardEvent> m_keyboardEventCallback;

  glm::mat4x4 m_transformationMatrixCache{};
  bool m_needTransformationMatrixCacheUpdate = true;

  std::vector<GUIWidgetStylesheet> m_stylesheets;

 private:
  friend class GUISystem;
};

class GUIWidgetRect : public GUIWidget {
 public:
  explicit GUIWidgetRect(const std::string& className)
    : GUIWidget(className)
  {

  }

  ~GUIWidgetRect() override = default;

  inline void setBackgroundColor(const glm::vec4& color,
    GUIWidgetVisualState visualState = GUIWidgetVisualState::Default)
  {
    getVisualParameters(visualState).setBackgroundColor(color);
  }

  [[nodiscard]] inline glm::vec4 getBackgroundColor(GUIWidgetVisualState visualState = GUIWidgetVisualState::Default) const
  {
    return getVisualParameters(visualState).getBackgroundColor().value();
  }

  inline void setBackgroundImage(std::shared_ptr<GLTexture> image,
    GUIWidgetVisualState visualState = GUIWidgetVisualState::Default)
  {
    return getVisualParameters(visualState).setBackgroundImage(std::move(image));
  }

  [[nodiscard]] inline std::shared_ptr<GLTexture> getBackgroundImage(
    GUIWidgetVisualState visualState = GUIWidgetVisualState::Default) const
  {
    return getVisualParameters(visualState).getBackgroundImage();
  }

  inline void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule) override
  {
    GUIWidget::applyStylesheetRule(stylesheetRule);

    stylesheetRule.visit([this](auto propertyName, auto property, GUIWidgetVisualState visualState) {
      if (propertyName == "background") {
        // Background
        std::visit(GUIWidgetStylesheetPropertyVisitor{
          [](auto arg) { ARG_UNUSED(arg); SW_ASSERT(false); },
          // Background color
          [this, visualState](const glm::vec4& color) {
            this->setBackgroundImage(nullptr, visualState);
            this->setBackgroundColor(color, visualState);
          },
          // Background image
          [this, visualState](const std::shared_ptr<GLTexture>& image) {
            this->setBackgroundImage(image, visualState);
          },
        }, property.getValue());
      }
    });
  }

};
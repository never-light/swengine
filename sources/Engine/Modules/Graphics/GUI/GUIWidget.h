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

struct GUIEvent {
};

struct GUIMouseButtonEvent : GUIEvent {
    MouseButtonEventType type;
    uint8_t button;

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

class GUIWidget : public std::enable_shared_from_this<GUIWidget>
{
public:
    template<class T>
    using EventCallback = std::function<void(const T&)>;

public:
    GUIWidget() = default;
    virtual ~GUIWidget() = default;

    void setOrigin(const glm::ivec2& origin);
    glm::ivec2 getOrigin() const;

    glm::ivec2 getAbsoluteOrigin() const;

    void setSize(const glm::ivec2& size);
    glm::ivec2 getSize() const;

    void setWidth(int width);
    void setHeight(int height);

    void addChildWidget(std::shared_ptr<GUIWidget> widget);
    void removeChildWidget(const std::shared_ptr<GUIWidget>& widget);

    const std::vector<std::shared_ptr<GUIWidget>>& getChildrenWidgets() const;

    void removeChildren();

    void show();
    void hide();

    bool isShown() const;
    bool isHovered() const;

    bool hasFocus() const;

    virtual bool canHaveFocus() const;

    virtual void update(float delta);
    virtual void render(GUISystem& guiSystem);

    void setBackgroundColor(const glm::vec4& color);
    glm::vec4 getBackgroundColor() const;

    void setBackgroundImage(std::shared_ptr<GLTexture> image);
    std::shared_ptr<GLTexture> getBackgroundImage() const;

    void setHoverBackgroundColor(const glm::vec4& color);
    glm::vec4 getHoverBackgroundColor() const;

    void setFocusBackgroundColor(const glm::vec4& color);
    glm::vec4 getFocusBackgroundColor() const;

    void setHoverBackgroundImage(std::shared_ptr<GLTexture> image);
    std::shared_ptr<GLTexture> getHoverBackgroundImage() const;

    void setBorderWidth(int width);
    int getBorderWidth() const;

    void setBorderColor(const glm::vec4& color);
    glm::vec4 getBorderColor() const;

    void setHoverBorderColor(const glm::vec4& color);
    glm::vec4 getHoverBorderColor() const;

    void setFocusBorderColor(const glm::vec4& color);
    glm::vec4 getFocusBorderColor() const;

    void setZIndex(int zIndex);
    int getZIndex() const;

    const glm::mat4x4& getTransformationMatrix();

    void setMouseButtonCallback(EventCallback<GUIMouseButtonEvent> callback);
    void setMouseEnterCallback(EventCallback<GUIMouseEnterEvent> callback);
    void setMouseLeaveCallback(EventCallback<GUIMouseLeaveEvent> callback);
    void setKeyboardEventCallback(EventCallback<GUIKeyboardEvent> callback);

    std::shared_ptr<GUIWidget> getParent() const;
    Rect getRect() const;

protected:
    void enableScaleTransform();
    void disableScaleTransform();

    void resetTransformationCache();

    virtual void transformationCacheUpdate();
    virtual void processKeyboardEvent(const GUIKeyboardEvent& event);

private:
    void triggerMouseButtonEvent(const GUIMouseButtonEvent& event);
    void triggerMouseEnterEvent(const GUIMouseEnterEvent& event);
    void triggerMouseLeaveEvent(const GUIMouseLeaveEvent& event);

    void triggerKeyboardEvent(const GUIKeyboardEvent& event);

    void setParent(std::weak_ptr<GUIWidget> parent);

    void setFocus();
    void resetFocus();

    void orderChildrenByZIndex();
private:
    glm::ivec2 m_origin = glm::ivec2(0);
    glm::ivec2 m_size = glm::ivec2(0);

    glm::vec4 m_backgroundColor = glm::vec4(0.0f);
    std::shared_ptr<GLTexture> m_backgroundImage;

    glm::vec4 m_focusBackgroundColor = glm::vec4(0.0f);

    glm::vec4 m_hoverBackgroundColor = glm::vec4(0.0f);
    std::shared_ptr<GLTexture> m_hoverBackgroundImage;

    int m_borderWidth = 0;

    glm::vec4 m_borderColor = glm::vec4(0.0f);
    glm::vec4 m_hoverBorderColor = glm::vec4(0.0f);
    glm::vec4 m_focusBorderColor = glm::vec4(0.0f);

    bool m_isScaleTransformEnabled = true;

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

    glm::mat4x4 m_transformationMatrixCache;
    bool m_needTransformationMatrixCacheUpdate = true;

private:
    friend class GUISystem;
};

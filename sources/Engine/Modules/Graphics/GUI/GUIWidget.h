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

class GUIWidget;

struct GUIEvent {
    GUIWidget* component = nullptr;
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

class GUISystem;

class GUIWidget
{
public:
    GUIWidget() = default;
    virtual ~GUIWidget() = default;

    void setOrigin(const glm::ivec2& origin);
    glm::ivec2 getOrigin() const;

    void setSize(const glm::ivec2& size);
    glm::ivec2 getSize() const;

    void addChildWidget(std::shared_ptr<GUIWidget> widget);
    void removeChildWidget(const std::shared_ptr<GUIWidget>& widget);

    const std::vector<std::shared_ptr<GUIWidget>>& getChildrenWidgets() const;

    void show();
    void hide();

    bool isShown() const;
    bool isHovered() const;

    bool hasFocus() const;

    virtual bool canHaveFocus() const;

    virtual void update(float delta);
    virtual void render(GUISystem& guiSystem);

    void setBackgroundColor(const glm::vec3& color);
    glm::vec3 getBackgroundColor() const;

    void setBackgroundImage(std::shared_ptr<GLTexture> image);
    std::shared_ptr<GLTexture> getBackgroundImage() const;

    void setHoverBackgroundColor(const glm::vec3& color);
    glm::vec3 getHoverBackgroundColor() const;

    void setHoverBackgroundImage(std::shared_ptr<GLTexture> image);
    std::shared_ptr<GLTexture> getHoverBackgroundImage() const;

    void setBorderWidth(int width);
    int getBorderWidth() const;

    void setBorderColor(const glm::vec3& color);
    glm::vec3 getBorderColor() const;

    void setHoverBorderColor(const glm::vec3& color);
    glm::vec3 getHoverBorderColor() const;

    const glm::mat4x4& getTransformationMatrix();
private:
    glm::ivec2 m_origin;
    glm::ivec2 m_size;

    glm::vec3 m_backgroundColor;
    std::shared_ptr<GLTexture> m_backgroundImage;

    glm::vec3 m_hoverBackgroundColor;
    std::shared_ptr<GLTexture> m_hoverBackgroundImage;

    int m_borderWidth = 0;

    glm::vec3 m_borderColor;
    glm::vec3 m_hoverBorderColor;

    bool m_isShown = true;
    bool m_isHovered = false;
    bool m_hasFocus = false;

    std::vector<std::shared_ptr<GUIWidget>> m_widgets;

    glm::mat4x4 m_transformationMatrixCache;
    bool m_needTransformationMatrixCacheUpdate = true;
private:
    friend class GUISystem;
};

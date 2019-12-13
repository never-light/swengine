#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Modules/ECS/ECS.h"
#include "Modules/Input/InputModule.h"
#include "Modules/Input/InputEvents.h"

#include "Modules/Graphics/OpenGL/GLShadersPipeline.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "Modules/Graphics/GraphicsSystem/Mesh.h"

#include "GUILayout.h"
#include "GUIButton.h"
#include "GUIImage.h"
#include "GUIText.h"
#include "GUITextBox.h"

class GUISystem : public GameSystem, public std::enable_shared_from_this<GUISystem>,
        public EventsListener<MouseButtonEvent>,
        public EventsListener<KeyboardEvent>
{
public:
    GUISystem(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<InputModule> inputModule,
              std::shared_ptr<GLGraphicsContext> graphicsContext,
              std::shared_ptr<GLShadersPipeline> guiShadersPipeline);

    void configure(GameWorld* gameWorld) override;
    void unconfigure(GameWorld* gameWorld) override;

    void update(GameWorld* gameWorld, float delta) override;
    void render(GameWorld* gameWorld) override;

    void setActiveLayout(std::shared_ptr<GUILayout> layout);
    std::shared_ptr<GUILayout> getActiveLayout();

    std::shared_ptr<GLGraphicsContext> getGraphicsContext() const;
    RenderTask getRenderTaskTemplate(GUIWidget* widget) const;

    EventProcessStatus receiveEvent(GameWorld* gameWorld, const MouseButtonEvent& event) override;
    EventProcessStatus receiveEvent(GameWorld *gameWorld, const KeyboardEvent &event) override;

private:
    void updateGUIWidget(GUIWidget* widget);
    void processGUIWidgetMouseButtonEvent(GUIWidget* widget, const MouseButtonEvent& event);

    bool isMouseInWidgetArea(const GUIWidget* widget) const;

    void renderGUIWidget(GUIWidget* widget);

private:
    std::unique_ptr<Mesh> m_guiNDCQuad;

    std::shared_ptr<GUILayout> m_activeLayout;

    std::shared_ptr<GameWorld> m_gameWorld;
    std::shared_ptr<InputModule> m_inputModule;
    std::shared_ptr<GLGraphicsContext> m_graphicsContext;
    std::shared_ptr<GLShadersPipeline> m_guiShadersPipeline;

    std::shared_ptr<GUIWidget> m_focusedWidget;

    glm::mat4x4 m_guiProjectionMatrix;
};


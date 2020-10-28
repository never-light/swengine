#pragma once

#include <memory>

#include "Modules/ECS/ECS.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "FrameStats.h"
#include "GraphicsScene.h"

struct LoadSceneCommandEvent {
  std::vector<GameObject> sceneObjects;
};

struct UnloadSceneCommandEvent {

};

struct AddObjectToSceneCommandEvent {
  GameObject object;
};

struct RemoveObjectFromSceneCommandEvent {
  GameObject object;
};

class GraphicsSceneManagementSystem : public GameSystem,
                                      public EventsListener<LoadSceneCommandEvent>,
                                      public EventsListener<UnloadSceneCommandEvent>,
                                      public EventsListener<AddObjectToSceneCommandEvent>,
                                      public EventsListener<RemoveObjectFromSceneCommandEvent>,
                                      public EventsListener<GameObjectAddEvent>,
                                      public EventsListener<GameObjectRemoveEvent> {
 public:
  explicit GraphicsSceneManagementSystem(std::shared_ptr<GraphicsScene> graphicsScene);

  ~GraphicsSceneManagementSystem() override;

  void configure() override;
  void unconfigure() override;

  EventProcessStatus receiveEvent(const LoadSceneCommandEvent& event) override;
  EventProcessStatus receiveEvent(const UnloadSceneCommandEvent& event) override;
  EventProcessStatus receiveEvent(const AddObjectToSceneCommandEvent& event) override;
  EventProcessStatus receiveEvent(const RemoveObjectFromSceneCommandEvent& event) override;
  EventProcessStatus receiveEvent(const GameObjectAddEvent& event) override;
  EventProcessStatus receiveEvent(const GameObjectRemoveEvent& event) override;

 private:
  std::shared_ptr<GraphicsScene> m_graphicsScene;
};

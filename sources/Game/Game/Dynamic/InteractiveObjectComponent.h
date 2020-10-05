#pragma once

#include <memory>
#include <string>
#include <functional>

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Graphics/OpenGL/GLTexture.h>

enum class InteractiveObjectActionType {
  Use,
  Take,
  Talk
};

struct InteractiveObjectActionTriggeredEvent {
 public:
  InteractiveObjectActionTriggeredEvent(GameObject initiator,
    GameObject target,
    InteractiveObjectActionType triggerType)
    : initiator(initiator),
      target(target),
      triggerType(triggerType)
  {

  }

 public:
  GameObject initiator;
  GameObject target;

  InteractiveObjectActionType triggerType;
};

struct InteractiveObjectComponent {
 public:
  // Owner, item
  using ActionCallback = std::function<void(GameObject, GameObject)>;

 public:
  InteractiveObjectComponent();

  void setName(const std::string& name);
  [[nodiscard]] const std::string& getName() const;

  void setTakeable(bool isTakeable);
  [[nodiscard]] bool isTakeable() const;

  void setUsable(bool isUsable);
  [[nodiscard]] bool isUsable() const;

  void setTalkable(bool isTalkable);
  [[nodiscard]] bool isTalkable() const;

  /*!
   * @brief Sets the use callback.
   *
   * @details Use callback is called on object use
   *
   * @param callback
   */
  void setUseCallback(const ActionCallback& callback);
  [[nodiscard]] ActionCallback getUseCallback() const;

  /*!
   * @brief Sets the take callback
   *
   * The task callback is called after some object is added
   * to inventory.
   *
   * @param callback
   */
  void setTakeCallback(const ActionCallback& callback);
  [[nodiscard]] ActionCallback getTakeCallback() const;

 private:
  std::string m_name;

  bool m_isTakeable{};
  bool m_isUsable{};
  bool m_isTalkable{};

  ActionCallback m_useCallback;
  ActionCallback m_takeCallback;

};


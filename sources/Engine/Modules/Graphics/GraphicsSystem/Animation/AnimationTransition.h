#pragma once

enum class AnimationStatesTransitionType {
  Straight, SmoothLinear
};

class AnimationTransition {
 public:
  AnimationTransition(AnimationStatesTransitionType type, float duration = 0.0f, bool isActive = true);

  void activate();
  void deactivate();
  [[nodiscard]] bool isActive() const;

  void setType(AnimationStatesTransitionType type);
  [[nodiscard]] AnimationStatesTransitionType getType() const;

  void setDuration(float duration);
  [[nodiscard]] float getDuration() const;

 private:
  bool m_isActive = false;

  AnimationStatesTransitionType m_type = AnimationStatesTransitionType::Straight;
  float m_duration = 0.0f;

};
#pragma once

#include "ConditionalNodes/ConditionalNode.h"
#include "ConditionalNodes/ComparisonNode.h"
#include "ConditionalNodes/ConstantNode.h"
#include "ConditionalNodes/VariableReferenceNode.h"

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

  void setCondition(std::shared_ptr<ConditionalNode> condition);
  [[nodiscard]] std::shared_ptr<ConditionalNode> getCondition() const;

 private:
  bool m_isActive = false;

  AnimationStatesTransitionType m_type = AnimationStatesTransitionType::Straight;
  float m_duration = 0.0f;

  std::shared_ptr<ConditionalNode> m_condition;
};

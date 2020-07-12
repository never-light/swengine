#pragma once

#include <variant>
#include "Modules/Graphics/GraphicsSystem/Animation/AnimationStatesMachineVariables.h"

using ConditionalExpressionResult = std::variant<bool, float>;

class ConditionalNode {
 public:
  ConditionalNode() = default;
  virtual ~ConditionalNode() = default;

  virtual ConditionalExpressionResult evaluate(const AnimationStatesMachineVariables& variables) = 0;
};

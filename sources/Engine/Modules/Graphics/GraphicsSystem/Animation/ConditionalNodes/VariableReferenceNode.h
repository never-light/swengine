#pragma once

#include "ConditionalNode.h"

class VariableReferenceNode : public ConditionalNode {
 public:
  VariableReferenceNode(SkeletalAnimationVariableId variableId);
  ~VariableReferenceNode() override = default;

  [[nodiscard]] SkeletalAnimationVariableId getVariableId() const;
  void setVariableId(SkeletalAnimationVariableId variableId);

  ConditionalExpressionResult evaluate(const AnimationStatesMachineVariables& variables) override;

 private:
  SkeletalAnimationVariableId m_variableId;
};
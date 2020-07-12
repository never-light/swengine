#pragma once

#include "ConditionalNode.h"

class ConstantNode : public ConditionalNode {
 public:
  ConstantNode(float constant);
  ~ConstantNode() override = default;

  [[nodiscard]] float getConstant() const;
  void setConstant(float constant);

  ConditionalExpressionResult evaluate(const AnimationStatesMachineVariables& variables) override;

 private:
  float m_constant;
};
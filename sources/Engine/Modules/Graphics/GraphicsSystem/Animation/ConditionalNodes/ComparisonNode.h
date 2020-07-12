#pragma once

#include <memory>
#include "ConditionalNode.h"

enum class ComparisonOperation {
  Equal,
  NotEqual,
  Less,
  LessEqual,
  Greater,
  GreaterEqual
};

class ComparisonNode : public ConditionalNode {
 public:
  ComparisonNode(std::unique_ptr<ConditionalNode> firstOperand, std::unique_ptr<ConditionalNode> secondOperand,
    ComparisonOperation operation);
  ~ComparisonNode() override = default;

  [[nodiscard]] ComparisonOperation getOperation() const;
  void setOperation(ComparisonOperation operation);

  ConditionalExpressionResult evaluate(const AnimationStatesMachineVariables& variables) override;

 private:
  std::unique_ptr<ConditionalNode> m_firstOperand;
  std::unique_ptr<ConditionalNode> m_secondOperand;
  ComparisonOperation m_operation;
};
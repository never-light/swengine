#include "precompiled.h"

#pragma hdrstop

#include "ComparisonNode.h"
#include "swdebug.h"

ComparisonNode::ComparisonNode(std::unique_ptr<ConditionalNode> firstOperand,
  std::unique_ptr<ConditionalNode> secondOperand,
  ComparisonOperation operation)
  : m_firstOperand(std::move(firstOperand)),
    m_secondOperand(std::move(secondOperand)),
    m_operation(operation)
{

}

ComparisonOperation ComparisonNode::getOperation() const
{
  return m_operation;
}

void ComparisonNode::setOperation(ComparisonOperation operation)
{
  m_operation = operation;
}

ConditionalExpressionResult ComparisonNode::evaluate(const AnimationStatesMachineVariables& variables)
{
  ConditionalExpressionResult firstValue = m_firstOperand->evaluate(variables);
  ConditionalExpressionResult secondValue = m_secondOperand->evaluate(variables);

  SW_ASSERT(firstValue.index() == secondValue.index());

  switch (m_operation) {
    case ComparisonOperation::Equal:
      // TODO: replace floats comparison with utility function
      return ConditionalExpressionResult{
        std::fabs(std::get<float>(firstValue) - std::get<float>(secondValue)) <= 1e-5};
      break;

    case ComparisonOperation::NotEqual:
      return ConditionalExpressionResult{
        std::fabs(std::get<float>(firstValue) - std::get<float>(secondValue)) > 1e-5};
      break;

    case ComparisonOperation::Less:
      return ConditionalExpressionResult{firstValue < secondValue};
      break;

    case ComparisonOperation::LessEqual:
      return ConditionalExpressionResult{firstValue <= secondValue};
      break;

    case ComparisonOperation::Greater:
      return ConditionalExpressionResult{firstValue > secondValue};
      break;

    case ComparisonOperation::GreaterEqual:
      return ConditionalExpressionResult{firstValue >= secondValue};
      break;

    default:
      SW_ASSERT(false);
  }

  return ConditionalExpressionResult{false};
}

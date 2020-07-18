#include "precompiled.h"

#pragma hdrstop

#include "ConstantNode.h"

ConstantNode::ConstantNode(float constant)
  : m_constant(constant)
{

}

float ConstantNode::getConstant() const
{
  return m_constant;
}

void ConstantNode::setConstant(float constant)
{
  m_constant = constant;
}

ConditionalExpressionResult ConstantNode::evaluate(const AnimationStatesMachineVariables& variables)
{
  ARG_UNUSED(variables);

  return ConditionalExpressionResult{m_constant};
}

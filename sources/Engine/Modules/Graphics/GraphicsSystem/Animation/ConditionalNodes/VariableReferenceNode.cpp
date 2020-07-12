#include "precompiled.h"

#pragma hdrstop

#include "VariableReferenceNode.h"

VariableReferenceNode::VariableReferenceNode(SkeletalAnimationVariableId variableId)
  : m_variableId(variableId)
{

}

SkeletalAnimationVariableId VariableReferenceNode::getVariableId() const
{
  return m_variableId;
}

void VariableReferenceNode::setVariableId(SkeletalAnimationVariableId variableId)
{
  m_variableId = variableId;
}

ConditionalExpressionResult VariableReferenceNode::evaluate(const AnimationStatesMachineVariables& variables)
{
  return ConditionalExpressionResult{variables.getVariableValue(m_variableId)};
}

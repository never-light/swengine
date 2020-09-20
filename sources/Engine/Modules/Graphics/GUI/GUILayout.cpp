#include "precompiled.h"

#pragma hdrstop

#include "GUILayout.h"

GUILayout::GUILayout()
  : GUIWidgetRect("layout")
{

}

GUILayout::GUILayout(const std::string& className)
  : GUIWidgetRect(className)
{

}

void GUILayout::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule, size_t selectorPartIndex)
{
  GUIWidgetRect::applyStylesheetRule(stylesheetRule, selectorPartIndex);
}

void GUILayout::applyStylesheetRuleToChildren(const GUIWidgetStylesheetRule& stylesheetRule, size_t selectorPartIndex)
{
  GUIWidgetRect::applyStylesheetRuleToChildren(stylesheetRule, selectorPartIndex);

  for (const auto& childWidget : getChildrenWidgets()) {
    childWidget->applyStylesheetRuleWithSelector(stylesheetRule, selectorPartIndex);
  }
}

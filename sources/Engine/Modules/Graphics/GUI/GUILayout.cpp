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

void GUILayout::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule)
{
  GUIWidgetRect::applyStylesheetRule(stylesheetRule);
}

void GUILayout::applyStylesheetRuleToChildren(
  const GUIWidgetStylesheetRule& stylesheetRule,
  const std::vector<GUIWidgetStylesheetSelectorPart>& currentPath)
{
  GUIWidgetRect::applyStylesheetRuleToChildren(stylesheetRule, currentPath);

  for (const auto& childWidget : getChildrenWidgets()) {
    childWidget->applyStylesheetRuleWithSelector(stylesheetRule, currentPath);
  }
}

#pragma once

#include <utility>

#include "GUIWidget.h"

class GUILayout : public GUIWidgetRect {
 public:
  GUILayout();
  explicit GUILayout(const std::string& className);

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule) override;
  void applyStylesheetRuleToChildren(
    const GUIWidgetStylesheetRule& stylesheetRule,
    const std::vector<GUIWidgetStylesheetSelectorPart>& currentPath) override;
};

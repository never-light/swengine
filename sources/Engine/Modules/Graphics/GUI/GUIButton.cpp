#include "precompiled.h"

#pragma hdrstop

#include "GUIButton.h"

GUIButton::GUIButton()
  : GUIWidgetRect("button")
{

}

void GUIButton::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule, size_t selectorPartIndex)
{
  GUIWidgetRect::applyStylesheetRule(stylesheetRule, selectorPartIndex);
}

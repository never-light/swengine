#include "precompiled.h"

#pragma hdrstop

#include "GUIButton.h"

GUIButton::GUIButton()
  : GUIWidgetRect("button")
{

}

void GUIButton::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule)
{
  GUIWidgetRect::applyStylesheetRule(stylesheetRule);
}

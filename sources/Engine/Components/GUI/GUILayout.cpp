#include "GUILayout.h"

#include <algorithm>
#include <Engine/Components/Graphics/OpenGL4/GL.h>
#include <iostream>

#include <algorithm>
#include <Engine/assertions.h>

GUILayout::GUILayout()
	: m_lastMouseEnteringWidget(nullptr), 
	m_backgroundColor(vector4(0.0f, 0.0f, 0.0f, 1.0f)), 
	m_backgroundImage(nullptr), 
	m_renderBackground(false)
{
}

GUILayout::~GUILayout()
{
	
}

void GUILayout::render(GeometryInstance* quad, GpuProgram * program)
{
	renderBackground(quad, program);
	renderWidgets(quad, program);
}

void GUILayout::update(const MousePosition& mousePosition)
{
	for (GUIWidget* widget : m_widgets)
		if (widget->isVisible())
			widget->update(mousePosition);
}

void GUILayout::onClick(const MousePosition & mousePosition, MouseButton button)
{
	for (GUIWidget* widget : m_widgets) {
		if (isMouseInWidgetArea(mousePosition, widget->getPosition(), widget->getSize())) {
			if (m_onFocusCallback != nullptr)
				m_onFocusCallback(widget);

			widget->onClick(mousePosition, button);
		}
	}
}

void GUILayout::onHover(const MousePosition & mousePosition)
{
	if (m_lastMouseEnteringWidget != nullptr && !isMouseInWidgetArea(mousePosition,
		m_lastMouseEnteringWidget->getPosition(), m_lastMouseEnteringWidget->getSize())) 
	{
		m_lastMouseEnteringWidget->onMouseLeave(mousePosition);
		m_lastMouseEnteringWidget = nullptr;
	}

	for (GUIWidget* widget : m_widgets) {
		vector2 widgetPosition = widget->getPosition();
		vector2 widgetSize = widget->getSize();

		if (isMouseInWidgetArea(mousePosition, widgetPosition, widgetSize)) {
			widget->onHover(mousePosition);

			if (m_lastMouseEnteringWidget != widget) {
				widget->onMouseEnter(mousePosition);
				m_lastMouseEnteringWidget = widget;
			}
		}
	}
}

void GUILayout::onMouseEnter(const MousePosition & mousePosition)
{
}

void GUILayout::onMouseLeave(const MousePosition & mousePosition)
{
}

bool GUILayout::isMouseInWidgetArea(const MousePosition & mousePosition, const uivector2 & widgetPosition, const uivector2 & widgetSize) const
{
	return widgetPosition.x <= mousePosition.x && mousePosition.x <= (widgetPosition.x + widgetSize.x) &&
		widgetPosition.y <= mousePosition.y && mousePosition.y <= (widgetPosition.y + widgetSize.y);
}

void GUILayout::renderBackground(GeometryInstance * quad, GpuProgram * program)
{
	if (m_renderBackground) {
		program->setParameter("transform.localToWorld", getTransformationMatrix());
		program->setParameter("quad.color", m_backgroundColor);

		if (m_backgroundImage) {
			m_backgroundImage->bind(0);

			program->setParameter("quad.texture", 0);
			program->setParameter("quad.useTexture", true);
		}
		else {
			program->setParameter("quad.useTexture", false);
		}

		program->setParameter("quad.useFirstChannel", false);

		quad->draw(GeometryInstance::DrawMode::Triangles, 0, 6);
	}
}

void GUILayout::renderWidgets(GeometryInstance * quad, GpuProgram * program)
{
	for (GUIWidget* widget : m_widgets)
		if (widget->isVisible())
			widget->render(quad, program);
}


void GUILayout::addWidget(GUIWidget * widget)
{
	_assert(widget != nullptr);

	m_widgets.push_back(widget);

	if (GUILayout* layout = dynamic_cast<GUILayout*>(widget))
		layout->setOnFocusCallback(m_onFocusCallback);

	std::sort(m_widgets.begin(), m_widgets.end(), [](const GUIWidget* w1, const GUIWidget* w2) {
		return w2->getZIndex() > w1->getZIndex();
	});
}

void GUILayout::removeWidget(GUIWidget* widget)
{
	auto widgetIt = std::find(m_widgets.begin(), m_widgets.end(), widget);

	if (widgetIt != m_widgets.end())
		m_widgets.erase(widgetIt);
}

void GUILayout::setBackgroundImage(Texture* image)
{
	m_backgroundImage = image;
}

Texture* GUILayout::getBackgroundImage() const
{
	return m_backgroundImage;
}

void GUILayout::setBackgroundColor(const vector4 & color)
{
	m_backgroundColor = color;
}

vector4 GUILayout::getBackgroundColor() const
{
	return m_backgroundColor;
}

void GUILayout::enableBackgroundRendering()
{
	m_renderBackground = true;
}

void GUILayout::disableBackgroundRendering()
{
	m_renderBackground = false;
}

bool GUILayout::isBackgroundRenderingEnabled() const
{
	return m_renderBackground;
}

void GUILayout::setOnFocusCallback(const std::function<void(GUIWidget*)>& callback)
{
	m_onFocusCallback = callback;
}
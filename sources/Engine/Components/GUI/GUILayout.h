#pragma once

#include "GUIWidget.h"

#include <vector>
#include <functional>

class GUILayout : public GUIWidget {
public:
	GUILayout();
	virtual ~GUILayout();

	virtual void render(GeometryInstance* quad, GpuProgram* program) override;
	virtual void update(const CursorPosition& mousePosition) override;

	virtual void onClick(const CursorPosition& mousePosition, MouseButton button) override;
	virtual void onHover(const CursorPosition& mousePosition) override;
	virtual void onMouseEnter(const CursorPosition& mousePosition) override;
	virtual void onMouseLeave(const CursorPosition& mousePosition) override;

	virtual void addWidget(GUIWidget* widget);
	virtual void removeWidget(GUIWidget* widget);

	void setBackgroundImage(Texture* image);
	Texture* getBackgroundImage() const;

	void setBackgroundColor(const vector4& color);
	vector4 getBackgroundColor() const;

	void enableBackgroundRendering();
	void disableBackgroundRendering();

	bool isBackgroundRenderingEnabled() const;

	void setOnFocusCallback(const std::function<void(GUIWidget*)>& callback);
protected:
	bool isMouseInWidgetArea(const CursorPosition& mousePosition, const uivector2& widgetPosition, const uivector2& widgetSize) const;

	void renderBackground(GeometryInstance* quad, GpuProgram* program);
	void renderWidgets(GeometryInstance* quad, GpuProgram* program);

protected:
	std::function<void(GUIWidget*)> m_onFocusCallback;

	GUIWidget* m_lastMouseEnteringWidget;
	std::vector<GUIWidget*> m_widgets;

	Texture* m_backgroundImage;
	vector4 m_backgroundColor;

	bool m_renderBackground;
};
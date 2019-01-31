#pragma once

#include <Engine/Components/GUI/GUIWidget.h>

class GUIBlock : public GUIWidget {
public:
	GUIBlock();
	~GUIBlock();

	const vector4& getColor() const;
	void setColor(const vector4& color);

	virtual void render(GeometryInstance* quad, GpuProgram* program) override;
	virtual void update(const MousePosition& mousePosition) override;

protected:
	vector4 m_color;
};
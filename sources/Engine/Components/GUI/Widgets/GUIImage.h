#pragma once

#include <Engine/Components/GUI/GUIWidget.h>

class GUIImage : public GUIWidget {
public:
	GUIImage(Texture* image);
	~GUIImage();

	Texture* getImage() const;
	void setImage(Texture* image);

	virtual void render(GeometryInstance* quad, GpuProgram* program) override;
	virtual void update(const MousePosition& mousePosition) override;

protected:
	Texture* m_image;
};
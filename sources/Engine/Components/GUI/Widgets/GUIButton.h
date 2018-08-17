#pragma once

#include <Engine\Components\GUI\GUIWidget.h>
#include <functional>

class GUIButton : public GUIWidget {
public:
	using ClickCallback = std::function<void(const MousePosition& mousePosition)>;

public:
	GUIButton();
	~GUIButton();

	Texture* getImage() const;
	void setImage(Texture* image);

	Texture* getHoverImage() const;
	void setHoverImage(Texture* image);

	virtual void render(GeometryStore* quad, GpuProgram* program) override;
	virtual void update(const MousePosition& mousePosition) override;

	virtual void onMouseEnter(const MousePosition& mousePosition) override;
	virtual void onMouseLeave(const MousePosition& mousePosition) override;

	virtual void onClick(const MousePosition& mousePosition, MouseButton button) override;
	virtual void onClick(const ClickCallback& callback);
protected:
	Texture* m_image;
	Texture* m_hoverImage;

	ClickCallback m_clickCallback;

	bool m_hover;
};
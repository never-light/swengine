#pragma once

#include <Engine\Components\GUI\GUIWidget.h>
#include <Engine\Components\GUI\Widgets\GUIText.h>
#include <functional>

class GUIButton : public GUIWidget {
public:
	using ClickCallback = std::function<void(const MousePosition& mousePosition)>;

public:
	GUIButton(GraphicsResourceFactory* grahicsResourceFactory, Font* font);
	virtual ~GUIButton();

	Texture* getImage() const;
	void setImage(Texture* image);

	Texture* getHoverImage() const;
	void setHoverImage(Texture* image);

	void setBackgroundColor(const vector4& color);
	const vector4& getBackgroundColor() const;

	void setHoverBackgroundColor(const vector4& color);
	const vector4& getHoverBackgroundColor() const;


	std::string getText() const;
	void setText(const std::string& text);
	void setTextColor(const vector3& color);
	void setTextFontSize(unsigned int size);

	void setPadding(const uivector2& padding);
	uivector2 getPadding() const;

	virtual void render(GeometryStore* quad, GpuProgram* program) override;
	virtual void update(const MousePosition& mousePosition) override;

	virtual void onMouseEnter(const MousePosition& mousePosition) override;
	virtual void onMouseLeave(const MousePosition& mousePosition) override;

	virtual void onClick(const MousePosition& mousePosition, MouseButton button) override;
	virtual void onClick(const ClickCallback& callback);

	virtual void setPosition(const uivector2& position) override;
	virtual void setPosition(uint32 x, uint32 y) override;

protected:
	Texture* m_image;
	Texture* m_hoverImage;

	vector4 m_backgroundColor;
	vector4 m_hoverBackgroundColor;

	uivector2 m_padding;

	GUIText* m_text;

	ClickCallback m_clickCallback; 

	bool m_hover;
protected:
	GraphicsResourceFactory* m_graphicsResourceFactory;
	Font* m_font;
};
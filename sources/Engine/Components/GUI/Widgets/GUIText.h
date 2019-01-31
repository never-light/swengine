#pragma once

#include <Engine/Components/GUI/GUIWidget.h>
#include <Engine/Components/GUI/Font.h>
#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>

class GUIText : public GUIWidget {
public:
	GUIText(GraphicsContext* graphicsContext);
	~GUIText();

	void setText(const std::string& text);
	std::string getText() const;

	void appendCharacter(unsigned char character);
	void removeLastCharacter();

	void setColor(const vector3& color);
	void setColor(float r, float g, float b);

	void setColor(const vector4& color);
	void setColor(float r, float g, float b, float a);

	vector4 getColor() const;

	void setFont(Font* font);
	Font* getFont() const;

	void setFontSize(unsigned int size);
	unsigned int getFontSize() const;

	virtual void render(GeometryInstance* quad, GpuProgram* program) override;
	virtual void update(const MousePosition& mousePosition) override;

protected:
	void updateTextGeometry();

protected:
	Font* m_font;
	vector4 m_color;
	unsigned int m_fontSize;

	std::string m_text;

	GeometryInstance* m_textGeometry;
	size_t m_textGeometryVerticesCount;

protected:
	GraphicsContext* m_graphicsContext;
};
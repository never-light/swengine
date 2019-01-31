#include "GUIButton.h"

#include <Engine/assertions.h>

GUIButton::GUIButton(GraphicsContext* graphicsContext, Font * font)
	: m_graphicsContext(graphicsContext),
	m_font(font),
	m_image(nullptr), 
	m_hoverImage(nullptr),
	m_hover(false),
	m_clickCallback(nullptr),
	m_text(nullptr),
	m_padding()
{

}

GUIButton::~GUIButton()
{
	if (m_text != nullptr)
		delete m_text;
}

Texture * GUIButton::getImage() const
{
	return m_image;
}

void GUIButton::setImage(Texture * image)
{
	_assert(image != nullptr);

	m_image = image;
}

Texture * GUIButton::getHoverImage() const
{
	return m_hoverImage;
}

void GUIButton::setHoverImage(Texture * image)
{
	_assert(image != nullptr);

	m_hoverImage = image;
}

void GUIButton::setBackgroundColor(const vector4 & color)
{
	m_backgroundColor = color;
}

const vector4 & GUIButton::getBackgroundColor() const
{
	return m_backgroundColor;
}

void GUIButton::setHoverBackgroundColor(const vector4 & color)
{
	m_hoverBackgroundColor = color;
}

const vector4 & GUIButton::getHoverBackgroundColor() const
{
	return m_hoverBackgroundColor;
}

std::string GUIButton::getText() const
{
	return m_text->getText();
}

void GUIButton::setText(const std::string & text)
{
	if (m_text == nullptr) {
		m_text = new GUIText(m_graphicsContext);

		m_text->setPosition(m_position + m_padding);
		m_text->setFont(m_font);
		m_text->setFontSize(10);
		m_text->setColor(1.0, 1.0, 1.0);
	}

	m_text->setText(text);
}

void GUIButton::setTextColor(const vector3 & color)
{
	m_text->setColor(color);
}

void GUIButton::setTextFontSize(unsigned int size)
{
	m_text->setFontSize(size);
}

void GUIButton::setPadding(const uivector2 & padding)
{
	m_padding = padding;
	m_text->setPosition(m_position + m_padding);
}

uivector2 GUIButton::getPadding() const
{
	return m_padding;
}

void GUIButton::render(GeometryInstance * quad, GpuProgram * program)
{
	if (m_image != nullptr) {

		if (m_hover && m_hoverImage != nullptr)
			m_hoverImage->bind(0);
		else
			m_image->bind(0);

		program->setParameter("quad.useTexture", true);
		program->setParameter("quad.texture", 0);
	}
	else {
		program->setParameter("quad.useTexture", false);

		if (m_hover)
			program->setParameter("quad.color", m_hoverBackgroundColor);
		else
			program->setParameter("quad.color", m_backgroundColor);
	}

	program->setParameter("transform.localToWorld", getTransformationMatrix());
	program->setParameter("quad.useFirstChannel", false);

	quad->draw(GeometryInstance::DrawMode::Triangles, 0, 6);

	if (m_text != nullptr) {
		m_text->render(quad, program);
	}
}

void GUIButton::update(const MousePosition & mousePosition)
{
}

void GUIButton::onMouseEnter(const MousePosition & mousePosition)
{
	m_hover = true;
}

void GUIButton::onMouseLeave(const MousePosition & mousePosition)
{
	m_hover = false;
}

void GUIButton::onClick(const MousePosition & mousePosition, MouseButton button)
{
	if (m_clickCallback != nullptr) {
		m_clickCallback(mousePosition);
	}
}

void GUIButton::onClick(const ClickCallback & callback)
{
	m_clickCallback = callback;
}

void GUIButton::setPosition(const uivector2 & position) {
	GUIWidget::setPosition(position);

	if (m_text != nullptr)
		m_text->setPosition(position + m_padding);
}

void GUIButton::setPosition(uint32 x, uint32 y) {
	GUIWidget::setPosition(x, y);

	if (m_text != nullptr)
		m_text->setPosition(x + m_padding.x, y + m_padding.y);
}

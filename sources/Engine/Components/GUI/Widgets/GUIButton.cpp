#include "GUIButton.h"

#include <Engine\assertions.h>

GUIButton::GUIButton()
	: m_image(nullptr), m_hoverImage(nullptr), m_hover(false), m_clickCallback(nullptr)
{
}

GUIButton::~GUIButton()
{
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

void GUIButton::render(GeometryStore * quad, GpuProgram * program)
{
	if (m_hover && m_hoverImage != nullptr)
		m_hoverImage->bind(0);
	else
		m_image->bind(0);

	program->setParameter("transform.localToWorld", getTransformationMatrix());

	program->setParameter("quad.texture", 0);
	program->setParameter("quad.useTexture", true);
	program->setParameter("quad.useFirstChannel", false);

	quad->drawArrays(GeometryStore::DrawType::Triangles, 0, 6);

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

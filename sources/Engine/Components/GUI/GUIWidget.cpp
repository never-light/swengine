#include "GUIWidget.h"

GUIWidget::GUIWidget()
	: m_zIndex(0)
{
}

GUIWidget::~GUIWidget()
{
}

uivector2 GUIWidget::getPosition() const
{
	return m_position;
}

void GUIWidget::setPosition(const uivector2 & position)
{
	m_position = position;
}

void GUIWidget::setPosition(uint32 x, uint32 y)
{
	m_position.x = x;
	m_position.y = y;
}

uivector2 GUIWidget::getSize() const
{
	return m_size;
}

void GUIWidget::setSize(const uivector2 & size)
{
	m_size = size;
}

void GUIWidget::setSize(uint32 x, uint32 y)
{
	m_size.x = x;
	m_size.y = y;
}

void GUIWidget::setWidth(uint32 width)
{
	m_size.x = width;
}

uint32 GUIWidget::getWidth() const
{
	return m_size.x;
}

void GUIWidget::setHeight(uint32 height)
{
	m_size.y = height;
}

uint32 GUIWidget::getHeight() const
{
	return m_size.y;
}

bool GUIWidget::isVisible() const
{
	return m_visible;
}

void GUIWidget::show()
{
	m_visible = true;
}

void GUIWidget::hide()
{
	m_visible = false;
}

matrix4 GUIWidget::getTransformationMatrix()
{
	matrix4 transformation;

	transformation = glm::translate(transformation, vector3(m_position, 0.0f));
	transformation = glm::scale(transformation, vector3(m_size, 1.0f));

	return transformation;
}

void GUIWidget::onClick(const MousePosition& mousePosition)
{
}

void GUIWidget::onHover(const MousePosition& mousePosition)
{
}

void GUIWidget::onMouseEnter(const MousePosition& mousePosition)
{
}

void GUIWidget::onMouseLeave(const MousePosition& mousePosition)
{
}

void GUIWidget::onKeyPress(Key key, KeyEvent::Modifier mod)
{
}

void GUIWidget::onKeyRepeat(Key key, KeyEvent::Modifier mod)
{
}

void GUIWidget::onCharacterEntered(unsigned char character)
{
}


void GUIWidget::setZIndex(unsigned int index)
{
	m_zIndex = index;
}

unsigned int GUIWidget::getZIndex() const
{
	return m_zIndex;
}

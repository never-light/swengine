#include "Viewport.h"

Viewport::Viewport(int width, int height)
	: m_width(width), m_height(height)
{

}

Viewport::~Viewport() {

}

int Viewport::getWidth() const {
	return m_width;
}

int Viewport::getHeight() const {
	return m_height;
}

void Viewport::setWidth(int width)  {
	m_width = width;
}

void Viewport::setHeight(int height) {
	m_height = height;
}
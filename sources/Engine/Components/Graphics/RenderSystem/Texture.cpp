#include "Texture.h"

Texture::Texture()
	: m_isAnisotropicFilteringEnabled(false), m_anisotropicFilteringQuality(0.0f)
{
}

Texture::~Texture()
{
}

void Texture::setTarget(Target target)
{
	m_target = target;
}

Texture::Target Texture::getTarget() const
{
	return m_target;
}

void Texture::setInternalFormat(InternalFormat format)
{
	m_internalFormat = format;
}

Texture::InternalFormat Texture::getInternalFormat() const
{
	return m_internalFormat;
}

void Texture::setSize(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
}

void Texture::setWidth(unsigned int width)
{
	m_width = width;
}

unsigned int Texture::getWidth() const
{
	return m_width;
}

void Texture::setHeight(unsigned int height)
{
	m_height = height;
}

unsigned int Texture::getHeight() const
{
	return m_height;
}

void Texture::setMinificationFilter(Filter filter)
{
	m_minificationFilter = filter;
}

Texture::Filter Texture::getMinificationFilter() const
{
	return m_minificationFilter;
}

void Texture::setMagnificationFilter(Filter filter)
{
	m_magnificationFilter = filter;
}

Texture::Filter Texture::getMagnificationFilter() const
{
	return m_magnificationFilter;
}

void Texture::setWrapMode(WrapMode mode)
{
	m_wrapMode = mode;
}

Texture::WrapMode Texture::getWrapMode() const
{
	return m_wrapMode;
}

void Texture::enableAnisotropicFiltering(float quality)
{
	m_isAnisotropicFilteringEnabled = true;
	m_anisotropicFilteringQuality = quality;
}

bool Texture::isAnisotropicFilteringEnabled() const
{
	return m_isAnisotropicFilteringEnabled;
}

float Texture::getAnisotropicFilteringQuality(float quality)
{
	return m_anisotropicFilteringQuality;
}

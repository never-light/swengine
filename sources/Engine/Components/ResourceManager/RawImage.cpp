#include "RawImage.h"

#include <cstring>

RawImage::RawImage()
	: m_width(0), m_height(0), m_data(nullptr)
{
}

RawImage::RawImage(Format format, uint32 width, uint32 height, const std::byte * data)
	: m_format(format), m_width(width), m_height(height), m_data(nullptr)
{
	size_t channelsCount = 0;
	size_t channelSize = 1;

	switch (format) {
	case Format::R8:
		channelsCount = 1;
		break;

	case Format::RG8:
		channelsCount = 2;
		break;

	case Format::RGB8:
		channelsCount = 3;
		break;

	case Format::RGBA8:
		channelsCount = 4;
		break;
	}

	size_t imageSize = width * height * channelsCount * channelSize;

	m_data = new std::byte[imageSize];
	memcpy(m_data, data, imageSize);
}

RawImage::~RawImage()
{
	if (m_data != nullptr) {
		delete m_data;
		m_data = nullptr;
	}
}

void RawImage::setWidth(uint32 width)
{
	m_width = width;
}

uint32 RawImage::getWidth() const
{
	return m_width;
}

void RawImage::setHeight(uint32 height)
{
	m_height = height;
}

uint32 RawImage::getHeight() const
{
	return m_height;
}

std::byte * RawImage::getRawData() const
{
	return m_data;
}

void RawImage::setFormat(Format format)
{
	m_format = format;
}

RawImage::Format RawImage::getFormat() const
{
	return m_format;
}

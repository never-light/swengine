#pragma once

#include <Engine\types.h>

class RawImage {
public:
	enum class Format {
		Invalid,
		R8,
		RG8,
		RGB8,
		RGBA8,
	};

public:
	RawImage();
	RawImage(Format format, uint32 width, uint32 height, const std::byte* data);
	~RawImage();

	void setWidth(uint32 width);
	uint32 getWidth() const;

	void setHeight(uint32 height);
	uint32 getHeight() const;

	std::byte* getRawData() const;

	void setFormat(Format format);
	Format getFormat() const;
protected:
	Format m_format;

	uint32 m_width;
	uint32 m_height;

	std::byte* m_data;
};
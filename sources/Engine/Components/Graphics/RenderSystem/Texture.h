#pragma once

#include <string>

enum class TextureInternalFormat {
	RGB16F, RGBA
};

enum class TextureFormat {
	RGB, RGBA
};

enum class TextureDataType {
	Float, UnsignedByte
};

class Texture {
public:
	Texture(int width, int height, TextureInternalFormat internalFormat, TextureFormat format, TextureDataType type) {}
	Texture(int width, int height, const unsigned char* data) {}
	virtual ~Texture() {}

	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
};
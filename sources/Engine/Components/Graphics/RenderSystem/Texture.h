#pragma once

#include <string>

class Texture {
public:
	enum class Type {
		_2D, CubeMap
	};

	enum class DataTarget {
		_2D, 
		CubeMapPositiveX, CubeMapNegativeX, 
		CubeMapPositiveY, CubeMapNegativeY,
		CubeMapPositiveZ, CubeMapNegativeZ,
	};

	enum class InternalFormat {
		RGB16F, RGBA16F
	};

	enum class PixelFormat {
		RGB, RGBA
	};

	enum class DataType {
		Float, UnsignedByte
	};

	enum class Parameter {
		MinFilter, MagFilter, WrapS, WrapT, WrapR
	};

	enum class ParameterValue {
		Repeat, ClampToEdge,
		Nearest, Linear, LinearMipmapLinear
	};

	Texture(Type type) { };
	virtual ~Texture() = default;

	virtual void lock(bool replace = false) = 0;
	virtual void setPlainData(DataTarget target, int width, int height,
		InternalFormat internalFormat,
		PixelFormat format, 
		DataType type,
		const unsigned char* data = nullptr
	) = 0;
	virtual void setParameter(Parameter parameter, ParameterValue value) = 0;
	virtual void generateMipmaps() = 0;
	virtual void unlock() = 0;

	virtual Type getType() const = 0;
};

Texture::DataTarget operator+(Texture::DataTarget target, int offset);
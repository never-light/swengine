#pragma once

#include <cstddef>

class Texture {
public:
	enum class Target {
		_2D, CubeMap, _2DMultisample
	};

	enum class InternalFormat {
		R8, RG8, RGB8, RGBA8, SRGB8, SRGBA8,
		R16, RG16, RGB16, RGBA16,
		R16F, RG16F, RGB16F, RGBA16F,
		R32F, RG32F, RGB32F, RGBA32F,
		R11FG11FB10F,
		Depth24Stencil8, Depth24, Depth32
	};

	enum class PixelFormat {
		R, RG, RGB, RGBA, DepthStencil, Depth
	};

	enum class PixelDataType {
		UnsignedByte, Float, UnsignedInt24_8
	};

	enum class CubeMapFace {
		PositiveX, NegativeX,
		PositiveY, NegativeY,
		PositiveZ, NegativeZ
	};

	enum class Filter {
		Nearest, Linear, LinearMipmapLinear
	};

	enum class WrapMode {
		Repeat, ClampToEdge
	};
public:
	Texture();
	virtual ~Texture();

	virtual void create() = 0;
	virtual void destroy() = 0;

	virtual void bind() = 0;
	virtual void bind(unsigned int unit) = 0;
	virtual void unbind() = 0;

	virtual void setTarget(Target target);
	virtual Target getTarget() const;

	virtual void setInternalFormat(InternalFormat format);
	virtual InternalFormat getInternalFormat() const;

	void setSize(unsigned int width, unsigned int height);
	
	void setWidth(unsigned int width);
	unsigned int getWidth() const;

	void setHeight(unsigned int height);
	unsigned int getHeight() const;

	virtual void fillMultisampleData(int samplesCount) = 0;
	virtual void setData(PixelFormat pixelFormat, PixelDataType pixelDataType, const std::byte* data) = 0;
	virtual void setData(CubeMapFace cubeMapFace, PixelFormat pixelFormat, PixelDataType pixelDataType, const std::byte* data) = 0;

	virtual void generateMipMaps() = 0;

	virtual void setMinificationFilter(Filter filter);
	virtual Filter getMinificationFilter() const;

	virtual void setMagnificationFilter(Filter filter);
	virtual Filter getMagnificationFilter() const;

	virtual void setWrapMode(WrapMode mode);
	virtual WrapMode getWrapMode() const;

	virtual void enableAnisotropicFiltering(float quality);
	virtual bool isAnisotropicFilteringEnabled() const;
	virtual float getAnisotropicFilteringQuality(float quality);
protected:
	Target m_target;
	InternalFormat m_internalFormat;

	unsigned int m_width;
	unsigned int m_height;

	Filter m_minificationFilter;
	Filter m_magnificationFilter;

	WrapMode m_wrapMode;

	bool m_isAnisotropicFilteringEnabled;
	float m_anisotropicFilteringQuality;
};
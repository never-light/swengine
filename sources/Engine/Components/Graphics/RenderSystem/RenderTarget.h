#pragma once

#include <Engine\Components\Math\types.h>
#include <Engine\Components\Math\Geometry\Rect.h>

#include "Texture.h"

class RenderTarget {
public:
	enum class Component {
		Color, Depth, DepthStencil
	};

	static const unsigned int CLEAR_DEPTH = 1;
	static const unsigned int CLEAR_COLOR = 2;
	static const unsigned int CLEAR_STENCIL = 4;

	enum class CopyFilter {
		Nearest, Linear
	};

	static const unsigned int COPY_DEPTH = 1;
	static const unsigned int COPY_COLOR = 2;
	static const unsigned int COPY_STENCIL = 4;

public:
	RenderTarget();
	virtual ~RenderTarget();

	virtual void create() = 0;
	virtual void destroy() = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void setClearColor(float r, float g, float b, float a = 1.0f) = 0;
	virtual void setClearColor(const vector3& color) = 0;
	virtual void setClearColor(const vector4& color) = 0;
	
	virtual void setDepthClearValue(float depthValue) = 0;
	virtual void setStencilClearValue(int stencilValue) = 0;

	virtual void clear(unsigned int mode) = 0 ;

	virtual void attachComponent(Component type, Texture* texture) = 0;

	virtual void copyComponentRawData(RenderTarget* destination, 
		const Rect& sourceArea, const Rect& destinationArea,
		unsigned int copyMode,
		CopyFilter copyFilter = CopyFilter::Linear) = 0;
};
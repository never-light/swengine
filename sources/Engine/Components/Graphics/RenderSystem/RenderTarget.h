#pragma once

#include <Engine\Components\Math\types.h>

class RenderTarget {
public:
	RenderTarget();
	virtual ~RenderTarget();

	virtual void create() = 0;
	virtual void destroy() = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void clear(const vector3& color) = 0;
};
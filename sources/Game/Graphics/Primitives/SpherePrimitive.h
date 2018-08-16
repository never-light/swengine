#pragma once

#include <vector>
#include <Engine\Components\Math\types.h>
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>

class SpherePrimitive {
public:
	SpherePrimitive(GraphicsResourceFactory* graphicsResourceFactory);
	~SpherePrimitive();

	void render();
private:
	GeometryStore * m_geometry;
};
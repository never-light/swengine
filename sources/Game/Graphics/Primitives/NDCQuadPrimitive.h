#pragma once

#include <vector>
#include <Engine\Components\Math\types.h>
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>

class NDCQuadPrimitive {
public:
	NDCQuadPrimitive(GraphicsResourceFactory* graphicsResourceFactory);
	~NDCQuadPrimitive();

	void render();
private:
	GeometryStore * m_geometry;
};
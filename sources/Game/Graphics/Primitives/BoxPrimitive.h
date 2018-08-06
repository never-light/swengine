#pragma once

#include <vector>
#include <Engine\Components\Math\types.h>
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>

class BoxPrimitive {
public:
	BoxPrimitive(GraphicsResourceFactory* graphicsResourceFactory);
	~BoxPrimitive();

	void setVertices(const std::vector<vector3>& vertices);

	void render();
private:
	GeometryStore * m_geometry;
	GeometryStore::BufferId m_vertexBufferId;
};
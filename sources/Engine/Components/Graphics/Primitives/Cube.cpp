#include "Cube.h"

Cube::Cube() {
	m_geometryBuffer->lock();
	m_geometryBuffer->setVertexFormat(VertexFormat::P1N1UV);

	std::vector<VertexP1N1UV> vertices = std::vector<VertexP1N1UV>{
		{ vector3(-1.0f,-1.0f,-1.0f), vector3(0.0f, -1.0f, 0.0), vector2(0.0f, 0.0f) },
		{ vector3(1.0f,-1.0f,-1.0f), vector3(0.0f, -1.0f, 0.0), vector2(1.0f, 0.0f) },
			{ vector3(-1.0f,-1.0f, 1.0f), vector3(0.0f, -1.0f, 0.0), vector2(0.0f, 1.0f) },
			{ vector3(1.0f,-1.0f,-1.0f), vector3(0.0f, -1.0f, 0.0), vector2(1.0f, 0.0f) },
			{ vector3(1.0f,-1.0f, 1.0f), vector3(0.0f, -1.0f, 0.0), vector2(1.0f, 1.0f) },
			{ vector3(-1.0f,-1.0f, 1.0f), vector3(0.0f, -1.0f, 0.0), vector2(0.0f, 1.0f) },

		// top
			{ vector3(-1.0f, 1.0f,-1.0f), vector3(0.0f, 1.0f, 0.0), vector2(0.0f, 0.0f) },
			{ vector3(-1.0f, 1.0f, 1.0f), vector3(0.0f, 1.0f, 0.0), vector2(0.0f, 1.0f) },
			{ vector3(1.0f, 1.0f,-1.0f), vector3(0.0f, 1.0f, 0.0), vector2(1.0f, 0.0f) },
			{ vector3(1.0f, 1.0f,-1.0f), vector3(0.0f, 1.0f, 0.0), vector2(1.0f, 0.0f) },
			{ vector3(-1.0f, 1.0f, 1.0f), vector3(0.0f, 1.0f, 0.0), vector2(0.0f, 1.0f) },
			{ vector3(1.0f, 1.0f, 1.0f), vector3(0.0f, 1.0f, 0.0), vector2(1.0f, 1.0f) },

		// front
			{ vector3(-1.0f,-1.0f, 1.0f), vector3(0.0f, 0.0f, 1.0), vector2(1.0f, 0.0f) },
			{ vector3(1.0f,-1.0f, 1.0f), vector3(0.0f, 0.0f, 1.0), vector2(0.0f, 0.0f) },
			{ vector3(-1.0f, 1.0f, 1.0f), vector3(0.0f, 0.0f, 1.0), vector2(1.0f, 1.0f) },
			{ vector3(1.0f,-1.0f, 1.0f), vector3(0.0f, 0.0f, 1.0), vector2(0.0f, 0.0f) },
			{ vector3(1.0f, 1.0f, 1.0f), vector3(0.0f, 0.0f, 1.0), vector2(0.0f, 1.0f) },
			{ vector3(-1.0f, 1.0f, 1.0f), vector3(0.0f, 0.0f, 1.0), vector2(1.0f, 1.0f) },

		// back
			{ vector3(-1.0f,-1.0f,-1.0f), vector3(0.0f, 0.0f, -1.0), vector2(0.0f, 0.0f) },
			{ vector3(-1.0f, 1.0f,-1.0f), vector3(0.0f, 0.0f, -1.0), vector2(0.0f, 1.0f) },
			{ vector3(1.0f,-1.0f,-1.0f), vector3(0.0f, 0.0f, -1.0), vector2(1.0f, 0.0f) },
			{ vector3(1.0f,-1.0f,-1.0f), vector3(0.0f, 0.0f, -1.0), vector2(1.0f, 0.0f) },
			{ vector3(-1.0f, 1.0f,-1.0f), vector3(0.0f, 0.0f, -1.0), vector2(0.0f, 1.0f) },
			{ vector3(1.0f, 1.0f,-1.0f), vector3(0.0f, 0.0f, -1.0), vector2(1.0f, 1.0f) },

		// left
			{ vector3(-1.0f,-1.0f, 1.0f), vector3(-1.0f, 0.0f, 0.0), vector2(0.0f, 1.0f) },
			{ vector3(-1.0f, 1.0f,-1.0f), vector3(-1.0f, 0.0f, 0.0), vector2(1.0f, 0.0f) },
			{ vector3(-1.0f,-1.0f,-1.0f), vector3(-1.0f, 0.0f, 0.0), vector2(0.0f, 0.0f) },
			{ vector3(-1.0f,-1.0f, 1.0f), vector3(-1.0f, 0.0f, 0.0), vector2(0.0f, 1.0f) },
			{ vector3(-1.0f, 1.0f, 1.0f), vector3(-1.0f, 0.0f, 0.0), vector2(1.0f, 1.0f) },
			{ vector3(-1.0f, 1.0f,-1.0f), vector3(-1.0f, 0.0f, 0.0), vector2(1.0f, 0.0f) },

		// right
			{ vector3(1.0f,-1.0f, 1.0f), vector3(1.0f, 0.0f, 0.0), vector2(1.0f, 1.0f) },
			{ vector3(1.0f,-1.0f,-1.0f), vector3(1.0f, 0.0f, 0.0), vector2(1.0f, 0.0f) },
			{ vector3(1.0f, 1.0f,-1.0f), vector3(1.0f, 0.0f, 0.0), vector2(0.0f, 0.0f) },
			{ vector3(1.0f,-1.0f, 1.0f), vector3(1.0f, 0.0f, 0.0), vector2(1.0f, 1.0f) },
			{ vector3(1.0f, 1.0f,-1.0f), vector3(1.0f, 0.0f, 0.0), vector2(0.0f, 0.0f) },
			{ vector3(1.0f, 1.0f, 1.0f), vector3(1.0f, 0.0f, 0.0f), vector2(0.0f, 1.0f) },
	};

	m_geometryBuffer->setVerticesData(vertices.size(), sizeof(VertexP1N1UV), vertices.data());
	m_geometryBuffer->unlock();
}

Cube::~Cube() {
	
}
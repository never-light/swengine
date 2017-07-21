#include "Cube.h"

Cube::Cube() {
	m_vertices = std::vector<Vertex>{
		{ vector3(-1.0f,-1.0f,-1.0f), vector2(0.0f, 0.0f), vector3(0.0f, -1.0f, 0.0) },
		{ vector3(1.0f,-1.0f,-1.0f), vector2(1.0f, 0.0f), vector3(0.0f, -1.0f, 0.0) },
		{ vector3(-1.0f,-1.0f, 1.0f), vector2(0.0f, 1.0f), vector3(0.0f, -1.0f, 0.0) },
		{ vector3(1.0f,-1.0f,-1.0f), vector2(1.0f, 0.0f), vector3(0.0f, -1.0f, 0.0) },
		{ vector3(1.0f,-1.0f, 1.0f), vector2(1.0f, 1.0f), vector3(0.0f, -1.0f, 0.0) },
		{ vector3(-1.0f,-1.0f, 1.0f), vector2(0.0f, 1.0f), vector3(0.0f, -1.0f, 0.0) },

		// top
		{ vector3(-1.0f, 1.0f,-1.0f), vector2(0.0f, 0.0f), vector3(0.0f, 1.0f, 0.0) },
		{ vector3(-1.0f, 1.0f, 1.0f), vector2(0.0f, 1.0f), vector3(0.0f, 1.0f, 0.0) },
		{ vector3(1.0f, 1.0f,-1.0f), vector2(1.0f, 0.0f), vector3(0.0f, 1.0f, 0.0) },
		{ vector3(1.0f, 1.0f,-1.0f), vector2(1.0f, 0.0f), vector3(0.0f, 1.0f, 0.0) },
		{ vector3(-1.0f, 1.0f, 1.0f), vector2(0.0f, 1.0f), vector3(0.0f, 1.0f, 0.0) },
		{ vector3(1.0f, 1.0f, 1.0f), vector2(1.0f, 1.0f), vector3(0.0f, 1.0f, 0.0) },

		// front
		{ vector3(-1.0f,-1.0f, 1.0f), vector2(1.0f, 0.0f), vector3(0.0f, 0.0f, 1.0) },
		{ vector3(1.0f,-1.0f, 1.0f), vector2(0.0f, 0.0f), vector3(0.0f, 0.0f, 1.0) },
		{ vector3(-1.0f, 1.0f, 1.0f), vector2(1.0f, 1.0f), vector3(0.0f, 0.0f, 1.0) },
		{ vector3(1.0f,-1.0f, 1.0f), vector2(0.0f, 0.0f), vector3(0.0f, 0.0f, 1.0) },
		{ vector3(1.0f, 1.0f, 1.0f), vector2(0.0f, 1.0f), vector3(0.0f, 0.0f, 1.0) },
		{ vector3(-1.0f, 1.0f, 1.0f), vector2(1.0f, 1.0f), vector3(0.0f, 0.0f, 1.0) },

		// back
		{ vector3(-1.0f,-1.0f,-1.0f), vector2(0.0f, 0.0f), vector3(0.0f, 0.0f, -1.0) },
		{ vector3(-1.0f, 1.0f,-1.0f), vector2(0.0f, 1.0f), vector3(0.0f, 0.0f, -1.0) },
		{ vector3(1.0f,-1.0f,-1.0f), vector2(1.0f, 0.0f), vector3(0.0f, 0.0f, -1.0) },
		{ vector3(1.0f,-1.0f,-1.0f), vector2(1.0f, 0.0f), vector3(0.0f, 0.0f, -1.0) },
		{ vector3(-1.0f, 1.0f,-1.0f), vector2(0.0f, 1.0f), vector3(0.0f, 0.0f, -1.0) },
		{ vector3(1.0f, 1.0f,-1.0f), vector2(1.0f, 1.0f), vector3(0.0f, 0.0f, -1.0) },

		// left
		{ vector3(-1.0f,-1.0f, 1.0f), vector2(0.0f, 1.0f), vector3(-1.0f, 0.0f, 0.0) },
		{ vector3(-1.0f, 1.0f,-1.0f), vector2(1.0f, 0.0f), vector3(-1.0f, 0.0f, 0.0) },
		{ vector3(-1.0f,-1.0f,-1.0f), vector2(0.0f, 0.0f), vector3(-1.0f, 0.0f, 0.0) },
		{ vector3(-1.0f,-1.0f, 1.0f), vector2(0.0f, 1.0f), vector3(-1.0f, 0.0f, 0.0) },
		{ vector3(-1.0f, 1.0f, 1.0f), vector2(1.0f, 1.0f), vector3(-1.0f, 0.0f, 0.0) },
		{ vector3(-1.0f, 1.0f,-1.0f), vector2(1.0f, 0.0f), vector3(-1.0f, 0.0f, 0.0) },

		// right
		{ vector3(1.0f,-1.0f, 1.0f), vector2(1.0f, 1.0f), vector3(1.0f, 0.0f, 0.0) },
		{ vector3(1.0f,-1.0f,-1.0f), vector2(1.0f, 0.0f), vector3(1.0f, 0.0f, 0.0) },
		{ vector3(1.0f, 1.0f,-1.0f), vector2(0.0f, 0.0f), vector3(1.0f, 0.0f, 0.0) },
		{ vector3(1.0f,-1.0f, 1.0f), vector2(1.0f, 1.0f), vector3(1.0f, 0.0f, 0.0) },
		{ vector3(1.0f, 1.0f,-1.0f), vector2(0.0f, 0.0f), vector3(1.0f, 0.0f, 0.0) },
		{ vector3(1.0f, 1.0f, 1.0f), vector2(0.0f, 1.0f), vector3(1.0f, 0.0f, 0.0f) },
	};

	updateBuffers();
}

Cube::~Cube() {
	
}
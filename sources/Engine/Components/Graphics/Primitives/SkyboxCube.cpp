#include "SkyboxCube.h"

SkyboxCube::SkyboxCube() {
	m_geometryBuffer->lock();
	m_geometryBuffer->setVertexFormat(VertexFormat::P1);

	std::vector<VertexP1> vertices = std::vector<VertexP1>{
		{ vector3(-1.0f,  1.0f, -1.0f) },
		{ vector3(-1.0f, -1.0f, -1.0f) },
		{ vector3(1.0f, -1.0f, -1.0f) },
		{ vector3(1.0f, -1.0f, -1.0f) },
		{ vector3(1.0f,  1.0f, -1.0f) },
		{ vector3(-1.0f,  1.0f, -1.0f) },

		{ vector3(-1.0f, -1.0f,  1.0f) },
		{ vector3(-1.0f, -1.0f, -1.0f) },
		{ vector3(-1.0f,  1.0f, -1.0f) },
		{ vector3(-1.0f,  1.0f, -1.0f) },
		{ vector3(-1.0f,  1.0f,  1.0f) },
		{ vector3(-1.0f, -1.0f,  1.0f) },

		{ vector3(1.0f, -1.0f, -1.0f) },
		{ vector3(1.0f, -1.0f,  1.0f) },
		{ vector3(1.0f,  1.0f,  1.0f) },
		{ vector3(1.0f,  1.0f,  1.0f) },
		{ vector3(1.0f,  1.0f, -1.0f) },
		{ vector3(1.0f, -1.0f, -1.0f) },

		{ vector3(-1.0f, -1.0f,  1.0f) },
		{ vector3(-1.0f,  1.0f,  1.0f) },
		{ vector3(1.0f,  1.0f,  1.0f) },
		{ vector3(1.0f,  1.0f,  1.0f) },
		{ vector3(1.0f, -1.0f,  1.0f) },
		{ vector3(-1.0f, -1.0f,  1.0f) },

		{ vector3(-1.0f,  1.0f, -1.0f) },
		{ vector3(1.0f,  1.0f, -1.0f) },
		{ vector3(1.0f,  1.0f,  1.0f) },
		{ vector3(1.0f,  1.0f,  1.0f) },
		{ vector3(-1.0f,  1.0f,  1.0f) },
		{ vector3(-1.0f,  1.0f, -1.0f) },

		{ vector3(-1.0f, -1.0f, -1.0f) },
		{ vector3(-1.0f, -1.0f,  1.0f) },
		{ vector3(1.0f, -1.0f, -1.0f) },
		{ vector3(1.0f, -1.0f, -1.0f) },
		{ vector3(-1.0f, -1.0f,  1.0f) },
		{ vector3(1.0f, -1.0f,  1.0f) }
	};

	m_geometryBuffer->setVerticesData(vertices.size(), sizeof(VertexP1), vertices.data());
	m_geometryBuffer->unlock();
}
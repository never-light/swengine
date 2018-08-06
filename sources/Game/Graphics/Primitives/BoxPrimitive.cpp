#include "BoxPrimitive.h"

#include <Engine\assertions.h>

BoxPrimitive::BoxPrimitive(GraphicsResourceFactory* graphicsResourceFactory)
	: m_geometry(graphicsResourceFactory->createGeometryStore())
{
	m_vertexBufferId = m_geometry->requireBuffer(GeometryStore::BufferType::Vertex,
		GeometryStore::BufferUsage::DynamicDraw, sizeof(vector3) * 8);

	GeometryStore::BufferId indexBufferId = m_geometry->requireBuffer(GeometryStore::BufferType::Index, 
		GeometryStore::BufferUsage::StaticDraw, sizeof(unsigned int) * 36);

	std::vector<unsigned int> indices{
		0, 2, 1, 0, 3, 2,
		3, 5, 7, 3, 7, 2,
		4, 5, 6, 4, 7, 6,
		0, 4, 1, 1, 4, 6,
		0, 5, 4, 0, 3, 5,
		1, 2, 7, 1, 7, 6
	};

	m_geometry->setBufferData(indexBufferId, 0, sizeof(unsigned int) * 36, (const std::byte*)indices.data());

	// Set vertex layout description
	m_geometry->setVertexLayoutAttribute(0, m_vertexBufferId, 3,
		GeometryStore::VertexLayoutAttributeBaseType::Float, false, 0, 0);

	m_geometry->create();
}

BoxPrimitive::~BoxPrimitive()
{
	delete m_geometry;
}

void BoxPrimitive::setVertices(const std::vector<vector3>& vertices)
{
	_assert(vertices.size() == 8);

	m_geometry->setBufferData(m_vertexBufferId, 0, sizeof(vector3) * 8, (const std::byte*)vertices.data());
}

void BoxPrimitive::render()
{
	m_geometry->bind();
	m_geometry->drawElements(GeometryStore::DrawType::Triangles, 0, 36, GeometryStore::IndicesType::UnsignedInt);
}

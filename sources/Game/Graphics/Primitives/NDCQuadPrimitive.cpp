#include "NDCQuadPrimitive.h"

const float QUAD_VERTICES_RAW_DATA[] = {
	-1.0f,  1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f,  1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f
};

NDCQuadPrimitive::NDCQuadPrimitive(GraphicsResourceFactory* graphicsResourceFactory)
	: m_geometry(graphicsResourceFactory->createGeometryStore())
{
	m_geometry = graphicsResourceFactory->createGeometryStore();

	GeometryStore::BufferId vertexBufferId = m_geometry->requireBuffer(GeometryStore::BufferType::Vertex, 
		GeometryStore::BufferUsage::StaticDraw, sizeof(QUAD_VERTICES_RAW_DATA));

	m_geometry->setBufferData(vertexBufferId, 0, sizeof(QUAD_VERTICES_RAW_DATA), 
		(const std::byte*)QUAD_VERTICES_RAW_DATA);

	// position and texture coordinates attribute
	m_geometry->setVertexLayoutAttribute(0, vertexBufferId, 4,
		GeometryStore::VertexLayoutAttributeBaseType::Float, false, 4 * sizeof(float), 0);

	m_geometry->create();
}

NDCQuadPrimitive::~NDCQuadPrimitive()
{
	delete m_geometry;
}

void NDCQuadPrimitive::render()
{
	m_geometry->bind();
	m_geometry->drawArrays(GeometryStore::DrawType::TrianglesStrip, 0, 4);
}
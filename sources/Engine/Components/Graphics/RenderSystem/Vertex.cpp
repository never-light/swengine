#include "Vertex.h"

VertexP1::VertexP1(const vector3& position)
	: position(position)
{

}

VertexP1UV::VertexP1UV(const vector3& position, const vector2& uv)
	: position(position), uv(uv)
{

}

VertexP1N1UV::VertexP1N1UV(const vector3& position, const vector3& normal, const vector2& uv)
	: position(position), normal(normal), uv(uv)
{

}
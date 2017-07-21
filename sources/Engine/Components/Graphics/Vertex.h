#pragma once

#include <Engine\Components\Math\Math.h>

struct Vertex {
	Vertex(const vector3&, const vector2&, const vector3&);

	vector3 position;
	vector3 normal;
	vector2 textureCoordinates;
};
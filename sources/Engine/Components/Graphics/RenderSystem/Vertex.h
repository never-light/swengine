#pragma once

#include <variant>
#include <vector>
#include <Engine\Components\Math\Math.h>

enum class VertexFormat {
	P1, P1UV, P1N1UV, Manual
};

struct VertexP1 {
	VertexP1(const vector3& position);

	vector3 position;
};

struct VertexP1UV {
	VertexP1UV(const vector3& position, const vector2& uv);

	vector3 position;
	vector2 uv;
};

struct VertexP1N1UV {
	VertexP1N1UV(const vector3& position, const vector3& normal, const vector2& uv);

	vector3 position;
	vector3 normal;
	vector2 uv;
};
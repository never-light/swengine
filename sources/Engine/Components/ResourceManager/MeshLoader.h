#pragma once

#include <string>
#include <Engine\Components\Graphics\RenderSystem\Mesh.h>

class MeshLoader {
private:
	enum class VertexComponent : size_t {
		Position = 0, Normal = 1, TexCoord = 2, Tangent = 3
	};

	struct HeaderData {
		std::uint32_t version;
		std::uint32_t meshesCount;
	};

	struct MeshDescription {
		char name[64];
		char material[64];
		std::uint32_t verticesCount;
		std::uint32_t indicesCount;
		std::uint32_t components;
	};

	struct vec3 {
		float x, y, z;
	};

	struct vec2 {
		float x, y;
	};

public:
	static void load(const std::string& filename, Mesh* mesh);
};
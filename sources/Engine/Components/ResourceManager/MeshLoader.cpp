#include "MeshLoader.h"

#include <Engine\Components\Debugging\Log.h>

#include <vector>
#include <bitset>
#include <fstream>

void MeshLoader::load(const std::string& filename, Mesh* mesh) {
	std::ifstream in(filename, std::ios::binary | std::ios::in);

	if (!in.is_open())
		throw std::exception();

	HeaderData header;
	in.read((char*)&header, sizeof header);

	for (size_t i = 0; i < header.meshesCount; i++) {
		MeshDescription description;
		in.read((char*)&description, sizeof description);

		std::vector<vector3> positions(description.verticesCount);
		in.read((char*)positions.data(), sizeof(vector3) * description.verticesCount);

		std::vector<vector3> normals(description.verticesCount);
		in.read((char*)normals.data(), sizeof(vector3) * description.verticesCount);

		std::vector<vector3> tangents(description.verticesCount);
		in.read((char*)tangents.data(), sizeof(vector3) * description.verticesCount);

		std::vector<vector2> uv(description.verticesCount);
		in.read((char*)uv.data(), sizeof(vector2) * description.verticesCount);

		std::vector<uint32> indices(description.indicesCount);
		in.read((char*)indices.data(), sizeof(uint32) * description.indicesCount);

		mesh->setName(description.name);

		mesh->setPositions(positions);
		mesh->setNormals(normals);
		mesh->setTangents(tangents);
		mesh->setUV(uv);

		mesh->setIndices(indices);
		mesh->create();

		return;
	}
}
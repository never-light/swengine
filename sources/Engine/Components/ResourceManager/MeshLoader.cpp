#include "MeshLoader.h"

#include <Engine\Components\Graphics\OpenGL3\OpenGL3Mesh.h>
#include <fstream>

struct ModelFileHeaderData {
	std::uint32_t version;
	std::uint32_t meshesCount;
};

struct ModelFileMeshData {
	char name[64];
	std::uint32_t verticesCount;
	std::uint32_t indicesCount;
};

struct ModelFileVertexData {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

MeshLoader::MeshLoader() {

}

MeshLoader::~MeshLoader() {

}

std::vector<Mesh*> MeshLoader::load(const std::string& pathToModel) {
	std::vector<Mesh*> meshes;

	std::ifstream in(pathToModel, std::ios::binary | std::ios::in);

	ModelFileHeaderData header;
	in.read((char*)&header, sizeof header);

	for (size_t i = 0; i < header.meshesCount; i++) {
		OpenGL3Mesh* mesh = new OpenGL3Mesh();

		ModelFileMeshData meshData;
		in.read((char*)&meshData, sizeof meshData);

		for (size_t j = 0; j < meshData.verticesCount; j++) {
			ModelFileVertexData vertexData;
			in.read((char*)&vertexData, sizeof vertexData);

			mesh->addVertex(Vertex(
				vector3(vertexData.x, vertexData.y, vertexData.z),
				vector2(vertexData.u, vertexData.v),
				vector3(vertexData.nx, vertexData.ny, vertexData.nz)
			));
		}

		for (size_t j = 0; j < meshData.indicesCount; j++) {
			uint32 index;
			in.read((char*)&index, sizeof index);

			mesh->addIndex(index);
		}

		mesh->setName(meshData.name);
		mesh->updateBuffers();
		meshes.push_back(mesh);
	}

	return meshes;
}
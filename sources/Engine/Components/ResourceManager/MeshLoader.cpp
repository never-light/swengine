#include "MeshLoader.h"

#include <Engine\ServiceLocator.h>
#include <Engine\Components\Debugging\Log.h>

#include <vector>
#include <bitset>
#include <fstream>


MeshLoader::MeshLoader() {

}

MeshLoader::~MeshLoader() {

}

Mesh* MeshLoader::load(const std::string& filename) {
	std::vector<Mesh*> subMeshes;

	std::ifstream in(filename, std::ios::binary | std::ios::in);

	HeaderData header;
	in.read((char*)&header, sizeof header);

	for (size_t i = 0; i < header.meshesCount; i++) {
		MeshDescription description;
		in.read((char*)&description, sizeof description);

		std::bitset<32> vertexDataComponents(description.components);

		HardwareBuffer* geometryBuffer = ServiceLocator::getGraphicsManager()->createHardwareBuffer();
		geometryBuffer->lock();
		geometryBuffer->setVertexFormat(VertexFormat::P1N1UVT1);

		vec3* vertexPositions = readVertexComponent<vec3>(in, description.verticesCount);
		vec3* vertexNormals = readVertexComponent<vec3>(in, description.verticesCount);
		vec2* vertexTextureCoordinates = readVertexComponent<vec2>(in, description.verticesCount);
		vec3* vertexTangents = readVertexComponent<vec3>(in, description.verticesCount);

		std::vector<VertexP1N1UVT1> vertices;

		for (size_t v = 0; v < description.verticesCount; v++) {
			vertices.push_back(VertexP1N1UVT1(
				vector3(vertexPositions[v].x, vertexPositions[v].y, vertexPositions[v].z),
				vector3(vertexNormals[v].x, vertexNormals[v].y, vertexNormals[v].z),
				vector2(vertexTextureCoordinates[v].x, vertexTextureCoordinates[v].y),
				vector3(vertexTangents[v].x, vertexTangents[v].y, vertexTangents[v].z)
			));
		}

		delete[] vertexPositions;
		delete[] vertexNormals;
		delete[] vertexTextureCoordinates;
		delete[] vertexTangents;

		geometryBuffer->setVerticesData(vertices.size(), sizeof(VertexP1N1UVT1), vertices.data());

		std::vector<uint32> indices;

		for (size_t j = 0; j < description.indicesCount; j++) {
			uint32 index;
			in.read((char*)&index, sizeof index);

			indices.push_back(index);
		}

		geometryBuffer->setIndicesData(indices.size(), sizeof(uint32), indices.data());
		geometryBuffer->unlock();

		Mesh* subMesh = new Mesh(geometryBuffer);
		subMesh->setName(description.name);

		subMeshes.push_back(subMesh);
	}

	Mesh* mesh = new Mesh();
	for (Mesh* subMesh : subMeshes) {
		mesh->addSubMesh(subMesh);
	}

	return mesh;
}

template<class T>
T* MeshLoader::readVertexComponent(std::ifstream& in, int verticesCount) {
	T* data = new T[verticesCount];
	in.read((char*)&data[0], sizeof(T) * verticesCount);

	return data;
}
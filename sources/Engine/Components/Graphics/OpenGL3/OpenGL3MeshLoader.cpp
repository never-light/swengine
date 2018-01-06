#include "OpenGL3MeshLoader.h"
#include "OpenGL3Mesh.h"

Mesh* OpenGL3MeshLoader::createMesh() {
	return new OpenGL3Mesh();
}
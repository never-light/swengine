#include "MeshComponent.h"

MeshComponent::MeshComponent(Mesh * mesh)
	: m_mesh(mesh)
{

}

Mesh * MeshComponent::getMesh() const
{
	return m_mesh;
}

void MeshComponent::setMesh(Mesh * mesh)
{
	m_mesh = mesh;
}

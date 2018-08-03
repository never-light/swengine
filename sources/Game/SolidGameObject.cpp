#include "SolidGameObject.h"

SolidGameObject::SolidGameObject(SolidMesh* mesh)
	: GameObject(), 
	m_mesh(mesh), 
	m_transform(new Transform())
{
	m_transform->setPosition(0, 0, 0);
	m_transform->setOrientation(quaternion());
	m_transform->setScale(1, 1, 1);
}

SolidGameObject::~SolidGameObject()
{
	delete m_transform;
}

void SolidGameObject::render(GpuProgram* gpuProgram) {
	gpuProgram->setParameter("transform.localToWorld", m_transform->getTransformationMatrix());

	m_mesh->render(gpuProgram);
}

Transform * SolidGameObject::getTransform() const
{
	return m_transform;
}

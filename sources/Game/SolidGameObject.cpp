#include "SolidGameObject.h"

SolidGameObject::SolidGameObject(SolidMesh* mesh, BaseMaterial* baseMaterial)
	: GameObject(), 
	Renderable(baseMaterial),
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

void SolidGameObject::render() {
	if (m_baseMaterial->isTransformsDataRequired())
		m_baseMaterial->getGpuProgram()->setParameter("transform.localToWorld", m_transform->getTransformationMatrix());

	m_mesh->render(m_baseMaterial);
}

Transform * SolidGameObject::getTransform() const
{
	return m_transform;
}

std::vector<OBB> SolidGameObject::getColliders() const
{
	return m_mesh->getColliders();
}

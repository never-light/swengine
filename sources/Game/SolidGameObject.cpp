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

void SolidGameObject::render(GraphicsContext* graphicsContext, GpuProgram* gpuProgram) {
	gpuProgram->setParameter("transform.localToWorld", m_transform->getTransformationMatrix());

	if (m_mesh->hasSkeleton()) {
		Skeleton* skeleton = m_mesh->getSkeleton();
		size_t bonesCount = skeleton->getBonesCount();

		size_t boneIndex = 0;

		for (const Bone& bone : skeleton->getBones()) {
			gpuProgram->setParameter("bonesTransforms[" + std::to_string(boneIndex) + "]", bone.getCurrentPoseTransform());
			boneIndex++;
		}
	}

	m_mesh->render(graphicsContext, gpuProgram);
}

Transform * SolidGameObject::getTransform() const
{
	return m_transform;
}

std::vector<OBB> SolidGameObject::getColliders() const
{
	return m_mesh->getColliders();
}

#pragma once

#include "GameObject.h"
#include <Game\Graphics\SolidMesh.h>
#include <Game\Graphics\Renderable.h>

class Player : public GameObject, public Renderable {
public:
	Player(SolidMesh* armsMesh, BaseMaterial* baseMaterial);
	virtual ~Player();

	virtual void render() override;
	Transform* getTransform() const;

	OBB getWorldPlacedCollider() const;
	vector3 getPosition() const;

	Skeleton* getSkeleton() const;

	void applyPose(const SkeletonPose& pose);
private:
	Transform * m_transform;
	SolidMesh* m_armsMesh;
};
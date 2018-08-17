#pragma once

#include "GameObject.h"
#include <Game\Graphics\SolidMesh.h>
#include <Game\Graphics\Renderable.h>

#include <Game\Game\Inventory\Inventory.h>

class Player : public GameObject, public Renderable {
public:
	Player(SolidMesh* armsMesh, BaseMaterial* baseMaterial);
	virtual ~Player();

	virtual void render() override;
	Transform* getTransform() const;

	OBB getWorldPlacedCollider() const;
	vector3 getPosition() const override;

	Skeleton* getSkeleton() const;

	void applyPose(const SkeletonPose& pose);

	Inventory* getInventory() const;
private:
	Transform * m_transform;
	SolidMesh* m_armsMesh;

	Inventory* m_inventory;
};
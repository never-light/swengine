#pragma once

#include <optional>

#include <Engine/Components/Physics/Colliders/AABB.h>
#include <Engine/Components/Graphics/RenderLayout/Materials/Material.h>
#include <Engine/Components/Graphics/RenderSystem/GeometryInstance.h>

class SubMesh {
public:
	SubMesh(GeometryInstance* instance, Material* material);
	~SubMesh();

	GeometryInstance* getGeometryInstance() const;

	void setMaterial(Material* material);
	Material* getMaterial() const;

	bool isSkinned() const;
	bool hasNormals() const;
	bool hasTangents() const;
	bool hasBitangents() const;
	bool hasUV() const;
	bool hasColors() const;

	bool hasAABB() const;

	void setAABB(const AABB& aabb);
	const AABB& getAABB() const;

private:
	Material* m_material;
	GeometryInstance* m_geometryInstance;

	std::optional<AABB> m_aabb;
};
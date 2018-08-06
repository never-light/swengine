#pragma once

#include <Engine\Components\Math\types.h>
#include <Engine\Components\Physics\Ray.h>
#include <Engine\Components\Physics\ProjectionBounds.h>
#include <Engine\Components\Physics\Intersection.h>

#include <vector>

class OBB {
public:
	OBB(const OBB& obb, const matrix4& transform);
	OBB(const vector3& origin, const vector3& v1, const vector3& v2, const vector3& v3);
	~OBB();

	bool intersects(const OBB& second, Intersection& intersection) const;
	bool intersects(const Ray& ray, float& distance) const;

	std::vector<vector3> getNormals() const;
	ProjectionBounds getProjection(const vector3& direction) const;

	std::vector<vector3> getVertices() const;

	vector3 getCenter() const;

private:
	void calculateProperties();

private:
	vector3 m_origin;
	vector3 m_vertex1;
	vector3 m_vertex2;
	vector3 m_vertex3;

	vector3 m_basis[3];
	vector3 m_size;

	vector3 m_center;
};
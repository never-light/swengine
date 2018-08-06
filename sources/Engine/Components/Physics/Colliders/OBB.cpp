#include "OBB.h"

#include <vector>
#include <algorithm>

OBB::OBB(const OBB & obb, const matrix4& transform)
	: m_origin(transform * vector4(obb.m_origin, 1.0f)),
	m_vertex1(transform * vector4(obb.m_vertex1, 1.0f)),
	m_vertex2(transform * vector4(obb.m_vertex2, 1.0f)),
	m_vertex3(transform * vector4(obb.m_vertex3, 1.0f))
{
	calculateProperties();
}

OBB::OBB(const vector3 & origin, const vector3 & v1, const vector3 & v2, const vector3 & v3)
	: m_origin(origin), m_vertex1(v1), m_vertex2(v2), m_vertex3(v3)
{
	calculateProperties();
}

OBB::~OBB()
{
}


void OBB::calculateProperties()
{
	vector3 bx = m_vertex1 - m_origin;
	vector3 by = m_vertex2 - m_origin;
	vector3 bz = m_vertex3 - m_origin;

	vector3 opposite = m_origin + bx + by + bz;

	m_center = 0.5f * (m_origin + opposite);

	m_basis[0] = glm::normalize(bx);
	m_basis[1] = glm::normalize(by);
	m_basis[2] = glm::normalize(bz);

	m_size.x = glm::length(bx);
	m_size.y = glm::length(by);
	m_size.z = glm::length(bz);
}

bool OBB::intersects(const OBB& second, Intersection& intersection) const
{
	std::vector<vector3> normalsFirst = this->getNormals();
	std::vector<vector3> normalsSecond = second.getNormals();

	// Собираем направления для проверки
	std::vector<vector3> checkedDirections;

	checkedDirections.insert(checkedDirections.end(), normalsFirst.begin(), normalsFirst.end());
	checkedDirections.insert(checkedDirections.end(), normalsSecond.begin(), normalsSecond.end());

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			if (normalsFirst[i] == normalsSecond[j])
				continue;

			checkedDirections.push_back(glm::normalize(glm::cross(normalsFirst[i], normalsSecond[j])));
		}

	vector3 mtv;
	float mtvLength = std::numeric_limits<float>::infinity();

	// Проверяем проекции OBB на направления
	for (const vector3& direction : checkedDirections) {
		ProjectionBounds firstProjection = this->getProjection(direction);
		ProjectionBounds secondProjection = second.getProjection(direction);

		float intersectionLength = ProjectionBounds::getOverlapLength(firstProjection, secondProjection);

		if (abs(intersectionLength) <= 1e-6f)
			return false;

		if (abs(intersectionLength) < abs(mtvLength)) {
			mtv = direction;
			mtvLength = intersectionLength;
		}
	}

	intersection.setDirection(mtv);
	intersection.setDepth(mtvLength);

	bool notPointingInTheSameDirection = glm::dot(getCenter() - second.getCenter(), mtv) < 0;

	if (notPointingInTheSameDirection) {
		intersection.setDirection(-mtv);
	}

	return true;
}

std::vector<vector3> OBB::getNormals() const {
	return std::vector<vector3>{
		glm::normalize(glm::cross(m_basis[0], m_basis[1])), 
			glm::normalize(glm::cross(m_basis[0], m_basis[2])), 
			glm::normalize(glm::cross(m_basis[1], m_basis[2]))
	};
}

ProjectionBounds OBB::getProjection(const vector3& direction) const {
	std::vector<vector3> vertices = getVertices();

	ProjectionBounds projection;
	projection.min = std::numeric_limits<float>::infinity();
	projection.max = -std::numeric_limits<float>::infinity();

	for (auto& vertex : vertices) {
		float projectionValue = glm::dot(vertex, direction);
		if (projectionValue < projection.min) { projection.min = projectionValue; }
		if (projectionValue > projection.max) { projection.max = projectionValue; }
	}

	return projection;
}

std::vector<vector3> OBB::getVertices() const
{
	vector3 bx = m_basis[0] * m_size[0];
	vector3 by = m_basis[1] * m_size[1];
	vector3 bz = m_basis[2] * m_size[2];

	return std::vector<vector3> {
		m_origin,
		m_vertex1,
		m_origin + bx + by,
		m_vertex2,
		m_vertex3,
		m_origin + by + bz,
		m_origin + bx + bz,
		m_origin + bx + by + bz,
	};
}

vector3 OBB::getCenter() const
{
	return m_center;
}

bool OBB::intersects(const Ray & ray, float & distance) const
{
	float eps = 1e-5f;

	float tNear = -std::numeric_limits<float>::infinity();
	float tFar = std::numeric_limits<float>::infinity();

	for (size_t i = 0; i < 3; i++) {
		float r = glm::dot(m_basis[i], m_center - ray.getOrigin());
		float s = glm::dot(m_basis[i], ray.getDirection());
		float t0 = (r + m_size[i] * 0.5) / s;
		float t1 = (r - m_size[i] * 0.5) / s;
		if (t0 > t1) {
			std::swap(t0, t1);
		}

		if (t0 > tNear) {
			tNear = t0;
		}
		if (t1 < tFar) {
			tFar = t1;
		}

		if (tNear > tFar) {
			return false;
		}

		if (tFar < 0) {
			return false;
		}
	}

	distance = tNear > 0 ? tNear : tFar;

	return true;
}

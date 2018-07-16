#pragma once

#include <string>
#include <vector>

#include "Material.h"
#include <Engine\Components\Math\Math.h>

class Mesh {
public:
	Mesh();
	virtual ~Mesh();

	virtual void render() = 0;

	virtual void create() = 0;
	virtual void destroy() = 0;

	std::vector<vector3> getPositions() const;
	void setPositions(const std::vector<vector3>& positions);

	std::vector<vector3> getNormals() const;
	void setNormals(const std::vector<vector3>& normals);

	std::vector<vector3> getTangents() const;
	void setTangents(const std::vector<vector3>& tangents);

	std::vector<vector2> getUV() const;
	void setUV(const std::vector<vector2>& uv);

	std::vector<uint32> getIndices() const;
	void setIndices(const std::vector<uint32>& indices);

	std::string getName() const;
	void setName(const std::string& name);

protected:
	std::string m_name;

protected:
	std::vector<vector3> m_positions;
	std::vector<vector3> m_normals;
	std::vector<vector3> m_tangents;
	std::vector<vector2> m_uv;

	std::vector<uint32> m_indices;
};
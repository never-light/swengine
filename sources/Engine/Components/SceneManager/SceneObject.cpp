#include "SceneObject.h"

SceneObject::SceneObject(const std::string& name)
	: m_id(-1), m_name(name)
{

}

SceneObject::~SceneObject()
{
}

void SceneObject::setId(SceneObjectId id)
{
	m_id = id;
}

SceneObjectId SceneObject::getId() const
{
	return m_id;
}

void SceneObject::setName(const std::string & name)
{
	m_name = name;
}

std::string SceneObject::getName() const
{
	return m_name;
}

#include "SceneObject.h"

SceneObject::SceneObject()
	: m_id(0)
{

}

SceneObject::~SceneObject()
{
}

void SceneObject::onRegister(SceneObjectId id)
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

#pragma once

#include <string>
#include <Engine\Components\Graphics\RenderSystem\Camera.h>

using SceneObjectId = size_t;

class SceneObject {
public:
	SceneObject(const std::string& name);
	virtual ~SceneObject();

	virtual void render(const Camera* camera) = 0;
	virtual void update() = 0;

	void setId(SceneObjectId id);
	SceneObjectId getId() const;
	
	void setName(const std::string& name);
	std::string getName() const;

protected:
	std::string m_name;

private:
	SceneObjectId m_id;
};
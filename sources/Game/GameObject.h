#pragma once

#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include <Engine\Components\SceneManager\SceneObject.h>
#include <Engine\Components\Math\Transform.h>

class GameObject : public SceneObject {
public:
	GameObject();
	virtual ~GameObject();

	virtual void render(GpuProgram* gpuProgram) = 0;
};
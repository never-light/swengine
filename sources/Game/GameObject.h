#pragma once

#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include <Engine\Components\SceneManager\SceneObject.h>
#include <Engine\Components\Math\Transform.h>
#include <Engine\Components\Graphics\RenderSystem\GraphicsContext.h>

class GameObject : public SceneObject {
public:
	GameObject();
	virtual ~GameObject();
};
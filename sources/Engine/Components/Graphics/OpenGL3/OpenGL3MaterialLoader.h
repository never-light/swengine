#pragma once

#include <Engine\Components\Graphics\RenderSystem\MaterialLoader.h>

class OpenGL3MaterialLoader : public MaterialLoader {
public:
	Material* createMaterial() override;
};
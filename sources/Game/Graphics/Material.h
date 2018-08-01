#pragma once

#include <Engine\Components\Graphics\OpenGL3\OpenGL3GpuProgram.h>
#include <Engine\Components\ResourceManager\Resource.h>

class Material : public Resource {
public:
	Material();
	virtual ~Material();
};
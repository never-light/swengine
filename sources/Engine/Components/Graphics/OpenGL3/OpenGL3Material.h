#pragma once

#include <unordered_map>
#include <Engine\Components\Graphics\RenderSystem\Material.h>

#include "OpenGL3.h"

class OpenGL3Material : public Material {
public:
	OpenGL3Material();
	virtual ~OpenGL3Material();

	virtual void create() override;
	virtual void destroy() override;

	virtual void bind() override;
	virtual void unbind() override;
};
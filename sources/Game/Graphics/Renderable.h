#pragma once

#include <string>
#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include <Game\Graphics\Materials\BaseMaterial.h>

class Renderable {
public:
	Renderable(BaseMaterial* baseMaterial);
	~Renderable();

	virtual void render() = 0;

	BaseMaterial* getBaseMaterial() const;

protected:
	BaseMaterial* m_baseMaterial;
};
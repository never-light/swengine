#include "Renderable.h"

Renderable::Renderable(BaseMaterial* baseMaterial)
	: m_baseMaterial(baseMaterial)
{
}

Renderable::~Renderable()
{
}

BaseMaterial * Renderable::getBaseMaterial() const
{
	return m_baseMaterial;
}

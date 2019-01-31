#include "Material.h"

void Material::setDomain(Domain domain)
{
	m_domain = domain;
}

Material::Domain Material::getDomain() const
{
	return m_domain;
}

void Material::setBlendMode(BlendMode mode)
{
	m_blendMode = mode;
}

Material::BlendMode Material::getBlendMode() const
{
	return m_blendMode;
}

void Material::setLightingMode(LightingMode mode)
{
	m_lightingMode = mode;
}

Material::LightingMode Material::getLightingMode() const
{
	return m_lightingMode;
}

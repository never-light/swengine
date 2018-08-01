#include "DefaultMaterial.h"

DefaultMaterial::DefaultMaterial() 
	: Material(), 
	m_emissiveColor(0.0, 0.0, 0.0), m_diffuseColor(1.0, 1.0, 1.0), m_specularColor(1.0, 1.0, 1.0),
	m_specularFactor(0.0f), 
	m_diffuseTexture(nullptr), m_specularTexture(nullptr), m_normalMap(nullptr)

{
}

DefaultMaterial::~DefaultMaterial()
{
}

vector3 DefaultMaterial::getEmissiveColor() const
{
	return m_emissiveColor;
}

void DefaultMaterial::setEmissiveColor(const vector3 & color)
{
	m_emissiveColor = color;
}

vector3 DefaultMaterial::getDiffuseColor() const
{
	return m_diffuseColor;
}

void DefaultMaterial::setDiffuseColor(const vector3 & color)
{
	m_diffuseColor = color;
}

vector3 DefaultMaterial::getSpecularColor() const
{
	return m_specularColor;
}

void DefaultMaterial::setSpecularColor(const vector3 & color)
{
	m_specularColor = color;
}

float DefaultMaterial::getSpecularFactor() const
{
	return m_specularFactor;
}

void DefaultMaterial::setSpecularFactor(float factor)
{
	m_specularFactor = factor;
}

Texture * DefaultMaterial::getDiffuseTexture() const
{
	return m_diffuseTexture;
}

void DefaultMaterial::setDiffuseTexture(Texture * texture)
{
	m_diffuseTexture = texture;
}

Texture * DefaultMaterial::getSpecularTexture() const
{
	return m_specularTexture;
}

void DefaultMaterial::setSpecularTexture(Texture * texture)
{
	m_specularTexture = texture;
}

Texture * DefaultMaterial::getNormalMap() const
{
	return m_normalMap;
}

void DefaultMaterial::setNormalMap(Texture * normalMap)
{
	m_normalMap = normalMap;
}

#include "PhongMaterialParameters.h"

PhongMaterialParameters::PhongMaterialParameters()
	: MaterialParameters(),
	m_emissiveColor(0.0, 0.0, 0.0), m_diffuseColor(1.0, 1.0, 1.0), m_specularColor(1.0, 1.0, 1.0),
	m_specularFactor(0.0f),
	m_diffuseTexture(nullptr), m_specularTexture(nullptr), m_normalMap(nullptr)

{
}

PhongMaterialParameters::~PhongMaterialParameters()
{
}

vector3 PhongMaterialParameters::getEmissiveColor() const
{
	return m_emissiveColor;
}

void PhongMaterialParameters::setEmissiveColor(const vector3 & color)
{
	m_emissiveColor = color;
}

vector3 PhongMaterialParameters::getDiffuseColor() const
{
	return m_diffuseColor;
}

void PhongMaterialParameters::setDiffuseColor(const vector3 & color)
{
	m_diffuseColor = color;
}

vector3 PhongMaterialParameters::getSpecularColor() const
{
	return m_specularColor;
}

void PhongMaterialParameters::setSpecularColor(const vector3 & color)
{
	m_specularColor = color;
}

float PhongMaterialParameters::getSpecularFactor() const
{
	return m_specularFactor;
}

void PhongMaterialParameters::setSpecularFactor(float factor)
{
	m_specularFactor = factor;
}

Texture * PhongMaterialParameters::getDiffuseTexture() const
{
	return m_diffuseTexture;
}

void PhongMaterialParameters::setDiffuseTexture(Texture * texture)
{
	m_diffuseTexture = texture;
}

Texture * PhongMaterialParameters::getSpecularTexture() const
{
	return m_specularTexture;
}

void PhongMaterialParameters::setSpecularTexture(Texture * texture)
{
	m_specularTexture = texture;
}

Texture * PhongMaterialParameters::getNormalMap() const
{
	return m_normalMap;
}

void PhongMaterialParameters::setNormalMap(Texture * normalMap)
{
	m_normalMap = normalMap;
}

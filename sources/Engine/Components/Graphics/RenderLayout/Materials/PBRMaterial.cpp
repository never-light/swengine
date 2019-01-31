#include "PBRMaterial.h"

PBRMaterial::PBRMaterial()
	: m_colorTexture(nullptr),
	m_normalTexture(nullptr),
	m_AOTexture(nullptr),
	m_roughnessTexture(nullptr),
	m_metallicTexture(nullptr),
	m_AOValue(0),
	m_metallicValue(0),
	m_roughnessValue(0),
	m_colorValue()
{
}

PBRMaterial::~PBRMaterial()
{
}


void PBRMaterial::setColor(Texture * color)
{
	m_colorTexture = color;
}

void PBRMaterial::setColor(vector3 color)
{
	m_colorValue = color;
}

bool PBRMaterial::hasColorMap() const
{
	return m_colorTexture != nullptr;
}

Texture * PBRMaterial::getColorMap() const
{
	return m_colorTexture;
}

vector3 PBRMaterial::getColorValue() const
{
	return m_colorValue;
}

void PBRMaterial::setMetallic(Texture * metallic)
{
	m_metallicTexture = metallic;
}

void PBRMaterial::setMetallic(float metallic)
{
	m_metallicValue = metallic;
}

bool PBRMaterial::hasMetallicMap() const
{
	return m_metallicTexture != nullptr;
}

Texture * PBRMaterial::getMetallicMap() const
{
	return m_metallicTexture;
}

float PBRMaterial::getMetallicValue() const
{
	return m_metallicValue;
}

void PBRMaterial::setRoughness(Texture * roughness)
{
	m_roughnessTexture = roughness;
}

void PBRMaterial::setRoughness(float roughness)
{
	m_roughnessValue = roughness;
}

bool PBRMaterial::hasRoughnessMap() const
{
	return m_roughnessTexture != nullptr;
}

Texture * PBRMaterial::getRoughnessMap() const
{
	return m_roughnessTexture;
}

float PBRMaterial::getRoughnessValue() const
{
	return m_roughnessValue;
}

void PBRMaterial::setAO(Texture * AO)
{
	m_AOTexture = AO;
}

void PBRMaterial::setAO(float AO)
{
	m_AOValue = AO;
}

bool PBRMaterial::hasAOMap() const
{
	return m_AOTexture != nullptr;
}

Texture * PBRMaterial::getAOMap() const
{
	return m_AOTexture;
}

float PBRMaterial::getAOValue() const
{
	return m_AOValue;
}

void PBRMaterial::setNormal(Texture * normal)
{
	m_normalTexture = normal;
}

bool PBRMaterial::hasNormalMap() const
{
	return m_normalTexture != nullptr;
}

Texture * PBRMaterial::getNormalMap() const
{
	return m_normalTexture;
}

#include "PhongLightingMaterial.h"

#include "PhongMaterialParameters.h"

PhongLightingMaterial::PhongLightingMaterial(const std::string & name, GpuProgram * gpuProgram)
	: BaseMaterial(name, gpuProgram)
{
	m_lightsDataRequired = true;
	m_transformsDataRequired = true;

	m_supportDeferred = true;
}

PhongLightingMaterial::~PhongLightingMaterial()
{
}

void PhongLightingMaterial::applySpecifier(const MaterialParameters* parameters)
{
	const PhongMaterialParameters* phongParameters = static_cast<const PhongMaterialParameters*>(parameters);

	m_gpuProgram->setParameter("material.diffuseColor", phongParameters->getDiffuseColor());
	m_gpuProgram->setParameter("material.specularColor", phongParameters->getSpecularColor());
	m_gpuProgram->setParameter("material.specularFactor", phongParameters->getSpecularFactor());
	m_gpuProgram->setParameter("material.emissiveColor", phongParameters->getEmissiveColor());

	Texture* diffuseTexture = phongParameters->getDiffuseTexture();

	if (diffuseTexture != nullptr) {
		diffuseTexture->bind(0);

		m_gpuProgram->setParameter("material.useDiffuseTexture", true);
		m_gpuProgram->setParameter("material.diffuseTexture", 0);
	}
	else {
		m_gpuProgram->setParameter("material.useDiffuseTexture", false);
	}

	Texture* specularTexture = phongParameters->getSpecularTexture();

	if (specularTexture != nullptr) {
		specularTexture->bind(1);
		m_gpuProgram->setParameter("material.useSpecularTexture", true);
		m_gpuProgram->setParameter("material.specularTexture", 1);
	}

	Texture* normalMap = phongParameters->getNormalMap();

	if (normalMap != nullptr) {
		normalMap->bind(2);
		m_gpuProgram->setParameter("material.useNormalMapping", true);
		m_gpuProgram->setParameter("material.normalMap", 2);
	}
}

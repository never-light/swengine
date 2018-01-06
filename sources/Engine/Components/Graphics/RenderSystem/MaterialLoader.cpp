#include "MaterialLoader.h"

#include <Engine\Components\Debugging\Log.h>

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <regex>
#include <exception>

MaterialLoader::MaterialLoader() {
	
}

MaterialLoader::~MaterialLoader() {

}

Material* MaterialLoader::createMaterial(const MaterialParametersList& parameters) {
	Material* material = createMaterial();

	setMaterialParameterIfExists(material, parameters, "program");
	setMaterialParameterIfExists(material, parameters, "ambientColor");
	setMaterialParameterIfExists(material, parameters, "diffuseColor");
	setMaterialParameterIfExists(material, parameters, "specularColor");
	setMaterialParameterIfExists(material, parameters, "specularLevel");
	setMaterialParameterIfExists(material, parameters, "diffuseMap");
	setMaterialParameterIfExists(material, parameters, "specularMap");

	return material;
}

void MaterialLoader::setMaterialParameterIfExists(Material* material, const MaterialParametersList& parameters, const std::string& parameterName) {
	auto it = parameters.find(parameterName);

	if (it == parameters.end()) {
		return;
	}

	if (parameterName == "ambientColor") {
		material->setAmbientColor(it->second.getColor());
	}
	else if (parameterName == "diffuseColor") {
		material->setDuffuseColor(it->second.getColor());
	}
	else if (parameterName == "specularColor") {
		material->setSpecularColor(it->second.getColor());
	}
	else if (parameterName == "specularLevel") {
		material->setShininess(it->second.getNumber());
	}
	else if (parameterName == "diffuseMap") {
		material->setLightmap(LightmapType::Diffuse, it->second.getTexture());
	}
	else if (parameterName == "specularMap") {
		material->setLightmap(LightmapType::Specular, it->second.getTexture());
	}
	else if (parameterName == "program") {
		material->setGpuProgram(it->second.getGpuProgam());
	}
}
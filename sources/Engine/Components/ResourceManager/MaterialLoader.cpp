#include "MaterialLoader.h"
#include "ResourceManager.h"

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

std::unordered_map<std::string, Material*> MaterialLoader::loadPackage(ResourceManager* resourceManager, const std::string& path) {
	m_resourceManager = resourceManager;

	std::unordered_map<std::string, Material*> package;

	std::ifstream in(path);
	std::string currentLine, currentMaterialName;
	std::unordered_map<std::string, MaterialParameterValue> parameters;

	std::regex materialNameRegex("^\\[([a-zA-Z0-9]+)\\]$");
	std::regex materialParameterNumberRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+([-+]?[0-9]*\\.?[0-9]+)$");
	std::regex materialParameterStringRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+\\\"([0-9a-zA-Z\\.\\\\\\/_]+)\\\"$");
	std::regex materialParameterColorRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+color\\(([-+]?[0-9]*\\.?[0-9]+),[\\s]+([-+]?[0-9]*\\.?[0-9]+),[\\s]+([-+]?[0-9]*\\.?[0-9]+)\\)$");
	std::regex materialParameterTextureRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+texture\\(\\\"([0-9a-zA-Z\\.\\\\\\/_]+)\\\"\\)$");

	std::smatch match;

	while (std::getline(in, currentLine)) {
		if (currentLine.empty()) {
			continue;
		}

		if (std::regex_search(currentLine, match, materialNameRegex)) {
			if (!currentMaterialName.empty()) {
				// Save material
				auto material = createMaterial(currentMaterialName, parameters);
				package.insert(std::make_pair(currentMaterialName, material));

				currentMaterialName.clear();
				parameters.clear();
			}

			currentMaterialName = match[1];
		}
		else if (!currentMaterialName.empty()) {
			// Number Value
			if (std::regex_search(currentLine, match, materialParameterNumberRegex)) {
				parameters.insert(std::make_pair(
					match[1], // Parameter name
					MaterialParameterValue(std::atof(std::string(match[2]).c_str())) // Number value
				));
			}
			// String value
			else if (std::regex_search(currentLine, match, materialParameterStringRegex)) {
				parameters.insert(std::make_pair(
					match[1], // Parameter name
					MaterialParameterValue(std::string(match[2])) // String value
				));
			}
			// Color value
			else if (std::regex_search(currentLine, match, materialParameterColorRegex)) {
				parameters.insert(std::make_pair(
					match[1], // Parameter name
					MaterialParameterValue(Color(
						std::atof(std::string(match[2]).c_str()), // r
						std::atof(std::string(match[3]).c_str()), // g
						std::atof(std::string(match[4]).c_str()) // b
					)) // Color value
				));

			}
			// Texture value
			else if (std::regex_search(currentLine, match, materialParameterTextureRegex)) {
				Texture* texture = resourceManager->loadTexture(match[2]);

				parameters.insert(std::make_pair(
					match[1], // Parameter name
					MaterialParameterValue(texture) // Texture value
				));
			}
			else {
				errlog() << "Invalid string in material file: " << currentLine;
				throw std::exception(std::string( "Invalid string in material file: " + currentLine).c_str());
			}
		}
	}

	// Save last material
	if (!currentMaterialName.empty()) {
		auto material = createMaterial(currentMaterialName, parameters);
		package.insert(std::make_pair(currentMaterialName, material));
	}

	return package;
}

Material* MaterialLoader::createMaterial(const std::string& name, const std::unordered_map<std::string, MaterialParameterValue>& parameters) {
	Material* material = new Material;

	setMaterialParameterIfExists(material, parameters, "program");
	setMaterialParameterIfExists(material, parameters, "ambientColor");
	setMaterialParameterIfExists(material, parameters, "diffuseColor");
	setMaterialParameterIfExists(material, parameters, "specularColor");
	setMaterialParameterIfExists(material, parameters, "specularLevel");
	setMaterialParameterIfExists(material, parameters, "diffuseMap");
	setMaterialParameterIfExists(material, parameters, "specularMap");

	return material;
}

void MaterialLoader::setMaterialParameterIfExists(Material* material, const std::unordered_map<std::string, MaterialParameterValue>& parameters, const std::string& parameterName) {
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
		material->setShader(m_resourceManager->loadShader(it->second.getString()));
	}
}
#include "ConfigManager.h"

ConfigManager::ConfigManager() {

}

ConfigManager::~ConfigManager() {

}

template <>
int ConfigManager::getOption(const std::string& name) {
	return std::get<int>(m_options[name]);
}

template <>
real ConfigManager::getOption(const std::string& name) {
	return std::get<real>(m_options[name]);
}

void ConfigManager::setOption(const std::string& name, const OptionValue& value) {
	m_options[name] = value;
}
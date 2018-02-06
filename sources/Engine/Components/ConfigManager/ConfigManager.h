#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

#include <Engine\types.h>

class ConfigManager {
public:
	using OptionValue = std::variant<int, real>;

public:
	ConfigManager();
	~ConfigManager();

	template <class T>
	T getOption(const std::string& name);
	void setOption(const std::string& name, const OptionValue& value);
private:
	std::unordered_map<std::string, OptionValue> m_options;
};
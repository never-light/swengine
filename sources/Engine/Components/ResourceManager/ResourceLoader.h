#pragma once

#include <string>

#include <optional>
#include <any>

#include "ResourceInstance.h"

class ResourceLoader {
public:
	ResourceLoader();
	virtual ~ResourceLoader();

	virtual BaseResourceInstance* load(const std::string& path, std::optional<std::any> options) = 0;
};
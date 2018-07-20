#pragma once

#include "Resource.h"
#include <string>

class ResourceLoader {
public:
	ResourceLoader();
	virtual ~ResourceLoader();

	virtual Resource* load(const std::string& filename) = 0;
};
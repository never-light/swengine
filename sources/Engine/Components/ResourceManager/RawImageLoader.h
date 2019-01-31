#pragma once

#include "ResourceLoader.h"

class RawImageLoader : public ResourceLoader {
public:
	RawImageLoader();
	virtual ~RawImageLoader();

	virtual BaseResourceInstance * load(const std::string & path, std::optional<std::any> options) override;
};
#pragma once

#include "ResourceLoader.h"
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>

class RawImageLoader : public ResourceLoader {
public:
	RawImageLoader();
	virtual ~RawImageLoader();

	virtual Resource* load(const std::string & filename) override;
};
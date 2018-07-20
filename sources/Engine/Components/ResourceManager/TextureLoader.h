#pragma once

#include "ResourceLoader.h"
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>

class TextureLoader : public ResourceLoader {
public:
	TextureLoader(GraphicsResourceFactory* graphicsResourceFactory);
	virtual ~TextureLoader();

	virtual Resource* load(const std::string & filename) override;
	
protected:
	GraphicsResourceFactory * m_graphicsResourceFactory;
};
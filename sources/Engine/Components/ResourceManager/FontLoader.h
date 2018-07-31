#pragma once

#include "ResourceLoader.h"
#include <Engine\Components\Graphics\RenderSystem\Texture.h>
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>

#include <Engine\Components\GUI\Font.h>

class FontLoader : public ResourceLoader {
public:
	FontLoader(GraphicsResourceFactory* graphicsResourceFactory);
	virtual ~FontLoader();

	virtual Resource* load(const std::string & filename) override;

protected:
	Texture * loadBitmap(const std::string& filename);

protected:
	GraphicsResourceFactory * m_graphicsResourceFactory;
};
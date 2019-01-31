#pragma once

#include "ResourceLoader.h"
#include <Engine/Components/Graphics/RenderSystem/Texture.h>
#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>

#include <Engine/Components/GUI/Font.h>

class FontLoader : public ResourceLoader {
public:
	FontLoader(GraphicsContext* graphicsContext);
	virtual ~FontLoader();

protected:
	Texture * loadBitmap(const std::string& filename);

protected:
	GraphicsContext * m_graphicsContext;

	// Inherited via ResourceLoader
	virtual BaseResourceInstance * load(const std::string & path, std::optional<std::any> options) override;
};
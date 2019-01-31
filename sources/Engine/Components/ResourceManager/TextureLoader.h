#pragma once

#include "ResourceLoader.h"
#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>

struct TextureLoadingOptions {
	enum class Format {

	};
};

class TextureLoader : public ResourceLoader {
public:
	TextureLoader(GraphicsContext* graphicsContext);
	virtual ~TextureLoader();

protected:
	GraphicsContext* m_graphicsContext;

	virtual BaseResourceInstance* load(const std::string& path, std::optional<std::any> options) override;
};
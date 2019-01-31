#pragma once

#include "ResourceLoader.h"
#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>

class GpuProgramLoader : public ResourceLoader {
public:
	GpuProgramLoader(GraphicsContext* graphicsContext);
	virtual ~GpuProgramLoader();

	virtual BaseResourceInstance * load(const std::string & path, std::optional<std::any> options) override;

protected:
	GraphicsContext* m_graphicsContext;

};
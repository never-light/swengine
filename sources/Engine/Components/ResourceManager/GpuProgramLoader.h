#pragma once

#include "ResourceLoader.h"
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>

class GpuProgramLoader : public ResourceLoader {
public:
	GpuProgramLoader(GraphicsResourceFactory* graphicsResourceFactory);
	virtual ~GpuProgramLoader();

	virtual Resource* load(const std::string & filename) override;
	
protected:
	GraphicsResourceFactory * m_graphicsResourceFactory;
};
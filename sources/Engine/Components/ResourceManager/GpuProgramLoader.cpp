#include "GpuProgramLoader.h"

#include "HoldingResource.h"
#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>

#include <fstream>
#include <regex>

#include <iostream>
#include "ResourceLoadingException.h"

GpuProgramLoader::GpuProgramLoader(GraphicsResourceFactory* graphicsResourceFactory)
	: ResourceLoader(), m_graphicsResourceFactory(graphicsResourceFactory)
{
}

GpuProgramLoader::~GpuProgramLoader()
{
}

Resource* GpuProgramLoader::load(const std::string & filename)
{
	std::ifstream gpuProgramStream(filename);

	if (!gpuProgramStream.is_open())
		throw ResourceLoadingException(ResourceLoadingError::FileNotAvailable, filename.c_str(), "", __FILE__, __LINE__, __FUNCTION__);

	std::string gpuProgramSource((std::istreambuf_iterator<char>(gpuProgramStream)), 
		std::istreambuf_iterator<char>());

	std::string vertexShaderSource;
	std::string fragmentShaderSource;

	try {
		std::smatch vertexShaderMatch;
		std::regex vertexShaderRegex("\\[vertex\\]([^]*)\\[\\/vertex\\]");
		std::regex_search(gpuProgramSource, vertexShaderMatch, vertexShaderRegex);

		std::smatch fragmentShaderMatch;
		std::regex fragmentShaderRegex("\\[fragment\\]([^]*)\\[\\/fragment\\]");
		std::regex_search(gpuProgramSource, fragmentShaderMatch, fragmentShaderRegex);

		vertexShaderSource = vertexShaderMatch[1].str();
		fragmentShaderSource = fragmentShaderMatch[1].str();
	}
	catch (const std::regex_error& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), exception.what(), __FILE__, __LINE__, __FUNCTION__);
	}

	GpuProgram* gpuProgram = nullptr;

	try {
		gpuProgram = m_graphicsResourceFactory->createGpuProgram();
		gpuProgram->create();
		gpuProgram->addShader(GpuProgram::ShaderType::Vertex, vertexShaderSource);
		gpuProgram->addShader(GpuProgram::ShaderType::Fragment, fragmentShaderSource);

		gpuProgram->link();
	}
	catch (const RenderSystemException& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), exception.what(), exception.getFile(), exception.getLine(), exception.getFunction());
	}

	return new HoldingResource<GpuProgram>(gpuProgram);
}

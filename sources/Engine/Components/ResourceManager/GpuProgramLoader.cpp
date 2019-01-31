#include "GpuProgramLoader.h"

#include <Engine/Components/Graphics/RenderSystem/GpuProgram.h>
#include <Engine/Components/Graphics/RenderSystem/RenderSystemException.h>

#include <fstream>
#include <regex>

#include <iostream>
#include "ResourceLoadingException.h"

GpuProgramLoader::GpuProgramLoader(GraphicsContext* graphicsContext)
	: ResourceLoader(), 
	m_graphicsContext(graphicsContext)
{
}

GpuProgramLoader::~GpuProgramLoader()
{
}

BaseResourceInstance * GpuProgramLoader::load(const std::string & path, std::optional<std::any> options)
{
	std::ifstream gpuProgramStream(path);

	if (!gpuProgramStream.is_open())
		throw ResourceLoadingException(ResourceLoadingError::FileNotAvailable, path.c_str(), "", __FILE__, __LINE__, __FUNCTION__);

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
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), exception.what(), __FILE__, __LINE__, __FUNCTION__);
	}

	GpuProgram* gpuProgram = nullptr;

	try {
		gpuProgram = m_graphicsContext->createGpuProgram();
		gpuProgram->create();
		gpuProgram->addShader(GpuProgram::ShaderType::Vertex, vertexShaderSource);
		gpuProgram->addShader(GpuProgram::ShaderType::Fragment, fragmentShaderSource);

		gpuProgram->link();
	}
	catch (const RenderSystemException& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), exception.what(), exception.getFile(), exception.getLine(), exception.getFunction());
	}

	return new ResourceInstance<GpuProgram>(gpuProgram);
}
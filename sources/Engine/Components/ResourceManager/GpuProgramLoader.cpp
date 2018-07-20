#include "GpuProgramLoader.h"

#include "HoldingResource.h"
#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>

#include <fstream>
#include <regex>

#include <iostream>

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
		throw std::exception(("GpuProgram loading error, program file is not available [" + filename + "]").c_str());

	std::string gpuProgramSource((std::istreambuf_iterator<char>(gpuProgramStream)), 
		std::istreambuf_iterator<char>());

	std::smatch vertexShaderMatch;
	std::regex vertexShaderRegex("\\[vertex\\]([^]*)\\[\\/vertex\\]");
	std::regex_search(gpuProgramSource, vertexShaderMatch, vertexShaderRegex);

	std::smatch fragmentShaderMatch;
	std::regex fragmentShaderRegex("\\[fragment\\]((.|\\n)*)\\[\\/fragment\\]");
	std::regex_search(gpuProgramSource, fragmentShaderMatch, fragmentShaderRegex);

	std::string vertexShaderSource = vertexShaderMatch[1].str();
	std::string fragmentShaderSource = fragmentShaderMatch[1].str();

	GpuProgram* gpuProgram = m_graphicsResourceFactory->createGpuProgram();
	gpuProgram->create();
	gpuProgram->addShader(GpuProgram::ShaderType::Vertex, vertexShaderSource);
	gpuProgram->addShader(GpuProgram::ShaderType::Fragment, fragmentShaderSource);

	gpuProgram->link();

	return new HoldingResource<GpuProgram>(gpuProgram);
}

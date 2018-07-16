#include "OpenGL3Material.h"

OpenGL3Material::OpenGL3Material()
{
}

OpenGL3Material::~OpenGL3Material()
{
}

void OpenGL3Material::create()
{
}

void OpenGL3Material::destroy()
{
}

void OpenGL3Material::bind()
{
	m_gpuProgram->bind();

	for (const auto& parameterIt : m_parameters)
		if (auto pval = std::get_if<int>(&parameterIt.second))
			m_gpuProgram->setParameter(parameterIt.first, pval);
		else if (auto pval = std::get_if<vector3>(&parameterIt.second))
			m_gpuProgram->setParameter(parameterIt.first, pval);
		else if (auto pval = std::get_if<const Texture*>(&parameterIt.second))
			m_gpuProgram->setParameter(parameterIt.first, pval);
}

void OpenGL3Material::unbind()
{
	m_gpuProgram->unbind();
}

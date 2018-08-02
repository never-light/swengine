#pragma once

#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include "OpenGL3.h"

#include <unordered_map>

class OpenGL3GpuProgram : public GpuProgram {
public:
	OpenGL3GpuProgram();
	~OpenGL3GpuProgram();

	virtual void create() override;
	virtual void destroy() override;

	virtual void bind() override;
	virtual void unbind() override;

	virtual void addShader(ShaderType type, const std::string & source) override;
	virtual void link() override;

	GLuint getGpuProgramPointer() const;

	void setParameter(const std::string& name, bool value) override;
	void setParameter(const std::string& name, int value) override;
	void setParameter(const std::string& name, float value) override;
	void setParameter(const std::string& name, const vector3& value) override;
	void setParameter(const std::string& name, const vector4& value) override;
	void setParameter(const std::string& name, const matrix4& value) override;

private:
	GLint getUniformLocation(const std::string& name) const;

private:
	GLuint m_program;

	GLuint m_vertexShader;
	GLuint m_fragmentShader;
};
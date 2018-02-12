#pragma once

#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include "OpenGL3.h"

class OpenGL3GpuProgram : public GpuProgram {
public:
	OpenGL3GpuProgram(const std::unordered_map<std::string, std::string>& sources);
	~OpenGL3GpuProgram();

	GLuint getShaderPointer() const;

	void setParameter(const std::string& name, bool value) override;
	void setParameter(const std::string& name, int value) override;
	void setParameter(const std::string& name, float32 value) override;
	void setParameter(const std::string& name, const vector3& value) override;
	void setParameter(const std::string& name, const matrix4& value) override;
	void setParameter(const std::string& name, Texture* value) override;

	void attachUniformBlock(const std::string& name, GLuint bindingPoint);

	const std::vector<GpuProgramRequiredParameter>& getRequiredParameters() const override;
	void setRequiredParameters(const std::vector<GpuProgramRequiredParameter>& parameters) override;
private:
	GLuint compileShader(const std::string& source, GLenum type);

	void checkShaderState(GLuint shader);
	void checkProgramLinkingState();
private:
	std::vector<GpuProgramRequiredParameter> m_requiredParameters;
	GLuint m_programId;

private:
	static const std::unordered_map<std::string, GLenum> m_availableShaders;
};
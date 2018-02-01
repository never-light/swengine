#pragma once

#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include "OpenGL3.h"

typedef GLuint shaderId;

class OpenGL3GpuProgram : public GpuProgram {
public:
	OpenGL3GpuProgram(const std::string& source);
	~OpenGL3GpuProgram();

	shaderId getShaderPointer() const;

	void setParameter(const std::string& name, bool value) const override;
	void setParameter(const std::string& name, int value) const override;
	void setParameter(const std::string& name, float32 value) const override;
	void setParameter(const std::string& name, const vector3& value) const override;
	void setParameter(const std::string& name, const matrix4& value) const override;

	const GpuProgramParametersList& OpenGL3GpuProgram::getRequiredParameters() const override;
	const GpuProgramParametersSection& getRequiredParametersSection(const std::string& name) const override;
	bool hasRequiredParametersSection(const std::string& name) const override;
private:
	void checkLoadingShaderError(GLuint);
	void checkCreatingShaderProgramError(shaderId);

private:
	GpuProgramParametersList m_requiredParameters;
	shaderId m_programId;
};
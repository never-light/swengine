#pragma once

#include <string>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "Texture.h"
#include "Color.h"

class GpuProgram {
public:
	virtual void setParameter(const std::string& name, bool value) const = 0;
	virtual void setParameter(const std::string& name, int value) const = 0;
	virtual void setParameter(const std::string& name, float32 value) const = 0;
	virtual void setParameter(const std::string& name, const vector3& value) const = 0;
	virtual void setParameter(const std::string& name, const matrix4& value) const = 0;
};
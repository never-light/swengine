#pragma once

#include <Engine/Components/Math/Math.h>
#include <Engine/Components/Graphics/RenderSystem/Texture.h>
#include <Engine/Components/Graphics/RenderSystem/GpuProgram.h>

class Material {
public:
	enum class Domain {
		Geometry
	};

	enum class BlendMode {
		Opaque
	};

	enum class LightingMode {
		Unlit, Default
	};

public:
	Material() = default;
	virtual ~Material() = default;

	void setDomain(Domain domain);
	Domain getDomain() const;
	
	void setBlendMode(BlendMode mode);
	BlendMode getBlendMode() const;

	void setLightingMode(LightingMode mode);
	LightingMode getLightingMode() const;

protected:
	Domain m_domain;
	BlendMode m_blendMode;
	LightingMode m_lightingMode;
};
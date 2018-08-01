#pragma once

#include "Material.h"
#include <Engine\Components\Math\types.h>
#include <Engine\Components\Graphics\RenderSystem\Texture.h>

class DefaultMaterial : public Material {
public:
	DefaultMaterial();
	virtual ~DefaultMaterial();

	vector3 getEmissiveColor() const;
	void setEmissiveColor(const vector3& color);

	vector3 getDiffuseColor() const;
	void setDiffuseColor(const vector3& color);

	vector3 getSpecularColor() const;
	void setSpecularColor(const vector3& color);

	float getSpecularFactor() const;
	void setSpecularFactor(float factor);

	Texture* getDiffuseTexture() const;
	void setDiffuseTexture(Texture* texture);

	Texture* getSpecularTexture() const;
	void setSpecularTexture(Texture* texture);

	Texture* getNormalMap() const;
	void setNormalMap(Texture* normalMap);
protected:
	vector3 m_emissiveColor;

	vector3 m_diffuseColor;
	Texture* m_diffuseTexture;

	vector3 m_specularColor;
	Texture* m_specularTexture;
	float m_specularFactor;

	Texture* m_normalMap;
};
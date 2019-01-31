#pragma once

#include <Engine/Components/Graphics/RenderLayout/Materials/Material.h>

class PBRMaterial : public Material {
public:
	PBRMaterial();
	virtual ~PBRMaterial();

	void setColor(Texture* color);
	void setColor(vector3 color);

	bool hasColorMap() const;
	Texture* getColorMap() const;
	vector3 getColorValue() const;

	void setMetallic(Texture* metallic);
	void setMetallic(float metallic);

	bool hasMetallicMap() const;
	Texture* getMetallicMap() const;
	float getMetallicValue() const;

	void setRoughness(Texture* roughness);
	void setRoughness(float roughness);

	bool hasRoughnessMap() const;
	Texture* getRoughnessMap() const;
	float getRoughnessValue() const;

	void setAO(Texture* AO);
	void setAO(float AO);

	bool hasAOMap() const;
	Texture* getAOMap() const;
	float getAOValue() const;

	void setNormal(Texture* normal);

	bool hasNormalMap() const;
	Texture* getNormalMap() const;

protected:
	Texture* m_colorTexture;
	vector3 m_colorValue;

	Texture* m_metallicTexture;
	float m_metallicValue;

	Texture* m_roughnessTexture;
	float m_roughnessValue;

	Texture* m_AOTexture;
	float m_AOValue;

	Texture* m_normalTexture;
};
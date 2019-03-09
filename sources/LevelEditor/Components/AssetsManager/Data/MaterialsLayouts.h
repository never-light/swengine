#pragma once

#include <QMap>

enum class MaterialLayoutType {
	PBRMaterial
};

class MaterialLayout : public XMLSerializable {
public:
	MaterialLayout(QtVariantPropertyManager* propertyManager) 
		: m_propertyManager(propertyManager) 
	{
		m_properties = m_propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(),
			QLatin1String("Material data"));
	}

	virtual ~MaterialLayout() {

	}

	QtVariantProperty* getProperty(const QString& propertyName) const { 
		return m_rawPropertiesMap[propertyName]; 
	}

	void setPropertyValue(const QString& propertyName, const QVariant& propertyValue) {
		m_rawPropertiesMap[propertyName]->setValue(propertyValue);
	}

	QtProperty* getPropertiesSection() const {
		return m_properties;
	}

	QVector<QtVariantProperty*> getPropertiesList() const {
		return m_rawPropertiesMap.values().toVector();
	}

public:
	virtual void serialize(pugi::xml_node& storage) const override {

	}

	virtual void deserialize(const pugi::xml_node& storage) override {

	}

protected:
	QtVariantProperty* registerProperty(const QString& name, QVariant::Type type, const QVariant& defaultValue, bool enabled = true) {
		QtVariantProperty* newProperty = m_propertyManager->addProperty(type, name);
		newProperty->setValue(defaultValue);
		newProperty->setEnabled(enabled);

		m_rawPropertiesMap.insert(name, newProperty);
		m_properties->addSubProperty(newProperty);

		return newProperty;
	}

	QtVariantProperty* registerTextureProperty(const QString& name, bool enabled = true) {
		return registerProperty(name, QVariant::Type::Int, -1, enabled);
	}

private:
	QtProperty* m_properties;
	QMap<QString, QtVariantProperty*> m_rawPropertiesMap;

	QtVariantPropertyManager* m_propertyManager;
};

class PBRMaterialLayout : public MaterialLayout {
public:
	
	PBRMaterialLayout(QtVariantPropertyManager* propertyManager) 
		: MaterialLayout(propertyManager) 
	{
		registerProperty("Albedo_color", QVariant::Color, QColor(255, 255, 255));
		registerTextureProperty("Albedo_map");

		auto metallicFactorProperty = registerProperty("Metallic_factor", QVariant::Double, 0.0);
		metallicFactorProperty->setAttribute(QLatin1String("minimum"), 0.0);
		metallicFactorProperty->setAttribute(QLatin1String("maximum"), 1.0);

		registerTextureProperty("Metallic_map");

		auto roughnessFactorProperty = registerProperty("Roughness_factor", QVariant::Double, 0.0);
		roughnessFactorProperty->setAttribute(QLatin1String("minimum"), 0.0);
		roughnessFactorProperty->setAttribute(QLatin1String("maximum"), 1.0);

		registerTextureProperty("Roughness_map");

		auto AOFactorProperty = registerProperty("AO_factor", QVariant::Double, 0.0);
		AOFactorProperty->setAttribute(QLatin1String("minimum"), 0.0);
		AOFactorProperty->setAttribute(QLatin1String("maximum"), 1.0);

		registerTextureProperty("AO_map");

		registerTextureProperty("Normal_map");
	}

	virtual ~PBRMaterialLayout() {

	}

public:
	virtual void serialize(pugi::xml_node& storage) const override {
		QColor albedoColor = getProperty("Albedo_color")->value().value<QColor>();
		qint32 albedoMapId = getProperty("Albedo_map")->value().toInt();

		float metallicFactor = getProperty("Metallic_factor")->value().toFloat();
		qint32 metallicMapId = getProperty("Metallic_map")->value().toInt();

		float roughnessFactor = getProperty("Roughness_factor")->value().toFloat();
		qint32 roughnessMapId = getProperty("Roughness_map")->value().toInt();

		float aoFactor = getProperty("AO_factor")->value().toFloat();
		qint32 aoMapId = getProperty("AO_map")->value().toInt();
		
		qint32 normalMapId = getProperty("Normal_map")->value().toInt();
		
		pugi::xml_node albedoNode = storage.append_child("albedo");
		albedoNode.append_attribute("r").set_value(albedoColor.red());
		albedoNode.append_attribute("g").set_value(albedoColor.green());
		albedoNode.append_attribute("b").set_value(albedoColor.blue());
		albedoNode.append_attribute("a").set_value(albedoColor.alpha());
		albedoNode.append_attribute("map").set_value(albedoMapId);

		pugi::xml_node metallicNode = storage.append_child("metallic");
		metallicNode.append_attribute("factor").set_value(metallicFactor);
		metallicNode.append_attribute("map").set_value(metallicMapId);

		pugi::xml_node roughnessNode = storage.append_child("roughness");
		roughnessNode.append_attribute("factor").set_value(roughnessFactor);
		roughnessNode.append_attribute("map").set_value(roughnessMapId);

		pugi::xml_node aoNode = storage.append_child("ao");
		aoNode.append_attribute("factor").set_value(aoFactor);
		aoNode.append_attribute("map").set_value(aoMapId);

		pugi::xml_node normalNode = storage.append_child("normal");
		normalNode.append_attribute("map").set_value(normalMapId);
	}

	virtual void deserialize(const pugi::xml_node& storage) override {
		// Albedo
		pugi::xml_node albedoNode = storage.child("albedo");

		QColor albedoColor;
		albedoColor.setRed(albedoNode.attribute("r").as_int());
		albedoColor.setGreen(albedoNode.attribute("g").as_int());
		albedoColor.setBlue(albedoNode.attribute("b").as_int());
		albedoColor.setAlpha(albedoNode.attribute("a").as_int());

		setPropertyValue("Albedo_color", albedoColor);
		setPropertyValue("Albedo_map", albedoNode.attribute("map").as_int());

		// Metallic
		pugi::xml_node metallicNode = storage.child("metallic");

		setPropertyValue("Metallic_factor", metallicNode.attribute("factor").as_float());
		setPropertyValue("Metallic_map", metallicNode.attribute("map").as_int());

		// Roughness
		pugi::xml_node roughnessNode = storage.child("roughness");

		setPropertyValue("Roughness_factor", roughnessNode.attribute("factor").as_float());
		setPropertyValue("Roughness_map", roughnessNode.attribute("map").as_int());

		// AO
		pugi::xml_node aoNode = storage.child("ao");

		setPropertyValue("AO_factor", aoNode.attribute("factor").as_float());
		setPropertyValue("AO_map", aoNode.attribute("map").as_int());

		// Normal
		pugi::xml_node normalNode = storage.child("normal");

		setPropertyValue("Normal_map", normalNode.attribute("map").as_int());
	}
};
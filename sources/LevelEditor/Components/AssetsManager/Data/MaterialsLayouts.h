#pragma once

#include <QMap>

class MaterialLayout {
public:
	MaterialLayout(QtVariantPropertyManager* propertyManager) 
		: m_propertyManager(propertyManager) 
	{
		QtVariantProperty* domainProperty = m_propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), QLatin1String("Domain"));
		QStringList enumNames;

		enumNames << "Surface" << "Post processing" << "UI";
		domainProperty->setAttribute(QLatin1String("enumNames"), enumNames);
		domainProperty->setValue(1);

		QtVariantProperty* processingStage = m_propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(),
			QLatin1String("Processing pass"));
		QStringList processingPassTypes;

		processingPassTypes << "Deferred" << "Forward" << "Transparency";
		processingStage->setAttribute(QLatin1String("enumNames"), processingPassTypes);
		processingStage->setValue(1);

		m_properties = m_propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(),
			QLatin1String("Material data"));

		m_properties->addSubProperty(domainProperty);
		m_properties->addSubProperty(processingStage);

		m_rawPropertiesMap.insert("domain", domainProperty);
		m_rawPropertiesMap.insert("stage", processingStage);
	}

	virtual ~MaterialLayout() {

	}

	QtVariantProperty* getProperty(const QString& propertyName) { return m_rawPropertiesMap[propertyName]; }

	void setPropertyValue(const QString& propertyName, const QVariant& propertyValue) {
		m_rawPropertiesMap[propertyName]->setValue(propertyValue);
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

		registerTextureProperty("Normals_map");
	}

	virtual ~PBRMaterialLayout() {

	}
};
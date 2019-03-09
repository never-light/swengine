#include "MaterialAsset.h"

#include <QDebug>

MaterialAsset::MaterialAsset()
	: AssetBase(),
	m_materialLayout(nullptr)
{
	m_typeProperty->setValue("Material");

	QStringList materialTypes;
	materialTypes << "PBR";

	m_materialType = m_attributesStorage->addProperty(QtVariantPropertyManager::enumTypeId(), QLatin1String("Scheme"));
	m_materialType->setAttribute(QLatin1String("enumNames"), materialTypes);
	m_materialType->setValue(0);

	QStringList processingPassTypes;
	processingPassTypes << "Deferred" << "Forward" << "Transparency";

	m_processingStage = m_attributesStorage->addProperty(QtVariantPropertyManager::enumTypeId(), QLatin1String("Processing stage"));
	m_processingStage->setAttribute(QLatin1String("enumNames"), processingPassTypes);
	m_processingStage->setValue(0);

	m_commonProperties = m_attributesStorage->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Material type"));
	m_commonProperties->addSubProperty(m_processingStage);
	m_commonProperties->addSubProperty(m_materialType);

	connect(m_attributesStorage, SIGNAL(valueChanged(QtProperty*, const QVariant&)), this, 
		SLOT(propertyValueChanged(QtProperty *, const QVariant&)));

	setMaterialLayout(new PBRMaterialLayout(m_attributesStorage));
}

MaterialAsset::~MaterialAsset()
{
}

QVector<QtProperty*> MaterialAsset::getEditableProperties() const
{
	auto properties = AssetBase::getEditableProperties();
	properties.push_back(m_commonProperties);

	if (m_materialLayout != nullptr)
		properties.push_back(m_materialLayout->getPropertiesSection());

	return properties;
}

void MaterialAsset::setMaterialType(MaterialLayoutType type)
{
	m_materialType->setValue((int)type);
}

MaterialLayoutType MaterialAsset::getMaterialType() const
{
	return (MaterialLayoutType)m_materialType->value().toInt();
}

void MaterialAsset::setProcessingStage(MaterialProcessingStage stage)
{
	m_processingStage->setValue((int)stage);
}

MaterialProcessingStage MaterialAsset::getProcessingStage() const
{
	return (MaterialProcessingStage)m_processingStage->value().toInt();
}

void MaterialAsset::setMaterialLayout(MaterialLayout * layout)
{
	if (m_materialLayout != nullptr)
		delete m_materialLayout;

	m_materialLayout = layout;
}

MaterialLayout * MaterialAsset::getMaterialLayout() const
{
	return m_materialLayout;
}

void MaterialAsset::serialize(pugi::xml_node& storage) const
{
	AssetBase::serialize(storage);

	storage.append_attribute("layout").set_value((int)getMaterialType());
	storage.append_attribute("stage").set_value((int)getProcessingStage());

	m_materialLayout->serialize(storage.append_child("parameters"));
}

void MaterialAsset::deserialize(const pugi::xml_node& storage)
{
	AssetBase::deserialize(storage);

	setMaterialType((MaterialLayoutType)storage.attribute("layout").as_int());
	setProcessingStage((MaterialProcessingStage)storage.attribute("stage").as_int());

	MaterialLayout* materialLayout;

	MaterialLayoutType layoutType = (MaterialLayoutType)getMaterialType();

	switch (layoutType) {
	case MaterialLayoutType::PBRMaterial:
		materialLayout = new PBRMaterialLayout(m_attributesStorage);
	}

	materialLayout->deserialize(storage.child("parameters"));
	setMaterialLayout(materialLayout);
}

void MaterialAsset::propertyValueChanged(QtProperty * property, const QVariant & value)
{
	if (property == m_materialType) {
		MaterialLayoutType layoutType = (MaterialLayoutType)(value.toInt());

		if (layoutType == MaterialLayoutType::PBRMaterial) {
			setMaterialLayout(new PBRMaterialLayout(m_attributesStorage));
		}
	}
}

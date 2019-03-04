#include "MaterialAsset.h"

MaterialAsset::MaterialAsset()
	: AssetBase(),
	m_materialLayout(nullptr)
{
}

MaterialAsset::~MaterialAsset()
{
}

QMap<QString, QVariant> MaterialAsset::getAttibutesRaw() const
{
	auto attributes = AssetBase::getAttibutesRaw();


	return attributes;
}

QVector<QtProperty*> MaterialAsset::getEditableProperties() const
{
	auto properties = AssetBase::getEditableProperties();
	properties.push_back(m_properties);

	return properties;
}

void MaterialAsset::replaceMaterialLayout(MaterialLayout * layout)
{
	if (m_materialLayout != nullptr)
		delete m_materialLayout;

	m_materialLayout = layout;
}

MaterialLayout * MaterialAsset::getMaterialLayout() const
{
	return m_materialLayout;
}

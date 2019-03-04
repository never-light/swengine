#include "AssetBase.h"

int32_t AssetBase::s_freeId = 0;

AssetBase::AssetBase()
	: m_category(nullptr), m_id(-1), m_attributesStorage(new QtVariantPropertyManager())
{
	m_typeProperty = m_attributesStorage->addProperty(QVariant::String, "Type");
	m_typeProperty->setValue("Unknown");
	m_typeProperty->setEnabled(false);

	m_commonProperties = m_attributesStorage->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Main"));
	m_commonProperties->addSubProperty(m_typeProperty);
}

AssetBase::~AssetBase()
{
	delete m_attributesStorage;
}

int32_t AssetBase::getId() const
{
	return m_id;
}

void AssetBase::setName(const QString & name)
{
	m_name = name;
}

QString AssetBase::getName() const
{
	return m_name;
}

void AssetBase::setCategory(AssetCategory * category)
{
	m_category = category;
}

AssetCategory * AssetBase::getCategory() const
{
	return m_category;
}

QtVariantPropertyManager* AssetBase::getAttributesStorage() const
{
	return m_attributesStorage;
}

QVector<QtProperty*> AssetBase::getEditableProperties() const
{
	return { m_commonProperties };
}

QMap<QString, QVariant> AssetBase::getAttibutesRaw() const
{
	return { };
}

void AssetBase::performDelete()
{
}

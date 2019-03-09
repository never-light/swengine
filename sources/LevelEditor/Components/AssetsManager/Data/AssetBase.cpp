#include "AssetBase.h"
#include "AssetCategory.h"

AssetBase::AssetBase()
	: m_category(nullptr), 
	m_categoryId(-1),
	m_id(-1),
	m_attributesStorage(new QtVariantPropertyManager())
{
	m_typeProperty = m_attributesStorage->addProperty(QVariant::String, "Type");
	m_typeProperty->setValue("Unknown");
	m_typeProperty->setEnabled(false);

	m_idProperty = m_attributesStorage->addProperty(QVariant::Int, "Id");
	m_idProperty->setValue(0);
	m_idProperty->setEnabled(false);

	m_commonProperties = m_attributesStorage->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Main"));
	m_commonProperties->addSubProperty(m_typeProperty);
	m_commonProperties->addSubProperty(m_idProperty);
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

void AssetBase::performDelete()
{
}

void AssetBase::setId(int32_t id)
{
	m_id = id;
	m_idProperty->setValue(id);
}

void AssetBase::serialize(pugi::xml_node& storage) const
{
	storage.append_attribute("id").set_value(m_id);
	storage.append_attribute("name").set_value(m_name.toStdString().c_str());
	storage.append_attribute("cid").set_value(m_categoryId);
}

void AssetBase::deserialize(const pugi::xml_node& storage)
{
	setId(storage.attribute("id").as_int());
	m_categoryId = storage.attribute("cid").as_int();
	m_name = storage.attribute("name").as_string();
}

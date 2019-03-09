#include "AssetCategory.h"

AssetCategory::AssetCategory()
	: m_id(-1),
	m_parentId(-1),
	m_parent(nullptr)
{

}

AssetCategory::~AssetCategory()
{
	for (AssetBase* asset : m_assets)
		delete asset;

	for (AssetCategory* childCategory : m_children) {
		delete childCategory;
	}
}

int32_t AssetCategory::getId() const {
	return m_id;
}

bool AssetCategory::isRoot() const
{
	return m_parent == nullptr;
}


int32_t AssetCategory::getParentId() const
{
	return m_parentId;
}

QString AssetCategory::getName() const
{
	return m_name;
}

void AssetCategory::setName(const QString & name)
{
	m_name = name;
}

QVector<AssetCategory*> AssetCategory::getChildCategories() const
{
	return m_children;
}

size_t AssetCategory::getChildrenCount() const
{
	return m_children.size();
}

QString AssetCategory::getPath() const
{
	if (isRoot())
		return getName();
	
	return m_parent->getPath() + "/" + getName();
}

QVector<AssetBase*> AssetCategory::getAssets() const
{
	return m_assets;
}

AssetCategory * AssetCategory::getParent() const
{
	return m_parent;
}

void AssetCategory::serialize(pugi::xml_node& storage) const
{
	storage.append_attribute("id").set_value(m_id);
	storage.append_attribute("name").set_value(m_name.toStdString().c_str());
	storage.append_attribute("pid").set_value(m_parentId);
}

void AssetCategory::deserialize(const pugi::xml_node& storage)
{
	m_id = storage.attribute("id").as_int();
	m_parentId = storage.attribute("pid").as_int();
	m_name = storage.attribute("name").as_string();
}
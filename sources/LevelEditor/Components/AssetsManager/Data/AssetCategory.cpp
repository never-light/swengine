#include "AssetCategory.h"

AssetCategory::AssetCategory(int32_t id)
	: m_id(id),
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
	return (m_parent == nullptr) ? -1 : m_parent->getId();
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

AssetCategory * AssetCategory::addChildCategory(const QString & name)
{
	AssetCategory* childCategory = new AssetCategory(getFreeIdAndIncrease());

	childCategory->setName(name);
	childCategory->m_parent = this;

	m_children.push_back(childCategory);

	return childCategory;
}

void AssetCategory::removeChildren()
{
	for (AssetCategory* child : m_children) {
		child->clearAssets();
		child->removeChildren();

		delete child;
	}

	m_children.clear();
}

void AssetCategory::removeChildCategory(AssetCategory * category)
{
	assert(category->m_parent == this);

	category->clearAssets();
	category->removeChildren();

	m_children.removeOne(category);
	delete category;
}

QString AssetCategory::getPath() const
{
	if (isRoot())
		return getName();
	
	return m_parent->getPath() + "/" + getName();
}

void AssetCategory::clearAssets()
{
	for (AssetBase* asset : m_assets) {
		asset->performDelete();
		delete asset;
	}

	m_assets.clear();
}

QVector<AssetBase*> AssetCategory::getAssets() const
{
	return m_assets;
}

void AssetCategory::addAsset(AssetBase * asset)
{
	assert(asset != nullptr && asset->getCategory() == nullptr);

	asset->setCategory(this);
	asset->m_id = AssetBase::getFreeIdAndIncrease();

	m_assets.push_back(asset);
}

void AssetCategory::removeAsset(AssetBase* asset)
{
	assert(asset->getCategory() == this);

	m_assets.removeOne(asset);

	asset->setCategory(nullptr);
}

AssetCategory * AssetCategory::getParent() const
{
	return m_parent;
}

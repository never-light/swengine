#include "AssetsCategoriesTreeItem.h"

AssetsCategoriesTreeItem::AssetsCategoriesTreeItem(AssetCategory* category)
	: QStandardItem(category->getName()),
	m_categoryDesc(category)
{

}

AssetCategory* AssetsCategoriesTreeItem::getAttachedCategory() const {
	return m_categoryDesc;
}

size_t AssetsCategoriesTreeItem::getChildrenCount() const
{
	return m_categoryDesc->getChildrenCount();
}

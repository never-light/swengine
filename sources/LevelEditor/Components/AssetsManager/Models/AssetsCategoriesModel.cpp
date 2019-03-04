#include "AssetsCategoriesModel.h"

AssetsCategoriesModel::AssetsCategoriesModel(AssetCategory* rootCategory, QObject * parent)
	: QStandardItemModel(parent),
	m_rootCategory(rootCategory)
{
	m_rootItem = new AssetsCategoriesTreeItem(m_rootCategory);
	processCategoriesTree(m_rootCategory, m_rootItem);

	appendRow(m_rootItem);
}

AssetsCategoriesModel::~AssetsCategoriesModel()
{
	delete m_rootItem;
}
AssetsCategoriesTreeItem * AssetsCategoriesModel::getRootItem() const
{
	return m_rootItem;
}

void AssetsCategoriesModel::processCategoriesTree(AssetCategory * parentCategory, AssetsCategoriesTreeItem * parentTreeItem)
{
	for (AssetCategory* childCategory : parentCategory->getChildCategories()) {
		AssetsCategoriesTreeItem* childIem = new AssetsCategoriesTreeItem(childCategory);

		parentTreeItem->appendRow(childIem);
		processCategoriesTree(childCategory, childIem);
	}
}

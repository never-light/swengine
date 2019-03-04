#pragma once

#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <pugixml.hpp>

#include "AssetsCategoriesTreeItem.h"

class AssetsCategoriesModel : public QStandardItemModel
{
	Q_OBJECT

public:
	AssetsCategoriesModel(AssetCategory* rootCategory,
		QObject *parent = 0);

	virtual ~AssetsCategoriesModel();

	AssetsCategoriesTreeItem* getRootItem() const;

private:
	void processCategoriesTree(AssetCategory* parentCategory, AssetsCategoriesTreeItem* parentTreeItem);

private:
	AssetCategory* m_rootCategory;

	AssetsCategoriesTreeItem* m_rootItem;
};

#pragma once

#include <QtGui/qstandarditemmodel.h>
#include "../AssetsDataBase.h"

class AssetsCategoriesTreeItem : public QStandardItem {
public:
	AssetsCategoriesTreeItem(AssetCategory* category);

	AssetCategory* getAttachedCategory() const;

	size_t getChildrenCount() const;

protected:
	AssetCategory* m_categoryDesc;
};
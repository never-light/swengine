#pragma once

#include <QListWidgetItem>
#include "../AssetsDataBase.h"

class AssetsAssetListItem : public QListWidgetItem {
public:
	AssetsAssetListItem(AssetBase* asset);
	~AssetsAssetListItem();

	AssetBase* getAttachedAsset() const;

private:
	AssetBase* m_assetDesc;
};
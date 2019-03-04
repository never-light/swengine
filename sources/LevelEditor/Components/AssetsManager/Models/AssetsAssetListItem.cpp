#include "AssetsAssetListItem.h"

AssetsAssetListItem::AssetsAssetListItem(AssetBase* asset)
	: QListWidgetItem(asset->getName()),
	m_assetDesc(asset)
{

}

AssetsAssetListItem::~AssetsAssetListItem()
{
}

AssetBase* AssetsAssetListItem::getAttachedAsset() const
{
	return m_assetDesc;
}

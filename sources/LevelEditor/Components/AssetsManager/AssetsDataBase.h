#pragma once

#include <type_traits>

#include <QString>
#include <pugixml.hpp>

#include <QHash>
#include <QVector>
#include <QDebug>
#include <QFile>

#include "Data/AssetCategory.h"
#include "Data/AssetBase.h"

#include "Data/MeshAsset.h"
#include "Data/TextureAsset.h"
#include "Data/GpuProgramAsset.h"
#include "Data/MaterialAsset.h"
#include "Data/SoundAsset.h"

class AssetsDataBase final {
public:
	AssetsDataBase(const QString& categoriesPath, const QString& assetsPath);
	~AssetsDataBase();

	AssetCategory* getRootCategory() const;
	void save();

	void addCategory(AssetCategory* parent, AssetCategory* category);
	void removeCategory(AssetCategory* category);
	
	void addAsset(AssetCategory* category, AssetBase* asset);
	void removeAsset(AssetBase* asset);

	QVector<AssetBase*> getAssets() const;
	QVector<AssetBase*> getAssetsByCategory(AssetCategory* category) const;

	void relocateAsset(AssetBase* asset, AssetCategory* destCategory);
	void relocateCategory(AssetCategory* category, AssetCategory* destCategory);

	size_t getFreeCategoryId() const;
	size_t getFreeAssetId() const;

private:
	void removeAssetsFromCategory(AssetCategory* category);
	void removeCategoryChildren(AssetCategory* category);

private:
	void readAssetsData(const QHash<int32_t, AssetCategory*>& categories);

private:
	void saveCategories();
	void saveAssets();

private:
	QVector<AssetCategory*> m_categories;
	QVector<AssetBase*> m_assets;

	size_t m_freeCategoryId;
	size_t m_freeAssetId;

	QString m_categoriesPath;
	QString m_assetsPath;

	AssetCategory* m_rootCategory;
};
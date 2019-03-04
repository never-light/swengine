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

class AssetsDataBase {
public:
	AssetsDataBase(const QString& categoriesPath, const QString& assetsPath);
	~AssetsDataBase();

	AssetCategory* getRootCategory() const;

	void save();

private:
	void readAssetsData(const QHash<int32_t, AssetCategory*>& categories);

	AssetBase* readMeshAssetData(const pugi::xml_node& node);
	AssetBase* readTextureAssetData(const pugi::xml_node& node);
	AssetBase* readGpuProgramAssetData(const pugi::xml_node& node);

private:
	void saveCategories();
	void saveAssets();

	void buildCategoriesListNode(AssetCategory* category, pugi::xml_node& categoriesListNode);
	void buildAssetsListNode(AssetCategory* category, pugi::xml_node& assetsListNode);
private:
	QMap<QString, std::function<AssetBase*(const pugi::xml_node&)>> m_assetsLoaders;

	QString m_categoriesPath;
	QString m_assetsPath;

	AssetCategory* m_rootCategory;
};
#include "AssetsDataBase.h"

#include <QTextStream>
#include <QDebug>
#include <QDataStream>

AssetsDataBase::AssetsDataBase(const QString& categoriesPath, const QString& assetsPath)
	: m_categoriesPath(categoriesPath),
	m_assetsPath(assetsPath)
{
	pugi::xml_document categoriesDoc;

	if (!categoriesDoc.load_file(m_categoriesPath.toStdString().c_str())) {
		return;
	}

	pugi::xml_node categoriesListNode = categoriesDoc.first_child();
	m_freeCategoryId = categoriesListNode.attribute("frid").as_int();

	QHash<int32_t, AssetCategory*> categories;

	for (pugi::xml_node categoryNode : categoriesListNode.children("category")) {
		int categoryId = categoryNode.attribute("id").as_int();

		AssetCategory* category = new AssetCategory();
		category->deserialize(categoryNode);

		categories.insert(category->getId(), category);
		m_categories.push_back(category);
	}
	 
	// Tree structure pass
	for (AssetCategory* category : m_categories) {
		int32_t parentId = category->getParentId();
		int32_t id = category->getId();

		if (parentId != -1) {
			categories[parentId]->m_children.push_back(categories[id]);
		}

		category->m_parent = (parentId != -1) ? categories[parentId] : nullptr;

		if (parentId == -1)
			m_rootCategory = category;
	}

	// Assets pass
	readAssetsData(categories);
}

AssetsDataBase::~AssetsDataBase()
{
	save();

	delete getRootCategory();
}

AssetCategory * AssetsDataBase::getRootCategory() const
{
	return m_rootCategory;
}

void AssetsDataBase::save()
{
	saveCategories();
	saveAssets();
}

void AssetsDataBase::addCategory(AssetCategory * parent, AssetCategory * category)
{
	assert(category->getParent() == nullptr);
	category->m_parent = parent;
	category->m_parentId = parent->getId();

	if (category->m_id == -1) {
		category->m_id = m_freeCategoryId++;
	}

	parent->m_children.push_back(category);
	m_categories.push_back(category);
}

void AssetsDataBase::removeCategory(AssetCategory * category)
{
	AssetCategory* parent = category->getParent();
	assert(parent != nullptr);

	removeAssetsFromCategory(category);
	removeCategoryChildren(category);

	parent->m_children.removeOne(category);
	m_categories.removeOne(category);
	delete category;
}

void AssetsDataBase::addAsset(AssetCategory * category, AssetBase * asset)
{
	assert(asset != nullptr && asset->getCategory() == nullptr);

	asset->m_category = category;
	asset->m_categoryId = category->getId();

	if (asset->m_id == -1) {
		asset->setId(m_freeAssetId++);
	}

	category->m_assets.push_back(asset);
	m_assets.push_back(asset);
}

void AssetsDataBase::removeAsset(AssetBase * asset)
{
	asset->getCategory()->m_assets.removeOne(asset);
	asset->performDelete();

	m_assets.removeOne(asset);
	delete asset;
}

QVector<AssetBase*> AssetsDataBase::getAssets() const
{
	return m_assets;
}

QVector<AssetBase*> AssetsDataBase::getAssetsByCategory(AssetCategory * category) const
{
	return category->m_assets;
}

void AssetsDataBase::relocateAsset(AssetBase * asset, AssetCategory * destCategory)
{
	asset->m_category->m_assets.removeOne(asset);
	asset->m_category = destCategory;
	asset->m_categoryId = destCategory->getId();

	destCategory->m_assets.push_back(asset);
}

void AssetsDataBase::relocateCategory(AssetCategory * category, AssetCategory * destCategory)
{
	category->m_parent = destCategory;
	category->m_parentId = destCategory->getId();
}

size_t AssetsDataBase::getFreeCategoryId() const
{
	return m_freeCategoryId;
}

size_t AssetsDataBase::getFreeAssetId() const
{
	return m_freeAssetId;
}

void AssetsDataBase::removeAssetsFromCategory(AssetCategory * category)
{
	for (AssetBase* asset : category->m_assets) {
		asset->performDelete();

		m_assets.removeOne(asset);
		delete asset;
	}

	category->m_assets.clear();
}

void AssetsDataBase::removeCategoryChildren(AssetCategory * category)
{
	for (AssetCategory* child : category->m_children) {
		removeAssetsFromCategory(child);
		removeCategoryChildren(child);

		m_categories.removeOne(child);
		delete child;
	}

	category->m_children.clear();
}

void AssetsDataBase::readAssetsData(const QHash<int32_t, AssetCategory*>& categories)
{
	pugi::xml_document assetsDoc;

	if (!assetsDoc.load_file(m_assetsPath.toStdString().c_str())) {
		return;
	}

	m_freeAssetId = assetsDoc.first_child().attribute("frid").as_int();

	for (pugi::xml_node assetNode : assetsDoc.first_child().children("asset")) {
		QString assetType = assetNode.attribute("type").as_string();

		AssetBase* asset = nullptr;

		if (assetType == "mesh")
			asset = new MeshAsset();
		else if (assetType == "material")
			asset = new MaterialAsset();
		else if (assetType == "gpuprogram")
			asset = new GpuProgramAsset();
		else if (assetType == "texture")
			asset = new TextureAsset();		
		else if (assetType == "sound")
			asset = new SoundAsset();
		else
			throw std::exception();

		asset->deserialize(assetNode);

		asset->m_category = categories[asset->m_categoryId];
		asset->m_category->m_assets.push_back(asset);
		
		m_assets.push_back(asset);
	}
}

void AssetsDataBase::saveCategories()
{
	pugi::xml_document categories;
	pugi::xml_node categoriesList = categories.append_child("categories");
	categoriesList.append_attribute("frid").set_value(m_freeCategoryId);

	for (AssetCategory* category : m_categories) {
		category->serialize(categoriesList.append_child("category"));
	}

	categories.save_file(m_categoriesPath.toStdString().c_str());
}

void AssetsDataBase::saveAssets()
{
	pugi::xml_document assets;
	pugi::xml_node assetsList = assets.append_child("assets");
	assetsList.append_attribute("frid").set_value(m_freeAssetId);

	for (AssetBase* asset : m_assets) {
		QString assetType = "unknown";

		if (dynamic_cast<MeshAsset*>(asset) != nullptr)
			assetType = "mesh";
		else if (dynamic_cast<TextureAsset*>(asset) != nullptr)
			assetType = "texture";
		else if (dynamic_cast<GpuProgramAsset*>(asset) != nullptr)
			assetType = "gpuprogram";
		else if (dynamic_cast<MaterialAsset*>(asset) != nullptr)
			assetType = "material";		
		else if (dynamic_cast<SoundAsset*>(asset) != nullptr)
			assetType = "sound";
		else
			throw std::exception();

		pugi::xml_node assetNode = assetsList.append_child("asset");
		assetNode.append_attribute("type").set_value(assetType.toStdString().c_str());

		asset->serialize(assetNode);
	}

	assets.save_file(m_assetsPath.toStdString().c_str());
}
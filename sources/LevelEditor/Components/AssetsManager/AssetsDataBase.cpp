#include "AssetsDataBase.h"

#include <QTextStream>
#include <QDebug>

int AssetCategory::s_freeId = 0;

AssetsDataBase::AssetsDataBase(const QString& categoriesPath, const QString& assetsPath)
	: m_categoriesPath(categoriesPath),
	m_assetsPath(assetsPath)
{
	m_assetsLoaders.insert("mesh", 
		std::bind(&AssetsDataBase::readMeshAssetData, this, std::placeholders::_1));

	m_assetsLoaders.insert("texture",
		std::bind(&AssetsDataBase::readTextureAssetData, this, std::placeholders::_1));

	m_assetsLoaders.insert("gpuprogram",
		std::bind(&AssetsDataBase::readGpuProgramAssetData, this, std::placeholders::_1));

	pugi::xml_document categoriesDoc;

	if (!categoriesDoc.load_file(m_categoriesPath.toStdString().c_str())) {
		return;
	}

	pugi::xml_node categoriesListNode = categoriesDoc.first_child();
	AssetCategory::s_freeId = categoriesListNode.attribute("frid").as_int();

	QHash<int32_t, AssetCategory*> categories;

	for (pugi::xml_node categoryNode : categoriesListNode.children("category")) {
		int categoryId = categoryNode.attribute("id").as_int();

		AssetCategory* categoryDesc = new AssetCategory(categoryId);

		categoryDesc->setName(categoryNode.attribute("name").as_string());
		categories.insert(categoryDesc->getId(), categoryDesc);
	}

	// Tree structure pass
	for (pugi::xml_node categoryNode : categoriesListNode.children("category")) {
		int32_t parentId = categoryNode.attribute("pid").as_int();
		int32_t id = categoryNode.attribute("id").as_int();

		if (parentId != -1) {
			categories[parentId]->m_children.push_back(categories[id]);
		}

		categories[id]->m_parent = (parentId != -1) ? categories[parentId] : nullptr;

		if (parentId == -1)
			m_rootCategory = categories[id];
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
	//m_assetsDoc.save_file(m_assetsPath.toStdString().c_str());
}

void AssetsDataBase::readAssetsData(const QHash<int32_t, AssetCategory*>& categories)
{
	pugi::xml_document assetsDoc;

	if (!assetsDoc.load_file(m_assetsPath.toStdString().c_str())) {
		return;
	}

	AssetBase::s_freeId = assetsDoc.first_child().attribute("frid").as_int();

	for (pugi::xml_node assetNode : assetsDoc.first_child().children("asset")) {
		QString assetType = assetNode.attribute("type").as_string();

		if (!m_assetsLoaders.contains(assetType))
			throw std::exception("Wrong asset type");

		AssetBase* assetDesc = m_assetsLoaders[assetType](assetNode);

		assetDesc->m_id = assetNode.attribute("id").as_int();
		assetDesc->m_name = assetNode.attribute("name").value();

		AssetCategory* assetCategory = categories[assetNode.attribute("cid").as_int()];
		assetCategory->addAsset(assetDesc);
	}
}

AssetBase * AssetsDataBase::readMeshAssetData(const pugi::xml_node & node)
{
	QString fileName = node.child("file").child_value();
	MeshAsset* asset = new MeshAsset(fileName);

	asset->setVerticesCount(QString(node.child("vertices").child_value()).toInt());
	asset->setIndicesCount(QString(node.child("indices").child_value()).toInt());
	asset->setSubMeshesCount(QString(node.child("submeshes").child_value()).toInt());
	asset->setSize(QString(node.child("size").child_value()).toInt());

	return asset;
}

AssetBase * AssetsDataBase::readTextureAssetData(const pugi::xml_node & node)
{
	QString fileName = node.child("file").child_value();
	TextureAsset* asset = new TextureAsset(fileName);

	int width = node.child("size").attribute("width").as_int();
	int height = node.child("size").attribute("height").as_int();

	asset->setSize(width, height);
	asset->setFileSize(QString(node.child("filesize").child_value()).toInt());
	asset->useAsSRGB(QString(node.child("srgb").child_value()).toInt());

	return asset;
}

AssetBase * AssetsDataBase::readGpuProgramAssetData(const pugi::xml_node & node)
{
	QString fileName = node.child("file").child_value();
	GpuProgramAsset* asset = new GpuProgramAsset(fileName);

	return asset;
}

void AssetsDataBase::saveCategories()
{
	pugi::xml_document categoriesDoc;

	pugi::xml_node categoriesListNode = categoriesDoc.append_child("categories");
	categoriesListNode.append_attribute("frid").set_value(AssetCategory::getFreeId());

	buildCategoriesListNode(getRootCategory(), categoriesListNode);

	categoriesDoc.save_file(m_categoriesPath.toStdString().c_str());
}

void AssetsDataBase::saveAssets()
{
	pugi::xml_document assetsDoc;

	pugi::xml_node assetsListNode = assetsDoc.append_child("assets");
	assetsListNode.append_attribute("frid").set_value(AssetBase::getFreeId());

	buildAssetsListNode(getRootCategory(), assetsListNode);

	assetsDoc.save_file(m_assetsPath.toStdString().c_str());

}

void AssetsDataBase::buildCategoriesListNode(AssetCategory * category, pugi::xml_node & categoriesListNode)
{
	pugi::xml_node categoryNode = categoriesListNode.append_child("category");

	categoryNode.append_attribute("id").set_value(category->getId());
	categoryNode.append_attribute("pid").set_value(category->getParentId());
	categoryNode.append_attribute("name").set_value(category->getName().toStdString().c_str());

	for (AssetCategory* childCategory : category->getChildCategories())
		buildCategoriesListNode(childCategory, categoriesListNode);
}

void AssetsDataBase::buildAssetsListNode(AssetCategory * category, pugi::xml_node & assetsListNode)
{
	for (AssetBase* asset : category->getAssets()) {
		pugi::xml_node assetNode = assetsListNode.append_child("asset");

		QString assetType = "unknown";

		if (dynamic_cast<MeshAsset*>(asset) != nullptr)
			assetType = "mesh";
		else if (dynamic_cast<TextureAsset*>(asset) != nullptr)
			assetType = "texture";
		else if (dynamic_cast<GpuProgramAsset*>(asset) != nullptr)
			assetType = "gpuprogram";

		assetNode.append_attribute("id").set_value(asset->getId());
		assetNode.append_attribute("cid").set_value(asset->getCategory()->getId());
		assetNode.append_attribute("name").set_value(asset->getName().toStdString().c_str());
		assetNode.append_attribute("type").set_value(assetType.toStdString().c_str());

		auto attributes = asset->getAttibutesRaw().toStdMap();

		for (auto attributeIt : attributes) {
			QVariant attributeValue = attributeIt.second;

			pugi::xml_node attributeNode = assetNode.append_child(attributeIt.first.toStdString().c_str());

			switch (attributeValue.type()) {
			case QMetaType::QString:
				attributeNode.append_child(pugi::node_pcdata).set_value(attributeValue.toString().toStdString().c_str());
				break;

			case QMetaType::Int:
			case QMetaType::UInt:
			case QMetaType::ULongLong:
			case QMetaType::ULong:
			case QMetaType::Long:
			case QMetaType::LongLong:
			case QMetaType::Short:
			case QMetaType::UShort:
				attributeNode.append_child(pugi::node_pcdata).set_value(std::to_string(attributeValue.toInt()).c_str());
				break;

			case QMetaType::Float:
				attributeNode.append_child(pugi::node_pcdata).set_value(std::to_string(attributeValue.toFloat()).c_str());
				break;

			case QMetaType::Bool:
				attributeNode.append_child(pugi::node_pcdata).set_value(std::to_string((int)attributeValue.toBool()).c_str());
				break;

			case QMetaType::QSize: {
				QSize size = attributeValue.toSize();
				attributeNode.append_attribute("width").set_value(std::to_string(size.width()).c_str());
				attributeNode.append_attribute("height").set_value(std::to_string(size.height()).c_str());

				break;
			}

			default:
				qDebug() << attributeValue.type();
				throw new std::exception("Wrong attribut type");
			}
		}
	}

	for (AssetCategory* childCategory : category->getChildCategories())
		buildAssetsListNode(childCategory, assetsListNode);

}

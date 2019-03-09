#include "AssetsManagerWindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QMimeData>

#include <LevelEditor/Config/filesystem.h>

AssetsManagerWindow::AssetsManagerWindow(QWidget *parent, AssetsDataBase* assetsDataBase)
	: QMainWindow(parent), 
	m_assetsDB(assetsDataBase)
{
	m_assetsManagerUI.setupUi(this);

	AssetCategory* rootCategory = assetsDataBase->getRootCategory();


	m_assetsTreeModel = new AssetsCategoriesModel(m_assetsDB->getRootCategory());

	m_assetsManagerUI.categoriesTree->setModel(m_assetsTreeModel);

	//selectCategory(m_assetsTreeModel->getRootItem()->index());

	connect(m_assetsManagerUI.categoriesTree, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(assetsTreeContextMenuRequest(const QPoint &)));
	connect(m_assetsManagerUI.categoriesTree, SIGNAL(activated(const QModelIndex&)), this, SLOT(assetsTreeCategorySelected(const QModelIndex&)));

	m_categoriesTreeContextMenu = new QMenu(this);

	m_categoriesTreeContextMenu->addAction(tr("Add subcategory"), this, SLOT(addCategory()));
	m_categoriesTreeContextMenu->addSeparator();
	m_categoriesTreeContextMenu->addAction(tr("Delete"), this, SLOT(deleteCategory()));

	connect(m_assetsTreeModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(renameCategory(QStandardItem*)));
	connect(m_assetsManagerUI.MainMenu_Assets_Import, SIGNAL(triggered()), this, SLOT(importNewAsset()));
	connect(m_assetsManagerUI.MainMenu_Assets_Create_GpuProgram, SIGNAL(triggered()), this, SLOT(createGpuProgramAsset()));
	connect(m_assetsManagerUI.MainMenu_Assets_Create_Material, SIGNAL(triggered()), this, SLOT(createMaterialAsset()));
	connect(m_assetsManagerUI.MainMenu_Assets_Exit, SIGNAL(triggered()), this, SLOT(close()));

	m_assetsListContextMenu = new QMenu(this);
	m_assetsListContextMenu->addAction(tr("Delete"), this, SLOT(deleteAsset()));

	connect(m_assetsManagerUI.assetsList, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(assetsListContextMenuRequest(const QPoint &)));
	connect(m_assetsManagerUI.assetsList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(renameAsset(QListWidgetItem*)));
	connect(m_assetsManagerUI.assetsList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(assetItemSelected(QListWidgetItem*)));

	m_variantEditorFactory = new QtVariantEditorFactory();
	m_propertyBrowser = new QtTreePropertyBrowser(m_assetsManagerUI.propertiesEditorSection);
	m_propertyBrowser->setPropertiesWithoutValueMarked(true);
	m_propertyBrowser->setRootIsDecorated(false);

	m_assetsManagerUI.propertiesEditorSection->setWidget(m_propertyBrowser);

	m_propertyBrowser->show();

	initializeAssetsImporters();
}

AssetsManagerWindow::~AssetsManagerWindow()
{
	clearAssetImporters();

	delete m_variantEditorFactory;
	delete m_propertyBrowser;

	delete m_categoriesTreeContextMenu;
	delete m_assetsTreeModel;
}

void AssetsManagerWindow::selectCategory(const QModelIndex & index)
{
	AssetsCategoriesTreeItem* item = static_cast<AssetsCategoriesTreeItem*>(m_assetsTreeModel->itemFromIndex(index));

	m_assetsManagerUI.assetsList->clear();

	auto assets = item->getAttachedCategory()->getAssets();

	for (const auto& asset : assets) {
		AssetsAssetListItem* assetItem = new AssetsAssetListItem(asset);
		assetItem->setFlags(assetItem->flags() | Qt::ItemFlag::ItemIsEditable);

		m_assetsManagerUI.assetsList->addItem(assetItem);
	}
}

AssetCategory * AssetsManagerWindow::getSelectedCategory() const
{
	QModelIndex currentCategoryIndex = m_assetsManagerUI.categoriesTree->currentIndex();

	AssetsCategoriesTreeItem* currentCategoryItem =
		static_cast<AssetsCategoriesTreeItem*>(m_assetsTreeModel->itemFromIndex(currentCategoryIndex));

	return currentCategoryItem->getAttachedCategory();
}

QPlainTextEdit * AssetsManagerWindow::getLogWidget() const
{
	return m_assetsManagerUI.logTextArea;
}

void AssetsManagerWindow::assetsListContextMenuRequest(const QPoint& point)
{
	QModelIndex index = m_assetsManagerUI.assetsList->indexAt(point);

	if (index.isValid()) {
		QListWidgetItem* item = m_assetsManagerUI.assetsList->item(index.row());
		m_assetsListContextMenu->exec(m_assetsManagerUI.assetsList->viewport()->mapToGlobal(point));
	}
}

void AssetsManagerWindow::assetsTreeCategorySelected(const QModelIndex & index)
{
	if (index.isValid())
		selectCategory(index);
}

void AssetsManagerWindow::addCategory()
{
	QModelIndex currentCategoryIndex = m_assetsManagerUI.categoriesTree->currentIndex();

	AssetsCategoriesTreeItem* currentCategoryItem =
		static_cast<AssetsCategoriesTreeItem*>(m_assetsTreeModel->itemFromIndex(currentCategoryIndex));

	AssetCategory* newCategory = new AssetCategory();
	newCategory->setName("New category");

	m_assetsDB->addCategory(currentCategoryItem->getAttachedCategory(), newCategory);

	currentCategoryItem->appendRow(new AssetsCategoriesTreeItem(newCategory));
}

void AssetsManagerWindow::deleteCategory()
{
	QModelIndex currentCategoryIndex = m_assetsManagerUI.categoriesTree->currentIndex();

	AssetsCategoriesTreeItem* currentCategoryItem =
		static_cast<AssetsCategoriesTreeItem*>(m_assetsTreeModel->itemFromIndex(currentCategoryIndex));
	
	if (currentCategoryItem->getAttachedCategory() == m_assetsDB->getRootCategory())
		return;

	m_assetsDB->removeCategory(currentCategoryItem->getAttachedCategory());
	m_assetsTreeModel->removeRow(currentCategoryIndex.row(), currentCategoryIndex.parent());
}

void AssetsManagerWindow::renameCategory(QStandardItem * categoryItem)
{
	AssetsCategoriesTreeItem* item = static_cast<AssetsCategoriesTreeItem*>(categoryItem);
	item->getAttachedCategory()->setName(categoryItem->text());
}

void AssetsManagerWindow::importNewAsset()
{
	AssetCategory* currentCategory = getSelectedCategory();

	if (currentCategory == nullptr)
		return;

	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Import asset"), "", tr("Meshes (*.obj *.fbx *.dae);;Textures (*.jpg *.png *.tga *.gif);;Shaders (*.fx);;Sounds (*.wav)"));

	if (fileName.isEmpty() || !QFile::exists(fileName))
		return;

	importNewAssetFromFile(fileName);
}

void AssetsManagerWindow::importNewAssetFromFile(const QString & fileName)
{
	AssetCategory* currentCategory = getSelectedCategory();

	QFile assetFile(fileName);
	QFileInfo assetFileInfo(assetFile);

	QString extension = assetFileInfo.suffix();

	AssetImporter* importer = getAssetImporter(extension);

	QDir rootDir = QDir::current();
	rootDir.cdUp();

	QString categoryPath = currentCategory->getPath();
	QString targetDirPath = rootDir.path() + QString("/") + EDITOR_DATA_ASSETS_DIR + QString("/") + categoryPath;

	QString newExtension = importer->getExtension();

	if (newExtension.isEmpty())
		newExtension = extension;

	QString targetPath = targetDirPath + QString("/")
		+ assetFileInfo.baseName() + QString::number(m_assetsDB->getFreeAssetId()) + QString(".") + newExtension;

	QDir targetDir(targetDirPath);

	if (!targetDir.exists()) {
		QDir().mkpath(targetDirPath);
	}

	AssetBase* asset = importer->importFromFile(fileName, targetPath);

	if (asset == nullptr) {
		throw std::exception("Failed to load asset");
	}

	m_assetsDB->addAsset(currentCategory, asset);
	selectCategory(m_assetsManagerUI.categoriesTree->currentIndex());
}

void AssetsManagerWindow::createGpuProgramAsset()
{
	
}

void AssetsManagerWindow::createMaterialAsset()
{
	MaterialAsset* material = new MaterialAsset();
	material->setName("New material");

	m_assetsDB->addAsset(getSelectedCategory(), material);
}

void AssetsManagerWindow::deleteAsset()
{
	QModelIndex assetIndex = m_assetsManagerUI.assetsList->currentIndex();

	AssetsAssetListItem* assetItem =
		static_cast<AssetsAssetListItem*>(m_assetsManagerUI.assetsList->item(assetIndex.row()));

	AssetCategory* category = getSelectedCategory();
	AssetBase* asset = assetItem->getAttachedAsset();

	assert(category == asset->getCategory());

	m_assetsManagerUI.assetsList->takeItem(assetIndex.row());
	m_assetsDB->removeAsset(asset);
}

void AssetsManagerWindow::renameAsset(QListWidgetItem * item)
{
	AssetsAssetListItem* assetItem = static_cast<AssetsAssetListItem*>(item);
	assetItem->getAttachedAsset()->setName(item->text());
}

void AssetsManagerWindow::assetItemSelected(QListWidgetItem * item)
{
	AssetsAssetListItem* assetItem = static_cast<AssetsAssetListItem*>(item);
	AssetBase* asset = assetItem->getAttachedAsset();

	m_propertyBrowser->clear();
	m_propertyBrowser->setFactoryForManager(asset->getAttributesStorage(), m_variantEditorFactory);
	
	for (QtProperty* prop : asset->getEditableProperties())
		m_propertyBrowser->addProperty(prop);
}

void AssetsManagerWindow::dragEnterEvent(QDragEnterEvent * e)
{
	if (e->mimeData()->hasUrls() && e->mimeData()->urls().size() == 1) {
		e->acceptProposedAction();
	}
}

void AssetsManagerWindow::dropEvent(QDropEvent * e)
{
	QUrl url = e->mimeData()->urls().first();
	QString fileName = url.toLocalFile();

	importNewAssetFromFile(fileName);
}

void AssetsManagerWindow::closeEvent(QCloseEvent * event)
{
	m_assetsDB->save();
}

void AssetsManagerWindow::initializeAssetsImporters()
{
	m_meshAssetImporter = new MeshAssetImporter(this);
	registerAssetsImporter({ "obj", "dae", "fbx" }, m_meshAssetImporter);

	m_textureImporter = new TextureImporter(this);
	registerAssetsImporter({ "jpg", "png", "gif", "tga" }, m_textureImporter);

	m_gpuProgramImporter = new GpuProgramImporter(this);
	registerAssetsImporter({ "fx" }, m_gpuProgramImporter);

	m_soundImporter = new SoundImporter(this);
	registerAssetsImporter({ "wav" }, m_soundImporter);
}

void AssetsManagerWindow::registerAssetImporter(const QString & extensions, AssetImporter * importer)
{
	m_assetImporters.insert(extensions, importer);
}

void AssetsManagerWindow::registerAssetsImporter(const QVector<QString>& extensions, AssetImporter * importer)
{
	for (const QString& extension : extensions)
		registerAssetImporter(extension, importer);
}

AssetImporter* AssetsManagerWindow::getAssetImporter(const QString & extension) const
{
	return m_assetImporters[extension];
}

void AssetsManagerWindow::clearAssetImporters()
{
	delete m_gpuProgramImporter;
	delete m_meshAssetImporter;
	delete m_textureImporter;
}

void AssetsManagerWindow::assetsTreeContextMenuRequest(const QPoint & point)
{
	QModelIndex index = m_assetsManagerUI.categoriesTree->indexAt(point);

	if (index.isValid()) {
		QStandardItem* item = m_assetsTreeModel->itemFromIndex(index);
		m_categoriesTreeContextMenu->exec(m_assetsManagerUI.categoriesTree->viewport()->mapToGlobal(point));
	}
}
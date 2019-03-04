#pragma once

#include <QMenu>
#include <QtTreePropertyBrowser>

#include "ui_AssetsManager.h"
#include "../AssetsDataBase.h"
#include "../Models/AssetsCategoriesModel.h"
#include "../Models/AssetsAssetListItem.h"

#include "../Data/AssetImporter.h"
#include "../Data/Import/MeshImporter.h"
#include "../Data/Import/TextureImporter.h"
#include "../Data/Import/GpuProgramImporter.h"

class AssetsManagerWindow : public QMainWindow
{
	Q_OBJECT

public:
	AssetsManagerWindow(QWidget *parent, AssetsDataBase* assetsDataBase);
	~AssetsManagerWindow();

	void selectCategory(const QModelIndex& index);
	AssetCategory* getSelectedCategory() const;

	QPlainTextEdit* getLogWidget() const;

public slots:
	void assetsTreeContextMenuRequest(const QPoint& point);
	void assetsListContextMenuRequest(const QPoint& point);

	void assetsTreeCategorySelected(const QModelIndex& index);

	void addCategory();
	void deleteCategory();
	void renameCategory(QStandardItem* categoryItem);

	void importNewAsset();
	void importNewAssetFromFile(const QString& fileName);

	void createGpuProgramAsset();

	void deleteAsset();
	void renameAsset(QListWidgetItem * item);
	void assetItemSelected(QListWidgetItem* item);

	virtual void dragEnterEvent(QDragEnterEvent *e) override;
	virtual void dropEvent(QDropEvent *e) override;

	virtual void closeEvent(QCloseEvent *event) override;

private:
	void initializeAssetsImporters();
	void clearAssetImporters();

	void registerAssetImporter(const QString& extension, AssetImporter* importer);
	void registerAssetsImporter(const QVector<QString>& extensions, AssetImporter* importer);

	AssetImporter* getAssetImporter(const QString& extension) const;

private:
	QtTreePropertyBrowser* m_propertyBrowser;
	QtVariantEditorFactory* m_variantEditorFactory;

private:
	Ui::AssetsManager m_assetsManagerUI;
	AssetsCategoriesModel * m_assetsTreeModel;

	QMenu* m_categoriesTreeContextMenu;
	QMenu* m_assetsListContextMenu;

private:
	AssetsDataBase* m_assetsDB;
	QMap<QString, AssetImporter*> m_assetImporters;

	MeshAssetImporter* m_meshAssetImporter;
	TextureImporter* m_textureImporter;
	GpuProgramImporter* m_gpuProgramImporter;
};
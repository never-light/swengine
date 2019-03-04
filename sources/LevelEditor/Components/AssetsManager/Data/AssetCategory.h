#pragma once

#include <QString>

#include "AssetBase.h"

class AssetDataBase;

class AssetCategory {
	friend class AssetsDataBase;

public:
	AssetCategory(int32_t id);
	~AssetCategory();

	int32_t getParentId() const;
	AssetCategory* getParent() const;

	int32_t getId() const;

	bool isRoot() const;

	QString getName() const;
	void setName(const QString& name);

	QVector<AssetCategory*> getChildCategories() const;
	size_t getChildrenCount() const;

	AssetCategory* addChildCategory(const QString& name);

	void removeChildren();
	void removeChildCategory(AssetCategory* category);

	QString getPath() const;

	QVector<AssetBase*> getAssets() const;
	
	void addAsset(AssetBase* asset);
	void removeAsset(AssetBase* asset);

	void clearAssets();


	static int32_t getFreeId() { return s_freeId; }

private:
	AssetCategory(const AssetCategory& category);

private:
	static void increaseFreeId() { s_freeId++; }
	static int32_t getFreeIdAndIncrease() { return s_freeId++; }

private:
	static int32_t s_freeId;


private:
	QVector<AssetCategory*> m_children;
	AssetCategory* m_parent;

	QVector<AssetBase*> m_assets;

	int32_t m_id;
	QString m_name;
};

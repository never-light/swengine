#pragma once

#include <QString>
#include <QDataStream>
#include <LevelEditor/Core/XMLSerializable.h>

#include "AssetBase.h"

class AssetDataBase;

class AssetCategory : public XMLSerializable {
	friend class AssetsDataBase;

public:
	AssetCategory();
	~AssetCategory();

	int32_t getParentId() const;
	AssetCategory* getParent() const;

	int32_t getId() const;

	bool isRoot() const;

	QString getName() const;
	void setName(const QString& name);

	QVector<AssetCategory*> getChildCategories() const;
	size_t getChildrenCount() const;

	QString getPath() const;

	QVector<AssetBase*> getAssets() const;

public:
	virtual void serialize(pugi::xml_node& storage) const override;
	virtual void deserialize(const pugi::xml_node& storage) override;

private:
	AssetCategory(const AssetCategory& category);

private:
	QVector<AssetCategory*> m_children;
	AssetCategory* m_parent;

	QVector<AssetBase*> m_assets;

	int32_t m_id;
	int32_t m_parentId;
	QString m_name;
};
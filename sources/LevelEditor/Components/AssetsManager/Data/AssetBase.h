#pragma once

#include <QVector>
#include <QString>
#include <QMap>
#include <variant>

#include <qtvariantproperty.h>

class AssetCategory;
class AssetsDataBase;

class AssetBase {
	friend class AssetCategory;
	friend class AssetsDataBase;

public:
	AssetBase();
	virtual ~AssetBase();

	int32_t getId() const;

	void setName(const QString& name);
	QString getName() const;

	void setCategory(AssetCategory* category);
	AssetCategory* getCategory() const;

	QtVariantPropertyManager* getAttributesStorage() const;
	virtual QVector<QtProperty*> getEditableProperties() const;

	virtual QMap<QString, QVariant> getAttibutesRaw() const;

	virtual void performDelete();

	static int32_t getFreeId() { return s_freeId; }

private:
	static void increaseFreeId() { s_freeId++; }
	static int32_t getFreeIdAndIncrease() { return s_freeId++; }

private:
	static int32_t s_freeId;


protected:
	AssetCategory* m_category;
	QtVariantPropertyManager* m_attributesStorage;
	QtProperty* m_commonProperties;

	QtVariantProperty* m_typeProperty;

	int32_t m_id;
	QString m_name;
};
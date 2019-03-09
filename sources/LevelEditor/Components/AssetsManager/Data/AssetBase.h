#pragma once

#include <QVector>
#include <QString>
#include <QMap>
#include <variant>

#include <qtvariantproperty.h>
#include <LevelEditor/Core/XMLSerializable.h>

class AssetCategory;
class AssetsDataBase;

class AssetBase : public XMLSerializable {
	friend class AssetCategory;
	friend class AssetsDataBase;

public:
	AssetBase();
	virtual ~AssetBase();

	int32_t getId() const;

	void setName(const QString& name);
	QString getName() const;

	AssetCategory* getCategory() const;

	QtVariantPropertyManager* getAttributesStorage() const;
	virtual QVector<QtProperty*> getEditableProperties() const;

	virtual void performDelete();

public:
	virtual void serialize(pugi::xml_node& storage) const override;
	virtual void deserialize(const pugi::xml_node& storage) override;

protected:
	void setId(int32_t id);

protected:
	AssetCategory* m_category;
	QtVariantPropertyManager* m_attributesStorage;
	QtProperty* m_commonProperties;

	QtVariantProperty* m_typeProperty;
	QtVariantProperty* m_idProperty;

	int32_t m_id;
	int32_t m_categoryId;
	QString m_name;
};
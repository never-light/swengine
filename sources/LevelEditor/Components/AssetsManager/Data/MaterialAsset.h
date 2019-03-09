#pragma once

#include <QObject>
#include <QVariant>

#include "AssetBase.h"
#include "MaterialsLayouts.h"

enum class MaterialProcessingStage {
	Forward, Deferred, Transparency
};

class MaterialAsset final : public QObject, public AssetBase {
	Q_OBJECT
public:
	MaterialAsset();
	virtual ~MaterialAsset();

	virtual QVector<QtProperty*> getEditableProperties() const override;

	void setMaterialType(MaterialLayoutType type);
	MaterialLayoutType getMaterialType() const;

	void setProcessingStage(MaterialProcessingStage stage);
	MaterialProcessingStage getProcessingStage() const;
	
	void setMaterialLayout(MaterialLayout* layout);
	MaterialLayout* getMaterialLayout() const;

public:
	virtual void serialize(pugi::xml_node& storage) const override;
	virtual void deserialize(const pugi::xml_node& storage) override;

private slots:
	void propertyValueChanged(QtProperty* property, const QVariant& value);

private:
	QtProperty* m_commonProperties;
	QtVariantProperty* m_processingStage;
	QtVariantProperty* m_materialType;

	MaterialLayout* m_materialLayout;
};
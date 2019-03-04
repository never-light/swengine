#pragma once

#include "AssetBase.h"
#include "MaterialsLayouts.h"

class MaterialAsset final : public AssetBase {
public:
	MaterialAsset();
	virtual ~MaterialAsset();

	virtual QMap<QString, QVariant> getAttibutesRaw() const override;
	virtual QVector<QtProperty*> getEditableProperties() const override;

	void replaceMaterialLayout(MaterialLayout* layout);
	MaterialLayout* getMaterialLayout() const;
private:
	QtProperty* m_properties;

	MaterialLayout* m_materialLayout;
};
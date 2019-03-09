#pragma once

#include "AssetBase.h"

class FileAsset : public AssetBase {
public:
	FileAsset();
	virtual ~FileAsset();

	void setFileName(const QString& fileName);
	QString getFileName() const;

	virtual void performDelete() override;

	virtual QVector<QtProperty*> getEditableProperties() const override;

public:
	virtual void serialize(pugi::xml_node& storage) const override;
	virtual void deserialize(const pugi::xml_node& storage) override;

protected:
	QtVariantProperty* m_fileNameProperty;
};
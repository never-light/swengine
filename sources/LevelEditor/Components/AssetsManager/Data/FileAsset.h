#pragma once

#include "AssetBase.h"

class FileAsset : public AssetBase {
public:
	FileAsset(const QString& fileName);
	virtual ~FileAsset();

	void setFileName(const QString& fileName);
	QString getFileName() const;

	virtual void performDelete() override;

	virtual QMap<QString, QVariant> getAttibutesRaw() const override;
	virtual QVector<QtProperty*> getEditableProperties() const override;

protected:
	QtVariantProperty* m_fileNameProperty;
};
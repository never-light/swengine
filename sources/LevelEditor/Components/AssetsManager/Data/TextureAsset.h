#pragma once

#include "FileAsset.h"

class TextureAsset final : public FileAsset {
public:
	TextureAsset();
	virtual ~TextureAsset();

	virtual QVector<QtProperty*> getEditableProperties() const override;

	int getWidth() const;
	void setWidth(int width);

	int getHeight() const;
	void setHeight(int height);

	QSize getSize() const;
	void setSize(const QSize& size);
	void setSize(int width, int height);

	size_t getFileSize() const;
	void setFileSize(size_t size);

	bool isSRGB() const;

	void useAsSRGB(bool use);

public:
	virtual void serialize(pugi::xml_node& storage) const override;
	virtual void deserialize(const pugi::xml_node& storage) override;

private:
	QtProperty* m_textureProperties;

	QtVariantProperty* m_sizeProperty;
	QtVariantProperty* m_fileSizeProperty;
	QtVariantProperty* m_gammaCorrectionProperty;

};
#include "TextureAsset.h"

TextureAsset::TextureAsset(const QString & fileName)
	: FileAsset(fileName)
{
	m_typeProperty->setValue("Texture");

	m_sizeProperty = m_attributesStorage->addProperty(QVariant::Size,
		"Size");
	m_sizeProperty->setValue(QSize(0, 0));
	m_sizeProperty->setAttribute(QLatin1String("minimum"), QSize(0, 0));
	m_sizeProperty->setAttribute(QLatin1String("maximum"), QSize(8192, 8192));

	m_sizeProperty->setEnabled(false);

	m_fileSizeProperty = m_attributesStorage->addProperty(QVariant::Int,
		"File size");
	m_fileSizeProperty->setValue(0);
	m_fileSizeProperty->setEnabled(false);

	m_gammaCorrectionProperty = m_attributesStorage->addProperty(QVariant::Bool,
		"Use as sRGB");
	m_gammaCorrectionProperty->setValue(false);
	m_gammaCorrectionProperty->setEnabled(true);

	m_textureProperties = m_attributesStorage->addProperty(QtVariantPropertyManager::groupTypeId(),
		QLatin1String("Texture"));

	m_textureProperties->addSubProperty(m_sizeProperty);
	m_textureProperties->addSubProperty(m_fileSizeProperty);
	m_textureProperties->addSubProperty(m_gammaCorrectionProperty);
}

TextureAsset::~TextureAsset()
{
}

QMap<QString, QVariant> TextureAsset::getAttibutesRaw() const
{
	auto attributes = FileAsset::getAttibutesRaw();

	attributes.insert("filesize", getFileSize());
	attributes.insert("size", getSize());
	attributes.insert("srgb", isSRGB());

	return attributes;
}

QVector<QtProperty*> TextureAsset::getEditableProperties() const
{
	auto properties = FileAsset::getEditableProperties();
	properties.push_back(m_textureProperties);

	return properties;
}

int TextureAsset::getWidth() const
{
	return m_sizeProperty->value().toSize().width();
}

void TextureAsset::setWidth(int width)
{
	m_sizeProperty->setValue(QSize(width, getHeight()));
}

int TextureAsset::getHeight() const
{
	return m_sizeProperty->value().toSize().height();
}

void TextureAsset::setHeight(int height)
{
	m_sizeProperty->setValue(QSize(getWidth(), height));
}

QSize TextureAsset::getSize() const
{
	return m_sizeProperty->value().toSize();
}

void TextureAsset::setSize(const QSize & size)
{
	m_sizeProperty->setValue(size);
}

void TextureAsset::setSize(int width, int height)
{
	m_sizeProperty->setValue(QSize(width, height));
}

size_t TextureAsset::getFileSize() const
{
	return m_fileSizeProperty->value().toInt();
}

void TextureAsset::setFileSize(size_t size)
{
	m_fileSizeProperty->setValue(size);
}

bool TextureAsset::isSRGB() const
{
	return m_gammaCorrectionProperty->value().toBool();
}

void TextureAsset::useAsSRGB(bool use)
{
	m_gammaCorrectionProperty->setValue(use);
}

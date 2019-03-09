#include "FileAsset.h"

#include <QFile>
#include <QDebug>

#include <LevelEditor/Components/Utils/filesystem.h>

FileAsset::FileAsset()
	: AssetBase()
{
	m_fileNameProperty = m_attributesStorage->addProperty(QVariant::String, "FileName");
	m_fileNameProperty->setValue("");
	m_fileNameProperty->setEnabled(false);

	m_commonProperties->addSubProperty(m_fileNameProperty);
}

FileAsset::~FileAsset()
{
}

void FileAsset::setFileName(const QString & fileName)
{
	m_fileNameProperty->setValue(fileName);
}

QString FileAsset::getFileName() const
{
	return m_fileNameProperty->value().toString();
}

void FileAsset::performDelete()
{
	qDebug() << getEditorAssetsDir() + QString("/") + m_fileNameProperty->value().toString();
	QFile::remove(getEditorAssetsDir() + QString("/") + m_fileNameProperty->value().toString());
}

QVector<QtProperty*> FileAsset::getEditableProperties() const
{
	return { m_commonProperties };
}

void FileAsset::serialize(pugi::xml_node& storage) const
{
	AssetBase::serialize(storage);

	storage.append_attribute("file").set_value(getFileName().toStdString().c_str());
}

void FileAsset::deserialize(const pugi::xml_node& storage)
{
	AssetBase::deserialize(storage);

	setFileName(storage.attribute("file").as_string());
}

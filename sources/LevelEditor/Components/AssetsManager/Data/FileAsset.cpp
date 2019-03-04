#include "FileAsset.h"

#include <QFile>
#include <QDebug>

#include <LevelEditor/Components/Utils/filesystem.h>

FileAsset::FileAsset(const QString & fileName)
	: AssetBase()
{
	m_fileNameProperty = m_attributesStorage->addProperty(QVariant::String,
		"FileName");
	m_fileNameProperty->setValue(fileName);
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

QMap<QString, QVariant> FileAsset::getAttibutesRaw() const
{
	auto attributes = AssetBase::getAttibutesRaw();

	attributes.insert("file", getFileName());

	return attributes;
}

QVector<QtProperty*> FileAsset::getEditableProperties() const
{
	return { m_commonProperties };
}

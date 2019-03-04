#include "MeshAsset.h"

MeshAsset::MeshAsset(const QString & fileName)
	: FileAsset(fileName)
{
	m_typeProperty->setValue("Mesh");

	m_verticesCountProperty = m_attributesStorage->addProperty(QVariant::Int,
		"Vertices count");
	m_verticesCountProperty->setValue(0);
	m_verticesCountProperty->setEnabled(false);

	m_indicesCountProperty = m_attributesStorage->addProperty(QVariant::Int,
		"Indices count");
	m_indicesCountProperty->setValue(0);
	m_indicesCountProperty->setEnabled(false);

	m_subMeshesCountProperty = m_attributesStorage->addProperty(QVariant::Int,
		"SubMeshes count");
	m_subMeshesCountProperty->setValue(0);
	m_subMeshesCountProperty->setEnabled(false);

	m_sizeProperty = m_attributesStorage->addProperty(QVariant::Int,
		"File size");
	m_sizeProperty->setValue(0);
	m_sizeProperty->setEnabled(false);

	m_meshProperties = m_attributesStorage->addProperty(QtVariantPropertyManager::groupTypeId(),
		QLatin1String("Mesh"));

	m_meshProperties->addSubProperty(m_verticesCountProperty);
	m_meshProperties->addSubProperty(m_indicesCountProperty);
	m_meshProperties->addSubProperty(m_subMeshesCountProperty);
	m_meshProperties->addSubProperty(m_sizeProperty);
}

MeshAsset::~MeshAsset()
{
}

QMap<QString, QVariant> MeshAsset::getAttibutesRaw() const
{
	auto attributes = FileAsset::getAttibutesRaw();

	attributes.insert("vertices", getVerticesCount());
	attributes.insert("indices", getIndicesCount());
	attributes.insert("submeshes", getSubMeshesCount());
	attributes.insert("size", getSize());

	return attributes;
}

QVector<QtProperty*> MeshAsset::getEditableProperties() const
{
	auto properties = FileAsset::getEditableProperties();
	properties.push_back(m_meshProperties);

	return properties;
}

size_t MeshAsset::getVerticesCount() const
{
	return m_verticesCountProperty->value().toInt();
}

void MeshAsset::setVerticesCount(size_t count)
{
	m_verticesCountProperty->setValue(count);
}

size_t MeshAsset::getIndicesCount() const
{
	return m_indicesCountProperty->value().toInt();
}

void MeshAsset::setIndicesCount(size_t count)
{
	m_indicesCountProperty->setValue(count);
}

size_t MeshAsset::getSubMeshesCount() const
{
	return m_subMeshesCountProperty->value().toInt();
}

void MeshAsset::setSubMeshesCount(size_t count)
{
	m_subMeshesCountProperty->setValue(count);
}

size_t MeshAsset::getSize() const
{
	return m_sizeProperty->value().toInt();
}

void MeshAsset::setSize(size_t size)
{
	m_sizeProperty->setValue(size);
}

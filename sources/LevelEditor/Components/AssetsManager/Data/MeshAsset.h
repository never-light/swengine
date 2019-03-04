#pragma once

#include "FileAsset.h"

class MeshAsset final : public FileAsset {
public:
	MeshAsset(const QString& fileName);
	virtual ~MeshAsset();

	virtual QMap<QString, QVariant> getAttibutesRaw() const override;
	virtual QVector<QtProperty*> getEditableProperties() const override;

	size_t getVerticesCount() const;
	void setVerticesCount(size_t count);

	size_t getIndicesCount() const;
	void setIndicesCount(size_t count);

	size_t getSubMeshesCount() const;
	void setSubMeshesCount(size_t count);

	size_t getSize() const;
	void setSize(size_t size);

private:
	QtProperty* m_meshProperties;

	QtVariantProperty* m_verticesCountProperty;
	QtVariantProperty* m_indicesCountProperty;
	QtVariantProperty* m_subMeshesCountProperty;
	QtVariantProperty* m_sizeProperty;
};
#pragma once

#include "FileAsset.h"

class MeshAsset final : public FileAsset {
public:
	MeshAsset();
	virtual ~MeshAsset();

	virtual QVector<QtProperty*> getEditableProperties() const override;

	size_t getVerticesCount() const;
	void setVerticesCount(size_t count);

	size_t getIndicesCount() const;
	void setIndicesCount(size_t count);

	size_t getSubMeshesCount() const;
	void setSubMeshesCount(size_t count);

	size_t getSize() const;
	void setSize(size_t size);

public:
	virtual void serialize(pugi::xml_node& storage) const override;
	virtual void deserialize(const pugi::xml_node& storage) override;

private:
	QtProperty* m_meshProperties;

	QtVariantProperty* m_verticesCountProperty;
	QtVariantProperty* m_indicesCountProperty;
	QtVariantProperty* m_subMeshesCountProperty;
	QtVariantProperty* m_sizeProperty;
};
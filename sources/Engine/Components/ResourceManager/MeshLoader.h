#pragma once

#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>
#include <Engine/Components/Graphics/RenderLayout/Mesh.h>

#include "ResourceLoader.h"

#define SOLID_MESH_LOADER_MAX_NAMES_LENGTH 256
#define SOLID_MESH_LOADER_MAX_PATH_LENGTH 256

class MeshLoader : public ResourceLoader {
private:
	struct HeaderData {
		std::uint32_t version;
	};

	struct MeshDescription {
		char name[SOLID_MESH_LOADER_MAX_NAMES_LENGTH];
		std::uint32_t verticesCount;
		std::uint32_t indicesCount;

		std::uint32_t partsCount;
		std::uint32_t materialsCount;
		std::uint32_t collidersCount;

		bool hasSkeleton;
	};

	struct MeshPartDescription {
		char name[SOLID_MESH_LOADER_MAX_NAMES_LENGTH];
		char materialName[SOLID_MESH_LOADER_MAX_NAMES_LENGTH];

		std::uint32_t verticesCount;
		std::uint32_t indicesCount;

	};

	struct ColliderDescription {
		vector3 origin;
		vector3 vertex1;
		vector3 vertex2;
		vector3 vertex3;
	};

	struct BoneDescription {
		std::uint32_t id;
		char name[SOLID_MESH_LOADER_MAX_NAMES_LENGTH];
		bool isHelper;

		std::int32_t parentId;
		std::uint32_t childrenCount;
		std::vector<std::uint32_t> children;

		matrix4 localToBoneSpaceTransform;
		matrix4 relativeToParentSpaceTransform;
	};

	struct SkeletonDescription {
		std::uint32_t bonesCount;
		matrix4 globalInverseTransform;
	};

public:
	MeshLoader(GraphicsContext* graphicsContext);
	virtual ~MeshLoader();

private:
	GraphicsContext* m_graphicsContext;

	virtual BaseResourceInstance * load(const std::string & path, std::optional<std::any> options) override;
};

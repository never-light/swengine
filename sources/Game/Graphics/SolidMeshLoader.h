#pragma once

#include <Engine\Components\ResourceManager\ResourceLoader.h>
#include <Engine\Components\ResourceManager\ResourceManager.h>

#include <Game\Graphics\Materials\PhongMaterialParameters.h>

#define SOLID_MESH_LOADER_MAX_NAMES_LENGTH 256
#define SOLID_MESH_LOADER_MAX_PATH_LENGTH 256

class SolidMeshLoader : public ResourceLoader {
private:
	struct MaterialDescription {
		char name[SOLID_MESH_LOADER_MAX_NAMES_LENGTH];
		vector3 emissiveColor;

		vector3 diffuseColor;
		char diffuseMap[SOLID_MESH_LOADER_MAX_PATH_LENGTH];

		vector3 specularColor;
		char specularMap[SOLID_MESH_LOADER_MAX_PATH_LENGTH];
		float specularFactor;

		char normalMap[SOLID_MESH_LOADER_MAX_PATH_LENGTH];
	};

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
	SolidMeshLoader(ResourceManager* resourceManager, GraphicsResourceFactory* graphicsResourceFactory);
	virtual ~SolidMeshLoader();

	virtual Resource* load(const std::string& filename) override;

private:
	PhongMaterialParameters* processConnectedMaterial(const MaterialDescription& materialDescription);
	Texture* processConnectedTexture(const std::string& filename);

private:
	ResourceManager* m_resourceManager;
	GraphicsResourceFactory* m_graphicsResourceFactory;
};
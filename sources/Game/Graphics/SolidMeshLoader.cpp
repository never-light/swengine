#include "SolidMeshLoader.h"
#include "SolidMesh.h"

#include <fstream>

SolidMeshLoader::SolidMeshLoader(ResourceManager * resourceManager, GraphicsResourceFactory* graphicsResourceFactory)
	: m_resourceManager(resourceManager), m_graphicsResourceFactory(graphicsResourceFactory)
{

}

SolidMeshLoader::~SolidMeshLoader()
{
}

Resource * SolidMeshLoader::load(const std::string & filename)
{
	try {
		std::ifstream in(filename, std::ios::binary | std::ios::in);

		if (!in.is_open())
			throw ResourceLoadingException(ResourceLoadingError::FileNotAvailable, filename.c_str(), "", __FILE__, __LINE__, __FUNCTION__);

		HeaderData header;
		in.read((char*)&header, sizeof header);

		MeshDescription description;
		in.read((char*)&description, sizeof description);

		// Positions
		std::vector<vector3> positions(description.verticesCount);
		in.read((char*)positions.data(), sizeof(vector3) * description.verticesCount);

		// Normals
		std::vector<vector3> normals(description.verticesCount);
		in.read((char*)normals.data(), sizeof(vector3) * description.verticesCount);

		// Tangents
		std::vector<vector3> tangents(description.verticesCount);
		in.read((char*)tangents.data(), sizeof(vector3) * description.verticesCount);

		// Bitangents
		std::vector<vector3> bitangents(description.verticesCount);
		in.read((char*)bitangents.data(), sizeof(vector3) * description.verticesCount);

		// UV
		std::vector<vector2> uv(description.verticesCount);
		in.read((char*)uv.data(), sizeof(vector2) * description.verticesCount);


		std::vector<ivector4> bonesIds(description.verticesCount);
		std::vector<vector4> bonesWeights(description.verticesCount);

		// Load bones data
		if (description.hasSkeleton) {
			in.read((char*)bonesIds.data(), sizeof(ivector4) * description.verticesCount);
			in.read((char*)bonesWeights.data(), sizeof(vector4) * description.verticesCount);
		}

		// Indices of materials
		std::vector<std::uint32_t> materialsIndices(description.verticesCount);
		in.read((char*)materialsIndices.data(), sizeof(std::uint32_t) * description.verticesCount);

		// Indices of vertices
		std::vector<uint32> indices(description.indicesCount);
		in.read((char*)indices.data(), sizeof(uint32) * description.indicesCount);

		// Offsets of per-materials groups
		std::vector<std::uint32_t> partsOffsets(description.partsCount);
		in.read((char*)partsOffsets.data(), sizeof(std::uint32_t) * description.partsCount);

		// Connected materials
		std::vector<MaterialDescription> meshMaterials(description.materialsCount);
		in.read((char*)meshMaterials.data(), sizeof(MaterialDescription) * description.materialsCount);

		std::vector<MaterialParameters*> connectedMaterialsParameters;

		for (const auto& material : meshMaterials)
			connectedMaterialsParameters.push_back(processConnectedMaterial(material));

		// Colliders
		std::vector<ColliderDescription> collidersDescriptions(description.collidersCount);
		in.read((char*)collidersDescriptions.data(), sizeof(ColliderDescription) * description.collidersCount);

		std::vector<OBB> colliders;

		for (const auto& collider : collidersDescriptions)
			colliders.push_back(OBB(collider.origin, collider.vertex1, collider.vertex2, collider.vertex3));

		// Skeleton
		Skeleton* skeleton = nullptr;

		if (description.hasSkeleton) {
			std::vector<Bone> bones;

			SkeletonDescription skeletonDescription;

			in.read((char*)&skeletonDescription.bonesCount, sizeof(SkeletonDescription::bonesCount));
			in.read((char*)&skeletonDescription.globalInverseTransform, sizeof(SkeletonDescription::globalInverseTransform));

			// Read bones
			for (size_t boneIndex = 0; boneIndex < skeletonDescription.bonesCount; boneIndex++) {
				BoneDescription boneDescription;

				in.read((char*)&boneDescription.id, sizeof(BoneDescription::id));
				in.read((char*)&boneDescription.name, sizeof(BoneDescription::name));
				in.read((char*)&boneDescription.isHelper, sizeof(BoneDescription::isHelper));
				in.read((char*)&boneDescription.parentId, sizeof(BoneDescription::parentId));
				in.read((char*)&boneDescription.childrenCount, sizeof(BoneDescription::childrenCount));

				boneDescription.children.resize(boneDescription.childrenCount);
				in.read((char*)boneDescription.children.data(), sizeof(boneDescription.children[0]) * boneDescription.childrenCount);

				in.read((char*)&boneDescription.localToBoneSpaceTransform, sizeof(BoneDescription::localToBoneSpaceTransform));
				in.read((char*)&boneDescription.relativeToParentSpaceTransform, sizeof(BoneDescription::relativeToParentSpaceTransform));

				bones.push_back(Bone(boneDescription.id,
					boneDescription.name,
					boneDescription.isHelper,
					boneDescription.parentId,
					boneDescription.children,
					boneDescription.localToBoneSpaceTransform,
					boneDescription.relativeToParentSpaceTransform));
			}

			skeleton = new Skeleton(bones, skeletonDescription.globalInverseTransform);
		}

		GeometryStore* geometryStore = nullptr;

		geometryStore = m_graphicsResourceFactory->createGeometryStore();

		// Create and fill vertex buffer
		size_t baseBufferComponentsSize = sizeof(vector3) * 4 + sizeof(vector2);

		if (description.hasSkeleton) {
			baseBufferComponentsSize += (sizeof(ivector4) + sizeof(vector4));
		}

		size_t requiredVertexBufferSize = baseBufferComponentsSize * description.verticesCount;
		GeometryStore::BufferId vertexBufferId = geometryStore->requireBuffer(GeometryStore::BufferType::Vertex, GeometryStore::BufferUsage::StaticDraw, requiredVertexBufferSize);

		size_t vertexBufferOffset = 0;

		geometryStore->setBufferData(vertexBufferId, vertexBufferOffset, sizeof(vector3) * description.verticesCount, (const std::byte*)positions.data());
		vertexBufferOffset += sizeof(vector3) * description.verticesCount;

		geometryStore->setBufferData(vertexBufferId, vertexBufferOffset, sizeof(vector3) * description.verticesCount, (const std::byte*)normals.data());
		vertexBufferOffset += sizeof(vector3) * description.verticesCount;

		geometryStore->setBufferData(vertexBufferId, vertexBufferOffset, sizeof(vector3) * description.verticesCount, (const std::byte*)tangents.data());
		vertexBufferOffset += sizeof(vector3) * description.verticesCount;

		geometryStore->setBufferData(vertexBufferId, vertexBufferOffset, sizeof(vector3) * description.verticesCount, (const std::byte*)bitangents.data());
		vertexBufferOffset += sizeof(vector3) * description.verticesCount;

		geometryStore->setBufferData(vertexBufferId, vertexBufferOffset, sizeof(vector2) * description.verticesCount, (const std::byte*)uv.data());
		vertexBufferOffset += sizeof(vector2) * description.verticesCount;

		if (description.hasSkeleton) {
			geometryStore->setBufferData(vertexBufferId, vertexBufferOffset, sizeof(ivector4) * description.verticesCount, (const std::byte*)bonesIds.data());
			vertexBufferOffset += sizeof(ivector4) * description.verticesCount;

			geometryStore->setBufferData(vertexBufferId, vertexBufferOffset, sizeof(vector4) * description.verticesCount, (const std::byte*)bonesWeights.data());
			vertexBufferOffset += sizeof(vector4) * description.verticesCount;
		}

		// Create and fill index buffer
		size_t requiredIndexBufferSize = sizeof(std::uint32_t) * indices.size();
		GeometryStore::BufferId indexBufferId = geometryStore->requireBuffer(GeometryStore::BufferType::Index, GeometryStore::BufferUsage::StaticDraw, requiredIndexBufferSize);

		geometryStore->setBufferData(indexBufferId, 0, sizeof(std::uint32_t)*description.indicesCount, (const std::byte*)indices.data());

		// Set vertex layout description

		// Positions
		geometryStore->setVertexLayoutAttribute(0, vertexBufferId, 3,
			GeometryStore::VertexLayoutAttributeBaseType::Float, false, 0, 0);

		// Normals
		geometryStore->setVertexLayoutAttribute(1, vertexBufferId, 3,
			GeometryStore::VertexLayoutAttributeBaseType::Float, false, 0, sizeof(vector3) * description.verticesCount);

		// Tangents
		geometryStore->setVertexLayoutAttribute(2, vertexBufferId, 3,
			GeometryStore::VertexLayoutAttributeBaseType::Float, false, 0, sizeof(vector3) * description.verticesCount * 2);

		// Bitangents
		geometryStore->setVertexLayoutAttribute(3, vertexBufferId, 3,
			GeometryStore::VertexLayoutAttributeBaseType::Float, false, 0, sizeof(vector3) * description.verticesCount * 3);

		// UV
		geometryStore->setVertexLayoutAttribute(4, vertexBufferId, 2,
			GeometryStore::VertexLayoutAttributeBaseType::Float, false, 0, sizeof(vector3) * description.verticesCount * 4);

		if (description.hasSkeleton) {
			size_t attributeOffset = sizeof(vector3) * description.verticesCount * 4 + sizeof(vector2) * description.verticesCount;

			// Bones IDs
			geometryStore->setVertexLayoutAttribute(5, vertexBufferId, 4,
				GeometryStore::VertexLayoutAttributeBaseType::Int, false, 0, attributeOffset);

			attributeOffset += sizeof(ivector4) * description.verticesCount;

			// Bones weights
			geometryStore->setVertexLayoutAttribute(6, vertexBufferId, 4,
				GeometryStore::VertexLayoutAttributeBaseType::Float, false, 0, attributeOffset);

			attributeOffset += sizeof(vector4) * description.verticesCount;
		}

		geometryStore->create();

		return new SolidMesh(geometryStore, partsOffsets, connectedMaterialsParameters, colliders, skeleton);
	}
	catch (const std::ifstream::failure& failture) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), failture.what(), __FILE__, __LINE__, __FUNCTION__);
	}
	catch (const std::length_error& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), exception.what(), __FILE__, __LINE__, __FUNCTION__);
	}
	catch (const RenderSystemException& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), exception.what(), exception.getFile(), exception.getLine(), exception.getFunction());
	}
}

PhongMaterialParameters* SolidMeshLoader::processConnectedMaterial(const MaterialDescription& materialDescription)
{
	PhongMaterialParameters* materialParameters = new PhongMaterialParameters();

	materialParameters->setEmissiveColor(materialDescription.emissiveColor);
	materialParameters->setDiffuseColor(materialDescription.diffuseColor);

	materialParameters->setSpecularColor(materialDescription.specularColor);
	materialParameters->setSpecularFactor(materialDescription.specularFactor);

	if (!std::string(materialDescription.diffuseMap).empty()) {
		materialParameters->setDiffuseTexture(processConnectedTexture(materialDescription.diffuseMap));
	}

	if (!std::string(materialDescription.specularMap).empty()) {
		materialParameters->setSpecularTexture(processConnectedTexture(materialDescription.specularMap));
	}

	if (!std::string(materialDescription.normalMap).empty()) {
		materialParameters->setNormalMap(processConnectedTexture(materialDescription.normalMap));
	}

	return materialParameters;
}

Texture * SolidMeshLoader::processConnectedTexture(const std::string & filename)
{
	Texture* texture = m_resourceManager->load<Texture>(filename);
	texture->bind();

	texture->generateMipMaps();
	texture->setMinificationFilter(Texture::Filter::LinearMipmapLinear);
	texture->setMagnificationFilter(Texture::Filter::Linear);

	texture->setWrapMode(Texture::WrapMode::Repeat);

	texture->enableAnisotropicFiltering(16.0f);

	return texture;
}

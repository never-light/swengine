#include "MeshLoader.h"

#include <fstream>
#include <Engine/Components/Physics/Colliders/OBB.h>

#include <Engine/Components/Graphics/RenderSystem/RenderSystemException.h>

#include "ResourceLoadingException.h"

MeshLoader::MeshLoader(GraphicsContext* graphicsContext)
	: m_graphicsContext(graphicsContext)
{

}

MeshLoader::~MeshLoader()
{
}

BaseResourceInstance* MeshLoader::load(const std::string & path, std::optional<std::any> options)
{
	try {
		std::ifstream in(path, std::ios::binary | std::ios::in);

		if (!in.is_open())
			throw ResourceLoadingException(ResourceLoadingError::FileNotAvailable, path.c_str(), "", __FILE__, __LINE__, __FUNCTION__);

		HeaderData header;
		in.read((char*)&header, sizeof header);

		MeshDescription description;
		in.read((char*)&description, sizeof description);

		std::vector<SubMesh*> subMeshes;

		for (size_t partIndex = 0; partIndex < description.partsCount; partIndex++) {
			MeshPartDescription partDescription;
			in.read((char*)&partDescription, sizeof(partDescription));

			// Positions
			std::vector<vector3> positions(partDescription.verticesCount);
			in.read((char*)positions.data(), sizeof(vector3) * partDescription.verticesCount);

			// Normals
			std::vector<vector3> normals(partDescription.verticesCount);
			in.read((char*)normals.data(), sizeof(vector3) * partDescription.verticesCount);

			// Tangents
			std::vector<vector3> tangents(partDescription.verticesCount);
			in.read((char*)tangents.data(), sizeof(vector3) * partDescription.verticesCount);

			// Bitangents
			std::vector<vector3> bitangents(partDescription.verticesCount);
			in.read((char*)bitangents.data(), sizeof(vector3) * partDescription.verticesCount);

			// UV
			std::vector<vector2> uv(partDescription.verticesCount);
			in.read((char*)uv.data(), sizeof(vector2) * partDescription.verticesCount);


			std::vector<ivector4> bonesIds(partDescription.verticesCount);
			std::vector<vector4> bonesWeights(partDescription.verticesCount);

			// Load bones data
			if (description.hasSkeleton) {
				in.read((char*)bonesIds.data(), sizeof(ivector4) * partDescription.verticesCount);
				in.read((char*)bonesWeights.data(), sizeof(vector4) * partDescription.verticesCount);
			}

			// Indices of vertices
			std::vector<uint32> indices(partDescription.indicesCount);
			in.read((char*)indices.data(), sizeof(uint32) * partDescription.indicesCount);


			GeometryInstance* subMeshGeometry = m_graphicsContext->createGeometryInstance();

			// Create and fill vertex buffer
			size_t baseBufferComponentsSize = sizeof(vector3) * 4 + sizeof(vector2);

			if (description.hasSkeleton) {
				baseBufferComponentsSize += (sizeof(ivector4) + sizeof(vector4));
			}

			size_t requiredVertexBufferSize = baseBufferComponentsSize * partDescription.verticesCount;


			subMeshGeometry->setVerticesData(partDescription.verticesCount, 
				requiredVertexBufferSize, nullptr, GeometryInstance::DataUsage::StaticDraw);

			size_t vertexBufferOffset = 0;

			subMeshGeometry->setVerticesDataPart(vertexBufferOffset, sizeof(vector3) * partDescription.verticesCount, (const std::byte*)positions.data());
			vertexBufferOffset += sizeof(vector3) * partDescription.verticesCount;

			subMeshGeometry->setVerticesDataPart(vertexBufferOffset, sizeof(vector3) * partDescription.verticesCount, (const std::byte*)normals.data());
			vertexBufferOffset += sizeof(vector3) * partDescription.verticesCount;

			subMeshGeometry->setVerticesDataPart(vertexBufferOffset, sizeof(vector3) * partDescription.verticesCount, (const std::byte*)tangents.data());
			vertexBufferOffset += sizeof(vector3) * partDescription.verticesCount;

			subMeshGeometry->setVerticesDataPart(vertexBufferOffset, sizeof(vector3) * partDescription.verticesCount, (const std::byte*)bitangents.data());
			vertexBufferOffset += sizeof(vector3) * partDescription.verticesCount;

			subMeshGeometry->setVerticesDataPart(vertexBufferOffset, sizeof(vector2) * partDescription.verticesCount, (const std::byte*)uv.data());
			vertexBufferOffset += sizeof(vector2) * partDescription.verticesCount;

			if (description.hasSkeleton) {
				subMeshGeometry->setVerticesDataPart(vertexBufferOffset, sizeof(ivector4) * partDescription.verticesCount, (const std::byte*)bonesIds.data());
				vertexBufferOffset += sizeof(ivector4) * partDescription.verticesCount;

				subMeshGeometry->setVerticesDataPart(vertexBufferOffset, sizeof(vector4) * partDescription.verticesCount, (const std::byte*)bonesWeights.data());
				vertexBufferOffset += sizeof(vector4) * partDescription.verticesCount;
			}

			// Create and fill index buffer
			size_t requiredIndexBufferSize = sizeof(std::uint32_t) * indices.size();

			subMeshGeometry->setIndicesData(indices.size(), 
				requiredIndexBufferSize, (const std::byte*)indices.data(), GeometryInstance::DataUsage::StaticDraw);

			// Set vertex layout description

			// Positions
			subMeshGeometry->setAttributeDesc(GeometryInstance::CommonAttrs::Position, 
				GeometryAttributeDesc(GeometryAttributeType::Float, 0, 3, 0));

			// Normals
			subMeshGeometry->setAttributeDesc(GeometryInstance::CommonAttrs::Normal,
				GeometryAttributeDesc(GeometryAttributeType::Float, sizeof(vector3) * partDescription.verticesCount, 3, 0));

			// Tangents
			subMeshGeometry->setAttributeDesc(GeometryInstance::CommonAttrs::Tangent,
				GeometryAttributeDesc(GeometryAttributeType::Float, sizeof(vector3) * partDescription.verticesCount * 2, 3, 0));

			// Bitangents
			subMeshGeometry->setAttributeDesc(GeometryInstance::CommonAttrs::Bitangent,
				GeometryAttributeDesc(GeometryAttributeType::Float, sizeof(vector3) * partDescription.verticesCount * 3, 3, 0));

			// UV
			subMeshGeometry->setAttributeDesc(GeometryInstance::CommonAttrs::UV,
				GeometryAttributeDesc(GeometryAttributeType::Float, sizeof(vector3) * partDescription.verticesCount * 4, 2, 0));

			if (description.hasSkeleton) {
				size_t attributeOffset = sizeof(vector3) * partDescription.verticesCount * 4 + sizeof(vector2) * partDescription.verticesCount;

				// Bones IDs
				subMeshGeometry->setAttributeDesc(GeometryInstance::CommonAttrs::JointsIds,
					GeometryAttributeDesc(GeometryAttributeType::Int, attributeOffset, 4, 0));

				attributeOffset += sizeof(ivector4) * partDescription.verticesCount;

				// Bones weights
				subMeshGeometry->setAttributeDesc(GeometryInstance::CommonAttrs::JointsWeights,
					GeometryAttributeDesc(GeometryAttributeType::Float, attributeOffset, 4, 0));

				attributeOffset += sizeof(vector4) * partDescription.verticesCount;
			}

			subMeshGeometry->create();

			subMeshes.push_back(new SubMesh(subMeshGeometry, nullptr));
		}

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
					(int)boneDescription.parentId,
					std::vector<size_t>(boneDescription.children.begin(), boneDescription.children.end()),
					boneDescription.localToBoneSpaceTransform,
					boneDescription.relativeToParentSpaceTransform));
			}

			skeleton = new Skeleton(bones, skeletonDescription.globalInverseTransform);
		}

		return new ResourceInstance<Mesh>(new Mesh(subMeshes, skeleton));
	}
	catch (const std::ifstream::failure& failture) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), failture.what(), __FILE__, __LINE__, __FUNCTION__);
	}
	catch (const std::length_error& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), exception.what(), __FILE__, __LINE__, __FUNCTION__);
	}
	catch (const RenderSystemException& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), exception.what(), exception.getFile(), exception.getLine(), exception.getFunction());
	}
}
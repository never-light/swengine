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

	std::vector<DefaultMaterial*> connectedMaterials;

	for (const auto& material : meshMaterials)
		connectedMaterials.push_back(processConnectedMaterial(material));

	GeometryStore* geometryStore = nullptr;

	try {
		geometryStore = m_graphicsResourceFactory->createGeometryStore();

		// Create and fill vertex buffer
		size_t requiredVertexBufferSize = (sizeof(vector3) * 4 + sizeof(vector2)) * description.verticesCount;
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

		geometryStore->create();
	}
	catch (const RenderSystemException& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), exception.what(), exception.getFile(), exception.getLine(), exception.getFunction());
	}
	
	return new SolidMesh(geometryStore, partsOffsets, connectedMaterials);
}

DefaultMaterial* SolidMeshLoader::processConnectedMaterial(const MaterialDescription& materialDescription)
{
	if (m_resourceManager->isResourceLoaded(materialDescription.name))
		return dynamic_cast<DefaultMaterial*>(m_resourceManager->getResource<Material>(materialDescription.name));

	DefaultMaterial* material = new DefaultMaterial();

	material->setEmissiveColor(materialDescription.emissiveColor);
	material->setDiffuseColor(materialDescription.diffuseColor);

	material->setSpecularColor(materialDescription.specularColor);
	material->setSpecularFactor(materialDescription.specularFactor);

	if (!std::string(materialDescription.diffuseMap).empty()) {
		material->setDiffuseTexture(processConnectedTexture(materialDescription.diffuseMap));
	}

	if (!std::string(materialDescription.specularMap).empty()) {
		material->setSpecularTexture(processConnectedTexture(materialDescription.specularMap));
	}

	if (!std::string(materialDescription.normalMap).empty()) {
		material->setNormalMap(processConnectedTexture(materialDescription.normalMap));
	}

	m_resourceManager->registerResource(materialDescription.name, material);

	return material;
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

#include "FontLoader.h"

#include <pugixml.hpp>

#include "HoldingResource.h"
#include <Engine\Components\Math\Geometry\Rect.h>
#include <Engine\Utils\string.h>

#include <stb_image.h>
#include "ResourceLoadingException.h"

FontLoader::FontLoader(GraphicsResourceFactory* graphicsResourceFactory)
	: m_graphicsResourceFactory(graphicsResourceFactory) 
{
}

FontLoader::~FontLoader()
{
}

Resource * FontLoader::load(const std::string & filename)
{
	pugi::xml_document fontDescription;
	
	pugi::xml_parse_result result = fontDescription.load_file(filename.c_str());

	if (!result)
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), result.description(), __FILE__, __LINE__, __FUNCTION__);

	pugi::xml_node fontNode = fontDescription.child("font");
	std::string bitmapFilename = fontNode.attribute("bitmap").as_string();

	unsigned int baseSize = fontNode.attribute("size").as_uint();
	
	Texture* bitmap = loadBitmap(bitmapFilename);
	
	Font* font = new Font(bitmap);
	font->setBaseSize(baseSize);

	for (auto charNode : fontNode.children()) {
		std::vector<std::string> bitmapParts = StringUtils::split(charNode.attribute("rect").as_string(), ' ');

		Rect bitmapRect(std::stoi(bitmapParts[0]), 
			std::stoi(bitmapParts[1]), 
			std::stoi(bitmapParts[2]), 
			std::stoi(bitmapParts[3]));

		std::vector<std::string> offsetParts = StringUtils::split(charNode.attribute("offset").as_string(), ' ');

		ivector2 offset(std::stoi(offsetParts[0]), std::stoi(offsetParts[1]));
		unsigned int xAdvance = charNode.attribute("width").as_int();

		unsigned char character = charNode.attribute("code").as_string()[0];

		Character characterDescription;
		characterDescription.rectangleInAtlas = bitmapRect;
		characterDescription.xOffset = offset.x;
		characterDescription.yOffset = offset.y;
		characterDescription.xAdvance = xAdvance;
		characterDescription.uv.x = (float)bitmapRect.getPosition().x / bitmap->getWidth();
		characterDescription.uv.y = (float)(bitmap->getHeight() - bitmapRect.getPosition().y) / bitmap->getHeight();

		font->addCharacter(character, characterDescription);
	}

	return new HoldingResource<Font>(font);
}

Texture * FontLoader::loadBitmap(const std::string & filename)
{
	int width, height;
	int nrChannels;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (data == 0)
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), "", __FILE__, __LINE__, __FUNCTION__);

	if (nrChannels != 1)
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), "Font bitmap must contain only one channel", __FILE__, __LINE__, __FUNCTION__);

	Texture* texture = nullptr;

	try {
		texture = m_graphicsResourceFactory->createTexture();
		texture->setTarget(Texture::Target::_2D);
		texture->setInternalFormat(Texture::InternalFormat::R8);
		texture->setSize(width, height);

		texture->create();
		texture->bind();
		texture->setMinificationFilter(Texture::Filter::Linear);
		texture->setMagnificationFilter(Texture::Filter::Linear);

		texture->setData(Texture::PixelFormat::R, Texture::PixelDataType::UnsignedByte, (const std::byte*)data);
	}
	catch (const RenderSystemException& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), exception.what(), exception.getFile(), exception.getLine(), exception.getFunction());
	}

	stbi_image_free(data);

	return texture;
}

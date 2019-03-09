#pragma once

#include <pugixml.hpp>

class XMLSerializable {
public:
	virtual void serialize(pugi::xml_node& storage) const = 0;
	virtual void deserialize(const pugi::xml_node& storage) = 0;
};
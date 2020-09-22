#pragma once

#include <string>
#include <pugixml.hpp>

class XMLUtils {
 public:
  XMLUtils() = delete;

  static std::tuple<pugi::xml_document, pugi::xml_node> openDescriptionFile(const std::string& resourcePath,
    const std::string& descriptionNodeName);
};


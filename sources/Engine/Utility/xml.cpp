#include "precompiled.h"

#pragma hdrstop

#include "xml.h"

#include "Exceptions/exceptions.h"
#include "files.h"


std::tuple<pugi::xml_document, pugi::xml_node> XMLUtils::openDescriptionFile(const std::string& resourcePath,
  const std::string& descriptionNodeName)
{
  if (!FileUtils::isFileExists(resourcePath)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("XML description file doesn't exist: {}", resourcePath));
  }

  pugi::xml_document descDocument;
  pugi::xml_parse_result descParseResult = descDocument.load_file(resourcePath.c_str());

  if (!descParseResult) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("XML description file has invalid format: {}", resourcePath));
  }

  pugi::xml_node descriptionNode = descDocument.child(descriptionNodeName.c_str());

  if (!descriptionNode) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("XML description file hasn't description node {}: {}", descriptionNodeName, resourcePath));
  }

  return std::tuple<pugi::xml_document, pugi::xml_node>(std::move(descDocument), descriptionNode);
}

#pragma once

#include <memory>
#include <variant>
#include <any>
#include <string>
#include <vector>

#include "Exceptions/exceptions.h"
#include "Utility/strings.h"
#include "Utility/xml.h"
#include "Utility/TypeIdentifier.h"

constexpr size_t RESOURCE_ID_INVALID = std::numeric_limits<size_t>::max();
constexpr size_t TYPE_ID_INVALID = std::numeric_limits<size_t>::max();

struct ResourceTypeIdentifierHelper {

};

using ResourceTypeIdentifier = TypeIdentifier<ResourceTypeIdentifierHelper>;

class Resource {
 public:
  Resource() = default;
  virtual ~Resource() = default;

  inline void setTypeId(size_t typeId)
  {
    m_typeId = typeId;
  }

  inline void setResourceId(size_t resourceId)
  {
    m_resourceId = resourceId;
  }

  [[nodiscard]] inline size_t getTypeId() const
  {
    return m_typeId;
  }

  [[nodiscard]] inline size_t getResourceId() const
  {
    return m_resourceId;
  }

 private:
  size_t m_typeId = TYPE_ID_INVALID;
  size_t m_resourceId = RESOURCE_ID_INVALID;
};
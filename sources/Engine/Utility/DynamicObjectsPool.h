#pragma once

#include <cstddef>
#include <vector>

#include "swdebug.h"

class DynamicDataPool {
 public:
  explicit DynamicDataPool(size_t objectSize, size_t chunkCapacity)
    : m_objectSize(objectSize),
      m_chunkSize(chunkCapacity)
  {

  }

  virtual ~DynamicDataPool()
  {
    for (std::byte* chunkPtr : m_chunks) {
      delete[] chunkPtr;
    }
  }

  virtual inline void freeObject(size_t objectIndex)
  {
    SW_ASSERT(objectIndex < m_size);

    ARG_UNUSED(objectIndex);
  }

  void fit(size_t objectsCount)
  {
    SW_ASSERT(objectsCount >= m_size);

    if (objectsCount > m_capacity) {
      while (m_capacity < objectsCount) {
        m_chunks.push_back(new std::byte[m_objectSize * m_chunkSize]);
        m_capacity += m_chunkSize;
      }
    }

    m_size = objectsCount;
  }

  [[nodiscard]] inline size_t getSize() const
  {
    return m_size;
  }

  [[nodiscard]] inline const void* getObject(size_t objectIndex) const
  {
    SW_ASSERT(objectIndex < m_size);

    return m_chunks[objectIndex / m_chunkSize] +
      (objectIndex % m_chunkSize) * m_objectSize;
  }

  [[nodiscard]] inline void* getObject(size_t objectIndex)
  {
    SW_ASSERT(objectIndex < m_size);

    return m_chunks[objectIndex / m_chunkSize] +
      (objectIndex % m_chunkSize) * m_objectSize;
  }

 protected:
  std::vector<std::byte*> m_chunks;

  const size_t m_objectSize;
  const size_t m_chunkSize;

  size_t m_size{};
  size_t m_capacity{};
};

template<class ObjectType>
class DynamicObjectsPool final : public DynamicDataPool {
 public:
  explicit DynamicObjectsPool(size_t chunkSize)
    : DynamicDataPool(sizeof(ObjectType), chunkSize)
  {

  }

  ~DynamicObjectsPool() override = default;

  inline void freeObject(size_t objectIndex) override
  {
    SW_ASSERT(objectIndex < m_size);

    // Call destructor manually to properly remove object
    auto* object = static_cast<ObjectType*>(getObject(objectIndex));
    object->~ObjectType();
  }
};

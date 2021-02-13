#pragma once

#include <vector>

class BaseResourcesStorage {
 public:
  BaseResourcesStorage() = default;
  virtual ~BaseResourcesStorage() = default;

  [[nodiscard]] virtual size_t increaseStorageSize() = 0;
  virtual void freeResource(size_t resourceIndex) = 0;

  [[nodiscard]] virtual size_t getSize() const = 0;
};

// NOTE: it is impossible to use resources pool here because some engine resources
// are polymorphic


/**
 * @brief Generic resources storage, responsible for resources memory management
 *
 * Current implementation allows to use polymorphic types
 *
 * @tparam ResourceType
 */
template<class ResourceType>
class ResourcesStorage : public BaseResourcesStorage {
 public:
  ResourcesStorage() = default;
  ~ResourcesStorage() override = default;

  [[nodiscard]] size_t increaseStorageSize() override
  {
    m_resources.push_back(nullptr);

    return m_resources.size() - 1;
  }

  template<class ResourceInheritor, class... Args>
  inline ResourceType* allocateResource(size_t resourceIndex, Args&& ... args)
  {
    ResourceType* resourcePointer = new ResourceInheritor(std::forward<Args>(args)...);

    m_resources[resourceIndex] = resourcePointer;

    return resourcePointer;
  }

  void freeResource(size_t resourceIndex) override
  {
    delete m_resources[resourceIndex];
    m_resources[resourceIndex] = nullptr;
  }

  [[nodiscard]] ResourceType* getResource(size_t resourceIndex) const
  {
    return m_resources[resourceIndex];
  }

  [[nodiscard]] size_t getSize() const override
  {
    return m_resources.size();
  }

 private:
  // TODO: dynamic memory allocations lead to memory fragmentation
  //  so it will be better to use continuous memory storage
  std::vector<ResourceType*> m_resources;
};
#pragma once

#include <string>
#include <utility>

enum class ResourceLoadingState {
  Loaded, Unloaded
};

class ResourceState {
 public:
  explicit ResourceState(std::string resourceName)
    : m_resourceName(std::move(resourceName))
  {

  }

  virtual ~ResourceState() = default;

  inline void setLoadingState(ResourceLoadingState loadingState)
  {
    m_loadingState = loadingState;
  }

  [[nodiscard]] inline ResourceLoadingState getLoadingState() const
  {
    return m_loadingState;
  }

  [[nodiscard]] inline size_t getReferencesCount() const
  {
    return m_referencesCount;
  }

  inline void increaseReferencesCount()
  {
    m_referencesCount++;
  }

  inline void decreaseReferencesCount()
  {
    m_referencesCount--;
  }

  [[nodiscard]] inline const std::string& getResourceName() const
  {
    return m_resourceName;
  }

  [[nodiscard]] inline bool isPersistent() const
  {
    return m_isPersistent;
  }

 private:
  std::string m_resourceName;

  ResourceLoadingState m_loadingState = ResourceLoadingState::Unloaded;
  size_t m_referencesCount{};

  bool m_isPersistent{};
};
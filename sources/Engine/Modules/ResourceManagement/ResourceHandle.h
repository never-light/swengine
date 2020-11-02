#pragma once

class ResourcesManager;

template<class T>
class ResourceHandle {
 public:
  ResourceHandle() = default;
  ResourceHandle(size_t resourceIndex, T* resourcePtr, ResourcesManager* resourcesManager);
  ResourceHandle(const ResourceHandle<T>& handle);
  ResourceHandle(ResourceHandle<T>&& handle) noexcept;

  ~ResourceHandle();

  ResourceHandle<T>& operator=(const ResourceHandle<T>& other);
  ResourceHandle<T>& operator=(ResourceHandle<T>&& other) noexcept;

  inline T* get()
  {
    return m_resourcePtr;
  }

  inline T* get() const
  {
    return m_resourcePtr;
  }

  inline T& operator*()
  {
    return *m_resourcePtr;
  }

  inline const T& operator*() const
  {
    return *m_resourcePtr;
  }

  inline T* operator->()
  {
    return m_resourcePtr;
  }

  inline T* operator->() const
  {
    return m_resourcePtr;
  }

  [[nodiscard]] inline size_t getResourceIndex() const
  {
    return m_resourceIndex;
  }

  inline bool isValid() {
    return m_resourceIndex != RESOURCE_ID_INVALID;
  }

 private:

  size_t m_resourceIndex = RESOURCE_ID_INVALID;
  T* m_resourcePtr{};
  ResourcesManager* m_resourcesManager{};

 private:
  friend class ResourcesManager;
};

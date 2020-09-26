#pragma once

#include <vector>
#include <span>

#include "Modules/ECS/ECS.h"
#include "Modules/Graphics/GraphicsSystem/Camera.h"

/*struct SceneObjectsQueryResult {
 public:
  SceneObjectsQueryResult(std::span<GameObject> objectsList, size_t queryId)
    : m_objectsList(objectsList),
      m_queryId(queryId)
  {

  }

  const std::span<GameObject>& getObjectsList()
  {
    return m_objectsList;
  }

  [[nodiscard]] size_t getQueryId() const
  {
    return m_queryId;
  }

 private:
  std::span<GameObject> m_objectsList;
  size_t m_queryId;
};

struct SceneObjectQueriesPool {
 public:

  size_t allocateQueryResult() {
    if (!m_freeQueries.empty()) {
      size_t freePoolId = *m_freeQueries.rbegin();
      m_freeQueries.pop_back();

      return freePoolId;
    }
    else {
      m_queries.emplace_back();

      return m_queries.size() - 1;
    }
  }

  void releaseQueryResult(size_t queryId) {
    SW_ASSERT(queryId < m_queries.size());
    m_freeQueries.push_back(queryId);
  }

  [[nodiscard]] std::vector<GameObject>& getQueryData(size_t queryId) {
    return m_queries[queryId];
  }

  [[nodiscard]] const std::vector<GameObject>& getQueryData(size_t queryId) const {
    return m_queries[queryId];
  }

 private:
  std::vector<std::vector<GameObject>> m_queries;
  std::vector<size_t> m_freeQueries;
};*/

class SceneAccelerationStructure {
 public:
  SceneAccelerationStructure() = default;
  virtual ~SceneAccelerationStructure() = default;

  virtual void buildFromObjectsList(std::vector<GameObject>& objects) = 0;
  virtual void addObject(GameObject object) = 0;
  virtual void removeObject(GameObject object) = 0;

  virtual void queryNearestDynamicNeighbors(
    const glm::vec3& origin,
    float radius,
    std::vector<GameObject>& result) = 0;

  virtual void queryVisibleObjects(Camera& camera, std::vector<GameObject>& result) = 0;

  virtual void clear() = 0;

  [[nodiscard]] virtual size_t getObjectsCount() const = 0;
};

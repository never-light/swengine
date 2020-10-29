#pragma once

#include <memory>

struct FrameStats {
 public:
  FrameStats() = default;

  void reset();

  void increasePrimitivesCount(size_t count);
  void increaseSubMeshesCount(size_t count);
  void increaseCulledSubMeshesCount(size_t count);

  [[nodiscard]] size_t getPrimitivesCount() const;
  [[nodiscard]] size_t getSubMeshesCount() const;
  [[nodiscard]] size_t getCulledSubMeshesCount() const;

 private:
  size_t m_primitivesCount = 0;

  size_t m_subMeshesCount = 0;
  size_t m_culledSubMeshesCount = 0;
};


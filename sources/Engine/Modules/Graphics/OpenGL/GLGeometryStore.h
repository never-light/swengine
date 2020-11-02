#pragma once

#include <cstddef>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_precision.hpp>

#include "GL.h"
#include "GLDebug.h"

struct VertexFormatAttributeSpec {
  GLuint attribIndex{};
  GLuint bindingIndex{};
  GLint size{};
  GLenum type{};
  GLboolean normalized{};
  GLuint relativeOffset{};
};

// TODO: use SoA instead of AoS here
// Vertex formats should be represented as structs of arrays


struct VertexPos3Norm3UV {
  glm::vec3 pos = {0.0f, 0.0f, 0.0f};
  glm::vec3 norm = {0.0f, 0.0f, 0.0f};
  glm::vec2 uv = {0.0f, 0.0f};

  static std::vector<VertexFormatAttributeSpec> s_vertexFormatAttributes;
};

struct VerticesPos3Norm3UVSoA {
  std::vector<glm::vec3>* positions;
  std::vector<glm::vec3>* normals;
  std::vector<glm::vec2>* uv;

  static std::vector<VertexFormatAttributeSpec> s_vertexFormatAttributes;
};

struct VertexPos3Norm3UVSkinnedSoA {
  std::vector<glm::vec3>* positions;
  std::vector<glm::vec3>* normals;
  std::vector<glm::vec2>* uv;
  std::vector<glm::u8vec4>* bonesIds;
  std::vector<glm::u8vec4>* bonesWeights;

  static std::vector<VertexFormatAttributeSpec> s_vertexFormatAttributes;
};

struct VertexPos3Color4SoA {
  std::vector<glm::vec3>* positions;
  std::vector<glm::vec4>* colors;

  static std::vector<VertexFormatAttributeSpec> s_vertexFormatAttributes;
};

class GLGeometryStore {
 public:
  explicit GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices);
  GLGeometryStore(const std::vector<VertexPos3Norm3UV>& vertices, const std::vector<std::uint16_t>& indices);

  explicit GLGeometryStore(const VerticesPos3Norm3UVSoA& vertices);
  GLGeometryStore(const VerticesPos3Norm3UVSoA& vertices, const std::vector<std::uint16_t>& indices);

  explicit GLGeometryStore(const VertexPos3Color4SoA& vertices);
  GLGeometryStore(const VertexPos3Color4SoA& vertices, const std::vector<std::uint16_t>& indices);

  explicit GLGeometryStore(const VertexPos3Norm3UVSkinnedSoA& vertices);
  GLGeometryStore(const VertexPos3Norm3UVSkinnedSoA& vertices, const std::vector<std::uint16_t>& indices);

  ~GLGeometryStore();

  [[nodiscard]] size_t getVerticesCount() const;
  [[nodiscard]] size_t getIndicesCount() const;

  [[nodiscard]] bool isIndexed() const;

  void draw(GLenum primitivesType = GL_TRIANGLES) const;
  void drawRange(size_t start, size_t count, GLenum primitivesType = GL_TRIANGLES) const;

 private:
  template<class T, class DescriptionType>
  void setupVAO(const T& vertices, const std::vector<std::uint16_t>& indices);

  template<class T>
  void setupVertexBuffers(const T& vertices);

 private:
  std::array<GLuint, 6> m_vertexBuffers = {0, 0, 0, 0, 0, 0};
  GLuint m_indexBuffer = 0;
  GLuint m_vertexArrayObject = 0;

  size_t m_verticesCount = 0;
  size_t m_indicesCount = 0;
};

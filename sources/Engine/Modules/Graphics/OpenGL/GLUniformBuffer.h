#pragma once

#include "GL.h"
#include "types.h"
#include "GLDebug.h"

template<class T>
class GLUniformBuffer {
 public:
  GLUniformBuffer();
  ~GLUniformBuffer();

  [[nodiscard]] inline const T& getBufferData() const;
  [[nodiscard]] inline T& getBufferData();

  inline void attachToBindingEntry(size_t bindingEntry);
  inline void synchronizeWithGpu();

 private:
  T m_bufferData{};

  GLuint m_uniformBuffer{};
};

template<class T>
GLUniformBuffer<T>::GLUniformBuffer()
{
  GL_CALL_BLOCK_BEGIN();

  glCreateBuffers(1, &m_uniformBuffer);
  glNamedBufferData(m_uniformBuffer, sizeof(T), &m_bufferData, GL_DYNAMIC_DRAW);

  GL_CALL_BLOCK_END();
}

template<class T>
GLUniformBuffer<T>::~GLUniformBuffer()
{
  GL_CALL(glDeleteBuffers(1, &m_uniformBuffer));
}

template<class T>
inline const T& GLUniformBuffer<T>::getBufferData() const
{
  return m_bufferData;
}

template<class T>
inline T& GLUniformBuffer<T>::getBufferData()
{
  return m_bufferData;
}

template<class T>
void GLUniformBuffer<T>::attachToBindingEntry(size_t bindingEntry)
{
  GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(bindingEntry), m_uniformBuffer));
}

template<class T>
void GLUniformBuffer<T>::synchronizeWithGpu()
{
  GL_CALL(glNamedBufferSubData(m_uniformBuffer, 0, sizeof(T), &m_bufferData));
}

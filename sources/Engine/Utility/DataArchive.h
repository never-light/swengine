#pragma once

#include "InputDataArchive.h"
#include "OutputDataArchive.h"

#include "Modules/ResourceManagement/ResourceHandleImpl.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace glm {

template<class Archive>
inline void serialize(Archive& archive, glm::vec2& v)
{
  archive(v.x, v.y);
}

template<class Archive>
inline void serialize(Archive& archive, glm::vec3& v)
{
  archive(v.x, v.y, v.z);
}

template<class Archive>
inline void serialize(Archive& archive, glm::vec4& v)
{
  archive(v.x, v.y, v.z, v.w);
}

template<class Archive>
inline void serialize(Archive& archive, glm::quat& q)
{
  archive(q.x, q.y, q.z, q.w);
}

}

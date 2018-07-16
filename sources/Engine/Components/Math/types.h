#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>

typedef glm::vec2 vector2;
typedef glm::vec3 vector3;
typedef glm::vec4 vector4;
typedef glm::vec2 point2;
typedef glm::vec3 point3;
typedef glm::vec4 point4;
typedef glm::mat2 matrix2;
typedef glm::mat3 matrix3;
typedef glm::mat4 matrix4;
typedef glm::quat quaternion;

enum class CoordinateSystemType {
	Local, World, Parent
};
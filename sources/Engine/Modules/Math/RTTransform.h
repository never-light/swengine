#pragma once

// TODO: move methods here from BonePose

class RTTransform {
 public:
  RTTransform();
};

//
//#pragma once
//
//#include <glm/vec3.hpp>
//#include <glm/mat4.hpp>
//
//// TODO: move methods here from BonePose
//
//class SRTTransform {
// public:
//  SRTTransform();
//
// private:
//  glm::vec3 m_position;
//  glm::vec3 m_rotation;
//};
//
//inline BonePose operator*(const BonePose& a, const BonePose& b)
//{
//  BonePose result;
//  result.orientation = a.orientation * b.orientation;
//  result.position = a.position + glm::vec3(a.orientation * glm::vec4(b.position, 1.0f));
//
//  return result;
//}
//
//
//

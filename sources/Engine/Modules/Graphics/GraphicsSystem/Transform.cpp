#include "Transform.h"

Transform::Transform()
    : m_position(0.0f, 0.0f, 0.0f),
    m_scale(1.0f, 1.0f, 1.0f),
    m_orientation(glm::identity<glm::quat>()),
    m_fixedYAxis(false)
{

}

void Transform::move(float x, float y, float z)
{
    move(glm::vec3(x, y, z));
}

void Transform::move(const glm::vec3& movement)
{
    m_position += movement;

    resetTransformationCache();
}

void Transform::setPosition(float x, float y, float z)
{
    setPosition(glm::vec3(x, y, z));
}

void Transform::setPosition(const glm::vec3& position)
{
    m_position = position;

    resetTransformationCache();
}

glm::vec3 Transform::getPosition() const
{
    return m_position;
}

void Transform::scale(float x, float y, float z)
{
    scale(glm::vec3(x, y, z));
}

void Transform::scale(const glm::vec3& scale)
{
    m_scale *= scale;

    resetTransformationCache();
}

void Transform::setScale(float x, float y, float z)
{
    setScale(glm::vec3(x, y, z));
}

void Transform::setScale(const glm::vec3& scale)
{
    m_scale = scale;

    resetTransformationCache();
}

glm::vec3 Transform::getScale() const
{
    return m_scale;
}

void Transform::rotate(float x, float y, float z, float angle)
{
    rotate(glm::vec3(x, y, z), angle);
}

void Transform::rotate(const glm::vec3& axis, float angle)
{
    m_orientation *= glm::angleAxis(glm::radians(angle), axis);
    m_orientation = glm::normalize(m_orientation);

    resetTransformationCache();
}

void Transform::setOrientation(const glm::quat& orientation)
{
    m_orientation = orientation;

    resetTransformationCache();
}

glm::quat Transform::getOrientation() const
{
    return m_orientation;
}

glm::vec3 Transform::getFrontDirection() const {
    return glm::normalize(m_orientation * glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 Transform::getRightDirection() const {
    return glm::normalize(m_orientation * glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 Transform::getUpDirection() const {
    return glm::normalize(m_orientation * glm::vec3(0.0f, 1.0f, 0.0f));
}

void Transform::fixYAxis(bool fixed)
{
    m_fixedYAxis = fixed;
}

bool Transform::isYAxisFixed() const
{
    return m_fixedYAxis;
}

void Transform::yaw(float angle)
{
    if (m_fixedYAxis) {
        m_orientation = glm::quat(glm::vec3(0.0, glm::radians(angle), 0.0)) * m_orientation;
    }
    else {
        m_orientation *= glm::quat(glm::vec3(0.0, glm::radians(angle), 0.0));
    }

    m_orientation = glm::normalize(m_orientation);

    resetTransformationCache();
}

void Transform::pitch(float angle)
{
    rotate(glm::vec3(1.0f, 0.0f, 0.0f), angle);

    resetTransformationCache();
}

void Transform::roll(float angle)
{
    rotate(glm::vec3(0.0f, 0.0f, 1.0f), angle);

    resetTransformationCache();
}

float Transform::getYawValue() const {
    return glm::degrees(glm::eulerAngles(m_orientation).y);
}

float Transform::getPitchValue() const {
    return glm::degrees(glm::eulerAngles(m_orientation).x);
}

float Transform::getRollValue() const {
    return glm::degrees(glm::eulerAngles(m_orientation).z);
}

void Transform::lookAt(float x, float y, float z)
{
    lookAt(glm::vec3(x, y, z));
}

void Transform::lookAt(const glm::vec3& target)
{
    glm::mat3x3 m;
    m[2] = glm::normalize(m_position - target);
    m[0] = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m[2]));
    m[1] = glm::cross(m[2], m[0]);

    m_orientation = quat_cast(m);

    resetTransformationCache();
}

glm::mat4x4 Transform::getTransformationMatrix()
{
    if (m_needTransformationMatrixCacheUpdate) {
        m_transformationMatrixCache = glm::translate(glm::identity<glm::mat4x4>(), m_position) *
                glm::toMat4(m_orientation) *
                glm::scale(glm::identity<glm::mat4x4>(), m_scale);

        m_needTransformationMatrixCacheUpdate = false;
    }

    return m_transformationMatrixCache;
}

bool Transform::isCacheOutdated() const
{
    return m_needTransformationMatrixCacheUpdate;
}

void Transform::resetTransformationCache()
{
    m_needTransformationMatrixCacheUpdate = true;
}

#include "TransformComponent.h"

TransformComponent::TransformComponent()
    : m_position(0.0f, 0.0f, 0.0f),
    m_scale(1.0f, 1.0f, 1.0f),
    m_orientation(glm::quat()),
    m_fixedYAxis(false)
{

}

void TransformComponent::move(float x, float y, float z)
{
    move(glm::vec3(x, y, z));
}

void TransformComponent::move(const glm::vec3& movement)
{
    m_position += movement;

    resetTransformationCache();
}

void TransformComponent::setPosition(float x, float y, float z)
{
    setPosition(glm::vec3(x, y, z));
}

void TransformComponent::setPosition(const glm::vec3& position)
{
    m_position = position;

    resetTransformationCache();
}

glm::vec3 TransformComponent::getPosition() const
{
    return m_position;
}

void TransformComponent::scale(float x, float y, float z)
{
    scale(glm::vec3(x, y, z));
}

void TransformComponent::scale(const glm::vec3& scale)
{
    m_scale *= scale;

    resetTransformationCache();
}

void TransformComponent::setScale(float x, float y, float z)
{
    setScale(glm::vec3(x, y, z));
}

void TransformComponent::setScale(const glm::vec3& scale)
{
    m_scale = scale;

    resetTransformationCache();
}

glm::vec3 TransformComponent::getScale() const
{
    return m_scale;
}

void TransformComponent::rotate(float x, float y, float z, float angle)
{
    rotate(glm::vec3(x, y, z), angle);
}

void TransformComponent::rotate(const glm::vec3& axis, float angle)
{
    m_orientation *= glm::angleAxis(glm::radians(angle), axis);
    m_orientation = glm::normalize(m_orientation);

    resetTransformationCache();
}

void TransformComponent::setOrientation(const glm::quat& orientation)
{
    m_orientation = orientation;

    resetTransformationCache();
}

glm::quat TransformComponent::getOrientation() const
{
    return m_orientation;
}

glm::vec3 TransformComponent::getFrontDirection() const {
    return glm::normalize(m_orientation * glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 TransformComponent::getRightDirection() const {
    return glm::normalize(m_orientation * glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 TransformComponent::getUpDirection() const {
    return glm::normalize(m_orientation * glm::vec3(0.0f, 1.0f, 0.0f));
}

void TransformComponent::fixYAxis(bool fixed)
{
    m_fixedYAxis = fixed;
}

bool TransformComponent::isYAxisFixed() const
{
    return m_fixedYAxis;
}

void TransformComponent::yaw(float angle)
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

void TransformComponent::pitch(float angle)
{
    rotate(glm::vec3(1.0f, 0.0f, 0.0f), angle);

    resetTransformationCache();
}

void TransformComponent::roll(float angle)
{
    rotate(glm::vec3(0.0f, 0.0f, 1.0f), angle);

    resetTransformationCache();
}

float TransformComponent::getYawValue() const {
    return glm::degrees(glm::eulerAngles(m_orientation).y);
}

float TransformComponent::getPitchValue() const {
    return glm::degrees(glm::eulerAngles(m_orientation).x);
}

float TransformComponent::getRollValue() const {
    return glm::degrees(glm::eulerAngles(m_orientation).z);
}

void TransformComponent::lookAt(float x, float y, float z)
{
    lookAt(glm::vec3(x, y, z));
}

void TransformComponent::lookAt(const glm::vec3& target)
{
    glm::mat3x3 m;
    m[2] = glm::normalize(m_position - target);
    m[0] = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m[2]));
    m[1] = glm::cross(m[2], m[0]);

    m_orientation = quat_cast(m);

    resetTransformationCache();
}

glm::mat4x4 TransformComponent::getTransformationMatrix()
{
    if (m_needTransformationMatrixCacheUpdate) {
        m_transformationMatrixCache = glm::translate(glm::mat4x4(), m_position) *
                glm::toMat4(m_orientation) *
                glm::scale(glm::mat4x4(), m_scale);

        m_needTransformationMatrixCacheUpdate = false;
    }

    return m_transformationMatrixCache;
}

void TransformComponent::resetTransformationCache()
{
    m_needTransformationMatrixCacheUpdate = true;
}

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>

class TransformComponent
{
public:
    TransformComponent();

    void move(float x, float y, float z);
    void move(const glm::vec3& movement);

    void setPosition(float x, float y, float z);
    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition() const;

    void scale(float x, float y, float z);
    void scale(const glm::vec3& scale);

    void setScale(float x, float y, float z);
    void setScale(const glm::vec3& scale);
    glm::vec3 getScale() const;

    void rotate(float x, float y, float z, float angle);
    void rotate(const glm::vec3& axis, float angle);

    void setOrientation(const glm::quat& orientation);
    glm::quat getOrientation() const;

    glm::vec3 getFrontDirection() const;
    glm::vec3 getRightDirection() const;
    glm::vec3 getUpDirection() const;

    void fixYAxis(bool fixed = true);
    bool isYAxisFixed() const;

    void yaw(float angle);
    void pitch(float angle);
    void roll(float angle);

    float getYawValue() const;
    float getPitchValue() const;
    float getRollValue() const;

    void lookAt(float x, float y, float z);
    void lookAt(const glm::vec3& target);

    glm::mat4x4 getTransformationMatrix();

private:
    void resetTransformationCache();

private:
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::quat m_orientation;

    bool m_fixedYAxis;

    glm::mat4x4 m_transformationMatrixCache;
    bool m_needTransformationMatrixCacheUpdate;
};


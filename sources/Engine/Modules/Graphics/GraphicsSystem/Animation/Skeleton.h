#pragma once

#include <string>
#include <glm/mat4x4.hpp>

struct Bone {
public:
    Bone();
    Bone(const std::string& name, uint8_t parentId, const glm::mat4& inverseBindPoseMatrix);
    ~Bone() = default;

    void setName(const std::string& name);
    const std::string& getName() const;

    void setParentId(const uint8_t parentId);
    uint8_t getParentId() const;

    void setInverseBindPoseMatrix(const glm::mat4& inverseBindPoseMatrix);
    const glm::mat4& getInverseBindPoseMatrix() const;

private:
    std::string m_name;

    uint8_t m_parentId;
    glm::mat4 m_inverseBindPoseMatrix;
};

class Skeleton
{
public:
    Skeleton(const std::vector<Bone>& bones);
    ~Skeleton() = default;

    const Bone& getRootBone() const;

    uint8_t getBonesCount() const;
    const Bone& getBone(uint8_t id) const;

    uint8_t getBoneParentId(uint8_t id) const;

private:
    std::vector<Bone> m_bones;
};


#pragma once

#include <string>
#include <glm/mat4x4.hpp>

struct Bone {
public:
    Bone(const std::string& name, size_t parentId, const glm::mat4& inverseBindPoseMatrix);
    ~Bone() = default;

    void setName(const std::string& name);
    const std::string& getName() const;

    void setParentId(const size_t parentId);
    size_t getParentId() const;

    void setInverseBindPoseMatrix(const glm::mat4& inverseBindPoseMatrix);
    const glm::mat4& getInverseBindPoseMatrix() const;

private:
    std::string m_name;

    size_t m_parentId;
    glm::mat4 m_inverseBindPoseMatrix;
};

class Skeleton
{
public:
    Skeleton(const std::vector<Bone>& bones);
    ~Skeleton() = default;

    const Bone& getRootBone() const;

    size_t getBonesCount() const;
    const Bone& getBone(size_t id) const;

private:
    std::vector<Bone> m_bones;
};


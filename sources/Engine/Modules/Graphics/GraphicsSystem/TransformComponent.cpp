#include "TransformComponent.h"

TransformComponent::TransformComponent()
    : m_transform(std::make_unique<Transform>())
{

}

Transform* TransformComponent::getTransform() const
{
    return m_transform.get();
}

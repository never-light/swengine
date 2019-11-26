#include "PlayerControlSystem.h"
#include <Engine/Modules/ECS/GameWorld.h>

#include <spdlog/spdlog.h>

#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>

#include "PlayerComponent.h"

PlayerControlSystem::PlayerControlSystem(std::shared_ptr<InputModule> inputModule)
    : m_inputModule(inputModule)
{

}

PlayerControlSystem::~PlayerControlSystem()
{

}

void PlayerControlSystem::update(GameWorld* gameWorld, float delta)
{
    ARG_UNUSED(delta);

    if (m_playerObject == nullptr) {
        const auto& playerObjectsView = gameWorld->allWith<PlayerComponent, CameraComponent, TransformComponent>();

        m_playerObject = playerObjectsView.begin().getGameObject();
    }

    const auto& playerComponent = m_playerObject->getComponent<PlayerComponent>();
    float movementSpeed = playerComponent->getMovementSpeed();

    Camera* playerCamera = getPlayerCamera();

    if (m_inputModule->isActionActive("forward")) {
        move(playerCamera->getTransform()->getFrontDirection() * movementSpeed);
    }

    if (m_inputModule->isActionActive("backward")) {
        move(playerCamera->getTransform()->getFrontDirection() * movementSpeed * (-1.0f));
    }

    if (m_inputModule->isActionActive("right")) {
        move(playerCamera->getTransform()->getRightDirection() * movementSpeed);
    }

    if (m_inputModule->isActionActive("left")) {
        move(playerCamera->getTransform()->getRightDirection() * movementSpeed * (-1.0f));
    }

    //playerCamera->getTransform()->setPosition(getPlayerTransform()->getPosition());
}

Camera* PlayerControlSystem::getPlayerCamera() const
{
    return m_playerObject->getComponent<CameraComponent>()->getCamera().get();
}

Transform* PlayerControlSystem::getPlayerTransform() const
{
    return m_playerObject->getComponent<TransformComponent>()->getTransform();
}

void PlayerControlSystem::move(const glm::vec3& movement)
{
    getPlayerCamera()->getTransform()->move(movement);
}

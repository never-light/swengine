#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Input/InputModule.h>

class PlayerControlSystem : public GameSystem
{
public:
    PlayerControlSystem(std::shared_ptr<InputModule> inputModule);
    ~PlayerControlSystem() override;

    void configure(GameWorld *gameWorld) override;
    void unconfigure(GameWorld *gameWorld) override;

    void update(GameWorld* gameWorld, float delta) override;

private:
    Camera* getPlayerCamera() const;
    Transform* getPlayerTransform() const;

    void move(const glm::vec3& movement);

private:
    GameObject* m_playerObject = nullptr;

    std::shared_ptr<InputModule> m_inputModule;

};


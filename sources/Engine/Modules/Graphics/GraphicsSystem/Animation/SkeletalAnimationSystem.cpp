#include "precompiled.h"

#pragma hdrstop

#include "SkeletalAnimationSystem.h"

SkeletalAnimationSystem::SkeletalAnimationSystem() {

}

SkeletalAnimationSystem::~SkeletalAnimationSystem() {

}

void SkeletalAnimationSystem::configure(GameWorld* gameWorld) {
  ARG_UNUSED(gameWorld);
}

void SkeletalAnimationSystem::unconfigure(GameWorld* gameWorld) {
  ARG_UNUSED(gameWorld);
}

void SkeletalAnimationSystem::update(GameWorld* gameWorld, float delta) {
  for (const GameObject* obj : gameWorld->allWith<SkeletalAnimationComponent>()) {
    const auto& animationComponent = obj->getComponent<SkeletalAnimationComponent>();

    updateAnimationStateMachine(animationComponent->getAnimationStatesMachine(), delta);
  }
}

void SkeletalAnimationSystem::updateAnimationStateMachine(SkeletalAnimationStatesMachine& stateMachine, float delta) {
  stateMachine.increaseCurrentTime(delta);
}

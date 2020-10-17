#ifndef PROJECTILEEMITCOMPONENT_H
#define PROJECTILEEMITCOMPONENT_H

#include "../../lib/glm/glm.hpp"
#include "../Game.h"
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "./TransformComponent.h"

class ProjectileEmitComponent : public Component {
   private:
    TransformComponent* transform;
    glm::vec2 origin;
    int speed;
    int range;
    // Radian is better for calculation for game engine
    float angleRad;
    bool isLoop;

   public:
    ProjectileEmitComponent(int speed, int degree, int range, bool isLoop)  // Degree is easier for user to input
        : speed(speed), range(range), isLoop(isLoop) {
        angleRad = glm::radians(static_cast<float>(degree));
    }

    void Initialize() override {
        transform = owner->GetComponent<TransformComponent>();
        origin = glm::vec2(transform->position.x, transform->position.y);
        transform->velocity = glm::vec2(glm::cos(angleRad) * this->speed, glm::sin(angleRad) * this->speed);
    }

    void Update(float deltaTime) override {
        if (glm::distance(transform->position, origin) > range) {
            if (isLoop) {
                transform->position.x = origin.x;
                transform->position.y = origin.y;
            } else {
                owner->Destroy();
            }
        }
    }
};

#endif
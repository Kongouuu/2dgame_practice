#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../Components/SpriteComponent.h"     // Change direction animation
#include "../Components/TransformComponent.h"  // Change direction
#include "../EntityManager.h"
#include "../Game.h"

class KeyboardControlComponent : public Component {
   public:
    std::string upKey;
    std::string leftKey;
    std::string rightKey;
    std::string downKey;
    std::string fireKey;
    TransformComponent *transform;
    SpriteComponent *sprite;
    KeyboardControlComponent() {
    }

    // Initialize the keys from lua script
    KeyboardControlComponent(std::string upKey, std::string downKey, std::string leftKey, std::string rightKey, std::string fireKey) {
        // SDL's input hook is not in <string>
        this->upKey = GetSDLKeyStringCode(upKey);
        this->downKey = GetSDLKeyStringCode(downKey);
        this->leftKey = GetSDLKeyStringCode(leftKey);
        this->rightKey = GetSDLKeyStringCode(rightKey);
        this->fireKey = GetSDLKeyStringCode(fireKey);
    }

    std::string GetSDLKeyStringCode(std::string key) {
        // Arrow keys are special
        if (key.compare("up") == 0)
            return "1073741906";
        if (key.compare("down") == 0)
            return "1073741905";
        if (key.compare("left") == 0)
            return "1073741904";
        if (key.compare("right") == 0)
            return "1073741903";
        // Other keys are in ascii
        // Cast to int for char to ascii code, then back to string
        return std::to_string(static_cast<int>(key[0]));
    }

    void Initialize() override {
        transform = owner->GetComponent<TransformComponent>();
        sprite = owner->GetComponent<SpriteComponent>();
    }

    void Update(float deltaTime) {
        // When keydown
        if (Game::event.type == SDL_KEYDOWN) {
            // Make input key into keycode
            std::string keyCode = std::to_string(Game::event.key.keysym.sym);
            if (keyCode.compare(upKey) == 0) {
                sprite->Play("UpAnimation");
                transform->velocity.x = 0;
                transform->velocity.y = -30;
            }
            if (keyCode.compare(downKey) == 0) {
                sprite->Play("DownAnimation");
                transform->velocity.x = 0;
                transform->velocity.y = 30;
            }
            if (keyCode.compare(leftKey) == 0) {
                sprite->Play("LeftAnimation");
                transform->velocity.x = -30;
                transform->velocity.y = 0;
            }
            if (keyCode.compare(rightKey) == 0) {
                sprite->Play("RightAnimation");
                transform->velocity.x = 30;
                transform->velocity.y = 0;
            }
            if (keyCode.compare(fireKey) == 0) {
                // TODO
            }
        }

        
        if (Game::event.type == SDL_KEYUP) {
            std::string keyCode = std::to_string(Game::event.key.keysym.sym);
            if (keyCode.compare(upKey) == 0) {
                transform->velocity.x = 0;
                transform->velocity.y = 0;
            }
            if (keyCode.compare(downKey) == 0) {
                transform->velocity.x = 0;
                transform->velocity.y = 0;
            }
            if (keyCode.compare(leftKey) == 0) {
                transform->velocity.x = 0;
                transform->velocity.y = 0;
            }
            if (keyCode.compare(rightKey) == 0) {
                transform->velocity.x = 0;
                transform->velocity.y = 0;
            }
            if (keyCode.compare(fireKey) == 0) {
                // TODO
            }
        }
    }
};

#endif
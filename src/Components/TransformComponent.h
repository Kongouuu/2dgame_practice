#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "../EntityManager.h" // It includes other header here
#include "../../lib/glm/glm.hpp"
#include <SDL2/SDL.h>
#include "../Game.h"
#include "../Constants.h"

// Inherit from Component
class TransformComponent: public Component {
    public:
        glm::vec2 position;
        glm::vec2 velocity;
        int width;
        int height;
        int scale;
        bool hasDirection;
        DirectionType direction;

        TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s, bool hasDirection){
            position = glm::vec2(posX,posY);
            velocity = glm::vec2(velX,velY);
            width = w;
            height = h;
            scale = s;
            this->hasDirection = hasDirection;
            direction = DOWN;
        }

        void Initialize() override {

        }

        void Update(float deltaTime) override {
            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;

        }

        void Render() override {
            // Dummy render
        }
        
        void List() override {
            std::cout<<"        Component<TransformComponent>"<<std::endl;
        }
};

#endif
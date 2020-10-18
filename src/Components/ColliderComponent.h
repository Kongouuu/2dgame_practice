#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL2/SDL.h>

#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"
#include "../AssetManager.h"


/* 每个entity虽然transform里面有xyhw
 * 但实际上渲染的时候hitbox不在那个位置（因为调了camera)
 * 这就是给那个sprite加一层触碰判定
 */


class ColliderComponent: public Component {
    public:
        std::string tag;
        SDL_Rect collider;
        SDL_Rect sourceRect;
        SDL_Rect destinationRect;
        SDL_Texture* texture;
        TransformComponent* transform;

        ColliderComponent(std::string tag)
        :tag(tag){
            collider = {0,0,0,0};
            texture = nullptr;
        }

        ColliderComponent(std::string textureId, std::string tag)
        :tag(tag){
            collider = {0,0,0,0};
            texture = Game::assetManager->GetTexture(textureId);
        }

        void Initialize() override {
            transform = owner->GetComponent<TransformComponent>();
            sourceRect = {0,0, transform->width, transform->height};
            destinationRect = {collider.x, collider.y, collider.w, collider.h};
        }

        void Update(float deltaTime) override {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
            destinationRect.x = collider.x - Game::camera.x;
            destinationRect.y = collider.y - Game::camera.y;
        }

        void Render() override {
            // Hard coded so far
            if(texture!=nullptr){
                SDL_Rect borderTexture = {0,0,32,32};
                TextureManager::Draw(texture, borderTexture, destinationRect, SDL_FLIP_NONE);
            }
        }
};

#endif
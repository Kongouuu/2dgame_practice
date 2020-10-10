#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>

#include "../Animation.h"
#include "../AssetManager.h"
#include "../TextureManager.h"
#include "TransformComponent.h"

class SpriteComponent : public Component {
   private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect sourceRect;
    SDL_Rect destinationRect;
    bool isAnimated;
    bool isFixed;  // For user interface, fixed position

    int numFrames;
    int animationSpeed;
    std::map<std::string, Animation> animations;
    std::string currentAnimationName;
    unsigned int animationIndex;

   public:
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    // For non-animating sprite
    SpriteComponent(const char* assetTextureId, bool isFixed)  // SDL dont use string, use char*
        : isFixed(isFixed) {
        isAnimated = false;
        SetTexture(assetTextureId);
    }

    // Animating sprite, the problem is with fixed numFrames and animationSpeed
    SpriteComponent(const char* assetTextureId, int numFrames, int animationSpeed, bool hasDirection, bool isFixed)
        : numFrames(numFrames), animationSpeed(animationSpeed), isFixed(isFixed) {
        this->isAnimated = true;
        if (hasDirection) {
            /* 
             * It seems like a waste to add an object of redundant "numFrames" "animation Speed
             * If the program wants to have an object with different animations of different frame/speed
             * The constructure should NOT be designed this way
             */
            Animation downAnimation = Animation(0, numFrames, animationSpeed);
            Animation rightAnimation = Animation(1, numFrames, animationSpeed);
            Animation leftAnimation = Animation(2, numFrames, animationSpeed);
            Animation upAnimation = Animation(3, numFrames, animationSpeed);

            animations["DownAnimation"] = downAnimation;
            animations["RightAnimation"] = rightAnimation;
            animations["LeftAnimation"] = leftAnimation;
            animations["UpAnimation"] = upAnimation;
            this->animationIndex = 0;
            // Initialize with down
            this->currentAnimationName = "DownAnimation";
        } else {
            // Objects without direction
            Animation singleAnimation = Animation(0, numFrames, animationSpeed);
            animations["SingleAnimation"] = singleAnimation;
            this->animationIndex = 0;
            this->currentAnimationName = "SingleAnimation";
        }

        // Play(this->currentAnimationName)  <------ WHY implement this? WHY?
        SetTexture(assetTextureId);
    }

    // Useless function that the course tells me to implement
    // void Play(std::string animationName) {
    //     this->numFrames = animations[animationName].numFrames;
    //     this->animationIndex = animations[animationName].index;
    //     this->animationSpeed = animations[animationName].animationSpeed;
    //     this->currentAnimationName = animationName;
    // }

    void SetTexture(std::string assetTextureId) {
        texture = Game::assetManager->GetTexture(assetTextureId);
    }

    void Initialize() override {
        // Owner is the entity
        transform = owner->GetComponent<TransformComponent>();
        // Set the source position as 0
        sourceRect.x = 0;
        sourceRect.y = 0;
        sourceRect.w = transform->width;
        sourceRect.h = transform->height;
    }

    void Update(float deltaTime) override {
        // This is static way of doing (int)value  ---- static_cast<int>(value)
        if (isAnimated) {
            sourceRect.x = sourceRect.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames);
        }
        sourceRect.y = animationIndex * transform->height;
        destinationRect.x = static_cast<int>(transform->position.x);
        destinationRect.y = static_cast<int>(transform->position.y);
        destinationRect.w = transform->width * transform->scale;
        destinationRect.h = transform->height * transform->scale;
    }

    void Render() override {
        TextureManager::Draw(texture, sourceRect, destinationRect, spriteFlip);
    }

    void List() override {
        std::cout << "        Component<SpriteComponent>" << std::endl;
    }
};

#endif
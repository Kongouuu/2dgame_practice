#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <SDL2/SDL.h>

#include "../AssetManager.h"
#include "../EntityManager.h"
#include "../TextureManager.h"
#include "../../lib/glm/glm.hpp"

class TileComponent : public Component {
   public:
    SDL_Texture* texture;
    SDL_Rect sourceRect;
    SDL_Rect destinationRect;
    glm::vec2 position;

    TileComponent(int sourceRectX, int sourceRectY, int x, int y, int tileSize, int tileScale, std::string assetTextureId){
        texture = Game::assetManager->GetTexture(assetTextureId);
        sourceRect.x = sourceRectX;
        sourceRect.y = sourceRectY;
        sourceRect.w = tileSize;
        sourceRect.h = tileSize;

        destinationRect.x = x;
        destinationRect.y = y;
        destinationRect.w = tileSize*tileScale;
        destinationRect.h = tileSize*tileScale;

        position.x = x;
        position.y = y;
    }

    ~TileComponent(){
        SDL_DestroyTexture(texture);
    }

    void Update(float deltaTime) override {
        // tile position is relative to player position
        // tile position - player position + width/2 makes the tile relative to player
        destinationRect.x = position.x - Game::camera.x;
        destinationRect.y = position.y - Game::camera.y;
    }

    void Render() override {
        TextureManager::Draw(texture, sourceRect, destinationRect, SDL_FLIP_NONE);
    }
};

#endif
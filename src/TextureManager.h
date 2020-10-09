#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "./Game.h" //SDL library

class TextureManager {
    public:
        // Static to be accessed everywhere (share same object)
        static SDL_Texture* LoadTexture(const char* fileName);
        static void Draw(SDL_Texture* texture, SDL_Rect sourceRect
        , SDL_Rect destinationRect, SDL_RendererFlip flip);
};

#endif
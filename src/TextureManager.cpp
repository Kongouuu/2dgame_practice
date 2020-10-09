#include "./TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName){
    // Loading a file surface
    SDL_Surface* surface = IMG_Load(fileName);
    // Create a texture from loaded surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    // Free surface buffer
    SDL_FreeSurface(surface);
    return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_RendererFlip flip){
    // Renderer, texture, source(from texture), destination (from renderer),angle, center, flip
    SDL_RenderCopyEx(Game::renderer, texture, &sourceRect, &destinationRect, 0, 0, flip);
}
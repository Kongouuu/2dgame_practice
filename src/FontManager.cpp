#include "./FontManager.h"

TTF_Font* FontManager::LoadFont(const char* fileName, int fontSize){
    // Loading a file surface
    return TTF_OpenFont(fileName, fontSize);
}

void FontManager::Draw(SDL_Texture* texture, SDL_Rect position){
    // Renderer, texture, source(from texture), destination (from renderer),angle, center, flip
    // The &source rect is how much from the texture paper (usualy many textures on one paper)
    // The destinationRect is what is originally rendering
    SDL_RenderCopyEx(Game::renderer, texture, NULL, &position, 0, 0, SDL_FLIP_NONE);
}
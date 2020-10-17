#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <SDL2/SDL_ttf.h>
#include "./Game.h" //SDL library

class FontManager {
    public:
        // Static to be accessed everywhere (share same object)
        static TTF_Font* LoadFont(const char* fileName, int fontSize);
        static void Draw(SDL_Texture* texture, SDL_Rect position);
};

#endif
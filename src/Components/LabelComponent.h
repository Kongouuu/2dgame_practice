#ifndef LABELCOMPONENT_H
#define LABELCOMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../FontManager.h"
#include "../EntityManager.h"
#include "../AssetManager.h"

class LabelComponent: public Component {
    private:
        SDL_Rect position;
        std::string text;
        std::string fontId;
        SDL_Color color;
        SDL_Texture* texture;
    public:
        LabelComponent(int x, int y, std::string text, std::string fontId, const SDL_Color &color)
        :text(text),fontId(fontId),color(color){
            this->position.x = x;
            this->position.y = y;
            SetLabel(text, fontId);
        }   

        void SetLabel(std::string text, std::string fontId){
            SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->GetFont(fontId), text.c_str(), color);
            texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
            SDL_FreeSurface(surface);
            SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
        }

        void Render() override {
            FontManager::Draw(texture,position);
        }
};

#endif
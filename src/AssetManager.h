#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include <SDL2/SDL_ttf.h>
#include "./TextureManager.h"
#include "./FontManager.h"
class EntityManager;

class AssetManager {
    private:
        EntityManager* manager;
        // Red black tree
        std::map<std::string,SDL_Texture*> textures;
        std::map<std::string,TTF_Font*> fonts;
    public:
        AssetManager(EntityManager* manager);
        ~AssetManager();
        void ClearData();

        void AddTexture(std::string textureId, const char* filePath);
        SDL_Texture* GetTexture(std::string textureID);

        void AddFont(std::string fontId, const char* filePath, int fontSize);
        TTF_Font* GetFont(std::string fontId);
};

#endif
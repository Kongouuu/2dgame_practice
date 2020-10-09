#include "./AssetManager.h"

// C++ way of initializing the argument to object variable
AssetManager::AssetManager(EntityManager* manager): manager(manager){

}

void AssetManager::ClearData(){
    textures.clear();
}

void AssetManager::AddTexture(std::string textureId, const char* filePath){
    textures[textureId] = TextureManager::LoadTexture(filePath);
}

SDL_Texture* AssetManager::GetTexture(std::string textureId) {
    return textures[textureId];
}
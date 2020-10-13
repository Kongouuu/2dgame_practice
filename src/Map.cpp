#include "./Game.h"
#include "./Map.h"
#include "./EntityManager.h"
#include "./Components/TileComponent.h"

extern EntityManager manager;
Map::Map(std::string textureId, int scale, int tileSize)
    :textureId(textureId), tileScale(scale), tileSize(tileSize){
       newMap = &(manager.AddEntity("Tile",TILEMAP_LAYER));
       // Originally empty
}

void Map::LoadMap(std::string filePath, int mapWidth, int mapHeight){
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
    // Read Map
    std::fstream mapFile;
    mapFile.open(filePath);
    for(int y=0; y<mapHeight;y++){
        for(int x=0; x<mapWidth; x++){
            /*
             * Here takes single char by single, becausethe first char is the row
             * The second char is the column from the TEXTURE SHEET
             */
            char ch;
            mapFile.get(ch);
            int sourceRectY = atoi(&ch) * tileSize; // atoi takes in pointer (since usually is char *)
            mapFile.get(ch);
            int sourceRectX = atoi(&ch) * tileSize;

            AddTile(sourceRectX,sourceRectY,x*tileScale*tileSize,y*tileScale*tileSize);
            mapFile.ignore(); // pop the next char and throw it away
        }
    }
    mapFile.close();
}

int Map::GetWidth(){
    return mapWidth*tileScale*tileSize;
}

int Map::GetHeight(){
    return mapHeight*tileScale*tileSize;

}
void Map::AddTile(int sourceRectX, int sourceRectY, int x, int y){
    // Entity& newTile(manager.AddEntity("Tile"));
    newMap->AddComponent<TileComponent>(sourceRectX,sourceRectY,x,y,tileSize,tileScale,textureId);
};
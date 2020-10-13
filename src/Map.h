#ifndef MAP_H
#define MAP_H

#include <string>
#include <fstream>
#include "./EntityManager.h"
#include "./Components/TileComponent.h"

extern EntityManager manager;

class Map {
    private:
        std::string textureId;
        int tileScale;
        int tileSize;
        int mapWidth;
        int mapHeight;
        Entity* newMap;
    public:
        Map(std::string textureId, int scale, int tileSize);
        ~Map();
        // Load the map file
        void LoadMap(std::string filePath, int mapWidth, int mapHeight);
        int GetWidth();
        int GetHeight();
        // Load the tile based on map file loaded
        void AddTile(int souceX, int sourceY, int x, int y);
        
};

#endif
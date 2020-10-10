#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "./Constants.h"
#include "./Entity.h"
#include "./Component.h"
#include "./EntityManager.h"

class AssetManager;

class Game {
    private:
        bool isRunning;
        int ticksLastFrame;
        SDL_Window *window;     // window of game
        
    public:
        Game(); //Constructor
        ~Game(); //Destructor
        static AssetManager * assetManager;
        static SDL_Renderer *renderer; // renderer of game
        void LoadLevel(int levelNumber);
        bool IsRunning() const;
        void Initialize(int width, int height);
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
};
#endif
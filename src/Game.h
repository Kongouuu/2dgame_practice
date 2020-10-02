#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Game {
    private:
        bool isRunning;
        int ticksLastFrame;
        SDL_Window *window;     // window of game
        SDL_Renderer *renderer; // renderer of game

    public:
        Game(); //Constructor
        ~Game(); //Destructor
        bool IsRunning() const;
        void Initialize(int width, int height);
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
};
#endif
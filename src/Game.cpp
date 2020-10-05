#include <iostream>
#include "./Game.h"

EntityManager manager;
SDL_Renderer* Game::renderer;

// Constructor
Game::Game() {
    isRunning = false;
}

// Destructor
Game::~Game() {
}

// Vector in glm library
glm::vec2 projectilePos = glm::vec2(0.0f, 0.0f);
glm::vec2 projectileVel = glm::vec2(40.0f, 30.0f);

// Getter of isRunning state
bool Game::IsRunning() const {
    return isRunning;
}

// Initializer
void Game::Initialize(int width, int height) {
    // SDL_Init initializes the library
    // If not zero, there is error
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    // *** Create Window ***
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,  // x position of window
        SDL_WINDOWPOS_CENTERED,  // y position of window
        width,
        height,
        0
    );

    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    // *** Create Renderer ***
    // Second option is about driver, -1 is default
    // Third option is about flag, set to nothing by 0
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    // If no error, set running to true
    isRunning = true;
    return;
};

// Input processor
void Game::ProcessInput() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        // Clicking the X on the window
        case SDL_QUIT: {
            isRunning = false;
            break;
        }
        // If keydown
        case SDL_KEYDOWN: {
            // If keydown is escape
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
        }
        default: {
            break;
        }
    }
};

void Game::Update() {
    // Wait until 16ms if last frame is too fast
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME))0;

    // Time passed in ticks since last frame
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    // Set maximum delta time
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Record current tick
    ticksLastFrame = SDL_GetTicks();

    // Update projectile position
    projectilePos = glm::vec2(
        projectilePos.x + projectileVel.y * deltaTime,
        projectilePos.y + projectileVel.y * deltaTime
    );
}

void Game::Render() {
    // Set up rgb and alpha
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    // Clear back buffer [SDL has double buffer]
    SDL_RenderClear(renderer);

    // Struct a rectangle
    SDL_Rect projectile{
        (int)projectilePos.x,
        (int)projectilePos.y,
        10,
        10
    };

    // *** Draw the rectangle ***
    // Set color of rectangle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &projectile);

    SDL_RenderPresent(renderer);
};

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
};
#include <iostream>
#include "./Game.h"
#include "./AssetManager.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/ColliderComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Map.h"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

Map* map;
// Constructor
Game::Game() {
    isRunning = false;
}

// Destructor
Game::~Game() {
}

// Vector in glm library


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
 
    LoadLevel(0);
    manager.ListEntity();
    // If no error, set running to true
    isRunning = true;
    return;
};

Entity& player(manager.AddEntity("Player", PLAYER_LAYER ));

void Game::LoadLevel(int levelNumber){
    // Load assets
    assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
    assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
    assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
    assetManager->AddTexture("jungle-map", std::string("./assets/tilemaps/jungle.png").c_str());
    assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
    assetManager->AddTexture("collision-border", std::string("./assets/images/collision-texture.png").c_str());

    // Add map
    map = new Map("jungle-map",2,32);
    map->LoadMap("./assets/tilemaps/jungle.map",25,20);

    

    // Adding entities&components
    Entity& tankEntity(manager.AddEntity("Tank",ENEMY_LAYER));
    tankEntity.AddComponent<TransformComponent>(0,0,20,0,32,32,1);
    tankEntity.AddComponent<SpriteComponent>("tank-image",false);
    tankEntity.AddComponent<ColliderComponent>("collision-border","ENEMY",32,32);

    player.AddComponent<TransformComponent>(400,300,0,0,32,32,1);
    player.AddComponent<SpriteComponent>("chopper-image",2,90,true,false); // (assetID, frames, speed, direction, fixed)
    player.AddComponent<KeyboardControlComponent>("up","down","left","right","fire");
    player.AddComponent<ColliderComponent>("collision-border","PLAYER",32,32);

    // Add the heliport
    Entity& heliportEntity(manager.AddEntity("Heliport",OBSTACLE_LAYER));
    heliportEntity.AddComponent<TransformComponent>(470,420,0,0,32,32,1);
    heliportEntity.AddComponent<SpriteComponent>("heliport-image");
    heliportEntity.AddComponent<SpriteComponent>("heliport-image");
    heliportEntity.AddComponent<ColliderComponent>("LEVEL_COMPLETE",32,32);
    
    Entity& radarEntity(manager.AddEntity("Radar",UI_LAYER));
    radarEntity.AddComponent<TransformComponent>(720,15,0,0,64,64,1);
    radarEntity.AddComponent<SpriteComponent>("radar-image",8,300,false,true);
    
}

// Input processor
void Game::ProcessInput() {
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
    manager.Update(deltaTime);

    HandleCameraMovement();
    CheckCollision();
}

void Game::Render() {
    // Set up rgb and alpha
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    // Clear back buffer [SDL has double buffer]
    SDL_RenderClear(renderer);
    if(manager.HasNoEntities()){
        return;
    }
    manager.Render();
    SDL_RenderPresent(renderer);
};

void Game::HandleCameraMovement(){
    TransformComponent *playerPos = player.GetComponent<TransformComponent>();
    // Keep the player in the middle of the screen 
    camera.x = playerPos->position.x - (WINDOW_WIDTH/2);
    camera.y = playerPos->position.y - (WINDOW_HEIGHT/2);

    // If player position - half window < 0, dont move camera
    camera.x = camera.x<0?0:camera.x;
    camera.y = camera.y<0?0:camera.y;
    // If reached outer border, stop camera at outer border
    camera.x = playerPos->position.x + (WINDOW_WIDTH/2)>map->GetWidth()?map->GetWidth() - WINDOW_WIDTH:camera.x;
    camera.y = playerPos->position.y + (WINDOW_HEIGHT/2)>map->GetHeight()?map->GetHeight() - WINDOW_HEIGHT:camera.y;

}

void Game::CheckCollision(){
    CollisionType collision = manager.CheckCollision();
    switch(collision){
        case PLAYER_ENEMY:
            ProcessGameOver();
            break;
        case PLAYER_LEVEL_COMPLETE:
            ProcessNextLevel();
            break;
        default:
            break;
    }
}

void Game::ProcessGameOver(){
    std::cout<<"Game Over!";
    isRunning = false;
}

void Game::ProcessNextLevel(){
    std::cout<<"Next Level!";
    isRunning = false;
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
};
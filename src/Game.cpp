#include "./Game.h"

#include <iostream>

#include "./AssetManager.h"
#include "./Components/ColliderComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/LabelComponent.h"
#include "./Components/ProjectileEmitComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/TransformComponent.h"
#include "./Map.h"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Entity* player = nullptr;

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

    if (TTF_Init() != 0) {
        std::cerr << "Error initializing SDL TTF" << std::endl;
    }

    // *** Create Window ***
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,  // x position of window
        SDL_WINDOWPOS_CENTERED,  // y position of window
        width,
        height,
        0);

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

    LoadLevel(1);
    manager.ListEntity();
    // If no error, set running to true
    isRunning = true;
    return;
};

void Game::LoadLevel(int levelNumber) {
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    std::string level = "Level" + std::to_string(levelNumber);
    lua.script_file("./assets/scripts/" + level + ".lua");
    sol::table levelData = lua[level];

    /******************/
    /* Loading Assets */
    /******************/
    sol::table levelAssets = levelData["assets"];

    unsigned int assetIndex = 0;
    while (true) {
        // Check exists
        sol::optional<sol::table> existsAssetNode = levelAssets[assetIndex];
        if (existsAssetNode == sol::nullopt) {
            break;
        } else {
            // If asset exist
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];
            // If asset type is a texture
            if (assetType == "texture") {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                assetManager->AddTexture(assetId, assetFile.c_str());
            }
        }
        assetIndex++;
    }

    /******************/
    /* Loading Map */
    /******************/
    sol::table levelMap = levelData["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    map = new Map(mapTextureId, static_cast<int>(levelMap["scale"]), static_cast<int>(levelMap["tileSize"]));
    map->LoadMap(mapFile, static_cast<int>(levelMap["mapSizeX"]), static_cast<int>(levelMap["mapSizeY"]));

    /******************/
    /* Loading Assets */
    /******************/
    sol::table levelEntities = levelData["entities"];
    unsigned int entityIndex = 0;
    while (true) {
        // Check exists
        sol::optional<sol::table> existsEntityNode = levelEntities[entityIndex];
        if (existsEntityNode == sol::nullopt) {
            break;
        } else {
            // If entity exist, add entity
            sol::table entityData = levelEntities[entityIndex];
            std::string entityName = entityData["name"];
            LayerType layer = static_cast<LayerType>(entityData["layer"]);
            Entity& entity(manager.AddEntity(entityName, layer));

            // If entity is player, bind it to player
            if (entityName == "player")
                player = &entity;

            // Load components of entity
            sol::table entityComponents = entityData["components"];
            sol::table component; // Dummy component

            // Check if component exist
            if (sol::optional<sol::table> existsComponent = entityComponents["transform"]) {
                component = entityComponents["transform"];
                entity.AddComponent<TransformComponent>(
                    static_cast<int>(component["position"]["x"]),
                    static_cast<int>(component["position"]["y"]),
                    static_cast<int>(component["velocity"]["x"]),
                    static_cast<int>(component["velocity"]["y"]),
                    static_cast<int>(component["width"]),
                    static_cast<int>(component["height"]),
                    static_cast<int>(component["scale"]),
                    static_cast<bool>(component["rotation"]));
            }

            // Check if component exist
            if (sol::optional<sol::table> existsComponent = entityComponents["sprite"]) {
                component = entityComponents["sprite"];
                std::string textureId = component["textureAssetId"]; // Lua string has to be bind to local variable
                if (!component["animated"] && !component["fixed"]) {
                    entity.AddComponent<SpriteComponent>(textureId.c_str());
                } else if (!component["animated"] && component["fixed"]) {
                    entity.AddComponent<SpriteComponent>(textureId.c_str(), static_cast<bool>(component["fixed"]));
                } else {
                    entity.AddComponent<SpriteComponent>(
                        textureId.c_str(),
                        static_cast<int>(component["frameCount"]),
                        static_cast<int>(component["animationSpeed"]),
                        static_cast<bool>(component["hasDirections"]),
                        static_cast<bool>(component["fixed"]));
                }
            }

            if (sol::optional<sol::table> existsComponent = entityComponents["collider"]) {
                component = entityComponents["collider"];
                std::string tag = component["tag"];
                if (component["collisionBorder"]) {
                    std::string textureId = component["collisionBorder"];
                    entity.AddComponent<ColliderComponent>(textureId.c_str(), tag);
                } else {
                    entity.AddComponent<ColliderComponent>(tag);
                }
            }

            if (sol::optional<sol::table> existsComponent = entityComponents["projectileEmitter"]) {
                component = entityComponents["projectileEmitter"];
                // Create projectile entity for objects having projectile
                Entity& projectileEntity(manager.AddEntity("Projectile", PROJECTILE_LAYER));
                std::string textureId = component["textureAssetId"];
                projectileEntity.AddComponent<TransformComponent>(
                    static_cast<int>(entityComponents["transform"]["position"]["x"]) + (static_cast<int>(entityComponents["transform"]["width"]) / 2),
                    static_cast<int>(entityComponents["transform"]["position"]["y"]) + (static_cast<int>(entityComponents["transform"]["height"]) / 2),
                    0, /*********PUT A LONG COMMENT FOR BEAUTIFY********/
                    0, /*********PUT A LONG COMMENT FOR BEAUTIFY********/
                    static_cast<int>(component["width"]),
                    static_cast<int>(component["height"]),
                    1, /*********PUT A LONG COMMENT FOR BEAUTIFY********/
                    false
                );
                projectileEntity.AddComponent<SpriteComponent>(textureId.c_str());
                projectileEntity.AddComponent<ColliderComponent>("PROJECTILE");
                projectileEntity.AddComponent<ProjectileEmitComponent>(
                    static_cast<int>(component["speed"]),
                    static_cast<int>(component["angle"]),
                    static_cast<int>(component["range"]),
                    static_cast<bool>(component["shouldLoop"])
                );
            }

            if (sol::optional<sol::table> existsComponent = entityComponents["input"]) {
                component = entityComponents["input"];
                std::string up    = component["keyboard"]["up"];
                std::string down  = component["keyboard"]["down"];
                std::string left  = component["keyboard"]["left"];
                std::string right = component["keyboard"]["right"];
                std::string shoot = component["keyboard"]["shoot"];
                entity.AddComponent<KeyboardControlComponent>(up,down,left,right,shoot);
            }
        }
        entityIndex++;
    }
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
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME)) 0;

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
    if (manager.HasNoEntities()) {
        return;
    }
    manager.Render();
    SDL_RenderPresent(renderer);
};

void Game::HandleCameraMovement() {
    if (player) {
        TransformComponent* playerPos = player->GetComponent<TransformComponent>();
        // Keep the player in the middle of the screen
        camera.x = playerPos->position.x - (WINDOW_WIDTH / 2);
        camera.y = playerPos->position.y - (WINDOW_HEIGHT / 2);

        // If player position - half window < 0, dont move camera
        camera.x = camera.x < 0 ? 0 : camera.x;
        camera.y = camera.y < 0 ? 0 : camera.y;
        // If reached outer border, stop camera at outer border
        camera.x = playerPos->position.x + (WINDOW_WIDTH / 2) > map->GetWidth() ? map->GetWidth() - WINDOW_WIDTH : camera.x;
        camera.y = playerPos->position.y + (WINDOW_HEIGHT / 2) > map->GetHeight() ? map->GetHeight() - WINDOW_HEIGHT : camera.y;
    }
}

void Game::CheckCollision() {
    CollisionType collision = manager.CheckCollision();
    switch (collision) {
        case PLAYER_ENEMY:
            ProcessGameOver();
            break;
        case PLAYER_LEVEL_COMPLETE:
            ProcessNextLevel();
            break;
        case PLAYER_PROJECTILE:
            ProcessGameOver();
            break;
        default:
            break;
    }
}

void Game::ProcessGameOver() {
    std::cout << "\nGame Over!\n";
    isRunning = false;
}

void Game::ProcessNextLevel() {
    std::cout << "\nNext Level!\n";
    isRunning = false;
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
};
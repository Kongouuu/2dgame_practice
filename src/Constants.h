#ifndef CONSTANTS_H
#define CONSTANTS_H

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const unsigned int FPS = 60;   // Target FPS
const unsigned int FRAME_TARGET_TIME = 1000 / FPS;  // Time for each frame
const unsigned int NUM_LAYERS = 7;

enum LayerType{
    TILEMAP_LAYER = 0,
    VEGETATION_LAYER = 1, // e.g. tree
    ENEMY_LAYER = 2,
    OBSTACLE_LAYER = 3,
    PLAYER_LAYER = 4,
    PROJECTILE_LAYER = 5,
    UI_LAYER = 6
};

enum CollisionType {
    NONE,
    PLAYER_ENEMY,
    PLAYER_PROJECTILE,
    ENEMY_PROJECTILE,
    PLAYER_VEGETATION,
    PLAYER_LEVEL_COMPLETE
};

#endif
#ifndef CONSTANTS_H
#define CONSTANTS_H

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const unsigned int FPS = 60;   // Target FPS
const unsigned int FRAME_TARGET_TIME = 1000 / FPS;  // Time for each frame

enum LayerType{
    TILEMAP_LAYER = 0,
    VEGETATION_LAYER = 1, // e.g. tree
    ENEMY_LAYER = 2,
    PLAYER_LAYER = 3,
    PROJECTILE_LAYER = 4,
    UI_LAYER = 5
};

const unsigned int NUM_LAYERS = 6;
#endif
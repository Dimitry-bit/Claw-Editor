#pragma once

#include "SFML/Graphics.hpp"

#define MAX_GRID_SIZE 50

extern const int gridSize;

struct scene_context_t {
    sf::Sprite tileGrid[MAX_GRID_SIZE][MAX_GRID_SIZE];
};

void SceneAllocAssets();
void SceneInitGrid(scene_context_t& context);
#pragma once

#include <list>
#include "SFML/Graphics.hpp"

#include "entity.h"

#define MAX_GRID_SIZE 50

extern const int gridSize;

struct scene_context_t {
    entity_t tileGrid[MAX_GRID_SIZE][MAX_GRID_SIZE];
    std::list<entity_t> objects;
};

void SceneAllocAssets();
void SceneInitGrid(scene_context_t& context);
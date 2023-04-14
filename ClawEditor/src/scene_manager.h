#pragma once

#include <list>
#include "SFML/Graphics.hpp"

#include "entity.h"

#define MAX_GRID_SIZE 50

extern const int gridSize;

struct scene_context_t {
    entity_t* tileGrid[MAX_GRID_SIZE][MAX_GRID_SIZE];
    std::list<entity_t*> objects;
};

void SceneAllocAssets();
void SceneInitGrid(scene_context_t& context);
void SceneDealloc(scene_context_t& sceneContext);

void SceneAddTile(scene_context_t& sceneContext, entity_t* entity, int x, int y);
void SceneAddObject(scene_context_t& sceneContext, entity_t* entity);

entity_t* SceneGetTile(scene_context_t& sceneContext, const sf::Vector2u& pos);
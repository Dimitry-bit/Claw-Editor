#pragma once

#include <list>
#include "SFML/Graphics.hpp"

#include "entity.h"

#define MAX_GRID_SIZE 50

extern const int gridSize;

void SceneAllocAssets();
void SceneInitGrid();
void SceneDealloc();

const std::array<std::array<entity_t*, MAX_GRID_SIZE>, MAX_GRID_SIZE>& SceneGetTileGrid();
const std::list<entity_t*>& SceneGetEntities();
entity_t* SceneGetTile(const sf::Vector2u& pos);
void SceneAddTile(entity_t* entity, int x, int y);
void SceneAddObject(entity_t* entity);
bool SceneIsEntityHit(const sf::Vector2f& point, entity_t** out);
#pragma once

#include <list>
#include "SFML/Graphics.hpp"

#include "entity.h"
#include "renderer.h"

#define MAX_GRID_SIZE 50

struct scene_context_t {
    unsigned int tileSize;
    unsigned int tileGridWidth;
    unsigned int tileGridHeight;

    entity_t** tileGrid;
    std::list<entity_t*> objects;
};

void SceneAllocAssets(scene_context_t* world);
void SceneDealloc(scene_context_t* world);

sf::Vector2u SceneGetGridPos(const scene_context_t* world, const sf::Vector2f& viewPos);
inline sf::Vector2f SceneGetTileStartPos(const scene_context_t* world, const sf::Vector2f& viewPos);

entity_t* SceneGetTile(const scene_context_t* world, float x, float y);
entity_t* SceneGetTile(const scene_context_t* world, const sf::Vector2f& pos);

void SceneAddTile(scene_context_t* world, entity_t* entity, unsigned int x, unsigned int y);
void SceneAddTile(scene_context_t* world, entity_t* entity, const sf::Vector2u& tilePos);
void SceneAddObject(scene_context_t* world, entity_t* entity);
entity_t* SceneRemoveEntity(scene_context_t* world, const entity_t* entity);

bool SceneIsEntityHit(const scene_context_t* world, float x, float y, entity_t** out);
bool SceneIsEntityHit(const scene_context_t* world, const sf::Vector2f& point, entity_t** out);

void DrawWorld(const render_context_t* renderContext, const scene_context_t* world);
#pragma once

#include <list>
#include "SFML/Graphics.hpp"

#include "entity.h"
#include "renderer.h"

#define MAX_GRID_WIDTH 300
#define MAX_GRID_HEIGHT 100

struct tilemap_t {
    entity_t** tileGrid;
};

struct scene_context_t {
    unsigned int tileSize;
    unsigned int tileGridWidth;
    unsigned int tileGridHeight;
    unsigned int tileMapIndex;
    unsigned int tileMapCount;

    tilemap_t* tileMaps;
    std::list<entity_t*> objects;
};

void SceneAllocAssets(scene_context_t* world);
void SceneDealloc(scene_context_t* world);
int ScenePopulatedTilesCount(const scene_context_t* world, int tileMapIndex);

sf::Vector2u SceneGetGridPos(const scene_context_t* world, const sf::Vector2f& viewPos);
sf::Vector2f SceneGetTileStartPos(const scene_context_t* world, const sf::Vector2f& viewPos);

entity_t* SceneGetTileWithIndex(const scene_context_t* world, int tileMapIndex, int x, int y);
entity_t* SceneGetTileWithPos(const scene_context_t* world, float x, float y);
entity_t* SceneGetTileWithPos(const scene_context_t* world, const sf::Vector2f& pos);

bool SceneAddTile(scene_context_t* world, entity_t* entity, int x, int y);
bool SceneAddTile(scene_context_t* world, entity_t* entity, const sf::Vector2i& tilePos);
void SceneAddObject(scene_context_t* world, entity_t* entity);
entity_t* SceneRemoveEntity(scene_context_t* world, const entity_t* entity);

bool SceneIsValidTile(const scene_context_t* world, int x, int y);
bool SceneIsValidTile(const scene_context_t* world, const sf::Vector2i& pos);
bool SceneIsTileOccupied(const scene_context_t* world, int x, int y);
bool SceneIsTileOccupied(const scene_context_t* world, const sf::Vector2i& pos);
bool SceneIsEntityHitAny(const scene_context_t* world, float x, float y, entity_t** out);
bool SceneIsEntityHitAny(const scene_context_t* world, const sf::Vector2f& point, entity_t** out);
bool SceneIsEntityHitTile(const scene_context_t* world, float x, float y, entity_t** out);
bool SceneIsEntityHitTile(const scene_context_t* world, const sf::Vector2f& point, entity_t** out);
bool SceneIsEntityHitObj(const scene_context_t* world, float x, float y, entity_t** out);
bool SceneIsEntityHitObj(const scene_context_t* world, const sf::Vector2f& point, entity_t** out);

void SceneSetTileIndex(scene_context_t* world, int index);

void DrawWorld(const render_context_t* renderContext, const scene_context_t* world);
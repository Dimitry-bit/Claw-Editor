#pragma once

#include <string>

#include "entity_components.h"

using std::string;

struct entity_t {
    entity_components_t type;
    string logic;

    c_render_t render;
    c_tile_t tile;
    c_platform_t platform;
    c_checkpoint_t checkpoint;
    c_pickup_t pickup;
    c_enemy_t enemy;
    c_sound_t sound;
};

struct entity_transform {
    sf::FloatRect bounds;
    sf::Vector2f position;
    sf::Vector2f origin;
};

entity_t* EntityAlloc();
void EntityDealloc(entity_t** entity);

void EntityInit(entity_t* self, const std::string_view& logic, const render_types_t& rType,
                const std::string_view graphicID = "");

bool EntityHas(entity_t* self, const entity_components_t& cType);
void EntitySet(entity_t* self, const entity_components_t& cType, const void* cValue);
void EntitySetPos(entity_t* self, float x, float y);
void EntitySetPos(entity_t* self, const sf::Vector2f& pos);
void EntitySetOrigin(entity_t* self, float x, float y);
void EntitySetOrigin(entity_t* self, const sf::Vector2f& origin);
entity_transform EntityGetTransform(const entity_t* self);

void EntityUpdate(entity_t* self, const entity_t* to);

void DrawEntity(const entity_t* entity);
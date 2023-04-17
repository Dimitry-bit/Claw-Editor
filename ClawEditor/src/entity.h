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

entity_t* EntityAlloc();
void EntityDealloc(entity_t** entity);

void EntityInit(entity_t* self,
                const std::string_view& logic,
                const render_types_t& rType,
                const std::string_view graphicID = "");

void EntitySet(entity_t* self, const entity_components_t& cType, const void* cValue);
bool EntityHas(entity_t* self, const entity_components_t& cType);
#pragma once

#include "entity.h"
#include "renderer.h"

enum action_types_t {
    ACTION_NONE,
    ACTION_TILE_PLACEMENT,
    ACTION_OBJECT_PLACEMENT,
};

struct action_t {
    action_types_t type;
    entity_t modifiedEntityCache;
};

entity_t* ActionPlaceTile(const entity_t& entity);
entity_t* ActionPlaceEntity(const entity_t& entity, sf::Vector2f origin = sf::Vector2f(0, 0));
void ActionDeleteEntity(entity_t** hitEntityPtr);
void ActionEntityMove(entity_t& entity);
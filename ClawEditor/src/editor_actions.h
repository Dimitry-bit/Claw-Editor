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

void ActionPlaceEntity(render_context_t& renderContext);
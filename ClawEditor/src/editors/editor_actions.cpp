#include <cassert>

#include "editor_actions.h"
#include "scene_manager.h"
#include "input.h"

entity_t* ActionPlaceTile(const entity_t& entity)
{
    const sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(*rWindow);
    const sf::Vector2f mouseViewPos = rWindow->mapPixelToCoords(mouseWindowPos, GetRenderContext().worldView);
    const sf::Vector2u mouseGridPos(mouseViewPos / (float) gridSize);
    const sf::Vector2f pos(mouseGridPos.x * gridSize, mouseGridPos.y * gridSize);

    entity_t* e = EntityAlloc();
    assert(e);

    EntityCreateTile(e, entity.graphicsID.c_str(), entity.colliderType, pos);
    SceneAddTile(e, mouseGridPos.x, mouseGridPos.y);

    return e;
}

entity_t* ActionPlaceEntity(const entity_t& entity, sf::Vector2f origin)
{
    entity_t* e = EntityAlloc();
    assert(e);

    *e = entity;

    sf::Vector2f pos = GetRenderContext().worldView.getCenter();
    EntityCreateOBJ(e, entity.graphicsID.c_str(), entity.logic.c_str(), pos, origin);
    SceneAddObject(e);

    return e;
}

void ActionDeleteEntity(entity_t** hitEntityPtr)
{
    if (!hitEntityPtr || !(*hitEntityPtr)) {
        return;
    }

    SceneRemoveEntity(*hitEntityPtr);
    EntityDealloc(hitEntityPtr);
}
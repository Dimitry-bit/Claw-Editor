#include <cassert>

#include "editor_actions.h"
#include "scene_manager.h"
#include "input.h"

entity_t* ActionPlaceTile(scene_context_t* world, const entity_t& entity)
{
    const sf::Vector2f mouseViewPos = rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow));
    const sf::Vector2u girdPos = SceneGetGridPos(world, mouseViewPos);
    const sf::Vector2f placementPos = SceneGetTileStartPos(world, mouseViewPos);

    entity_t* e = EntityAlloc();
    assert(e);

    EntityInit(e, entity.logic, entity.render.type, entity.render.graphicsID);
    EntitySet(e, entity.type, &entity.tile);
    EntitySetPos(e, placementPos);

    SceneAddTile(world, e, girdPos);
    return e;
}

entity_t* ActionPlaceEntity(scene_context_t* world, const entity_t& entity, sf::Vector2f origin)
{
    entity_t* e = EntityAlloc();
    assert(e);

    const sf::Vector2f placementPos = rWindow->getView().getCenter();

    *e = entity;
    EntityInit(e, entity.logic, entity.render.type, entity.render.graphicsID);
    EntitySetOrigin(e, origin);
    EntitySetPos(e, placementPos);

    SceneAddObject(world, e);
    return e;
}

void ActionDeleteEntity(scene_context_t* world, entity_t** hitEntityPtr)
{
    if (!hitEntityPtr || !(*hitEntityPtr)) {
        return;
    }

    SceneRemoveEntity(world, *hitEntityPtr);
    EntityDealloc(hitEntityPtr);
}

void ActionEntityMove(scene_context_t* world, entity_t& entity)
{
    const sf::Vector2f mouseViewPos = rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow));
    const sf::Vector2u girdPos = SceneGetGridPos(world, mouseViewPos);
    const sf::Vector2f gridSnapPos = SceneGetTileStartPos(world, mouseViewPos);

    // TODO(Tony): Add Renders
//    if (isSnapToGrid) {
//        EntitySetPos(&entity, gridSnapPos);
//    } else {
    EntitySetPos(&entity, mouseViewPos);
//    }
}
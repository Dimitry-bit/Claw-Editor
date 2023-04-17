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

    EntityInit(e, entity.logic, entity.render.type, entity.render.graphicsID);
    EntitySet(e, C_TILE, &entity.tile);
    e->render.sprite.setPosition(pos);

    SceneAddTile(e, mouseGridPos.x, mouseGridPos.y);

    return e;
}

entity_t* ActionPlaceEntity(const entity_t& entity, sf::Vector2f origin)
{
    entity_t* e = EntityAlloc();
    assert(e);

    *e = entity;
    EntityInit(e, entity.logic, entity.render.type, entity.render.graphicsID);
    EntitySet(e, entity.type, &entity.tile);
    sf::Vector2f pos = GetRenderContext().worldView.getCenter();
    e->render.sprite.setPosition(pos);
    e->render.sprite.setOrigin(origin);

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

void ActionEntityMove(entity_t& entity)
{
    const sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(*rWindow);
    const sf::Vector2f mouseViewPos = rWindow->mapPixelToCoords(mouseWindowPos, GetRenderContext().worldView);
    const sf::Vector2u mouseGridPos(mouseViewPos / (float) gridSize);
    const sf::Vector2f pos(mouseGridPos.x * gridSize, mouseGridPos.y * gridSize);
    const bool isSnapToGrid = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

    // TODO(Tony): Add Renders
    if (isSnapToGrid) {
        entity.render.sprite.setPosition(pos);
    } else {
        entity.render.sprite.setPosition(mouseViewPos);
    }
}
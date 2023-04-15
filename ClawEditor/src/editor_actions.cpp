#include "editor_actions.h"
#include "editor_internal.h"
#include "scene_manager.h"
#include "input.h"

void ActionPlaceEntity(render_context_t& renderContext)
{
    const sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(*rWindow);
    const sf::Vector2f mouseViewPos = rWindow->mapPixelToCoords(mouseWindowPos, renderContext.worldView);
    const sf::Vector2u mouseGridPos(mouseViewPos / (float) gridSize);

    const entity_t* selectedEntity = editorContext.selectedEntity;
    if (!selectedEntity) {
        return;
    }

    if (editorContext.mode == EDITOR_MODE_TILE) {
        sf::Vector2f pos(mouseGridPos.x * gridSize, mouseGridPos.y * gridSize);
        entity_t* entity = EntityAlloc();
        EntityCreateTile(entity, selectedEntity->graphicsID.c_str(), selectedEntity->colliderType, pos);
        SceneAddTile(entity, mouseGridPos.x, mouseGridPos.y);
    } else if (editorContext.mode == EDITOR_MODE_OBJ) {
        entity_t* entity = EntityAlloc();
        EntityCreateOBJ(entity, selectedEntity->graphicsID.c_str(), selectedEntity->logic.c_str(),
                        mouseViewPos, selectedEntity->sprite.getOrigin());
        SceneAddObject(entity);
    }
}

void ActionDeleteEntity(entity_t** hitEntityPtr)
{
    if (!hitEntityPtr || !(*hitEntityPtr)) {
        return;
    }

    SceneRemoveEntity(*hitEntityPtr);
    EntityDealloc(hitEntityPtr);
}
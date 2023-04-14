#include "editor_actions.h"
#include "editor.h"
#include "scene_manager.h"
#include "input.h"

void EditorActionPlaceEntity(render_context_t& renderContext, editor_context_t& editorContext)
{
    if (!isMousePressed(sf::Mouse::Left)) {
        return;
    }

    const entity_t* selectedEntity = editorContext.selectedEntity;

    if (!selectedEntity) {
        return;
    }

    if (editorContext.mode == EDITOR_MODE_TILE) {
        const sf::Vector2u& mousePosGrid = editorContext.editorHit.girdPosition;
        sf::Vector2f pos(mousePosGrid.x * gridSize, mousePosGrid.y * gridSize);
        entity_t* entity = EntityAlloc();
        EntityCreateTile(entity, selectedEntity->graphicsID.c_str(), selectedEntity->colliderType, pos);
        SceneAddTile(renderContext.sceneContext, entity, mousePosGrid.x, mousePosGrid.y);
    } else if (editorContext.mode == EDITOR_MODE_OBJ) {
        entity_t* entity = EntityAlloc();
        const sf::Vector2f& pos = editorContext.editorHit.viewPosition;
        EntityCreateOBJ(entity,
                        selectedEntity->graphicsID.c_str(),
                        selectedEntity->logic.c_str(),
                        pos,
                        selectedEntity->sprite.getOrigin());
        SceneAddObject(renderContext.sceneContext, entity);
    }
}
#include <cassert>
#include <cmath>

#include "scene_manager.h"
#include "renderer.h"
#include "resource_manager.h"
#include "editors/editor_inworld.h"
#include "entity.h"

void SceneAllocAssets(scene_context_t* world)
{
    AssetPushType(ASSET_SPRITESHEET);
    ResLoadFromFile("tilesets/LEVEL1_TILES.png", ASSET_TAG_TILE);
    ResLoadFromFile("objects/LEVEL1_OBJECTS.png", ASSET_TAG_OBJ | ASSET_TAG_EYECANDY);
    ResLoadFromFile("objects/GAME.png", ASSET_TAG_OBJ | ASSET_TAG_PICKUP);
    ResLoadFromFile("objects/CHECKPOINT.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("objects/SUPERCHECKPOINT.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("objects/COINS.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResLoadFromFile("objects/CRUMBLINGPEG.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("objects/EXTRALIFE.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResLoadFromFile("objects/HANDS4.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile("objects/MANICAL1.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile("objects/TORCH.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile("objects/TORSHSTAND.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResLoadFromFile("objects/CRATES.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    AssetPopType();
    ResTextureLoadFromSpriteSheet("tilesets/LEVEL1_TILES.png", ASSET_TAG_TILE);
    ResTextureLoadFromSpriteSheet("objects/LEVEL1_OBJECTS.png", ASSET_TAG_OBJ | ASSET_TAG_EYECANDY);
    ResTextureLoadFromSpriteSheet("objects/GAME.png", ASSET_TAG_OBJ | ASSET_TAG_PICKUP);
    ResTextureLoadFromSpriteSheet("objects/CHECKPOINT.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("objects/SUPERCHECKPOINT.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("objects/COINS.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResTextureLoadFromSpriteSheet("objects/CRUMBLINGPEG.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("objects/EXTRALIFE.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResTextureLoadFromSpriteSheet("objects/HANDS4.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResTextureLoadFromSpriteSheet("objects/MANICAL1.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResTextureLoadFromSpriteSheet("objects/TORCH.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResTextureLoadFromSpriteSheet("objects/TORSHSTAND.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
    ResTextureLoadFromSpriteSheet("objects/CRATES.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_EYECANDY);
}

void SceneDealloc(scene_context_t* world)
{
    for (int y = 0; y < world->tileGridHeight; ++y) {
        for (int x = 0; x < world->tileGridWidth; ++x) {
            EntityDealloc(&world->tileGrid[y * world->tileGridWidth + x]);
        }
    }
//    delete world->tileGrid;

    for (auto& object: world->objects) {
        EntityDealloc(&object);
    }
    world->objects.clear();

    printf("[INFO][SceneManager]: Scene deallocated successfully.\n");
}

sf::Vector2u SceneGetGridPos(const scene_context_t* world, const sf::Vector2f& viewPos)
{
    return sf::Vector2u(viewPos / (float) world->tileSize);
}

sf::Vector2f SceneGetTileStartPos(const scene_context_t* world, const sf::Vector2f& viewPos)
{
    sf::Vector2u girdPos = SceneGetGridPos(world, viewPos);
    sf::Vector2f tileStartPos(girdPos.x * world->tileSize, girdPos.y * world->tileSize);
    return tileStartPos;
}

entity_t* SceneGetTileWithPos(const scene_context_t* world, const sf::Vector2f& pos)
{
    return SceneGetTileWithPos(world, pos.x, pos.y);
}

entity_t* SceneGetTileWithPos(const scene_context_t* world, float x, float y)
{
    entity_t* result = nullptr;
    int gridPosX = (int) roundf(x / (float) world->tileSize);
    int gridPosY = (int) roundf(y / (float) world->tileSize);

    if (gridPosX >= 0 && gridPosX < MAX_GRID_SIZE &&
        gridPosY >= 0 && gridPosY < MAX_GRID_SIZE) {
        result = world->tileGrid[gridPosY * world->tileGridWidth + gridPosX];
    }

    return result;
}

entity_t* SceneGetTileWithIndex(const scene_context_t* world, int x, int y)
{
    entity_t* result = nullptr;
    if (x >= 0 && x < MAX_GRID_SIZE &&
        y >= 0 && y < MAX_GRID_SIZE) {
        result = world->tileGrid[y * world->tileGridWidth + x];
    }

    return result;
}

bool SceneAddTile(scene_context_t* world, entity_t* entity, int x, int y)
{
    if (!SceneIsValidTile(world, x, y)) {
        printf("[ERROR][SceneManager]: Tile grid out of bound access.\n");
        return false;
    }

    entity_t* e = SceneGetTileWithIndex(world, x, y);
    if (e) {
        SceneRemoveEntity(world, e);
        EntityDealloc(&e);
    }

    world->tileGrid[y * world->tileGridWidth + x] = entity;
    printf("[INFO][SceneManager]: Tile Placed.\n");
    return true;
}

bool SceneAddTile(scene_context_t* world, entity_t* entity, const sf::Vector2i& tilePos)
{
    return SceneAddTile(world, entity, tilePos.x, tilePos.y);
}

void SceneAddObject(scene_context_t* world, entity_t* entity)
{
    assert(entity);

    world->objects.push_back(entity);
    printf("[INFO][SceneManager]: Object Placed.\n");
}

entity_t* SceneRemoveEntity(scene_context_t* world, const entity_t* entity)
{
    assert(entity);

    for (int y = 0; y < world->tileGridHeight; ++y) {
        for (int x = 0; x < world->tileGridWidth; ++x) {
            entity_t* e = SceneGetTileWithIndex(world, x, y);
            if (e == entity) {
                printf("[Info][SceneManager]: Entity removed.\n");
                world->tileGrid[y * world->tileGridWidth + x] = nullptr;
                return e;
            }
        }
    }

    for (auto it = world->objects.begin(); it != world->objects.end(); ++it) {
        if (*it == entity) {
            entity_t* obj = *it;
            world->objects.erase(it);
            printf("[Info][SceneManager]: Entity removed.\n");
            return obj;
        }
    }

    return nullptr;
}

bool SceneIsValidTile(const scene_context_t* world, int x, int y)
{
    return (x >= 0 && x < MAX_GRID_SIZE) && (y >= 0 && y < MAX_GRID_SIZE);
}

bool SceneIsValidTile(const scene_context_t* world, const sf::Vector2i& pos)
{
    return SceneIsValidTile(world, pos.x, pos.y);
}

bool SceneIsTileOccupied(const scene_context_t* world, int x, int y)
{
    if (!(x >= 0 && x < MAX_GRID_SIZE) || !(y >= 0 && y < MAX_GRID_SIZE)) {
        printf("[ERROR][SceneManager]: Tile grid out of bound access.\n");
        return true;
    }

    return SceneGetTileWithIndex(world, x, y);
}

bool SceneIsTileOccupied(const scene_context_t* world, const sf::Vector2i& pos)
{
    return SceneIsTileOccupied(world, pos.x, pos.y);
}

bool SceneIsEntityHit(const scene_context_t* world, float x, float y, entity_t** out)
{
    entity_t* e = SceneGetTileWithPos(world, x, y);
    bool isHit = (e != nullptr);

    for (auto it = world->objects.rbegin(); it != world->objects.rend(); ++it) {
        entity_t* entity = *it;
        if (entity->render.sprite.getGlobalBounds().contains(x, y)) {
            e = entity;
            isHit = true;
            break;
        }
    }

    if (out) {
        *out = e;
    }

    return isHit;
}

bool SceneIsEntityHit(const scene_context_t* world, const sf::Vector2f& point, entity_t** out)
{
    return SceneIsEntityHit(world, point.x, point.y, out);
}

void DrawWorld(const render_context_t* renderContext, const scene_context_t* world)
{
    assert(renderContext);
    assert(world);

    const sf::Vector2f drawCenter = rWindow->getView().getCenter();
    const sf::Vector2f viewSize = rWindow->getView().getSize();
    const float width = viewSize.x / 2;
    const float height = viewSize.y / 2;
    const int size = world->tileSize;

    int fromX = (drawCenter.x - width) / size - 2;
    int toX = (drawCenter.x + width) / size + 2;
    int fromY = (drawCenter.y - height) / size - 2;
    int toY = (drawCenter.y + height) / size + 2;
    fromX = std::clamp(fromX, 0, MAX_GRID_SIZE - 1);
    toX = std::clamp(toX, 0, MAX_GRID_SIZE - 1);
    fromY = std::clamp(fromY, 0, MAX_GRID_SIZE - 1);
    toY = std::clamp(toY, 0, MAX_GRID_SIZE - 1);

    for (int x = fromX; x < toX; ++x) {
        for (int y = fromY; y < toY; ++y) {
            entity_t* tile = SceneGetTileWithIndex(world, x, y);
            if (!tile)
                continue;

            DrawEntity(tile);
            if (renderContext->settings.isDrawTileCollider) {
                DrawTileInfo(tile);
            }
        }
    }

    for (auto& entity: world->objects) {
        DrawEntity(entity);
    }
}
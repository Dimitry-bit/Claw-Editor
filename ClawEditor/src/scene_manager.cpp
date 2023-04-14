#include <cassert>

#include "scene_manager.h"
#include "resource_manager.h"

const int gridSize = 64;

void SceneAllocAssets()
{
    AssetPushType(ASSET_SPRITESHEET);
    ResLoadFromFile("tilesets/LEVEL1_TILES.png", ASSET_TAG_TILE);
    ResLoadFromFile("objects/LEVEL1_OBJECTS.png", ASSET_TAG_OBJ);
    ResLoadFromFile("objects/GAME.png", ASSET_TAG_OBJ | ASSET_TAG_PICKUP);
    ResLoadFromFile("objects/CHECKPOINT.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("objects/COINS.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResLoadFromFile("objects/CRUMBLINGPEG.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("objects/EXTRALIFE.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResLoadFromFile("objects/HANDS4.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("objects/MANICAL1.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("objects/TORCH.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResLoadFromFile("objects/TORSHSTAND.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    AssetPopType();
    ResTextureLoadFromSpriteSheet("tilesets/LEVEL1_TILES.png", ASSET_TAG_TILE);
    ResTextureLoadFromSpriteSheet("objects/LEVEL1_OBJECTS.png", ASSET_TAG_OBJ);
    ResTextureLoadFromSpriteSheet("objects/GAME.png", ASSET_TAG_OBJ | ASSET_TAG_PICKUP);
    ResTextureLoadFromSpriteSheet("objects/CHECKPOINT.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("objects/COINS.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResTextureLoadFromSpriteSheet("objects/CRUMBLINGPEG.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("objects/EXTRALIFE.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION | ASSET_TAG_PICKUP);
    ResTextureLoadFromSpriteSheet("objects/HANDS4.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("objects/MANICAL1.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("objects/TORCH.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
    ResTextureLoadFromSpriteSheet("objects/TORSHSTAND.png", ASSET_TAG_OBJ | ASSET_TAG_ANIMATION);
}

void SceneInitGrid(scene_context_t& sceneContext)
{
    memset(sceneContext.tileGrid, 0, sizeof(sceneContext.tileGrid));

    entity_t* tile = EntityAlloc();
    EntityCreateTile(tile, "tilesets/LEVEL1_TILES/ACTION/012", COLLIDER_SOLID, sf::Vector2f(0, 0));
    SceneAddTile(sceneContext, tile, 0, 0);

    entity_t* coin = EntityAlloc();
    const spriteSheet_t& spriteSheet = ResSpriteSheetGet("objects/COINS.png");
    coin->graphicsID = spriteSheet.frames[0].id;
    coin->logic = "LOGIC_PICKUP";
    coin->sprite.setTexture(spriteSheet.texture);
    coin->sprite.setTextureRect(spriteSheet.frames[0].area);
    coin->sprite.setOrigin(spriteSheet.frames[0].pivot);
    coin->sprite.setPosition(gridSize * 2, gridSize * 2);
    SceneAddObject(sceneContext, coin);
}

void SceneAddTile(scene_context_t& sceneContext, entity_t* entity, int x, int y)
{
    if (!(x >= 0 && x < MAX_GRID_SIZE) || !(y >= 0 && y < MAX_GRID_SIZE)) {
        printf("[ERROR][SceneManager]: Tile grid out of bound access.\n");
        return;
    }

    if (sceneContext.tileGrid[x][y]) {
        EntityDealloc(sceneContext.tileGrid[x][y]);
    }

    sceneContext.tileGrid[x][y] = entity;
    printf("[INFO][SceneManager]: Tile Placed.\n");
}

void SceneAddObject(scene_context_t& sceneContext, entity_t* entity)
{
    assert(entity);
    sceneContext.objects.push_back(entity);
    printf("[INFO][SceneManager]: Object Placed.\n");
}

void SceneDealloc(scene_context_t& sceneContext)
{
    for (int i = 0; i < MAX_GRID_SIZE; ++i) {
        for (int j = 0; j < MAX_GRID_SIZE; ++j) {
            EntityDealloc(sceneContext.tileGrid[i][j]);
        }
    }

    for (auto it = sceneContext.objects.begin(); it != sceneContext.objects.end(); ++it) {
        EntityDealloc(*it);
    }
    sceneContext.objects.clear();

    printf("[INFO][SceneManager]: Scene deallocated successfully.\n");
}

entity_t* SceneGetTile(scene_context_t& sceneContext, const sf::Vector2u& pos)
{
    if (!(pos.x >= 0 && pos.x < MAX_GRID_SIZE) || !(pos.y >= 0 && pos.y < MAX_GRID_SIZE)) {
        return nullptr;
    }

    return sceneContext.tileGrid[pos.x][pos.y];
}
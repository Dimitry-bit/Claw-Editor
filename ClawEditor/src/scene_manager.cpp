#include <cassert>

#include "scene_manager.h"
#include "resource_manager.h"
#include "entity.h"

struct scene_context_t {
    std::array<std::array<entity_t*, MAX_GRID_SIZE>, MAX_GRID_SIZE> tileGrid;
    std::list<entity_t*> objects;
};

static scene_context_t sceneContext;

const int gridSize = 64;

void SceneAllocAssets()
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

void SceneInitGrid()
{
    entity_t* tile = EntityAlloc();
    EntityCreateTile(tile, "tilesets/LEVEL1_TILES/ACTION/012", COLLIDER_SOLID, sf::Vector2f(0, 0));
    SceneAddTile(tile, 0, 0);

    tile = EntityAlloc();
    EntityCreateTile(tile, "tilesets/LEVEL1_TILES/ACTION/012", COLLIDER_CLIMBABLE, sf::Vector2f(64, 0));
    SceneAddTile(tile, 1, 0);

    tile = EntityAlloc();
    EntityCreateTile(tile, "tilesets/LEVEL1_TILES/ACTION/012", COLLIDER_DEATH, sf::Vector2f(2 * 64, 0));
    SceneAddTile(tile, 2, 0);

    tile = EntityAlloc();
    EntityCreateTile(tile, "tilesets/LEVEL1_TILES/ACTION/012", COLLIDER_GROUND, sf::Vector2f(3 * 64, 0));
    SceneAddTile(tile, 3, 0);

    tile = EntityAlloc();
    EntityCreateTile(tile, "tilesets/LEVEL1_TILES/ACTION/012", COLLIDER_CLEAR, sf::Vector2f(4 * 64, 0));
    SceneAddTile(tile, 4, 0);

    entity_t* coin = EntityAlloc();
    const spriteSheet_t& spriteSheet = ResSpriteSheetGet("objects/COINS.png");
    coin->graphicsID = spriteSheet.frames[0].id;
    coin->logic = "LOGIC_PICKUP";
    coin->sprite.setTexture(spriteSheet.texture);
    coin->sprite.setTextureRect(spriteSheet.frames[0].area);
    coin->sprite.setOrigin(spriteSheet.frames[0].pivot);
    coin->sprite.setPosition(gridSize * 2, gridSize * 2);
    SceneAddObject(coin);
}

void SceneDealloc()
{
    for (int i = 0; i < MAX_GRID_SIZE; ++i) {
        for (int j = 0; j < MAX_GRID_SIZE; ++j) {
            EntityDealloc(&sceneContext.tileGrid[i][j]);
        }
    }

    for (auto it = sceneContext.objects.begin(); it != sceneContext.objects.end(); ++it) {
        EntityDealloc(&*it);
    }
    sceneContext.objects.clear();

    printf("[INFO][SceneManager]: Scene deallocated successfully.\n");
}

const std::array<std::array<entity_t*, MAX_GRID_SIZE>, MAX_GRID_SIZE>& SceneGetTileGrid()
{
    return sceneContext.tileGrid;
}

const std::list<entity_t*>& SceneGetEntities()
{
    return sceneContext.objects;
}

entity_t* SceneGetTile(const sf::Vector2u& pos)
{
    const sf::Vector2u gridPos = pos / (unsigned int) gridSize;

    if (!(gridPos.x >= 0 && gridPos.x < MAX_GRID_SIZE) || !(gridPos.y >= 0 && gridPos.y < MAX_GRID_SIZE)) {
        return nullptr;
    }

    return sceneContext.tileGrid[gridPos.x][gridPos.y];
}

void SceneAddTile(entity_t* entity, int x, int y)
{
    if (!(x >= 0 && x < MAX_GRID_SIZE) || !(y >= 0 && y < MAX_GRID_SIZE)) {
        printf("[ERROR][SceneManager]: Tile grid out of bound access.\n");
        return;
    }

    if (sceneContext.tileGrid[x][y]) {
        EntityDealloc(&sceneContext.tileGrid[x][y]);
    }

    sceneContext.tileGrid[x][y] = entity;
    printf("[INFO][SceneManager]: Tile Placed.\n");
}

void SceneAddObject(entity_t* entity)
{
    assert(entity);
    sceneContext.objects.push_back(entity);
    printf("[INFO][SceneManager]: Object Placed.\n");
}

void SceneRemoveEntity(const entity_t* const entity)
{
    assert(entity);

    for (int i = 0; i < MAX_GRID_SIZE; ++i) {
        for (int j = 0; j < MAX_GRID_SIZE; ++j) {
            if (sceneContext.tileGrid[i][j] == entity) {
                sceneContext.tileGrid[i][j] = nullptr;
                printf("[Info][SceneManager]: Entity removed.\n");
                return;
            }
        }
    }

    for (auto it = sceneContext.objects.begin(); it != sceneContext.objects.end(); ++it) {
        if (*it == entity) {
            sceneContext.objects.erase(it);
            printf("[Info][SceneManager]: Entity removed.\n");
            return;
        }
    }
}

bool SceneIsEntityHit(const sf::Vector2f& point, entity_t** out)
{
    if (!out) {
        return false;
    }

    *out = SceneGetTile(sf::Vector2u(point));
    if (*out) {
        return true;
    }

    for (auto& entity: sceneContext.objects) {
        if (entity && entity->sprite.getGlobalBounds().contains(point)) {
            *out = entity;
            return true;
        }
    }

    return false;
}
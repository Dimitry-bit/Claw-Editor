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
    auto& tileGrid = sceneContext.tileGrid;

    tileGrid[0][0].graphicsID = "ACTION/012";
    tileGrid[0][0].sprite.setTexture(ResTextureGet("ACTION/304"));
    tileGrid[0][0].sprite.setPosition(0 * gridSize, 0 * gridSize);
}
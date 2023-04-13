#pragma once
#include <string>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "spritesheet_parser.h"

enum asset_tags_t {
    ASSET_TAG_NONE,
    ASSET_TAG_TILE,
    ASSET_TAG_TREASURE,
    ASSET_TAG_PICKUP,
};

enum asset_types_t {
    ASSET_NONE,
    ASSET_FONT,
    ASSET_IMAGE,
    ASSET_TEXTURE,
    ASSET_SOUNDBUFFER,
    ASSET_SPRITESHEET,
};

struct asset_header_t {
    string fileName;
    string fileExtension;
    string id;
};

struct asset_slot_t {
    asset_header_t header;
    asset_types_t type;
    int assetTags;
    union {
        void* data;
        sf::Font* font;
        sf::Image* image;
        sf::Texture* texture;
        sf::SoundBuffer* soundBuffer;
        spriteSheet_t* spriteSheet;
    };
};

struct asset_context_t {
    asset_types_t pushType;

    std::map<std::string, asset_slot_t> fontTBL;
    std::map<std::string, asset_slot_t> imageTBL;
    std::map<std::string, asset_slot_t> textureTBL;
    std::map<std::string, asset_slot_t> soundTBL;
    std::map<std::string, asset_slot_t> spriteSheetTBL;
};

void AssetPopType();
void AssetPushType(asset_types_t type);

bool ResLoadFromFile(const char* identifier, int tags = 0);
bool ResTextureLoadFromSpriteSheet(const char* identifier, int tags = 0);
bool ResTextureLoadFromImage(const sf::Image& image, const char* identifier, const sf::IntRect& area = sf::IntRect());

void ResFontUnload(const char* identifier, bool erase = true);
void ResImageUnload(const char* identifier, bool erase = true);
void ResTextureUnload(const char* identifier, bool erase = true);
void ResSoundBufferUnload(const char* identifier, bool erase = true);
void ResSpriteSheetUnload(const char* identifier, bool erase = true);
void ResTextureSpriteSheetUnload(const spriteSheet_t& spriteSheet);

void ResUnloadAll();

asset_slot_t* ResGetAssetSlot(asset_types_t type, const char* identifier);
std::vector<asset_slot_t*> ResGetAllAssetSlots(asset_types_t type, int tags);
const sf::Font& ResFontGet(const char* identifier);
const sf::Image& ResImageGet(const char* identifier);
const sf::Texture& ResTextureGet(const char* identifier);
const sf::SoundBuffer& ResSoundBuffGet(const char* identifier);
const spriteSheet_t& ResSpriteSheetGet(const char* identifier);
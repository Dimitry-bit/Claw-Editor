#pragma once

#include <string>
#include "SFML/Graphics.hpp"

using std::string;

struct entity_t;

enum entity_tags_t {
    ENTITY_OBJ,
    ENTITY_TILE,
};

enum entity_timed_logic_t {
    ENTITY_TIMED_LOGIC_ELEVATOR,
    ENTITY_TIMED_LOGIC_CRUMBLINGPEG,
};

enum colliders_t {
    COLLIDER_CLEAR = 0,
    COLLIDER_SOLID,
    COLLIDER_DEATH,
    COLLIDER_CLIMBABLE,
    COLLIDER_GROUND,
};

struct entity_t {
    string graphicsID;
    string logic;
    entity_timed_logic_t timedLogic;

    colliders_t colliderType;
    std::vector<entity_t> drops;

    string soundFile;
    bool isLoop;

    sf::Sprite sprite;
    sf::RectangleShape rectangle;

    sf::Vector2f a;
    sf::Vector2f b;

    int drawLayer;
    union {
        int value;
        int healthValue;
        int goldValue;
    };
};

entity_t* EntityAlloc();
void EntityDealloc(entity_t** entity);

void EntityCreateTile(entity_t* entity,
                      const char* graphics,
                      colliders_t colliderType,
                      const sf::Vector2f& position,
                      const sf::Vector2f& origin = sf::Vector2f(0, 0));

void EntityCreateOBJ(entity_t* entity,
                     const char* graphics,
                     const char* logic,
                     const sf::Vector2f& position = sf::Vector2f(0, 0),
                     const sf::Vector2f& origin = sf::Vector2f(0, 0));
#pragma once

#include <string>
#include "SFML/Graphics.hpp"

using std::string;

struct entity_t;

enum colliders_t {
    COLLIDER_CLEAR,
    COLLIDER_SOLID,
    COLLIDER_DEATH,
    COLLIDER_CLIMBABLE,
    COLLIDER_GROUND,
};

struct entity_t {
    string logic;

    string graphicsID;

    colliders_t colliderType;
    std::vector<entity_t> drops;

    string soundFile;
    bool isLoop;

    int drawLayer;

    sf::Sprite sprite;
    sf::RectangleShape rectangle;

    union {
        int value;
        int healthValue;
        int goldValue;
    };

    sf::Vector2f a;
    sf::Vector2f b;
};

entity_t* EntityAlloc();
void EntityDealloc(entity_t* entity);

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
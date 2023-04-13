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
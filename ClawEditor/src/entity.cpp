#include <cassert>

#include "entity.h"
#include "resource_manager.h"

entity_t* EntityAlloc()
{
    printf("[Info][Entity]: Entity Created.\n");
    entity_t* entity = new entity_t;
    return entity;
}

void EntityDealloc(entity_t** entity)
{
    assert(entity);

    delete *entity;
    *entity = nullptr;
}

void EntityCreateTile(entity_t* entity,
                      const char* graphics,
                      colliders_t colliderType,
                      const sf::Vector2f& position,
                      const sf::Vector2f& origin)
{
    assert(entity);

    const sf::Texture& texture = ResTextureGet(graphics);

    entity->logic = "LOGIC_TILES";
    entity->graphicsID = graphics;
    entity->colliderType = colliderType;

    entity->sprite.setTexture(texture);
    entity->sprite.setOrigin(origin);
    entity->sprite.setPosition(position);
}

void EntityCreateOBJ(entity_t* entity,
                     const char* graphics,
                     const char* logic,
                     const sf::Vector2f& position,
                     const sf::Vector2f& origin)
{
    assert(entity);

    const sf::Texture& texture = ResTextureGet(graphics);

    entity->logic = logic;
    entity->graphicsID = graphics;

    entity->sprite.setTexture(texture);
    entity->sprite.setOrigin(origin);
    entity->sprite.setPosition(position);
}
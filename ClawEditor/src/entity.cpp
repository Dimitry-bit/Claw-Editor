#include <cassert>

#include "entity.h"
#include "resource_manager.h"

static void EntityInitRender(entity_t* self, const render_types_t& rType, const std::string_view& graphicID = "");

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

void EntityInit(entity_t* self,
                const std::string_view& logic,
                const render_types_t& rType,
                const std::string_view graphicID)
{
    assert(self);
    EntityInitRender(self, rType, graphicID);
    self->logic = logic;
}

void EntitySet(entity_t* self, const entity_components_t& cType, const void* cValue)
{
    assert(self);
    assert(cValue);

    self->type = cType;
    switch (cType) {
        case C_NONE:break;
        case C_PICKUP: {
            self->pickup = *static_cast<const c_pickup_t*>(cValue);
        }
            break;
        case C_CHECKPOINT: {
            self->checkpoint = *static_cast<const c_checkpoint_t*>(cValue);
        }
            break;
        case C_ENEMY: {
            self->enemy = *static_cast<const c_enemy_t*>(cValue);
        }
            break;
        case C_PLATFORM: {
            self->platform = *static_cast<const c_platform_t*>(cValue);
        }
            break;
        case C_SOUND: {
            self->sound = *static_cast<const c_sound_t*>(cValue);
        }
            break;
        case C_TILE: {
            self->tile = *static_cast<const c_tile_t*>(cValue);
        }
            break;
    }
}

bool EntityHas(entity_t* self, const entity_components_t& cType)
{
    return self->type == cType;
}

static void EntityInitRender(entity_t* self, const render_types_t& rType, const std::string_view& graphicID)
{
    assert(self);

    self->render.type = rType;
    self->render.graphicsID = graphicID;
    if (self->render.type == RENDER_SPRITE && !self->render.graphicsID.empty()) {
        self->render.sprite.setTexture(ResTextureGet(self->render.graphicsID.c_str()));
    }
}

//void EntityCreateTile(entity_t* entity,
//                      const char* graphics,
//                      tile_types_t type,
//                      const sf::Vector2f& position,
//                      const sf::Vector2f& origin)
//{
//    assert(entity);
//
//    const sf::Texture& texture = ResTextureGet(graphics);
//
//    entity->logic = "LOGIC_TILES";
//    entity->graphicsID = graphics;
//    entity->type = type;
//
//    entity->sprite.setTexture(texture);
//    entity->sprite.setOrigin(origin);
//    entity->sprite.setPosition(position);
//}
//
//void EntityCreateOBJ(entity_t* entity,
//                     const char* graphics,
//                     const char* logic,
//                     const sf::Vector2f& position,
//                     const sf::Vector2f& origin)
//{
//    assert(entity);
//
//    const sf::Texture& texture = ResTextureGet(graphics);
//
//    entity->logic = logic;
//    entity->graphicsID = graphics;
//
//    entity->sprite.setTexture(texture);
//    entity->sprite.setOrigin(origin);
//    entity->sprite.setPosition(position);
//}
#include <cassert>

#include "entity.h"
#include "renderer.h"
#include "resource_manager.h"

static void EntityInitRender(entity_t* self, const render_types_t& rType, const std::string_view& graphicID = "");

entity_t* EntityAlloc()
{
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
        const sf::Texture& tex = ResTextureGet(self->render.graphicsID.c_str());
        self->render.sprite.setTexture(tex);
        self->render.sprite.setTextureRect(sf::IntRect(0, 0, tex.getSize().x, tex.getSize().y));
    }
}

void EntityUpdate(entity_t* self, const entity_t* to)
{
    assert(self);
    assert(to);

    *self = *to;
    EntityInit(self, to->logic, to->render.type, to->render.graphicsID);
}

void EntitySetPos(entity_t* self, float x, float y)
{
    assert(self);

    self->render.sprite.setPosition(x, y);
    self->render.rectangleShape.setPosition(x, y);
    self->render.circleShape.setPosition(x, y);
}

void EntitySetPos(entity_t* self, const sf::Vector2f& pos)
{
    EntitySetPos(self, pos.x, pos.y);
}

void EntitySetOrigin(entity_t* self, float x, float y)
{
    assert(self);

    self->render.sprite.setOrigin(x, y);
    self->render.rectangleShape.setOrigin(x, y);
    self->render.circleShape.setOrigin(x, y);
}

void EntitySetOrigin(entity_t* self, const sf::Vector2f& origin)
{
    EntitySetOrigin(self, origin.x, origin.y);
}

entity_transform EntityGetTransform(const entity_t* self)
{
    assert(self);

    entity_transform transform = {};
    if (self->render.type == RENDER_SPRITE) {
        transform.bounds = self->render.sprite.getGlobalBounds();
        transform.position = self->render.sprite.getPosition();
        transform.origin = self->render.sprite.getOrigin();
    } else if (self->render.type == RENDER_RECTANGLE) {
        transform.bounds = self->render.rectangleShape.getGlobalBounds();
        transform.position = self->render.rectangleShape.getPosition();
        transform.origin = self->render.rectangleShape.getOrigin();
    } else if (self->render.type == RENDER_CIRCLE) {
        transform.bounds = self->render.circleShape.getGlobalBounds();
        transform.position = self->render.circleShape.getPosition();
        transform.origin = self->render.circleShape.getOrigin();
    }

    return transform;
}

void DrawEntity(const entity_t* entity)
{
    assert(entity);
    if (entity->render.type == RENDER_NONE) {
        return;
    }

    if (entity->render.type == RENDER_SPRITE) {
        rWindow->draw(entity->render.sprite);
    } else if (entity->render.type == RENDER_RECTANGLE) {
        rWindow->draw(entity->render.rectangleShape);
    } else if (entity->render.type == RENDER_CIRCLE) {
        rWindow->draw(entity->render.circleShape);
    }
}
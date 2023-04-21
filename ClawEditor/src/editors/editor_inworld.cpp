#include <cassert>
#include <sstream>
#include "SFML/Graphics.hpp"

#include "editor_inworld.h"
#include "editor_internal.h"
#include "editor_constants.h"
#include "entity.h"
#include "renderer.h"
#include "resource_manager.h"

static std::map<tile_types_t, sf::Color> colorMap = {
    {TILE_CLEAR, sf::Color::Transparent},
    {TILE_SOLID, colliderSolidColor},
    {TILE_DEATH, colliderDeathColor},
    {TILE_CLIMBABLE, colliderClimbColor},
    {TILE_GROUND, colliderGroundColor},
};

void EditorUpdateInWorldEditors(const editor_context_t* editorContext, const scene_context_t* world)
{
    assert(editorContext);

    DrawOnScreenSpriteData(editorContext->editorHit.entity);

    if (editorContext->mode == EDITOR_MODE_TILE) {
        DrawGridMouseHover(world);
    }
}

void DrawOnScreenSpriteData(const entity_t* entity)
{
    if (!entity) {
        return;
    }

    entity_transform transform = EntityGetTransform(entity);

    sf::Text text;
    text.setFont(ResFontGet(BASE_FONT_FILE_NAME));
    text.setFillColor(spriteDebugColor);
    text.setCharacterSize(sfmlTextSize);
    text.setPosition(transform.position.x + transform.bounds.width + 5.0f, transform.position.y);

    std::stringstream ss;
    ss << "Position: " << transform.position.x << "x" << transform.position.y << '\n'
       << "Logic: " << entity->logic << '\n'
       << "Graphics: " << entity->render.graphicsID << '\n';
    text.setString(ss.str());

    rWindow->draw(text);
    DrawCollider(entity);
    DrawPivotPoint(entity);
}

void DrawMouseCoordinates(const render_context_t* renderContext, const scene_context_t* world)
{
    sf::Vector2i mousePosScreen = sf::Mouse::getPosition();
    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*rWindow);
    sf::Vector2f mousePosView = rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow), renderContext->worldView);
    sf::Vector2u mousePosGrid = SceneGetGridPos(world, mousePosView);

    sf::Text text;
    text.setFont(ResFontGet(BASE_FONT_FILE_NAME));
    text.setFillColor(mouseCoordinatesColor);
    text.setCharacterSize(sfmlTextSize);
    text.setPosition(boundaryOffsetX, boundaryOffsetY);

    std::stringstream ss;
    ss << "Screen: x=" << mousePosScreen.x << " y=" << mousePosScreen.y << '\n'
       << "Window: x=" << mousePosWindow.x << " y=" << mousePosWindow.y << '\n'
       << "View: x=" << mousePosView.x << " y=" << mousePosView.y << '\n'
       << "Grid: x=" << mousePosGrid.x << " y=" << mousePosGrid.y << '\n';
    text.setString(ss.str());

    rWindow->draw(text);
}

void DrawFrameTime(float deltaTime)
{
    sf::Text text;
    text.setFont(ResFontGet(BASE_FONT_FILE_NAME));
    text.setFillColor(frameTimeColor);
    text.setCharacterSize(sfmlTextSize);
    text.setOrigin(0.0f, text.getCharacterSize());
    text.setPosition(boundaryOffsetX, rWindow->getSize().y - boundaryOffsetY);

    std::stringstream ss;
    ss << "FrameTime: " << deltaTime * 100.0f << "ms";
    text.setString(ss.str());

    rWindow->draw(text);
}

void DrawGridMouseHover(const scene_context_t* world)
{
    sf::Vector2f mousePosView = rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow));
    sf::Vector2f tilePos = SceneGetTileStartPos(world, mousePosView);

    if (!(tilePos.x >= 0 && tilePos.x < world->tileSize * world->tileGridWidth) ||
        !(tilePos.y >= 0 && tilePos.y < world->tileSize * world->tileGridHeight)) {
        return;
    }

    static sf::RectangleShape mouseRect(sf::Vector2f(world->tileSize, world->tileSize));
    mouseRect.setFillColor(hoverFillColor);
    mouseRect.setOutlineColor(hoverFrameColor);
    mouseRect.setOutlineThickness(1.0f);
    mouseRect.setPosition(tilePos);

    rWindow->draw(mouseRect);
}

void DrawTileInfo(const entity_t* entity)
{
    if (!entity) {
        return;
    }

    entity_transform transform = EntityGetTransform(entity);

    sf::RectangleShape collider;
    collider.setFillColor(colorMap[entity->tile.type]);
    collider.setSize(sf::Vector2f(transform.bounds.width, transform.bounds.height));
    collider.setOrigin(transform.origin);
    collider.setPosition(transform.position);

    rWindow->draw(collider);
}

void DrawCollider(const entity_t* entity)
{
    if (!entity) {
        return;
    }

    entity_transform transform = EntityGetTransform(entity);

    sf::RectangleShape collider;
    collider.setFillColor(sf::Color::Transparent);
    collider.setOutlineColor(colliderColor);
    collider.setOutlineThickness(1.0f);
    collider.setOrigin(transform.origin);
    collider.setPosition(transform.position);
    collider.setSize(sf::Vector2f(transform.bounds.width, transform.bounds.height));

    rWindow->draw(collider);
}

void DrawPivotPoint(const entity_t* entity)
{
    entity_transform transform = EntityGetTransform(entity);

    sf::CircleShape pivotPoint;
    pivotPoint.setRadius(5.0f);
    pivotPoint.setFillColor(sf::Color::Transparent);
    pivotPoint.setOutlineThickness(1.0f);
    pivotPoint.setOutlineColor(sf::Color::Cyan);
    pivotPoint.setOrigin(pivotPoint.getRadius(), pivotPoint.getRadius());
    pivotPoint.setPosition(transform.bounds.left + transform.origin.x,
                           transform.bounds.top + transform.origin.y);

    rWindow->draw(pivotPoint);
}
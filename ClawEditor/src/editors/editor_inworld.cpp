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

void EditorUpdateInWorldEditors(const render_context_t& renderContext, sf::Time deltaTime)
{
    DrawOnScreenSpriteData(renderContext, editorContext.editorHit.entity);

    if (editorContext.mode == EDITOR_MODE_TILE) {
        DrawGridMouseHover(renderContext);
    }

    DrawMouseCoordinates(renderContext);
    DrawFrameTime(renderContext, deltaTime.asSeconds());
}

void DrawOnScreenSpriteData(const render_context_t& renderContext, const entity_t* entity)
{
    if (!entity || entity->render.type == RENDER_NONE) {
        return;
    }

    static sf::Text text;
    static bool isInit = false;
    if (!isInit) {
        text.setFont(ResFontGet(BASE_FONT_FILE_NAME));
        text.setFillColor(spriteDebugColor);
        text.setCharacterSize(sfmlTextSize);
        isInit = true;
    }

    sf::Vector2f pos;
    sf::FloatRect size;
    if (entity->render.type == RENDER_SPRITE) {
        size = entity->render.sprite.getLocalBounds();
        pos = entity->render.sprite.getPosition();
    } else if (entity->render.type == RENDER_RECTANGLE) {
        size = entity->render.rectangleShape.getLocalBounds();
        pos = entity->render.rectangleShape.getPosition();
    } else if (entity->render.type == RENDER_CIRCLE) {
        size = entity->render.circleShape.getLocalBounds();
        pos = entity->render.circleShape.getPosition();
    }

    text.setPosition(pos.x + size.width + 5.0f, pos.y);

    std::stringstream ss;
    ss << "Position: " << pos.x << "x" << pos.y << '\n'
       << "Logic: " << entity->logic << '\n'
       << "Graphics: " << entity->render.graphicsID << '\n';

    text.setString(ss.str());

    const sf::View& cacheView = rWindow->getView();
    rWindow->setView(renderContext.worldView);
    rWindow->draw(text);
    DrawCollider(entity);
    DrawPivotPoint(entity);
    rWindow->setView(cacheView);
}

void DrawMouseCoordinates(const render_context_t& renderContext)
{
    sf::Vector2i mousePosScreen = sf::Mouse::getPosition();
    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*rWindow);
    sf::Vector2f mousePosView = rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow), renderContext.worldView);
    sf::Vector2u mousePosGrid(mousePosView / (float) gridSize);

    static bool isInit = false;
    static sf::Text text;
    if (!isInit) {
        text.setFont(ResFontGet(BASE_FONT_FILE_NAME));
        text.setFillColor(mouseCoordinatesColor);
        text.setCharacterSize(sfmlTextSize);
        text.setPosition(boundaryOffsetX, boundaryOffsetY);
        isInit = true;
    }

    std::stringstream ss;
    ss << "Screen: x=" << mousePosScreen.x << " y=" << mousePosScreen.y << '\n'
       << "Window: x=" << mousePosWindow.x << " y=" << mousePosWindow.y << '\n'
       << "View: x=" << mousePosView.x << " y=" << mousePosView.y << '\n'
       << "Grid: x=" << mousePosGrid.x << " y=" << mousePosGrid.y << '\n';

    text.setString(ss.str());

    const sf::View& cacheView = rWindow->getView();
    rWindow->setView(renderContext.uiView);
    rWindow->draw(text);
    rWindow->setView(cacheView);
}

void DrawFrameTime(const render_context_t& renderContext, float deltaTime)
{
    static sf::Text text;
    static bool initText = false;
    if (!initText) {
        text.setFont(ResFontGet(BASE_FONT_FILE_NAME));
        text.setFillColor(frameTimeColor);
        text.setCharacterSize(sfmlTextSize);
        text.setOrigin(0.0f, text.getCharacterSize());
        initText = true;
    }

    text.setPosition(boundaryOffsetX, rWindow->getSize().y - boundaryOffsetY);

    std::stringstream ss;
    ss << "FrameTime: " << deltaTime * 100.0f << "ms";

    text.setString(ss.str());

    const sf::View& cacheView = rWindow->getView();
    rWindow->setView(renderContext.uiView);
    rWindow->draw(text);
    rWindow->setView(cacheView);
}

void DrawGridMouseHover(const render_context_t& renderContext)
{
    const sf::View& cacheView = rWindow->getView();
    rWindow->setView(renderContext.worldView);

    sf::Vector2f mousePosView = rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow));
    sf::Vector2u mousePosGrid(mousePosView / (float) gridSize);

    static bool isInit = false;
    static sf::RectangleShape mouseRect(sf::Vector2f(gridSize, gridSize));
    if (!isInit) {
        mouseRect.setFillColor(hoverFillColor);
        mouseRect.setOutlineColor(hoverFrameColor);
        mouseRect.setOutlineThickness(1.0f);
        isInit = true;
    }
    mouseRect.setPosition(sf::Vector2f(mousePosGrid * (unsigned int) gridSize));

    rWindow->draw(mouseRect);
    rWindow->setView(cacheView);
}

void DrawTileInfo(const entity_t* entity)
{
    if (!entity) {
        return;
    }

    if (entity->render.type == RENDER_NONE) {
        return;
    }

    sf::FloatRect size;
    sf::Vector2f pos;
    sf::Vector2f originPos;
    if (entity->render.type == RENDER_SPRITE) {
        size = entity->render.sprite.getGlobalBounds();
        pos = entity->render.sprite.getPosition();
        originPos = entity->render.sprite.getOrigin();
    } else if (entity->render.type == RENDER_RECTANGLE) {
        size = entity->render.rectangleShape.getLocalBounds();
        pos = entity->render.rectangleShape.getPosition();
        originPos = entity->render.rectangleShape.getOrigin();
    } else if (entity->render.type == RENDER_CIRCLE) {
        size = entity->render.circleShape.getLocalBounds();
        pos = entity->render.circleShape.getPosition();
        originPos = entity->render.circleShape.getOrigin();
    }

    static sf::RectangleShape collider;
    collider.setFillColor(colorMap[entity->tile.type]);
    collider.setSize(sf::Vector2f(size.width, size.height));
    collider.setOrigin(originPos);
    collider.setPosition(pos);

    rWindow->draw(collider);
}

void DrawCollider(const entity_t* entity)
{
    if (!entity) {
        return;
    }

    sf::FloatRect size;
    sf::Vector2f pos;
    sf::Vector2f originPos;
    sf::RectangleShape globalBounds;
    if (entity->render.type == RENDER_SPRITE) {
        size = entity->render.sprite.getGlobalBounds();
        pos = entity->render.sprite.getPosition();
        originPos = entity->render.sprite.getOrigin();
    } else if (entity->render.type == RENDER_RECTANGLE) {
        size = entity->render.rectangleShape.getLocalBounds();
        pos = entity->render.rectangleShape.getPosition();
        originPos = entity->render.rectangleShape.getOrigin();
    } else if (entity->render.type == RENDER_CIRCLE) {
        size = entity->render.circleShape.getLocalBounds();
        pos = entity->render.circleShape.getPosition();
        originPos = entity->render.circleShape.getOrigin();
    }

    sf::RectangleShape collider;
    collider.setFillColor(sf::Color::Transparent);
    collider.setOutlineColor(colliderColor);
    collider.setOutlineThickness(1.0f);
    collider.setOrigin(originPos);
    collider.setPosition(pos);
    collider.setSize(sf::Vector2f(size.width, size.height));

    rWindow->draw(collider);
}

void DrawPivotPoint(const entity_t* entity)
{
    static sf::CircleShape pivotPoint;

    sf::FloatRect size;
    sf::Vector2f originPos;
    sf::RectangleShape globalBounds;
    if (entity->render.type == RENDER_SPRITE) {
        size = entity->render.sprite.getGlobalBounds();
        originPos = entity->render.sprite.getOrigin();
    } else if (entity->render.type == RENDER_RECTANGLE) {
        size = entity->render.rectangleShape.getLocalBounds();
        originPos = entity->render.rectangleShape.getOrigin();
    } else if (entity->render.type == RENDER_CIRCLE) {
        size = entity->render.circleShape.getLocalBounds();
        originPos = entity->render.circleShape.getOrigin();
    }

    pivotPoint.setRadius(5.0f);
    pivotPoint.setFillColor(sf::Color::Transparent);
    pivotPoint.setOutlineThickness(1.0f);
    pivotPoint.setOutlineColor(sf::Color::Cyan);
    pivotPoint.setOrigin(pivotPoint.getRadius(), pivotPoint.getRadius());
    pivotPoint.setPosition(size.left + originPos.x, size.top + originPos.y);

    rWindow->draw(pivotPoint);
}
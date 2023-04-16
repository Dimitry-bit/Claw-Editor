#include <sstream>
#include "SFML/Graphics.hpp"

#include "editor_inworld.h"
#include "editor_internal.h"
#include "editor_constants.h"
#include "entity.h"
#include "renderer.h"
#include "resource_manager.h"

static std::map<colliders_t, sf::Color> colorMap = {
    {COLLIDER_CLEAR, sf::Color::Transparent},
    {COLLIDER_SOLID, colliderSolidColor},
    {COLLIDER_DEATH, colliderDeathColor},
    {COLLIDER_CLIMBABLE, colliderClimbColor},
    {COLLIDER_GROUND, colliderGroundColor},
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

void DrawOnScreenSpriteData(const render_context_t& renderContext, const entity_t* drawable)
{
    if (!drawable) {
        return;
    }

    static bool isInit = false;

    sf::FloatRect drawableSize = drawable->sprite.getLocalBounds();
    sf::Vector2f drawablePos = drawable->sprite.getPosition();

    static sf::RectangleShape collider;
    static sf::CircleShape origin;
    if (!isInit) {
        origin.setRadius(5.0f);
        origin.setFillColor(sf::Color::Transparent);
        origin.setOutlineThickness(1.0f);
        origin.setOutlineColor(sf::Color::Cyan);
        origin.setOrigin(origin.getRadius(), origin.getRadius());
        collider.setFillColor(sf::Color::Transparent);
        collider.setOutlineColor(colliderColor);
        collider.setOutlineThickness(1.0f);
    }

    origin.setPosition(drawable->sprite.getGlobalBounds().left + drawable->sprite.getOrigin().x,
                       drawable->sprite.getGlobalBounds().top + drawable->sprite.getOrigin().y);

    collider.setSize(sf::Vector2f(drawableSize.width, drawableSize.height));
    collider.setOrigin(drawable->sprite.getOrigin());
    collider.setPosition(drawable->sprite.getPosition());

    static sf::Text text;
    if (!isInit) {
        text.setFont(ResFontGet(BASE_FONT_FILE_NAME));
        text.setFillColor(spriteDebugColor);
        text.setCharacterSize(sfmlTextSize);
        isInit = true;
    }
    text.setPosition(collider.getPosition().x + drawableSize.width + 5.0f, collider.getPosition().y);

    std::stringstream ss;
    ss << "Position: " << drawablePos.x << "x" << drawablePos.y << '\n'
       << "Logic: " << drawable->logic << '\n';

    if (!drawable->graphicsID.empty()) {
        ss << "Graphics: " << drawable->graphicsID << '\n';
    }

    text.setString(ss.str());

    const sf::View& cacheView = rWindow->getView();
    rWindow->setView(renderContext.worldView);
    rWindow->draw(text);
    rWindow->draw(collider);
    rWindow->draw(origin);
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

void DrawCollider(const entity_t* entity)
{
    if (!entity) {
        return;
    }

    sf::FloatRect drawableSize = entity->sprite.getLocalBounds();

    static sf::RectangleShape collider;
    collider.setFillColor(colorMap[entity->colliderType]);
    collider.setSize(sf::Vector2f(drawableSize.width, drawableSize.height));
    collider.setOrigin(entity->sprite.getOrigin());
    collider.setPosition(entity->sprite.getPosition());

    rWindow->draw(collider);
}
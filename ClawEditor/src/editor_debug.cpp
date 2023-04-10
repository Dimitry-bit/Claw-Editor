#include <sstream>
#include "SFML/Graphics.hpp"

#include "editor_debug.h"
#include "editor_constants.h"
#include "renderer.h"
#include "resource_manager.h"

const sf::Color frameTimeColor = sf::Color::White;
const sf::Color mouseCoordinatesColor = sf::Color::White;
const sf::Color spriteDebugColor = sf::Color::Yellow;
const sf::Color colliderColor = sf::Color::Red;

void DrawOnScreenSpriteData(const sf::Sprite& drawable)
{
    sf::FloatRect drawableSize = drawable.getLocalBounds();
    sf::Vector2f drawablePos = drawable.getPosition();
    sf::RectangleShape collider(sf::Vector2f(drawableSize.width, drawableSize.height));
    collider.setFillColor(sf::Color::Transparent);
    collider.setOutlineColor(colliderColor);
    collider.setOutlineThickness(1.0f);
    collider.setOrigin(drawable.getOrigin());
    collider.setPosition(drawable.getPosition());

    sf::Text text;
    text.setFont(ResFontGet(BASE_FONT_FILE_NAME));
    text.setFillColor(spriteDebugColor);
    text.setCharacterSize(10);
    text.setPosition(collider.getPosition().x + drawableSize.width + 5.0f, collider.getPosition().y);

    std::stringstream ss;
    ss << "Position: " << drawablePos.x << "x" << drawablePos.y << '\n';

    text.setString(ss.str());
    rWindow->draw(text);
    rWindow->draw(collider);
}

void DrawMouseCoordinates()
{
    sf::Vector2i mousePosScreen = sf::Mouse::getPosition();
    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*rWindow);
    sf::Vector2f mousePosView = rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow), view);
    sf::Vector2u mousePosGrid;
    mousePosGrid.x = mousePosView.x / gridSize;
    mousePosGrid.y = mousePosView.y / gridSize;

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
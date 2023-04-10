#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include "renderer.h"
#include "editor.h"
#include "editor_debug.h"
#include "resource_manager.h"

const int windowSizeX = 1280;
const int windowSizeY = 720;
const int gridSize = 64;

sf::View view;

void RenderWindow(sf::Time deltaTime)
{
    sf::Sprite rShap;
    rShap.setTexture(ResTextureGet("ACTION/012"));

    rWindow->clear();
    rWindow->setView(view);

    if (rShap.getGlobalBounds().contains(sf::Vector2f(rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow))))) {
        sf::Color col = rShap.getColor();
        col.a = 250;
        rShap.setColor(col);
    } else {
        sf::Color col = rShap.getColor();
        col.a = 255;
        rShap.setColor(col);
    }

    DrawOnScreenSpriteData(rShap);

    rWindow->draw(rShap);

    rWindow->setView(rWindow->getDefaultView());
    EditorTick(deltaTime);
    rWindow->display();
}

void RendererInit()
{
    view.setSize(windowSizeX, windowSizeY);
    view.setCenter(rWindow->getSize().x / 2.0f, rWindow->getSize().y / 2.0f);
}

void DrawGrid()
{
}

void DrawCollider(const sf::Sprite& s)
{
    sf::FloatRect globalBounds = s.getGlobalBounds();
    sf::RectangleShape colliderBoundaries;
    colliderBoundaries.setFillColor(sf::Color::Transparent);
    colliderBoundaries.setOutlineColor(sf::Color::Green);
    colliderBoundaries.setOutlineThickness(1.0f);
    colliderBoundaries.setPosition(globalBounds.left, globalBounds.top);
    colliderBoundaries.setSize(sf::Vector2f(globalBounds.width, globalBounds.height));

    rWindow->draw(colliderBoundaries);
}

void DrawOrigin(const sf::Sprite& s)
{
    sf::CircleShape origin;
    origin.setRadius(3.0f);
    origin.setOrigin(origin.getRadius(), origin.getRadius());
    origin.setFillColor(sf::Color::Magenta);
    origin.setPosition(s.getGlobalBounds().left + s.getOrigin().x, s.getGlobalBounds().top + s.getOrigin().y);

    rWindow->draw(origin);
}
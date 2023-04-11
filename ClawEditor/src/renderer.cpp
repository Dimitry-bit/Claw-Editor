#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include "renderer.h"
#include "renderer_grid.h"
#include "editor.h"
#include "editor_debug.h"
#include "resource_manager.h"

const int windowSizeX = 1280;
const int windowSizeY = 720;
const int gridSize = 64;

sf::View view;
sf::View defaultView;

void RenderWindow(sf::Time deltaTime)
{
    static bool isInit = false;
    static sf::Sprite tileGrid[MAX_GRID_SIZE][MAX_GRID_SIZE];
    if (!isInit) {
        for (int x = 0; x < MAX_GRID_SIZE; ++x) {
            for (int y = 0; y < MAX_GRID_SIZE; ++y) {
                tileGrid[x][y].setTexture(ResTextureGet("ACTION/012"));
                tileGrid[x][y].setPosition(x * gridSize, y * gridSize);
            }
        }
        isInit = true;
    }

    rWindow->clear();
    rWindow->setView(view);

    DrawGrid(tileGrid);
    DrawGridMouseHover();

    rWindow->setView(defaultView);
    EditorTick(deltaTime);
    rWindow->display();
}

void RendererInit(int rWidth, int rHeight)
{
    defaultView = sf::View(sf::FloatRect(0.0f, 0.0f, rWidth, rHeight));
    view.setSize(rWidth, rHeight);
    view.setCenter(rWindow->getSize().x / 2.0f, rWindow->getSize().y / 2.0f);
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
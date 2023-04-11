#include <algorithm>

#include "renderer_grid.h"
#include "renderer.h"

const sf::Color hoverFillColor = sf::Color(255, 255, 255, 35);
const sf::Color hoverFrameColor = sf::Color(255, 255, 255, 255);

void DrawGrid(const sf::Sprite tileGrid[][MAX_GRID_SIZE])
{
    const sf::Vector2f drawCenter = rWindow->getView().getCenter();
    const sf::Vector2f viewSize = rWindow->getView().getSize();
    const float width = viewSize.x / 2;
    const float height = viewSize.y / 2;

    int fromX = (drawCenter.x - width) / gridSize - 2;
    int toX = (drawCenter.x + width) / gridSize + 2;
    int fromY = (drawCenter.y - height) / gridSize - 2;
    int toY = (drawCenter.y + height) / gridSize + 2;

    fromX = std::clamp(fromX, 0, MAX_GRID_SIZE - 1);
    toX = std::clamp(toX, 0, MAX_GRID_SIZE - 1);
    fromY = std::clamp(fromY, 0, MAX_GRID_SIZE - 1);
    toY = std::clamp(toY, 0, MAX_GRID_SIZE - 1);

    for (int x = fromX; x < toX; ++x) {
        for (int y = fromY; y < toY; ++y) {
            rWindow->draw(tileGrid[x][y]);
        }
    }
}

void DrawGridMouseHover()
{
    sf::Vector2f mousePosView = rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow), view);
    sf::Vector2u mousePosGrid(mousePosView.x / gridSize, mousePosView.y / gridSize);

    sf::RectangleShape mouseRect(sf::Vector2f(gridSize, gridSize));
    mouseRect.setFillColor(hoverFillColor);
    mouseRect.setOutlineColor(hoverFrameColor);
    mouseRect.setOutlineThickness(1.0f);
    mouseRect.setPosition(mousePosGrid.x * gridSize, mousePosGrid.y * gridSize);

    rWindow->draw(mouseRect);
}
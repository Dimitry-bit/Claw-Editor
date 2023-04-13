#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include "renderer.h"
#include "editor.h"

sf::RenderWindow* rWindow;

static void DrawGrid(const entity_t tileGrid[][MAX_GRID_SIZE], int size);

void RendererInit(render_context_t& renderContext, int rWidth, int rHeight)
{
    renderContext.uiView = sf::View(sf::FloatRect(0.0f, 0.0f, rWidth, rHeight));
    renderContext.worldView.setSize(rWidth, rHeight);
    renderContext.worldView.setCenter(rWindow->getSize().x / 2.0f, rWindow->getSize().y / 2.0f);
}

void UpdateAndRenderWindow(render_context_t& renderContext, sf::Time deltaTime)
{
    rWindow->clear();

    rWindow->setView(renderContext.worldView);
    DrawGrid(renderContext.sceneContext.tileGrid, gridSize);

    rWindow->setView(renderContext.uiView);
    UpdateAndRenderEditor(renderContext, deltaTime);

    rWindow->display();
}

void DrawGrid(const entity_t tileGrid[][MAX_GRID_SIZE], int size)
{
    const sf::Vector2f drawCenter = rWindow->getView().getCenter();
    const sf::Vector2f viewSize = rWindow->getView().getSize();
    const float width = viewSize.x / 2;
    const float height = viewSize.y / 2;

    int fromX = (drawCenter.x - width) / size - 2;
    int toX = (drawCenter.x + width) / size + 2;
    int fromY = (drawCenter.y - height) / size - 2;
    int toY = (drawCenter.y + height) / size + 2;

    fromX = std::clamp(fromX, 0, MAX_GRID_SIZE - 1);
    toX = std::clamp(toX, 0, MAX_GRID_SIZE - 1);
    fromY = std::clamp(fromY, 0, MAX_GRID_SIZE - 1);
    toY = std::clamp(toY, 0, MAX_GRID_SIZE - 1);

    for (int x = fromX; x < toX; ++x) {
        for (int y = fromY; y < toY; ++y) {
            rWindow->draw(tileGrid[x][y].sprite);
        }
    }
}
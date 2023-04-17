#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include "renderer.h"
#include "editor.h"
#include "editors/editor_inworld.h"

sf::RenderWindow* rWindow;

static render_context_t renderContext;

static void DrawGrid(const std::array<std::array<entity_t*, MAX_GRID_SIZE>, MAX_GRID_SIZE>& tileGrid);
static void DrawEntities(const std::list<entity_t*>& entities);

void RendererInit(int rWidth, int rHeight)
{
    renderContext.uiView = sf::View(sf::FloatRect(0.0f, 0.0f, rWidth, rHeight));
    renderContext.worldView.setSize(rWidth, rHeight);
    renderContext.worldView.setCenter(rWindow->getSize().x / 2.0f, rWindow->getSize().y / 2.0f);
}

void UpdateAndRenderWindow(sf::Time deltaTime)
{
    rWindow->clear();

    rWindow->setView(renderContext.worldView);
    DrawGrid(SceneGetTileGrid());
    DrawEntities(SceneGetEntities());

    rWindow->setView(renderContext.uiView);
    UpdateAndRenderEditor(renderContext, deltaTime);

    rWindow->display();
}

static void DrawGrid(const std::array<std::array<entity_t*, MAX_GRID_SIZE>, MAX_GRID_SIZE>& tileGrid)
{
    const sf::Vector2f drawCenter = rWindow->getView().getCenter();
    const sf::Vector2f viewSize = rWindow->getView().getSize();
    const float width = viewSize.x / 2;
    const float height = viewSize.y / 2;
    const int size = tileGrid.size();

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
            if (!tileGrid[x][y])
                continue;

            if (tileGrid[x][y]->render.type == RENDER_SPRITE) {
                rWindow->draw(tileGrid[x][y]->render.sprite);
            } else if (tileGrid[x][y]->render.type == RENDER_RECTANGLE) {
                rWindow->draw(tileGrid[x][y]->render.rectangleShape);
            }

            if (renderContext.isDrawCollider) {
                DrawTileInfo(tileGrid[x][y]);
            }
        }
    }
}

static void DrawEntities(const std::list<entity_t*>& entities)
{
    for (auto& entity: entities) {
        if (entity->render.type == RENDER_NONE) {
            continue;
        }

        if (entity->render.type == RENDER_SPRITE) {
            rWindow->draw(entity->render.sprite);
        } else if (entity->render.type == RENDER_RECTANGLE) {
            rWindow->draw(entity->render.rectangleShape);
        } else if (entity->render.type == RENDER_CIRCLE) {
            rWindow->draw(entity->render.circleShape);
        }
    }
}

render_context_t& GetRenderContext()
{
    return renderContext;
}
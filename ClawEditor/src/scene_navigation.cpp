#include "SFML/System.hpp"

#include "scene_navigation.h"
#include "renderer.h"
#include "input.h"

const float zoomFactor = 0.1f;
const float maxZoom = 2.0f;

float gameZoom = 1.0f;

sf::Cursor cursorHand;
sf::Cursor cursorArrow;

static void SceneMovement();
static void SceneZoom();

void SceneNavigationInit()
{
    cursorHand.loadFromSystem(sf::Cursor::Hand);
    cursorArrow.loadFromSystem(sf::Cursor::Arrow);
}

void SceneNavigationUpdate()
{
    SceneMovement();
    SceneZoom();
}

void SceneMovement()
{
    static sf::Vector2i oldPos;
    sf::Vector2i currentPos;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        currentPos = sf::Mouse::getPosition(*rWindow);
        sf::Vector2f pos = rWindow->mapPixelToCoords(sf::Vector2i(oldPos.x - currentPos.x, oldPos.y - currentPos.y));
        view.move(pos.x / gameZoom, pos.y / gameZoom);

        rWindow->setView(view);
        oldPos = currentPos;
        rWindow->setMouseCursor(cursorHand);
    } else {
        oldPos = sf::Mouse::getPosition(*rWindow);
        rWindow->setMouseCursor(cursorArrow);
    }
}

void SceneZoom()
{
    if (!IsMouseWheelScrolled() || GetMouseWheelScroll().wheel != sf::Mouse::Wheel::VerticalWheel) {
        return;
    }

    gameZoom += zoomFactor * GetMouseWheelScroll().delta;

    if (gameZoom >= maxZoom) {
        gameZoom = maxZoom;
    } else if (gameZoom <= 1 / maxZoom) {
        gameZoom = 1 / maxZoom;
    }

    sf::Vector2f viewSize = sf::Vector2f(rWindow->getSize());
    viewSize.x /= gameZoom;
    viewSize.y /= gameZoom;
    view.setSize(viewSize);
}
#include <iostream>
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include "input.h"
#include "renderer.h"
#include "editor.h"
#include "resource_manager.h"

sf::RenderWindow* rWindow;

void HandleEvent();

int main()
{
    sf::RenderWindow window(sf::VideoMode(windowSizeX, windowSizeY), "Claw Editor");
    rWindow = &window;

//    if (ResTextureLoadFromFile("characters/CLAW_RUN.png")) {
//        sf::Texture tex = ResTextureGet("characters/CLAW_RUN.png");
//        sf::Sprite s;

    }

    ResSpriteSheetLoadFromFile("tilesets/LEVEL1_TILES.png");
    ResTextureLoadFromSpriteSheet(*ResSpriteSheetGet("tilesets/LEVEL1_TILES.png"));

    EditorInit();

    RendererInit();
    sf::Vector2i oldPos;
    sf::Vector2i currentPos;

    sf::Clock deltaClock;
    while (window.isOpen()) {
        HandleEvent();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            currentPos = sf::Mouse::getPosition(*rWindow);
            sf::Vector2f pos = window.mapPixelToCoords(sf::Vector2i(oldPos.x - currentPos.x, oldPos.y - currentPos.y));
            view.move(pos.x, pos.y);
            window.setView(view);
            oldPos = currentPos;
        } else {
            oldPos = sf::Mouse::getPosition(*rWindow);
        }

        RenderWindow(deltaClock.restart());
        ClearKeyStatus();
    }
    EditorShutdown();
    ResUnloadAll();

    return 0;
}

void HandleEvent()
{
    sf::Event event;
    while (rWindow->pollEvent(event)) {
        InputEvent(event);
        EditorEvent(event);
        if (event.type == sf::Event::Closed) {
            rWindow->close();
        }
    }
}
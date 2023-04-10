#include <iostream>
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include "input.h"
#include "renderer.h"
#include "editor.h"
#include "resource_manager.h"
#include "scene_navigation.h"

sf::RenderWindow* rWindow;

void HandleEvent();

int main()
{
    sf::RenderWindow window(sf::VideoMode(windowSizeX, windowSizeY), "Claw Editor");
    rWindow = &window;

    ResSpriteSheetLoadFromFile("tilesets/LEVEL1_TILES.png");
    ResTextureLoadFromSpriteSheet(*ResSpriteSheetGet("tilesets/LEVEL1_TILES.png"));

    EditorInit();
    RendererInit();
    SceneNavigationInit();

    sf::Clock deltaClock;
    while (window.isOpen()) {
        HandleEvent();
        SceneNavigationUpdate();
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
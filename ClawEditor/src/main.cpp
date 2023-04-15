#include "SFML/Graphics.hpp"

#include "input.h"
#include "renderer.h"
#include "editor.h"
#include "resource_manager.h"
#include "scene_navigation.h"
#include "scene_manager.h"

const int initWindowWidth = 1280;
const int initWindowHeight = 720;
const char* windowName = "Claw Editor";

void ProgramCleanUp();

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(initWindowWidth, initWindowHeight),
                            windowName,
                            sf::Style::Default,
                            contextSettings);
    rWindow = &window;

    render_context_t renderContext = {
    };

    SceneAllocAssets();
    SceneInitGrid();

    EditorInit();
    RendererInit(renderContext, initWindowWidth, initWindowHeight);
    SceneNavigationInit();

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (rWindow->pollEvent(event)) {
            InputEvent(event);
            EditorEvent(event);
            if (event.type == sf::Event::Closed) {
                rWindow->close();
            } else if (event.type == sf::Event::Resized) {
                RendererInit(renderContext, event.size.width, event.size.height);
            }
        }

        SceneNavigationUpdate(renderContext);
        UpdateAndRenderWindow(renderContext, deltaClock.restart());
        ClearKeyStatus();
    }
    ProgramCleanUp();

    return 0;
}

void ProgramCleanUp()
{
    EditorShutdown();
    SceneDealloc();
    ResUnloadAll();
}
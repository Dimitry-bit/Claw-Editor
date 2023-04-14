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

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(initWindowWidth, initWindowHeight),
                            windowName,
                            sf::Style::Default,
                            contextSettings);
    rWindow = &window;

    scene_context_t sceneContext;
    render_context_t renderContext = {
        .sceneContext  = sceneContext,
        .assetContext {
        }
    };

    SceneAllocAssets();
    SceneInitGrid(sceneContext);

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
    EditorShutdown();
    SceneDealloc(sceneContext);
    ResUnloadAll();

    return 0;
}
#include "imgui.h"
#include "SFML/Graphics.hpp"

#include "claw_editor.h"
#include "input.h"
#include "renderer.h"
#include "editor.h"
#include "resource_manager.h"
#include "scene_navigation.h"
#include "scene_manager.h"
#include "editors/editor_inworld.h"
#include "editors/editor_imgui.h"

const int initWindowWidth = 1280;
const int initWindowHeight = 720;

static void HandleEvent(render_context_t* renderContext, editor_context_t* editorContext);
static void ProgramCleanUp(scene_context_t* world);

static void UpdateAndRenderWindow(render_context_t* renderContext,
                                  scene_context_t* world,
                                  editor_context_t* editorContext,
                                  sf::Time deltaTime);

void ClawEditMain()
{
    sf::ContextSettings contextSettings;
    contextSettings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(initWindowWidth, initWindowHeight),
                            windowName, sf::Style::Default, contextSettings);
    rWindow = &window;

    sf::Image icon;
    if (icon.loadFromFile("icon.png")) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    render_context_t renderContext = {};
    editor_context_t editorContext = {};
    scene_context_t world;
    world.tileGridWidth = MAX_GRID_SIZE;
    world.tileGridHeight = MAX_GRID_SIZE;
    world.tileSize = 64;
    entity_t* tileGrid[MAX_GRID_SIZE][MAX_GRID_SIZE] = {};
    world.tileGrid = (entity_t**) tileGrid;

    SceneAllocAssets(&world);
    RendererInit(&renderContext, initWindowWidth, initWindowHeight);
    EditorInit(&editorContext);
    SceneNavigationInit();

    sf::Clock deltaClock;
    while (window.isOpen()) {
        UpdateAndRenderWindow(&renderContext, &world, &editorContext, deltaClock.restart());
    }
    ProgramCleanUp(&world);
}

static void HandleEvent(render_context_t* renderContext, editor_context_t* editorContext)
{
    sf::Event event;
    while (rWindow->pollEvent(event)) {
        InputEvent(event);
        EditorEvent(editorContext, event);
        if (event.type == sf::Event::Closed) {
            rWindow->close();
        } else if (event.type == sf::Event::Resized) {
            RendererInit(renderContext, event.size.width, event.size.height);
        }
    }
}

static void ProgramCleanUp(scene_context_t* world)
{
    EditorShutdown();
    SceneDealloc(world);
    ResUnloadAll();
}

static void UpdateAndRenderWindow(render_context_t* renderContext,
                                  scene_context_t* world,
                                  editor_context_t* editorContext,
                                  sf::Time deltaTime)
{
    HandleEvent(renderContext, editorContext);

    rWindow->clear();

    rWindow->setView(renderContext->worldView);
    DrawWorld(renderContext, world);
    EditorUpdateInWorldEditors(editorContext, world);

    rWindow->setView(renderContext->uiView);
    SceneNavigationUpdate(renderContext);
    DrawMouseCoordinates(renderContext, world);
    DrawFrameTime(deltaTime.asSeconds());

    rWindow->setView(renderContext->worldView);
    EditorUpdateImGuiEditors(editorContext, renderContext, world, deltaTime);

    rWindow->display();

    ClearKeyStatus();
}
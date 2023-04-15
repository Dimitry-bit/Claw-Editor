#include <iostream>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"
#include "fonts/IconsMaterialDesign.h"

#include "editor.h"
#include "editor_internal.h"
#include "editor_imgui.h"
#include "editor_debug.h"
#include "editor_actions.h"
#include "input.h"
#include "renderer.h"
#include "resource_manager.h"

editor_context_t editorContext;

static void EditorInitFont();
static void EditorUpdateWindows();
static void EditorRegisterWindow(const char* tab,
                                 const char* name,
                                 editorwindowCallback_t callback,
                                 sf::Keyboard::Key shortcutKey = sf::Keyboard::Unknown,
                                 bool defaultState = false);

void EditorInit()
{
    ImGui::SFML::Init(*rWindow);
    EditorInitFont();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard;

    EditorRegisterWindow("Tools", ICON_MD_COLLECTIONS "Image Set", DrawImageSet, sf::Keyboard::I, true);
    EditorRegisterWindow("Tools", ICON_MD_BRUSH "Tile Painter", DrawTilePainter, sf::Keyboard::T, true);
    EditorRegisterWindow("Tools", ICON_MD_VIEW_IN_AR "Object Painter", DrawObjectPainter, sf::Keyboard::O, true);
    EditorRegisterWindow("Help", "About", DrawAboutWindow);
}

static void EditorInitFont()
{
    AssetPushType(ASSET_FONT);
    ResLoadFromFile(BASE_FONT_FILE_NAME);
    AssetPopType();

    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig fontConfig;
    fontConfig.MergeMode = true;
    fontConfig.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("../resources/fonts/" BASE_FONT_FILE_NAME, baseFontSize, &fontConfig);

    static const ImWchar iconsRanges[] = {ICON_MIN_MD, ICON_MAX_16_MD, 0};
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    iconsConfig.GlyphOffset.y += 5.0f;
    iconsConfig.GlyphExtraSpacing.x += 5.0f;
    io.Fonts->AddFontFromFileTTF("../resources/fonts/" FONT_ICON_FILE_NAME_MD, iconFontSize, &iconsConfig, iconsRanges);
    io.Fonts->Build();
    ImGui::SFML::UpdateFontTexture();
}

void EditorShutdown()
{
    ResFontUnload(BASE_FONT_FILE_NAME);
    ImGui::SFML::Shutdown();
}

void EditorEvent(sf::Event event)
{
    ImGui::SFML::ProcessEvent(event);

    if (event.type != sf::Event::KeyPressed) {
        return;
    }

    for (auto& tabEditors: editorContext.editorsMap) {
        for (auto& eWindow: tabEditors.second) {
            if (isKeyPressed(eWindow.shortcutKey)) {
                eWindow.isOpen = !eWindow.isOpen;
                return;
            }
        }
    }

}

void UpdateAndRenderEditor(render_context_t& renderContext, sf::Time deltaTime)
{
    ImGui::SFML::Update(*rWindow, deltaTime);

    DrawOnScreenSpriteData(renderContext, editorContext.editorHit.entity);

    if (editorContext.mode == EDITOR_MODE_TILE) {
        DrawGridMouseHover(renderContext);
    }

    DrawMouseCoordinates(renderContext);
    DrawFrameTime(renderContext, deltaTime.asSeconds());
    DrawMainMenuBar(renderContext);
    DrawStatusBar();

    EditorUpdateWindows();

    ImGuiIO& io = ImGui::GetIO();
    if (!(io.WantCaptureMouse || io.WantCaptureKeyboard)) {
        if (isMousePressed(sf::Mouse::Left)) {
            ActionPlaceEntity(renderContext);
        }
        if (isKeyPressed(sf::Keyboard::Delete)) {
            ActionDeleteEntity(&editorContext.editorHit.entity);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            const sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(*rWindow);
            const sf::Vector2f mouseViewPos = rWindow->mapPixelToCoords(mouseWindowPos, renderContext.worldView);
            SceneIsEntityHit(mouseViewPos, &editorContext.editorHit.entity);
        }
    }

    ImGui::SFML::Render(*rWindow);
}

static void EditorUpdateWindows()
{
    for (auto& tabEditors: editorContext.editorsMap) {
        for (auto& eWindow: tabEditors.second) {
            if (!eWindow.isOpen || !eWindow.callback)
                continue;

            eWindow.callback(eWindow);
        }
    }
}

static void EditorRegisterWindow(const char* tab,
                                 const char* identifier,
                                 editorwindowCallback_t callback,
                                 sf::Keyboard::Key shortcutKey,
                                 bool defaultState)
{
    editorwindow_t eWindow{
        .name = identifier,
        .isOpen = defaultState,
        .shortcutKey = shortcutKey,
        .callback = callback
    };

    editorContext.editorsMap[tab].push_back(eWindow);
    printf("[INFO][Editor]: \"%s\" -> \"%s\" window registered successfully.\n", tab, identifier);
}
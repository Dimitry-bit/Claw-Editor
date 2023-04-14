#include <iostream>
#include <vector>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"
#include "fonts/IconsMaterialDesign.h"
#include "SFML/Graphics.hpp"

#include "editor.h"
#include "editor_constants.h"
#include "editor_imgui.h"
#include "editor_debug.h"
#include "editor_actions.h"
#include "version.h"
#include "input.h"
#include "renderer.h"
#include "resource_manager.h"
#include "key_to_string.h"

static std::map<string, std::vector<editorwindow_t>> editorsMap;

static void EditorRegisterWindow(const char* tab,
                                 const char* name,
                                 editorwindowCallback_t callback,
                                 sf::Keyboard::Key shortcutKey = sf::Keyboard::Unknown,
                                 bool defaultState = false);
static void EditorUnRegisterWindow(editorwindowCallback_t callback);

static void EditorInitFont();
static void DrawMainMenuBar(render_context_t& renderContext);
static void DrawStatusBar(editor_context_t& editorContext);
static void DrawAboutWindow(editor_context_t& context, editorwindow_t& editorwindow);
static bool IsHitEntity(editor_context_t& editorContext, const render_context_t& renderContext);

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
    io.Fonts->AddFontFromFileTTF("../resources/fonts/" FONT_ICON_FILE_NAME_MD,
                                 iconFontSize, &iconsConfig, iconsRanges);
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

    for (auto& tabEditors: editorsMap) {
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
    static editor_context_t editorContext;

    IsHitEntity(editorContext, renderContext);

    ImGui::SFML::Update(*rWindow, deltaTime);
    DrawOnScreenSpriteData(renderContext, editorContext.editorHit.entity);
    DrawGridMouseHover(renderContext);
    DrawMouseCoordinates(renderContext);
    DrawFrameTime(renderContext, deltaTime.asSeconds());
    DrawMainMenuBar(renderContext);
    DrawStatusBar(editorContext);

    for (auto& tabEditors: editorsMap) {
        for (auto& eWindow: tabEditors.second) {
            if (!eWindow.isOpen || !eWindow.callback)
                continue;

            eWindow.callback(editorContext, eWindow);
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    if (!(io.WantCaptureMouse || io.WantCaptureKeyboard)) {
        EditorActionPlaceEntity(renderContext, editorContext);
    }

    ImGui::SFML::Render(*rWindow);
}

static void EditorRegisterWindow(const char* tab, const char* identifier, editorwindowCallback_t callback,
                                 sf::Keyboard::Key shortcutKey, bool defaultState)
{
    editorwindow_t eWindow{
        .name = identifier,
        .isOpen = defaultState,
        .shortcutKey = shortcutKey,
        .callback = callback
    };

    editorsMap[tab].push_back(eWindow);
    printf("[INFO][Editor]: \"%s\" -> \"%s\" window registered successfully.\n", tab, identifier);
}

static void EditorUnRegisterWindow(const char* tab, const char* identifier)
{
    if (!editorsMap.count(identifier)) {
        printf("[ERROR][Editor]: \"%s\" - > \"%s\" is not registered.\n", tab, identifier);
        return;
    }

    auto& tabEditors = editorsMap[tab];
    for (auto it = tabEditors.begin(); it != tabEditors.end(); ++it) {
        if (it->name == identifier) {
            tabEditors.erase(it);
            break;
        }
    }

    printf("[INFO][Editor]: \"%s\" -> \"%s\" window unregistered successfully.\n", tab, identifier);
}

static void DrawMainMenuBar(render_context_t& renderContext)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 5.0f));
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu(ICON_MD_INSERT_DRIVE_FILE "File")) {
        if (ImGui::MenuItem(ICON_MD_INSERT_DRIVE_FILE "New document")) {

        }
        if (ImGui::MenuItem(ICON_MD_FILE_OPEN "Open")) {

        }
        if (ImGui::MenuItem(ICON_MD_SAVE "Save")) {

        }
        if (ImGui::MenuItem(ICON_MD_SAVE_AS "Save As")) {

        }

        for (auto& eWindow: editorsMap["Files"]) {
            ImGui::MenuItem(eWindow.name.c_str(), keyToString[eWindow.shortcutKey], &eWindow.isOpen);
        }

        if (ImGui::MenuItem(ICON_MD_CLOSE "Close")) {

        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_EDIT "Edit")) {
        if (ImGui::MenuItem(ICON_MD_DATA_OBJECT "Object Properties")) {

        }
        if (ImGui::MenuItem(ICON_MD_TERRAIN "Tile Properties")) {

        }
        for (auto& eWindow: editorsMap["Edit"]) {
            ImGui::MenuItem(eWindow.name.c_str(), keyToString[eWindow.shortcutKey], &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_VISIBILITY "View")) {
        if (ImGui::MenuItem(ICON_MD_GRID_3X3 "Show Guides")) {

        }
        ImGui::MenuItem(ICON_MD_TERRAIN "Show Tile Properties", nullptr, &renderContext.isDrawCollider);
        for (auto& eWindow: editorsMap["View"]) {
            ImGui::MenuItem(eWindow.name.c_str(), keyToString[eWindow.shortcutKey], &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_HANDYMAN "Tools")) {
        if (ImGui::MenuItem(ICON_MD_TERRAIN "Tiles")) {

        }

        for (auto& eWindow: editorsMap["Tools"]) {
            ImGui::MenuItem(eWindow.name.c_str(), keyToString[eWindow.shortcutKey], &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_HELP "Help")) {
        for (auto& eWindow: editorsMap["Help"]) {
            ImGui::MenuItem(eWindow.name.c_str(), keyToString[eWindow.shortcutKey], &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }
    ImGui::PopStyleVar();
    ImGui::EndMainMenuBar();
}

static void DrawAboutWindow(editor_context_t& context, editorwindow_t& editorwindow)
{
    if (!ImGui::Begin(editorwindow.name.c_str(), &editorwindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    ImGui::Text(ABOUT_MESSAGE);
    ImGui::Separator();
    ImGui::Text("Author: " ABOUT_AUTHOR_MESSAGE);
    ImGui::Text("Version: " VERSION);

    ImGui::End();
}

static bool IsHitEntity(editor_context_t& editorContext, const render_context_t& renderContext)
{
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        return false;
    }

    const sf::Vector2f mousePosView =
        rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow), renderContext.worldView);
    editorContext.editorHit.viewPosition = mousePosView;

    if (editorContext.mode == EDITOR_MODE_TILE) {
        const sf::Vector2u mousePosGrid(mousePosView.x / gridSize, mousePosView.y / gridSize);
        editorContext.editorHit.girdPosition = mousePosGrid;
        editorContext.editorHit.entity = SceneGetTile(renderContext.sceneContext, mousePosGrid);
    } else if (editorContext.mode == EDITOR_MODE_OBJ) {
        auto& objects = renderContext.sceneContext.objects;
        if (objects.empty()) {
            return false;
        }

        for (auto& obj: objects) {
            if (!obj || !obj->sprite.getGlobalBounds().contains(mousePosView)) {
                continue;
            }

            editorContext.editorHit.entity = obj;
        }
    }

    return true;
}

static void DrawStatusBar(editor_context_t& editorContext)
{
    ImGuiWindowFlags
        window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    float height = ImGui::GetFrameHeight();

    if (!ImGui::BeginViewportSideBar("##SecondaryMenuBar", NULL, ImGuiDir_Up, height, window_flags)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::Selectable(ICON_MD_TERRAIN"Tile",
                              editorContext.mode == EDITOR_MODE_TILE,
                              0,
                              ImGui::CalcTextSize(ICON_MD_TERRAIN"Tile"))) {
            editorContext.mode = EDITOR_MODE_TILE;
            editorContext.selectedEntity = nullptr;
        }

        if (ImGui::Selectable(ICON_MD_VIEW_IN_AR"Object",
                              editorContext.mode == EDITOR_MODE_OBJ,
                              0,
                              ImGui::CalcTextSize(ICON_MD_DATA_OBJECT"Object"))) {
            editorContext.mode = EDITOR_MODE_OBJ;
            editorContext.selectedEntity = nullptr;
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}
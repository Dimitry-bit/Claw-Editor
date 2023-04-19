#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_internal.h"
#include "fonts/IconsMaterialDesign.h"

#include "editor_internal.h"
#include "editor_constants.h"
#include "editor_imgui.h"
#include "renderer.h"
#include "input.h"
#include "sfml_key_map.h"
#include "version.h"

static void EditorUpdateWindows(editor_context_t* editorContext, scene_context_t* world);
static void EditorRegisterWindow(editor_context_t* editorContext, const char* tab, const char* name,
                                 editorwindowCallback_t callback, sf::Keyboard::Key shortcutKey = sf::Keyboard::Unknown,
                                 bool defaultState = false);

void EditorInitImGuiWindows(editor_context_t* editorContext)
{
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_COLLECTIONS "Image Set", DrawImageSet, sf::Keyboard::I);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_BRUSH "Tile Painter", DrawTilePainter, sf::Keyboard::T);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_VIEW_IN_AR "Treasure Properties",
                         DrawTreasurePropertiesWindow);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_VIEW_IN_AR "Pickup Properties", DrawPickupPropertiesWindow);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_VIEW_IN_AR "Timed Object Properties",
                         DrawTimeObjPropertiesWindow);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_VIEW_IN_AR "Checkpoint Properties",
                         DrawCheckpointPropertiesWindow);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_VIEW_IN_AR "Eyecandy Properties",
                         DrawEyeCandyPropertiesWindow);
    EditorRegisterWindow(editorContext, "Help", "About", DrawAboutWindow);
}

void EditorUpdateImGuiEditors(editor_context_t* editorContext, render_context_t* renderContext,
                              scene_context_t* world, sf::Time deltaTime)
{
    ImGui::SFML::Update(*rWindow, deltaTime);

    DrawMainMenuBar(editorContext, *renderContext);
    DrawStatusBar(editorContext);
    EditorUpdateWindows(editorContext, world);

    static float dragTimer = 0;
    if (editorContext->mode == EDITOR_MODE_OBJ) {
        (sf::Mouse::isButtonPressed(sf::Mouse::Left)) ? dragTimer += deltaTime.asSeconds() : dragTimer = 0;
        if (dragTimer >= 0.1f && editorContext->editorHit.entity) {
            ActionEntityMove(world, *editorContext->editorHit.entity);
        }
    }

    ImGui::SFML::Render(*rWindow);
}

static void EditorUpdateWindows(editor_context_t* editorContext, scene_context_t* world)
{
    for (auto& tabEditors: editorContext->editorsMap) {
        for (auto& eWindow: tabEditors.second) {
            if (!eWindow.isOpen || !eWindow.callback)
                continue;

            eWindow.callback(world, eWindow);
        }
    }
}

void DrawMainMenuBar(editor_context_t* editorContext, render_context_t& renderContext)
{
    auto& editors = editorContext->editorsMap;

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

        for (auto& eWindow: editors["Files"]) {
            ImGui::MenuItem(eWindow.name.c_str(), sfmlKeyMap.at(eWindow.shortcutKey), &eWindow.isOpen);
        }

        if (ImGui::MenuItem(ICON_MD_CLOSE "Close")) {
            exit(0);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_VISIBILITY "View")) {
        if (ImGui::MenuItem(ICON_MD_GRID_3X3 "Show Guides")) {

        }

        ImGui::MenuItem(ICON_MD_TERRAIN "Show Tile Properties", nullptr, &renderContext.settings.isDrawTileCollider);

        for (auto& eWindow: editors["View"]) {
            ImGui::MenuItem(eWindow.name.c_str(), sfmlKeyMap.at(eWindow.shortcutKey), &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_HANDYMAN "Tools")) {
        for (auto& eWindow: editors["Tools"]) {
            ImGui::MenuItem(eWindow.name.c_str(), sfmlKeyMap.at(eWindow.shortcutKey), &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_HELP "Help")) {
        for (auto& eWindow: editors["Help"]) {
            ImGui::MenuItem(eWindow.name.c_str(), sfmlKeyMap.at(eWindow.shortcutKey), &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }
    ImGui::PopStyleVar();
    ImGui::EndMainMenuBar();
}

void DrawStatusBar(editor_context_t* editorContext)
{
    ImGuiWindowFlags window_flags;
    window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;

    float height = ImGui::GetFrameHeight();
    if (!ImGui::BeginViewportSideBar("##SecondaryMenuBar", nullptr, ImGuiDir_Up, height, window_flags)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) {
        ImVec2 textSize = ImGui::CalcTextSize(ICON_MD_TERRAIN"Tile");
        if (ImGui::Selectable(ICON_MD_TERRAIN"Tile", editorContext->mode == EDITOR_MODE_TILE, 0, textSize)) {
            editorContext->mode = EDITOR_MODE_TILE;
        }

        textSize = ImGui::CalcTextSize(ICON_MD_DATA_OBJECT"Object");
        if (ImGui::Selectable(ICON_MD_VIEW_IN_AR"Object", editorContext->mode == EDITOR_MODE_OBJ, 0, textSize)) {
            editorContext->mode = EDITOR_MODE_OBJ;
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void DrawAboutWindow(scene_context_t* world, editorwindow_t& eWindow)
{
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    ImGui::Text(ABOUT_MESSAGE);
    ImGui::Separator();
    ImGui::Text("Author: " ABOUT_AUTHOR_MESSAGE);
    ImGui::Text("Version: " VERSION);

    ImGui::End();
}

static void EditorRegisterWindow(editor_context_t* editorContext, const char* tab, const char* name,
                                 editorwindowCallback_t callback, sf::Keyboard::Key shortcutKey,
                                 bool defaultState)
{
    editorwindow_t eWindow{
        .name = name,
        .isOpen = defaultState,
        .shortcutKey = shortcutKey,
        .callback = callback,
        .context = editorContext,
    };

    editorContext->editorsMap[tab].push_back(eWindow);
    printf("[INFO][Editor]: \"%s\" -> \"%s\" window registered successfully.\n", tab, name);
}
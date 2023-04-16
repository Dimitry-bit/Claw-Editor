#include "imgui.h"
#include "imgui_internal.h"
#include "fonts/IconsMaterialDesign.h"

#include "editor_internal.h"
#include "editor_constants.h"
#include "editor_imgui.h"
#include "renderer.h"
#include "sfml_key_map.h"
#include "version.h"

extern void ProgramCleanUp();
static void EditorUpdateWindows();
static void EditorRegisterWindow(const char* tab, const char* name, editorwindowCallback_t callback,
                                 sf::Keyboard::Key shortcutKey = sf::Keyboard::Unknown, bool defaultState = false);

void EditorInitImGuiWindows()
{
    EditorRegisterWindow("Tools", ICON_MD_COLLECTIONS "Image Set", DrawImageSet, sf::Keyboard::I);
    EditorRegisterWindow("Tools", ICON_MD_BRUSH "Tile Painter", DrawTilePainter, sf::Keyboard::T);
    EditorRegisterWindow("Tools", ICON_MD_VIEW_IN_AR "Treasure Properties", DrawTreasurePropertiesWindow);
    EditorRegisterWindow("Tools", ICON_MD_VIEW_IN_AR "Pickup Properties", DrawPickupPropertiesWindow);
    EditorRegisterWindow("Tools", ICON_MD_VIEW_IN_AR "Timed Object Properties", DrawTimeObjPropertiesWindow);
    EditorRegisterWindow("Tools", ICON_MD_VIEW_IN_AR "Checkpoint Properties", DrawCheckpointPropertiesWindow);
    EditorRegisterWindow("Tools", ICON_MD_VIEW_IN_AR "Eyecandy Properties", DrawEyeCandyPropertiesWindow);

    EditorRegisterWindow("Help", "About", DrawAboutWindow);
}

void EditorUpdateImGuiEditors(render_context_t& renderContext)
{
    DrawMainMenuBar(renderContext);
    DrawStatusBar();
    EditorUpdateWindows();

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
        return;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        const sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(*rWindow);
        const sf::Vector2f mouseViewPos = rWindow->mapPixelToCoords(mouseWindowPos, renderContext.worldView);
        SceneIsEntityHit(mouseViewPos, &editorContext.editorHit.entity);
    }
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

void DrawMainMenuBar(render_context_t& renderContext)
{
    auto& editors = editorContext.editorsMap;

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
            ProgramCleanUp();
            exit(0);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_EDIT "Edit")) {
        if (ImGui::MenuItem(ICON_MD_DATA_OBJECT "Object Properties")) {

        }
        if (ImGui::MenuItem(ICON_MD_TERRAIN "Tile Properties")) {

        }
        for (auto& eWindow: editors["Edit"]) {
            ImGui::MenuItem(eWindow.name.c_str(), sfmlKeyMap.at(eWindow.shortcutKey), &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_VISIBILITY "View")) {
        if (ImGui::MenuItem(ICON_MD_GRID_3X3 "Show Guides")) {

        }

        ImGui::MenuItem(ICON_MD_TERRAIN "Show Tile Properties", nullptr, &renderContext.isDrawCollider);

        for (auto& eWindow: editors["View"]) {
            ImGui::MenuItem(eWindow.name.c_str(), sfmlKeyMap.at(eWindow.shortcutKey), &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_HANDYMAN "Tools")) {
        if (ImGui::MenuItem(ICON_MD_TERRAIN "Tiles")) {

        }

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

void DrawStatusBar()
{
    ImGuiWindowFlags window_flags;
    window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;

    float height = ImGui::GetFrameHeight();
    if (!ImGui::BeginViewportSideBar("##SecondaryMenuBar", nullptr, ImGuiDir_Up, height, window_flags)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) {
        ImVec2 textSize = ImGui::CalcTextSize(ICON_MD_TERRAIN
                                              "Tile");
        if (ImGui::Selectable(ICON_MD_TERRAIN
                              "Tile", editorContext.mode == EDITOR_MODE_TILE, 0, textSize)) {
            editorContext.mode = EDITOR_MODE_TILE;
            editorContext.selectedEntity = nullptr;
        }

        textSize = ImGui::CalcTextSize(ICON_MD_DATA_OBJECT
                                       "Object");
        if (ImGui::Selectable(ICON_MD_VIEW_IN_AR
                              "Object", editorContext.mode == EDITOR_MODE_OBJ, 0, textSize)) {
            editorContext.mode = EDITOR_MODE_OBJ;
            editorContext.selectedEntity = nullptr;
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void DrawAboutWindow(editorwindow_t& eWindow)
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

static void EditorRegisterWindow(const char* tab, const char* identifier, editorwindowCallback_t callback,
                                 sf::Keyboard::Key shortcutKey, bool defaultState)
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
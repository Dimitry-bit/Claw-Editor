#include <iostream>
#include <sstream>
#include <vector>
#include "imgui.h"
#include "imgui-SFML.h"
#include "fonts/IconsMaterialDesign.h"
#include "SFML/Graphics.hpp"

#include "editor.h"
#include "editor_constants.h"
#include "editor_tile_painter.h"
#include "editor_debug.h"
#include "input.h"
#include "renderer.h"
#include "resource_manager.h"

static std::map<string, std::vector<editorwindow_t>> editorsMap;

void EditorInitFont();
void DrawMainMenuBar();

void EditorInit()
{
    ImGui::SFML::Init(*rWindow);
    EditorInitFont();

    EditorRegisterWindow("Tools", ICON_MD_BRUSH "Tile Painter", DrawTilePainter, sf::Keyboard::T, true);
}

void EditorInitFont()
{
    ResFontLoadFromFile(BASE_FONT_FILE_NAME);

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

void EditorTick(sf::Time deltaTime)
{
    ImGui::SFML::Update(*rWindow, deltaTime);
    DrawMainMenuBar();
    DrawMouseCoordinates();
    DrawFrameTime(deltaTime.asSeconds());

    for (auto& tabEditors: editorsMap) {
        for (auto& eWindow: tabEditors.second) {
            if (!eWindow.isOpen || !eWindow.callback)
                continue;

            eWindow.callback(eWindow);
        }
    }

    ImGui::SFML::Render(*rWindow);
}

void EditorRegisterWindow(const char* tab, const char* identifier, editorwindowCallback_t callback,
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

void EditorUnRegisterWindow(const char* tab, const char* identifier)
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

void DrawMainMenuBar()
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
            ImGui::MenuItem(eWindow.name.c_str(), nullptr, &eWindow.isOpen);
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
            ImGui::MenuItem(eWindow.name.c_str(), nullptr, &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_VISIBILITY "View")) {
        if (ImGui::MenuItem(ICON_MD_GRID_3X3 "Show Guides")) {

        }
        if (ImGui::MenuItem(ICON_MD_TERRAIN "Show Tile Properties")) {

        }
        for (auto& eWindow: editorsMap["View"]) {
            ImGui::MenuItem(eWindow.name.c_str(), nullptr, &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_HANDYMAN "Tools")) {
        if (ImGui::MenuItem(ICON_MD_TERRAIN "Tiles")) {

        }
        if (ImGui::MenuItem(ICON_MD_COLLECTIONS "Image Sets")) {

        }

        for (auto& eWindow: editorsMap["Tools"]) {
            ImGui::MenuItem(eWindow.name.c_str(), nullptr, &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_HELP "Help")) {
        if (ImGui::MenuItem("About")) {

        }

        ImGui::EndMenu();
    }
    ImGui::PopStyleVar();
    ImGui::EndMainMenuBar();
}
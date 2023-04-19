#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_internal.h"
#include "fonts/IconsMaterialDesign.h"

#include "editor_internal.h"
#include "editor_constants.h"
#include "editor_imgui.h"
#include "editor_actions.h"
#include "renderer.h"
#include "input.h"
#include "sfml_key_map.h"
#include "utility.h"
#include "scene_serialization.h"
#include "version.h"

using std::string;

static void EditorUpdateWindows(editor_context_t* editorContext, scene_context_t* world);
static void EditorRegisterWindow(editor_context_t* editorContext, const char* tab, const char* name,
                                 editorwindowCallback_t callback, sf::Keyboard::Key shortcutKey = sf::Keyboard::Unknown,
                                 bool defaultState = false);

static void ActionDeSerializeWorld(editor_context_t* editorContext, scene_context_t* world);
static void ActionSerializeWorld(editor_context_t* editorContext, const scene_context_t* world,
                                 const std::string& file);
static void ActionNewDocument(editor_context_t* editorContext, scene_context_t* world);

void EditorInitImGuiWindows(editor_context_t* editorContext)
{
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_COLLECTIONS "Image Set", DrawImageSet, sf::Keyboard::I);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_BRUSH "Tile Painter", DrawTilePainter, sf::Keyboard::T);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_VIEW_IN_AR "Treasure Properties",
                         DrawTreasurePropertiesWindow);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_VIEW_IN_AR "Pickup Properties", DrawPickupPropertiesWindow,
                         sf::Keyboard::P);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_VIEW_IN_AR "Timed Object Properties",
                         DrawTimeObjPropertiesWindow, sf::Keyboard::O);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_VIEW_IN_AR "Checkpoint Properties",
                         DrawCheckpointPropertiesWindow, sf::Keyboard::C);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_VIEW_IN_AR "Eyecandy Properties",
                         DrawEyeCandyPropertiesWindow, sf::Keyboard::M);
    EditorRegisterWindow(editorContext, "Tools", ICON_MD_VIEW_IN_AR "Enemy Properties",
                         DrawEnemyPropertiesWindow, sf::Keyboard::E);
    EditorRegisterWindow(editorContext, "Help", "About", DrawAboutWindow);
}

void EditorUpdateImGuiEditors(editor_context_t* editorContext, render_context_t* renderContext,
                              scene_context_t* world, sf::Time deltaTime)
{
    ImGui::SFML::Update(*rWindow, deltaTime);

    static float pressTimer = 0;
    pressTimer += deltaTime.asSeconds();

    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse && !io.WantCaptureKeyboard) {
        bool (* hitFunction)(const scene_context_t* world, const sf::Vector2f& point, entity_t** out);
        if (editorContext->mode == EDITOR_MODE_OBJ) {
            hitFunction = SceneIsEntityHitObj;
        } else if (editorContext->mode == EDITOR_MODE_TILE) {
            hitFunction = SceneIsEntityHitTile;
        }

        if (editorContext->brushMode == BRUSH_MODE_ERASE) {
            bool (* inputFunction)(sf::Mouse::Button);
            if (editorContext->brushType == BRUSH_TYPE_WHEE) {
                inputFunction = sf::Mouse::isButtonPressed;
            } else if (editorContext->brushType == BRUSH_TYPE_CLICKY) {
                inputFunction = isMousePressed;
            }
            if (inputFunction(sf::Mouse::Left)) {
                editorContext->editorHit.mouseViewPos = rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow));
                hitFunction(world, editorContext->editorHit.mouseViewPos, &editorContext->editorHit.entity);
            }
        } else {
            if (isMousePressed(sf::Mouse::Left)) {
                if (pressTimer <= doubleClickThreshold) {
                    editorContext->editorHit.mouseViewPos = rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow));
                    hitFunction(world, editorContext->editorHit.mouseViewPos, &editorContext->editorHit.entity);
                } else {
                    editorContext->editorHit.entity = nullptr;
                }
                pressTimer = 0;
            }
        }

        static float dragTimer = 0;
        if (editorContext->mode == EDITOR_MODE_OBJ) {
            (sf::Mouse::isButtonPressed(sf::Mouse::Left)) ? dragTimer += deltaTime.asSeconds() : dragTimer = 0;
            if (dragTimer >= dragThreshold && editorContext->editorHit.entity) {
                ActionEntityMove(world, *editorContext->editorHit.entity);
            }

            if (isKeyPressed(sf::Keyboard::Delete)) {
                ActionDeleteEntity(world, &editorContext->editorHit.entity);
            }
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if (editorContext->saveFile.empty()) {
            string s = FileDialogSaveFile("cscene (*.cscene)\0*.cscene\0");
            ActionSerializeWorld(editorContext, world, s);
        } else {
            ActionSerializeWorld(editorContext, world, editorContext->saveFile);
        }
    }

    DrawMainMenuBar(editorContext, *renderContext, world);
    DrawStatusBar(editorContext);
    EditorUpdateWindows(editorContext, world);

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

void DrawMainMenuBar(editor_context_t* editorContext, render_context_t& renderContext, scene_context_t* world)
{
    auto& editors = editorContext->editorsMap;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 5.0f));
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu(ICON_MD_INSERT_DRIVE_FILE "File")) {
        if (ImGui::MenuItem(ICON_MD_INSERT_DRIVE_FILE "New document")) {
            ActionNewDocument(editorContext, world);
        }
        if (ImGui::MenuItem(ICON_MD_FILE_OPEN "Open")) {
            std::string s(FileDialogOpenFile("cscene (*.cscene)\0*.cscene\0"));
            if (!s.empty()) {
                editorContext->saveFile = s;
                ActionDeSerializeWorld(editorContext, world);
            }
        }

        if (ImGui::MenuItem(ICON_MD_SAVE "Save")) {
            if (editorContext->saveFile.empty()) {
                string s = FileDialogSaveFile("cscene (*.cscene)\0*.cscene\0");
                ActionSerializeWorld(editorContext, world, s);
            } else {
                ActionSerializeWorld(editorContext, world, editorContext->saveFile);
            }
        }

        if (ImGui::MenuItem(ICON_MD_SAVE_AS "Save As")) {
            string s(FileDialogSaveFile("cscene (*.cscene)\0*.cscene\0"));
            ActionSerializeWorld(editorContext, world, s);
        }

        for (auto& eWindow: editors["Files"]) {
            ImGui::MenuItem(eWindow.name.c_str(), sfmlKeyMap.at(eWindow.shortcutKey), &eWindow.isOpen);
        }

        if (ImGui::MenuItem(ICON_MD_CLOSE "Close")) {
            // TODO(Tony): Free Memory
            exit(0);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_VISIBILITY "View")) {
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

static void ActionDeSerializeWorld(editor_context_t* editorContext, scene_context_t* world)
{
    assert(world);
    assert(editorContext);

    editorContext->undoStack = std::stack<action_t>();
    editorContext->redoStack = std::stack<action_t>();
    editorContext->editorHit.entity = nullptr;
    editorContext->editorHit.mouseViewPos = sf::Vector2f(0, 0);

    SceneDealloc(world);
    SceneDeserialize(world, editorContext->saveFile);
}

static void ActionSerializeWorld(editor_context_t* editorContext, const scene_context_t* world, const std::string& file)
{
    assert(world);
    assert(editorContext);

    if (file.empty()) {
        return;
    }

    if (file.find(".cscene")) {
        editorContext->saveFile = file;
    } else {
        editorContext->saveFile = file + ".cscene";
    }

    SceneSerialize(world, editorContext->saveFile);
}

static void ActionNewDocument(editor_context_t* editorContext, scene_context_t* world)
{
    editorContext->editorHit.entity = nullptr;
    editorContext->undoStack = std::stack<action_t>();
    editorContext->redoStack = std::stack<action_t>();
    editorContext->editorHit.entity = nullptr;
    editorContext->editorHit.mouseViewPos = sf::Vector2f(0, 0);
    editorContext->saveFile.clear();

    SceneDealloc(world);
}
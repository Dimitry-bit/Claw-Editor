#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"
#include "fonts/IconsMaterialDesign.h"

#include "editor_imgui.h"
#include "editor_internal.h"
#include "editor_constants.h"
#include "resource_manager.h"
#include "renderer.h"
#include "sfml_key_map.h"
#include "input.h"
#include "version.h"

extern void ProgramCleanUp();
static bool ImGuiTextureGrid(const std::vector<asset_slot_t*>& assets, string& selectedTex, int rowSize = 10);

bool ImGuiTextureGrid(const std::vector<asset_slot_t*>& assets, string& selectedTex, int rowSize)
{
    bool status = false;
    int count = 0;

    for (int i = 0; i < assets.size(); ++i) {
        const sf::Vector2f size(32.0f, 32.0f);
        const asset_slot_t& spriteSlot = *assets.at(i);

        if (ImGui::ImageButton(*spriteSlot.texture, size, 1)) {
            selectedTex = spriteSlot.header.id;
            status = true;
        }

        if ((++count % rowSize) != 0) {
            ImGui::SameLine();
        }

        if (spriteSlot.assetTags & ASSET_TAG_ANIMATION) {
            string AnimationName = spriteSlot.header.fileName;
            while (assets.at(++i)->header.fileName == AnimationName) {
            }
            --i;
        }
    }

    return status;
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

void DrawPainterBrushSelection()
{
    const ImGuiTreeNodeFlags treeFlags =
        ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

    const ImVec2 size(40, 40);
    const ImVec2 alignment(0.5f, 0.5f);
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, alignment);

    if (ImGui::CollapsingHeader(ICON_MD_TUNE"Brush Modes:", treeFlags)) {
        if (ImGui::Selectable(ICON_MD_DRAG_INDICATOR"\nNone", editorContext.brushMode == BRUSH_MODE_NONE, 0, size)) {
            editorContext.brushMode = BRUSH_MODE_NONE;
        }
        ImGui::SameLine();
        if (ImGui::Selectable(ICON_MD_BRUSH"\nPaint", editorContext.brushMode == BRUSH_MODE_PAINT, 0, size)) {
            editorContext.brushMode = BRUSH_MODE_PAINT;
        }
        ImGui::SameLine();
        if (ImGui::Selectable(ICON_MD_DELETE"\nErase", editorContext.brushMode == BRUSH_MODE_ERASE, 0, size)) {
            editorContext.brushMode = BRUSH_MODE_ERASE;
        }
    }

    if (ImGui::CollapsingHeader(ICON_MD_FORMAT_PAINT"Brush Types:", treeFlags)) {
        if (ImGui::Selectable(ICON_MD_ADS_CLICK"\nClicky", editorContext.brushType == BRUSH_TYPE_CLICKY, 0, size)) {
            editorContext.brushType = BRUSH_TYPE_CLICKY;
        }
        ImGui::SameLine();
        if (ImGui::Selectable(ICON_MD_MOUSE"\nWHEEE", editorContext.brushType == BRUSH_TYPE_WHEE, 0, size)) {
            editorContext.brushType = BRUSH_TYPE_WHEE;
        }
    }

    ImGui::PopStyleVar();
}

void DrawTilePainter(editorwindow_t& eWindow)
{
    eWindow.isOpen = editorContext.mode == EDITOR_MODE_TILE;

    ImGui::SetNextWindowPos(ImVec2(rWindow->getSize()), ImGuiCond_Once, ImVec2(1, 1));
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    static entity_t defaultEntity;
    static const auto spriteSheets = ResGetAllAssetSlots(ASSET_SPRITESHEET, ASSET_TAG_TILE);
    const float textureScale = 0.6f;
    const int rowMax = 10;

    static bool isInit = false;
    if (!isInit) {
        defaultEntity.logic.reserve(50);
        defaultEntity.logic = "Logic_Tile";
        isInit = true;
    }

    DrawPainterBrushSelection();
    ImGui::Separator();

    ImGui::SeparatorText("Entity Data");
    ImGui::InputText("Logic", defaultEntity.logic.data(), defaultEntity.logic.capacity());

    ImGui::RadioButton("Collider_CLEAR", (int*) &defaultEntity.colliderType, COLLIDER_CLEAR);
    ImGui::SameLine();
    ImGui::RadioButton("Collider_GROUND", (int*) &defaultEntity.colliderType, COLLIDER_GROUND);
    ImGui::SameLine();
    ImGui::RadioButton("Collider_CLIMBABLE", (int*) &defaultEntity.colliderType, COLLIDER_CLIMBABLE);
    ImGui::RadioButton("Collider_DEATH", (int*) &defaultEntity.colliderType, COLLIDER_DEATH);
    ImGui::SameLine();
    ImGui::RadioButton("Collider_SOLID", (int*) &defaultEntity.colliderType, COLLIDER_SOLID);

    ImGui::Text("Graphics: %s", defaultEntity.graphicsID.c_str());

    ImGui::SeparatorText("Select Graphics");

    int count = 0;
    for (auto& spriteSlot: spriteSheets) {
        for (auto& frame: spriteSlot->spriteSheet->frames) {
            const sf::Texture& texture = ResTextureGet(frame.id.c_str());

            if (ImGui::ImageButton(texture, sf::Vector2f(gridSize * textureScale, gridSize * textureScale), 1)) {
                defaultEntity.graphicsID = frame.id;
            }

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_NoSharedDelay | ImGuiHoveredFlags_DelayShort)) {
                ImGui::SetTooltip("FileName: %s\nFileID: %s\nSize: %dx%d",
                                  spriteSlot->header.fileName.c_str(), frame.id.c_str(),
                                  frame.area.width, frame.area.height);
            }

            if ((++count % rowMax) != 0) {
                ImGui::SameLine();
            }
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    if (!(io.WantCaptureMouse || io.WantCaptureKeyboard)) {
        bool (* mouseInputFunction)(sf::Mouse::Button);
        if (editorContext.brushType == BRUSH_TYPE_WHEE) {
            mouseInputFunction = sf::Mouse::isButtonPressed;
        } else if (editorContext.brushType == BRUSH_TYPE_CLICKY) {
            mouseInputFunction = isMousePressed;
        }

        if (editorContext.brushMode == BRUSH_MODE_PAINT) {
            if (mouseInputFunction(sf::Mouse::Left) && !defaultEntity.graphicsID.empty()) {
                editorContext.editorHit.entity = ActionPlaceTile(defaultEntity);
            }
        } else if (editorContext.brushMode == BRUSH_MODE_ERASE) {
            if (mouseInputFunction(sf::Mouse::Left)) {
                ActionDeleteEntity(&editorContext.editorHit.entity);
            }
        }
    }

    ImGui::End();
}

void DrawImageSet(editorwindow_t& eWindow)
{
    ImGui::SetNextWindowPos(ImVec2(rWindow->getSize() / (unsigned int) 2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_NoDocking)) {
        ImGui::End();
        return;
    }

    const static auto spriteSheets = ResGetAllAssetSlots(ASSET_SPRITESHEET, ASSET_TAG_ANY);
    static asset_slot_t* selectedAsset = nullptr;

    {
        const ImVec2 childSize(ImGui::GetContentRegionAvail().x * 0.5f, ImGui::GetContentRegionAvail().y);
        ImGui::BeginChild("Loaded Image Sets", childSize, true, ImGuiWindowFlags_HorizontalScrollbar);
        for (auto& spriteSlot: spriteSheets) {
            auto& spriteSheet = spriteSlot->spriteSheet;
            const sf::Texture& texture = ResTextureGet(spriteSheet->frames[0].id.c_str());
            const sf::Vector2f size(50.0f, 50.0f);

            if (ImGui::ImageButton(texture, size)) {
                selectedAsset = spriteSlot;
            }

            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Text, sf::Color(190, 190, 190, 255));
            ImGui::Text("%s\n%d images\n", spriteSlot->header.fileName.c_str(), spriteSheet->frameCount);
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();
    }

    ImGui::SameLine();

    {
        ImVec2 childSize(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
        ImGui::BeginChild("Image Sets Viewer", childSize, true, ImGuiWindowFlags_HorizontalScrollbar);

        if (selectedAsset) {
            static int selectedFrame = 0;
            int count = 0;

            if (selectedFrame >= selectedAsset->spriteSheet->frameCount) {
                selectedFrame = 0;
            }

            auto& metaData = selectedAsset->spriteSheet->frames[selectedFrame];
            ImGui::PushStyleColor(ImGuiCol_Text, sf::Color(190, 190, 190, 255));
            ImGui::Text("FileName: %s\nFrameID: %s\nDimensions: x=%-3d y=%-3d w=%-3d h=%-3d\n",
                        selectedAsset->header.fileName.c_str(),
                        metaData.id.c_str(),
                        metaData.area.left,
                        metaData.area.top,
                        metaData.area.width,
                        metaData.area.height);

            ImGui::PopStyleColor();
            ImGui::Separator();

            const int rowMax = 10;
            auto& spriteSheet = selectedAsset->spriteSheet;
            for (int i = 0; i < spriteSheet->frameCount; i++) {
                const sf::Texture& texture = ResTextureGet(spriteSheet->frames[i].id.c_str());

                if (ImGui::ImageButton(texture)) {
                    selectedFrame = i;
                }

                if ((++count % rowMax) != 0) {
                    ImGui::SameLine();
                }
            }
        }
        ImGui::EndChild();
    }

    ImGui::End();
}

void DrawTreasurePropertiesWindow(editorwindow_t& eWindow)
{
    ImGui::SetNextWindowPos(ImVec2(0, rWindow->getSize().y), ImGuiCond_Once, ImVec2(0, 1));
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    static entity_t defaultEntity;
    const static auto spriteSheets = ResGetAllAssetSlots(ASSET_TEXTURE, std::regex("(TREASURE|COIN)"), ASSET_TAG_OBJ);

    static bool isInit = false;
    if (!isInit) {
        defaultEntity.logic.reserve(50);
        defaultEntity.logic = "Logic_Treasure";
        defaultEntity.value = 500;
        isInit = true;
    }

    ImGui::SeparatorText("Entity Data");
    ImGui::InputText("Logic", defaultEntity.logic.data(), defaultEntity.logic.capacity());
    ImGui::SliderInt("Gold Value", &defaultEntity.goldValue, 0, 20000);
    ImGui::Text("Graphics: %s", defaultEntity.graphicsID.c_str());

    ImGui::SeparatorText("Select Graphics");
    ImGuiTextureGrid(spriteSheets, defaultEntity.graphicsID);

    ImGui::NewLine();
    ImGui::SeparatorText("");
    const ImVec2 addButtonSize(100, 20);
    if (ImGui::Button("Add", addButtonSize) && !defaultEntity.graphicsID.empty()) {
        editorContext.editorHit.entity = ActionPlaceEntity(defaultEntity);
    }

    ImGui::End();
}

void DrawPickupPropertiesWindow(editorwindow_t& eWindow)
{
    ImGui::SetNextWindowPos(ImVec2(0, rWindow->getSize().y), ImGuiCond_Once, ImVec2(0, 1));
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    static entity_t defaultEntity;
    static const auto spriteSheets =
        ResGetAllAssetSlots(ASSET_TEXTURE, std::regex("(HEALTH|AMMO|LIVE|LIFE)"), ASSET_TAG_PICKUP);

    static bool isInit = false;
    if (!isInit) {
        defaultEntity.logic.reserve(50);
        defaultEntity.logic = "Logic_Pickup";
        defaultEntity.value = 500;
        isInit = true;
    }

    static const char* pickupLogics[] = {
        "Logic_pickup",
        "Logic_Health",
        "Logic_Ammo",
    };
    static int logicIndex = 0;

    ImGui::SeparatorText("Entity Data");
    if (ImGui::Combo("Predefined Logic", &logicIndex, pickupLogics, IM_ARRAYSIZE(pickupLogics))) {
        defaultEntity.logic = pickupLogics[logicIndex];
    }
    ImGui::InputText("Logic", defaultEntity.logic.data(), defaultEntity.logic.capacity());
    ImGui::SliderInt("Pickup Value", &defaultEntity.value, 0, 20000);
    ImGui::Text("Graphics: %s", defaultEntity.graphicsID.c_str());

    ImGui::SeparatorText("Select Graphics");
    ImGuiTextureGrid(spriteSheets, defaultEntity.graphicsID);

    ImGui::NewLine();
    ImGui::SeparatorText("");
    const ImVec2 addButtonSize(100, 20);
    if (ImGui::Button("Add", addButtonSize) && !defaultEntity.graphicsID.empty()) {
        editorContext.editorHit.entity = ActionPlaceEntity(defaultEntity);
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
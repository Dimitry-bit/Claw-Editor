#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"
#include "fonts/IconsMaterialDesign.h"

#include "editor.h"
#include "editor_imgui.h"
#include "editor_constants.h"
#include "resource_manager.h"
#include "renderer.h"
#include "key_to_string.h"
#include "version.h"

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

        for (auto& eWindow: editors.at("Files")) {
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
        for (auto& eWindow: editors.at("Edit")) {
            ImGui::MenuItem(eWindow.name.c_str(), keyToString[eWindow.shortcutKey], &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_VISIBILITY "View")) {
        if (ImGui::MenuItem(ICON_MD_GRID_3X3 "Show Guides")) {

        }
        ImGui::MenuItem(ICON_MD_TERRAIN "Show Tile Properties", nullptr, &renderContext.isDrawCollider);
        for (auto& eWindow: editors.at("View")) {
            ImGui::MenuItem(eWindow.name.c_str(), keyToString[eWindow.shortcutKey], &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_HANDYMAN "Tools")) {
        if (ImGui::MenuItem(ICON_MD_TERRAIN "Tiles")) {

        }

        for (auto& eWindow: editors.at("Tools")) {
            ImGui::MenuItem(eWindow.name.c_str(), keyToString[eWindow.shortcutKey], &eWindow.isOpen);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_HELP "Help")) {
        for (auto& eWindow: editors.at("Help")) {
            ImGui::MenuItem(eWindow.name.c_str(), keyToString[eWindow.shortcutKey], &eWindow.isOpen);
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

void DrawTilePainter(editorwindow_t& eWindow)
{
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    const float textureScale = 0.6f;
    const int rowMax = 10;

    auto spriteSheets = ResGetAllAssetSlots(ASSET_SPRITESHEET, ASSET_TAG_TILE);

    int count = 1;
    for (auto& spriteSlot: spriteSheets) {
        for (auto& frame: spriteSlot->spriteSheet->frames) {
            const sf::Texture& texture = ResTextureGet(frame.id.c_str());

            if (ImGui::ImageButton(texture, sf::Vector2f(gridSize * textureScale, gridSize * textureScale), 1)) {
                if (editorContext.mode == EDITOR_MODE_TILE) {
                    if (!editorContext.selectedEntity) {
                        editorContext.selectedEntity = EntityAlloc();
                    }

                    editorContext.selectedEntity->graphicsID = frame.id;
                    // NOTE(TONY): Should change when collider implemented
                    editorContext.selectedEntity->colliderType = COLLIDER_SOLID;
                }
            }

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_NoSharedDelay | ImGuiHoveredFlags_DelayShort)) {
                ImGui::SetTooltip("FileName: %s\nFileID: %s\nSize: %dx%d",
                                  spriteSlot->header.fileName.c_str(),
                                  frame.id.c_str(),
                                  frame.area.width,
                                  frame.area.height);
            }

            if (count <= rowMax) {
                ImGui::SameLine();
            } else {
                count = 0;
            }

            count++;
        }
    }

    ImGui::End();
}

void DrawObjectPainter(editorwindow_t& eWindow)
{
    if (!ImGui::Begin(eWindow.name.c_str(),
                      &eWindow.isOpen,
                      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar)) {
        ImGui::End();
        return;
    }

    const int rowMax = 10;
    auto spriteSheets = ResGetAllAssetSlots(ASSET_SPRITESHEET, ASSET_TAG_OBJ);

    int count = 1;
    for (auto& spriteSlot: spriteSheets) {
        for (auto& frame: spriteSlot->spriteSheet->frames) {
            const sf::Texture& texture = ResTextureGet(frame.id.c_str());

            const float floatWidth = std::clamp(frame.area.width, 20, 64);
            const float floatHeight = std::clamp(frame.area.height, 20, 64);

            if (ImGui::ImageButton(texture, sf::Vector2f(floatWidth, floatHeight), 1)) {
                if (editorContext.mode == EDITOR_MODE_OBJ) {
                    if (!editorContext.selectedEntity) {
                        editorContext.selectedEntity = EntityAlloc();
                    }

                    editorContext.selectedEntity->graphicsID = frame.id;
                    editorContext.selectedEntity->sprite.setOrigin(frame.pivot);
                }
            }

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_NoSharedDelay | ImGuiHoveredFlags_DelayShort)) {
                ImGui::SetTooltip("FileName: %s\nFileID: %s\nSize: %dx%d",
                                  spriteSlot->header.fileName.c_str(),
                                  frame.id.c_str(),
                                  frame.area.width,
                                  frame.area.height);
            }

            if (count <= rowMax) {
                ImGui::SameLine();
            } else {
                count = 0;
            }

            count++;

            if (spriteSlot->assetTags & ASSET_TAG_ANIMATION) {
                break;
            }
        }
    }

    ImGui::End();
}

void DrawImageSet(editorwindow_t& eWindow)
{
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_NoDocking)) {
        ImGui::End();
        return;
    }

    static asset_slot_t* selectedAsset = nullptr;
    auto spriteSheets = ResGetAllAssetSlots(ASSET_SPRITESHEET, ASSET_TAG_ANY);

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
            int count = 1;

            if (selectedFrame >= selectedAsset->spriteSheet->frameCount) {
                selectedFrame = 0;
            }

            auto& metaData = selectedAsset->spriteSheet->frames[selectedFrame];
            ImGui::PushStyleColor(ImGuiCol_Text, sf::Color(190, 190, 190, 255));
            ImGui::Text("FileName: %s\n"
                        "FrameID: %s\n"
                        "Dimensions: x=%-3d y=%-3d w=%-3d h=%-3d\n",
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

                if (count <= rowMax) {
                    ImGui::SameLine();
                } else {
                    count = 0;
                }

                count++;
            }
        }
        ImGui::EndChild();
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
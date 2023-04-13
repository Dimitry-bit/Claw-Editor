#include "imgui.h"
#include "imgui-SFML.h"
#include "resource_manager.h"

#include "editor_imgui.h"
#include "editor.h"
#include "renderer.h"

void DrawImageSet(editorwindow_t& eWindow)
{
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_NoDocking)) {
        ImGui::End();
        return;
    }

    static asset_slot_t* selectedAsset = nullptr;
    auto spriteSheets = ResGetAllAssetSlots(ASSET_SPRITESHEET, ASSET_TAG_OBJ);

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

            ImGui::ImageButton(texture, sf::Vector2f(gridSize * textureScale, gridSize * textureScale), 1);
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
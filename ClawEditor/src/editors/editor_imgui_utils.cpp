#include "imgui.h"
#include "imgui-SFML.h"

#include "editor_imgui_utils.h"
#include "editor_constants.h"
#include "resource_manager.h"

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

        DrawAssetTooltip(spriteSlot.header);

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

void DrawAssetTooltip(const asset_header_t& header, const frameMetadata_t* frameMetadata)
{
    if (!ImGui::IsItemHovered(ImGuiHoveredFlags_NoSharedDelay | ImGuiHoveredFlags_DelayShort)) {
        return;
    }

    ImGui::PushStyleColor(ImGuiCol_Text, colorLightGrey);
    if (frameMetadata) {
        ImGui::SetTooltip("FileName: %s\nFrameID: %s\nSize: %dx%d",
                          header.fileName.c_str(),
                          frameMetadata->id.c_str(),
                          frameMetadata->area.width,
                          frameMetadata->area.height);
    } else {
        ImGui::SetTooltip("FileName: %s\nFileID: %s\n", header.fileName.c_str(), header.id.c_str());
    }
    ImGui::PopStyleColor();
}
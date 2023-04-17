#include "imgui.h"

#include "editor_internal.h"
#include "editor_imgui_utils.h"
#include "resource_manager.h"

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
        EntityInit(&defaultEntity, "Logic_Treasure", RENDER_SPRITE);
        defaultEntity.logic.reserve(50);
        c_pickup_t c = {
            .type = PICKUP_TREASURE,
            .value = 0,
        };
        EntitySet(&defaultEntity, C_PICKUP, &c);
        isInit = true;
    }

    ImGui::SeparatorText("Entity Data");
    ImGui::InputText("Logic", defaultEntity.logic.data(), defaultEntity.logic.capacity());
    ImGui::SliderInt("Gold Value", &defaultEntity.pickup.value, 0, 20000);
    ImGui::Text("Graphics: %s", defaultEntity.render.graphicsID.c_str());

    ImGui::SeparatorText("Select Graphics");
    ImGuiTextureGrid(spriteSheets, defaultEntity.render.graphicsID);

    ImGui::NewLine();
    ImGui::SeparatorText("");
    const ImVec2 addButtonSize(100, 20);
    if (ImGui::Button("Add", addButtonSize) && !defaultEntity.render.graphicsID.empty()) {
        editorContext.editorHit.entity = ActionPlaceEntity(defaultEntity);
    }

    ImGui::End();
}
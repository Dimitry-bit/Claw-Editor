#include "imgui.h"

#include "editor_internal.h"
#include "editor_imgui_utils.h"
#include "resource_manager.h"

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
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
        EntityInit(&defaultEntity, "Logic_Treasure", RENDER_SPRITE);
        defaultEntity.logic.reserve(50);
        c_pickup_t c = {
            .type = PICKUP_NONE,
            .value = 0,
        };
        EntitySet(&defaultEntity, C_PICKUP, &c);
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
        if (defaultEntity.logic == "Logic_Health") {
            defaultEntity.pickup.type = PICKUP_HEALTH;
        } else if (defaultEntity.logic == "Logic_Ammo") {
            defaultEntity.pickup.type = PICKUP_AMMO;
        } else {
            defaultEntity.pickup.type = PICKUP_NONE;
        }
    }
    ImGui::InputText("Logic", defaultEntity.logic.data(), defaultEntity.logic.capacity());
    ImGui::SliderInt("Pickup Value", &defaultEntity.pickup.value, 0, 20000);
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
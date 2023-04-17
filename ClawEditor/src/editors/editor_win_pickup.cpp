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

    entity_t* editedEntityRef = &defaultEntity;
    bool isEditMode = editorContext.editorHit.entity && editorContext.editorHit.entity->type == C_PICKUP
        && editorContext.editorHit.entity->pickup.type != PICKUP_TREASURE;
    if (isEditMode) {
        editedEntityRef = editorContext.editorHit.entity;
        EntityUpdate(editedEntityRef, editedEntityRef);
    }

    static const char* pickupLogics[] = {
        "Logic_Pickup",
        "Logic_Health",
        "Logic_Ammo",
    };
    static int logicIndex = 0;

    ImGui::SeparatorText("Entity Data");
    if (ImGui::Combo("Predefined Logic", &logicIndex, pickupLogics, IM_ARRAYSIZE(pickupLogics))) {
        editedEntityRef->logic = pickupLogics[logicIndex];
        if (editedEntityRef->logic == "Logic_Health") {
            editedEntityRef->pickup.type = PICKUP_HEALTH;
        } else if (editedEntityRef->logic == "Logic_Ammo") {
            editedEntityRef->pickup.type = PICKUP_AMMO;
        } else {
            editedEntityRef->pickup.type = PICKUP_NONE;
        }
    }
    ImGui::InputText("Logic", editedEntityRef->logic.data(), editedEntityRef->logic.capacity());
    ImGui::SliderInt("Pickup Value", &editedEntityRef->pickup.value, 0, 20000);
    ImGui::Text("Graphics: %s", editedEntityRef->render.graphicsID.c_str());

    ImGui::SeparatorText("Select Graphics");
    ImGuiTextureGrid(spriteSheets, editedEntityRef->render.graphicsID);

    ImGui::NewLine();
    ImGui::SeparatorText("");
    const ImVec2 addButtonSize(100, 20);
    if (ImGui::Button("Add", addButtonSize) && !editedEntityRef->render.graphicsID.empty()) {
        editorContext.editorHit.entity = ActionPlaceEntity(*editedEntityRef);
    }
    ImGui::SameLine();
    ImGui::BeginDisabled();
    if (ImGui::Button("Edit", addButtonSize)) {
    }
    ImGui::EndDisabled();

    ImGui::End();
}
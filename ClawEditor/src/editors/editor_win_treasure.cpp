#include "imgui.h"

#include "editor_internal.h"
#include "editor_imgui_utils.h"
#include "resource_manager.h"

void DrawTreasurePropertiesWindow(scene_context_t* world, editorwindow_t& eWindow)
{
    ImGui::SetNextWindowPos(ImVec2(0, rWindow->getSize().y), ImGuiCond_Once, ImVec2(0, 1));
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    const static auto spriteSheets = ResGetAllAssetSlots(ASSET_TEXTURE, std::regex("(TREASURE|COIN)"), ASSET_TAG_OBJ);
    static entity_t defaultEntity;
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

    entity_t* editedEntityRef = &defaultEntity;
    bool isEditMode = eWindow.context->editorHit.entity && eWindow.context->editorHit.entity->type == C_PICKUP
        && eWindow.context->editorHit.entity->pickup.type == PICKUP_TREASURE;
    if (isEditMode) {
        editedEntityRef = eWindow.context->editorHit.entity;
        EntityUpdate(editedEntityRef, editedEntityRef);
    }

    ImGui::SeparatorText("Entity Data");
    ImGui::InputText("Logic", editedEntityRef->logic.data(), editedEntityRef->logic.capacity());
    ImGui::SliderInt("Gold Value", &editedEntityRef->pickup.value, 0, 20000);
    ImGui::Text("Graphics: %s", editedEntityRef->render.graphicsID.c_str());

    ImGui::SeparatorText("Select Graphics");
    ImGuiTextureGrid(spriteSheets, editedEntityRef->render.graphicsID);

    ImGui::NewLine();
    ImGui::SeparatorText("");
    const ImVec2 buttonSize(100, 20);
    if (ImGui::Button("Add", buttonSize) && !editedEntityRef->render.graphicsID.empty()) {
        eWindow.context->editorHit.entity = ActionPlaceEntity(world, *editedEntityRef);
    }
    ImGui::SameLine();

    ImGui::End();
}
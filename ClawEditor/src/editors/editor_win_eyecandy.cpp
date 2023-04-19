#include "imgui.h"

#include "editor_internal.h"
#include "editor_imgui_utils.h"
#include "resource_manager.h"

void DrawEyeCandyPropertiesWindow(scene_context_t* world, editorwindow_t& eWindow)
{
    ImGui::SetNextWindowPos(ImVec2(0, rWindow->getSize().y), ImGuiCond_Once, ImVec2(0, 1));
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    static const auto spriteSheets =
        ResGetAllAssetSlots(ASSET_TEXTURE, std::regex("(?!(HEALTH|AMMO|LIVE|LIFE))"), ASSET_TAG_EYECANDY);
    static entity_t defaultEntity;
    static bool isInit = false;
    if (!isInit) {
        EntityInit(&defaultEntity, "Logic_Eyecandy", RENDER_SPRITE);
        defaultEntity.logic.reserve(50);
        isInit = true;
    }

    entity_t* editedEntityRef = &defaultEntity;
    bool isEditMode = eWindow.context->editorHit.entity && eWindow.context->editorHit.entity->type == C_NONE;
    if (isEditMode) {
        editedEntityRef = eWindow.context->editorHit.entity;
        EntityUpdate(editedEntityRef, editedEntityRef);
    }

    ImGui::SeparatorText("Entity Data");
    ImGui::InputText("Logic", editedEntityRef->logic.data(), editedEntityRef->logic.capacity());
    ImGui::Text("Graphics: %s", editedEntityRef->render.graphicsID.c_str());

    ImGui::SeparatorText("Select Graphics");
    ImGuiTextureGrid(spriteSheets, editedEntityRef->render.graphicsID);

    ImGui::NewLine();
    ImGui::SeparatorText("");
    const ImVec2 addButtonSize(100, 20);
    if (ImGui::Button("Add", addButtonSize) && !editedEntityRef->render.graphicsID.empty()) {
        eWindow.context->editorHit.entity = ActionPlaceEntity(world, *editedEntityRef);
    }

    ImGui::End();
}
#include "imgui.h"

#include "editor_internal.h"
#include "editor_imgui_utils.h"
#include "resource_manager.h"

void DrawCheckpointPropertiesWindow(editorwindow_t& eWindow)
{
    ImGui::SetNextWindowPos(ImVec2(0, rWindow->getSize().y), ImGuiCond_Once, ImVec2(0, 1));
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    static entity_t defaultEntity;
    static const auto spriteSheets = ResGetAllAssetSlots(ASSET_TEXTURE, std::regex("CHECKPOINT"), ASSET_TAG_OBJ);

    static bool isInit = false;
    if (!isInit) {
        EntityInit(&defaultEntity, "Logic_Checkpoint", RENDER_SPRITE);
        defaultEntity.logic.reserve(50);
        c_checkpoint_t c{
            .keepInventory = false,
        };
        EntitySet(&defaultEntity, C_CHECKPOINT, &c);
        isInit = true;
    }

    ImGui::SeparatorText("Entity Data");
    ImGui::BeginDisabled();
    ImGui::InputText("Logic", defaultEntity.logic.data(), defaultEntity.logic.capacity());
    ImGui::EndDisabled();
    ImGui::Checkbox("Keep Inventory", &defaultEntity.checkpoint.keepInventory);
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
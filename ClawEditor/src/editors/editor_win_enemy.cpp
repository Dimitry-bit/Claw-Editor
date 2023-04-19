#include "imgui.h"

#include "editor_internal.h"
#include "resource_manager.h"

void DrawEnemyPropertiesWindow(scene_context_t* world, editorwindow_t& eWindow)
{
    ImGui::SetNextWindowPos(ImVec2(0, rWindow->getSize().y), ImGuiCond_Once, ImVec2(0, 1));
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    const static auto soliderAsset = ResGetAllAssetSlots(ASSET_TEXTURE, std::regex("SOLDIER.*IDLE"), ASSET_TAG_OBJ);
    const static auto officerAsset = ResGetAllAssetSlots(ASSET_TEXTURE, std::regex("OFFICER.*IDLE"), ASSET_TAG_OBJ);

    static entity_t defaultEntity;
    static bool isInit = false;
    if (!isInit) {
        EntityInit(&defaultEntity, "Logic_ENEMY", RENDER_SPRITE);
        defaultEntity.logic.reserve(50);
        c_enemy_t c = {
            .type = ENEMY_NONE,
            .min = sf::Vector2f(0, 0),
            .max = sf::Vector2f(0, 0),
        };
        EntitySet(&defaultEntity, C_ENEMY, &c);
        isInit = true;
    }

    entity_t* editedEntityRef = &defaultEntity;
    bool isEditMode = eWindow.context->editorHit.entity && eWindow.context->editorHit.entity->type == C_ENEMY;
    if (isEditMode) {
        editedEntityRef = eWindow.context->editorHit.entity;
        EntityUpdate(editedEntityRef, editedEntityRef);
    }

    ImGui::SeparatorText("Entity Data");
    ImGui::InputText("Logic", editedEntityRef->logic.data(), editedEntityRef->logic.capacity());
    ImGui::DragFloat2("From", &editedEntityRef->enemy.min.x);
    ImGui::DragFloat2("To", &editedEntityRef->enemy.max.x);

    int enemyTypeIndex = static_cast<int>(editedEntityRef->enemy.type);
    ImGui::RadioButton("ENEMY_SOLDIER", &enemyTypeIndex, ENEMY_SOLDIER);
    ImGui::SameLine();
    ImGui::RadioButton("ENEMY_OFFICER", &enemyTypeIndex, ENEMY_OFFICER);
    editedEntityRef->enemy.type = static_cast<enemy_types_t>(enemyTypeIndex);

    if (editedEntityRef->enemy.type == ENEMY_SOLDIER) {
        editedEntityRef->render.graphicsID = soliderAsset.at(0)->header.id;
    } else if (editedEntityRef->enemy.type == ENEMY_OFFICER) {
        editedEntityRef->render.graphicsID = officerAsset.at(0)->header.id;
    }

    ImGui::Text("Graphics: %s", editedEntityRef->render.graphicsID.c_str());

    if (isEditMode) {
        sf::RectangleShape rec;
        entity_transform transform = EntityGetTransform(editedEntityRef);
        rec.setSize(editedEntityRef->enemy.max + editedEntityRef->enemy.min);
        rec.setFillColor(sf::Color(190, 190, 190, 100));
        rec.setPosition(transform.position - editedEntityRef->enemy.min);
        rWindow->draw(rec);
    }

    ImGui::NewLine();
    ImGui::SeparatorText("");
    const ImVec2 buttonSize(100, 20);
    if (ImGui::Button("Add", buttonSize) && !editedEntityRef->render.graphicsID.empty()) {
        eWindow.context->editorHit.entity = ActionPlaceEntity(world, *editedEntityRef);
    }
    ImGui::SameLine();

    ImGui::End();
}
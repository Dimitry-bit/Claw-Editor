#include "imgui.h"

#include "editor_internal.h"
#include "resource_manager.h"

void DrawTimeObjPropertiesWindow(editorwindow_t& eWindow)
{
    ImGui::SetNextWindowPos(ImVec2(0, rWindow->getSize().y), ImGuiCond_Once, ImVec2(0, 1));
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    static const auto assetElevator = ResGetAllAssetSlots(ASSET_TEXTURE, std::regex("ELEVATOR"), ASSET_TAG_OBJ);
    static const auto assetCrumbingpeg = ResGetAllAssetSlots(ASSET_TEXTURE, std::regex("CRUMBLINGPEG"), ASSET_TAG_OBJ);

    static entity_t defaultEntity;
    static bool isInit = false;
    if (!isInit) {
        EntityInit(&defaultEntity, "Logic_Elevator", RENDER_SPRITE, assetElevator.at(0)->header.id);
        c_platform_t c = {
            .type = PLATFORM_ELEVATOR,
            .a = sf::Vector2f(0, 0),
            .b = sf::Vector2f(0, 0),
            .time = 0,
        };
        EntitySet(&defaultEntity, C_PLATFORM, &c);
        isInit = true;
    }

    entity_t* editedEntityRef = &defaultEntity;
    bool isEditMode = editorContext.editorHit.entity && editorContext.editorHit.entity->type == C_PLATFORM;
    if (isEditMode) {
        editedEntityRef = editorContext.editorHit.entity;
        EntityUpdate(editedEntityRef, editedEntityRef);
    }

    ImGui::SeparatorText("Entity Data");
    string radioName("Logic_Elevator");
    if (ImGui::RadioButton(radioName.c_str(), editedEntityRef->platform.type == PLATFORM_ELEVATOR)) {
        editedEntityRef->platform.type = PLATFORM_ELEVATOR;
        editedEntityRef->logic = radioName;
        editedEntityRef->render.graphicsID = assetElevator.at(0)->header.id;
    }
    radioName = "Logic_Crumblingpeg";
    if (ImGui::RadioButton(radioName.c_str(), editedEntityRef->platform.type == PLATFORM_CRUMBLINGPEG)) {
        editedEntityRef->platform.type = PLATFORM_CRUMBLINGPEG;
        editedEntityRef->logic = radioName;
        editedEntityRef->render.graphicsID = assetCrumbingpeg.at(0)->header.id;
    }

    ImGui::InputInt("Time(ms)", &editedEntityRef->platform.time);

    ImGui::BeginDisabled(editedEntityRef->platform.type != PLATFORM_ELEVATOR);
    ImGui::DragFloat2("To", &editedEntityRef->platform.a.x);
    ImGui::EndDisabled();
    ImGui::Text("Graphics: %s", editedEntityRef->render.graphicsID.c_str());

    if (editedEntityRef->platform.type == PLATFORM_ELEVATOR && editedEntityRef->platform.a != sf::Vector2f(0, 0)) {
        const render_context_t& renderContext = GetRenderContext();
        const sf::View cacheView = rWindow->getView();
        entity_t a, b;
        EntityInit(&a, "Logic_Elevator", RENDER_SPRITE, assetElevator.at(0)->header.id);
        EntityInit(&b, "Logic_Elevator", RENDER_SPRITE, assetElevator.at(0)->header.id);
        c_platform_t c = {
            .type = PLATFORM_ELEVATOR,
            .a = sf::Vector2f(0, 0),
            .b = sf::Vector2f(0, 0),
            .time = 0,
        };
        EntitySet(&a, C_PLATFORM, &c);
        EntitySet(&b, C_PLATFORM, &c);
        a.render.sprite.setColor(sf::Color(255, 255, 255, 150));
        b.render.sprite.setColor(sf::Color(255, 255, 255, 100));

        rWindow->setView(renderContext.worldView);
        const sf::Vector2f
            aPos = (isEditMode) ? editedEntityRef->render.sprite.getPosition() : renderContext.worldView.getCenter();
        const sf::Vector2f bPos = aPos + editedEntityRef->platform.a;

        a.render.sprite.setPosition(aPos);
        b.render.sprite.setPosition(bPos);

        rWindow->draw(a.render.sprite);
        rWindow->draw(b.render.sprite);
        rWindow->setView(cacheView);
    }

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
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
        defaultEntity.logic = "Logic_Elevator";
        defaultEntity.graphicsID = assetElevator.at(0)->header.id;
        defaultEntity.timedLogic = ENTITY_TIMED_LOGIC_ELEVATOR;
        defaultEntity.a = sf::Vector2f(0, 0);
        defaultEntity.b = sf::Vector2f(0, 0);
        defaultEntity.value = 0;
        isInit = true;
    }

    ImGui::SeparatorText("Entity Data");
    string radioName("Logic_Elevator");
    if (ImGui::RadioButton(radioName.c_str(), defaultEntity.timedLogic == ENTITY_TIMED_LOGIC_ELEVATOR)) {
        defaultEntity.timedLogic = ENTITY_TIMED_LOGIC_ELEVATOR;
        defaultEntity.logic = radioName;
        defaultEntity.graphicsID = assetElevator.at(0)->header.id;
    }
    radioName = "Logic_Crumblingpeg";
    if (ImGui::RadioButton(radioName.c_str(), defaultEntity.timedLogic == ENTITY_TIMED_LOGIC_CRUMBLINGPEG)) {
        defaultEntity.timedLogic = ENTITY_TIMED_LOGIC_CRUMBLINGPEG;
        defaultEntity.logic = radioName;
        defaultEntity.graphicsID = assetCrumbingpeg.at(0)->header.id;
    }

    ImGui::InputInt("Time(ms)", &defaultEntity.value);

    ImGui::BeginDisabled(defaultEntity.timedLogic != ENTITY_TIMED_LOGIC_ELEVATOR);
    ImGui::DragFloat2("To", &defaultEntity.a.x);
    ImGui::EndDisabled();
    ImGui::Text("Graphics: %s", defaultEntity.graphicsID.c_str());

    if (defaultEntity.timedLogic == ENTITY_TIMED_LOGIC_ELEVATOR && defaultEntity.a != sf::Vector2f(0, 0)) {
        const render_context_t& renderContext = GetRenderContext();
        const sf::View cacheView = rWindow->getView();
        entity_t a, b;
        a.sprite.setColor(sf::Color(255, 255, 255, 150));
        b.sprite.setColor(sf::Color(255, 255, 255, 100));

        rWindow->setView(renderContext.worldView);
        const sf::Vector2f aPos = renderContext.worldView.getCenter();
        const sf::Vector2f bPos = aPos + defaultEntity.a;
        EntityCreateOBJ(&a, defaultEntity.graphicsID.c_str(), defaultEntity.logic.c_str(), aPos);
        EntityCreateOBJ(&b, defaultEntity.graphicsID.c_str(), defaultEntity.logic.c_str(), bPos);

        rWindow->draw(a.sprite);
        rWindow->draw(b.sprite);
        rWindow->setView(cacheView);
    }

    ImGui::NewLine();
    ImGui::SeparatorText("");
    const ImVec2 addButtonSize(100, 20);
    if (ImGui::Button("Add", addButtonSize) && !defaultEntity.graphicsID.empty()) {
        editorContext.editorHit.entity = ActionPlaceEntity(defaultEntity);
    }

    ImGui::End();
}
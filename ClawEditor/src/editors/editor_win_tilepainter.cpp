#include "imgui.h"
#include "imgui-SFML.h"
#include "fonts/IconsMaterialDesign.h"

#include "editor_internal.h"
#include "editor_imgui_utils.h"
#include "resource_manager.h"
#include "input.h"

static void DrawPainterBrushSelection();

void DrawPainterBrushSelection()
{
    const ImGuiTreeNodeFlags treeFlags =
        ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

    const ImVec2 size(40, 40);
    const ImVec2 alignment(0.5f, 0.5f);
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, alignment);

    if (ImGui::CollapsingHeader(ICON_MD_TUNE"Brush Modes:", treeFlags)) {
        if (ImGui::Selectable(ICON_MD_DRAG_INDICATOR"\nNone", editorContext.brushMode == BRUSH_MODE_NONE, 0, size)) {
            editorContext.brushMode = BRUSH_MODE_NONE;
        }
        ImGui::SameLine();
        if (ImGui::Selectable(ICON_MD_BRUSH"\nPaint", editorContext.brushMode == BRUSH_MODE_PAINT, 0, size)) {
            editorContext.brushMode = BRUSH_MODE_PAINT;
        }
        ImGui::SameLine();
        if (ImGui::Selectable(ICON_MD_DELETE"\nErase", editorContext.brushMode == BRUSH_MODE_ERASE, 0, size)) {
            editorContext.brushMode = BRUSH_MODE_ERASE;
        }
    }

    if (ImGui::CollapsingHeader(ICON_MD_FORMAT_PAINT"Brush Types:", treeFlags)) {
        if (ImGui::Selectable(ICON_MD_ADS_CLICK"\nClicky", editorContext.brushType == BRUSH_TYPE_CLICKY, 0, size)) {
            editorContext.brushType = BRUSH_TYPE_CLICKY;
        }
        ImGui::SameLine();
        if (ImGui::Selectable(ICON_MD_MOUSE"\nWHEEE", editorContext.brushType == BRUSH_TYPE_WHEE, 0, size)) {
            editorContext.brushType = BRUSH_TYPE_WHEE;
        }
    }

    ImGui::PopStyleVar();
}

void DrawTilePainter(editorwindow_t& eWindow)
{
    eWindow.isOpen = editorContext.mode == EDITOR_MODE_TILE;

    ImGui::SetNextWindowPos(ImVec2(rWindow->getSize()), ImGuiCond_Once, ImVec2(1, 1));
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    static entity_t defaultEntity;
    static const auto spriteSheets = ResGetAllAssetSlots(ASSET_SPRITESHEET, ASSET_TAG_TILE);
    const float textureScale = 0.6f;
    const int rowMax = 10;

    static bool isInit = false;
    if (!isInit) {
        EntityInit(&defaultEntity, "Logic_Tile", RENDER_SPRITE);
        c_tile_t c = {
            .type = TILE_CLEAR,
        };
        EntitySet(&defaultEntity, C_TILE, &c);
        isInit = true;
    }

    DrawPainterBrushSelection();
    ImGui::Separator();

    ImGui::SeparatorText("Entity Data");
    ImGui::InputText("Logic", defaultEntity.logic.data(), defaultEntity.logic.capacity());

    int tileTypeIndex = static_cast<int>(defaultEntity.tile.type);
    ImGui::RadioButton("Collider_CLEAR", &tileTypeIndex, TILE_CLEAR);
    ImGui::SameLine();
    ImGui::RadioButton("Collider_GROUND", &tileTypeIndex, TILE_GROUND);
    ImGui::SameLine();
    ImGui::RadioButton("Collider_CLIMBABLE", &tileTypeIndex, TILE_CLIMBABLE);
    ImGui::RadioButton("Collider_DEATH", &tileTypeIndex, TILE_DEATH);
    ImGui::SameLine();
    ImGui::RadioButton("Collider_SOLID", &tileTypeIndex, TILE_SOLID);
    defaultEntity.tile.type = static_cast<tile_types_t>(tileTypeIndex);

    ImGui::Text("Graphics: %s", defaultEntity.render.graphicsID.c_str());

    ImGui::SeparatorText("Select Graphics");

    int count = 0;
    for (auto& spriteSlot: spriteSheets) {
        for (auto& frame: spriteSlot->spriteSheet->frames) {
            const sf::Texture& texture = ResTextureGet(frame.id.c_str());

            if (ImGui::ImageButton(texture, sf::Vector2f(gridSize * textureScale, gridSize * textureScale), 1)) {
                defaultEntity.render.graphicsID = frame.id;
            }

            DrawAssetTooltip(spriteSlot->header, &frame);

            if ((++count % rowMax) != 0) {
                ImGui::SameLine();
            }
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    if (!(io.WantCaptureMouse || io.WantCaptureKeyboard)) {
        bool (* mouseInputFunction)(sf::Mouse::Button);
        if (editorContext.brushType == BRUSH_TYPE_WHEE) {
            mouseInputFunction = sf::Mouse::isButtonPressed;
        } else if (editorContext.brushType == BRUSH_TYPE_CLICKY) {
            mouseInputFunction = isMousePressed;
        }

        if (editorContext.brushMode == BRUSH_MODE_PAINT) {
            if (mouseInputFunction(sf::Mouse::Left) && !defaultEntity.render.graphicsID.empty()) {
                editorContext.editorHit.entity = ActionPlaceTile(defaultEntity);
            }
        } else if (editorContext.brushMode == BRUSH_MODE_ERASE) {
            if (mouseInputFunction(sf::Mouse::Left)) {
                ActionDeleteEntity(&editorContext.editorHit.entity);
            }
        }
    }

    ImGui::End();
}
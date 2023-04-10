#include "imgui.h"
#include "imgui-SFML.h"
#include "resource_manager.h"

#include "editor_tile_painter.h"
#include "editor.h"
#include "renderer.h"

void DrawTilePainter(editorwindow_t& eWindow)
{
    if (!ImGui::Begin(eWindow.name.c_str(), &eWindow.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }

    const spriteSheet_t* spriteSheet = ResSpriteSheetGet("tilesets/LEVEL1_TILES.png");
    const float textureScale = 0.6f;
    const int rowMax = 10;

    int count = 1;
    for (auto& frame: spriteSheet->frames) {
        const sf::Texture& texture = ResTextureGet(frame.id.c_str());

        ImGui::ImageButton(texture, sf::Vector2f(gridSize * textureScale, gridSize * textureScale), 1);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_NoSharedDelay | ImGuiHoveredFlags_DelayShort)) {
            ImGui::SetTooltip("%s", frame.id.c_str());
        }

        if (count <= rowMax) {
            ImGui::SameLine();
        } else {
            count = 0;
        }

        count++;
    }

    ImGui::End();
}
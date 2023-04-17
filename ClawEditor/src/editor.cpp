#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"
#include "fonts/IconsMaterialDesign.h"

#include "editor.h"
#include "editors/editor_internal.h"
#include "editors/editor_imgui.h"
#include "editors/editor_inworld.h"
#include "input.h"
#include "renderer.h"
#include "resource_manager.h"

editor_context_t editorContext;

static void EditorInitFont();

void EditorInit()
{
    ImGui::SFML::Init(*rWindow);
    EditorInitFont();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard;

    EditorInitImGuiWindows();
}

static void EditorInitFont()
{
    AssetPushType(ASSET_FONT);
    ResLoadFromFile(BASE_FONT_FILE_NAME);
    AssetPopType();

    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig fontConfig;
    fontConfig.MergeMode = true;
    fontConfig.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("../resources/fonts/" BASE_FONT_FILE_NAME, baseFontSize, &fontConfig);

    static const ImWchar iconsRanges[] = {ICON_MIN_MD, ICON_MAX_16_MD, 0};
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    iconsConfig.GlyphOffset.y += 5.0f;
    iconsConfig.GlyphExtraSpacing.x += 5.0f;
    io.Fonts->AddFontFromFileTTF("../resources/fonts/" FONT_ICON_FILE_NAME_MD, iconFontSize, &iconsConfig, iconsRanges);
    io.Fonts->Build();
    ImGui::SFML::UpdateFontTexture();
}

void EditorShutdown()
{
    ResFontUnload(BASE_FONT_FILE_NAME);
    ImGui::SFML::Shutdown();
}

void EditorEvent(sf::Event event)
{
    ImGui::SFML::ProcessEvent(event);

    if (ImGui::IsAnyItemFocused() || ImGui::IsAnyItemActive()) {
        return;
    }

    if (event.type != sf::Event::KeyPressed) {
        return;
    }

    for (auto& tabEditors: editorContext.editorsMap) {
        for (auto& eWindow: tabEditors.second) {
            if (isKeyPressed(eWindow.shortcutKey)) {
                eWindow.isOpen = !eWindow.isOpen;
                return;
            }
        }
    }
}

void UpdateAndRenderEditor(render_context_t& renderContext, sf::Time deltaTime)
{
    ImGui::SFML::Update(*rWindow, deltaTime);

    EditorUpdateInWorldEditors(renderContext, deltaTime);
    EditorUpdateImGuiEditors(renderContext, deltaTime);

    ImGui::SFML::Render(*rWindow);
}
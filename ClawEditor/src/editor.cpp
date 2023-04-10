#include <iostream>
#include <sstream>
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include "editor.h"
#include "renderer.h"
#include "resource_manager.h"

const int characterSize = 14;
const float boundaryOffsetX = 20.0f;
const float boundaryOffsetY = 20.0f;
const sf::Color textColor = sf::Color::White;

void EditorInit()
{
    ImGui::SFML::Init(*rWindow);
    ResFontLoadFromFile("JetBrainsMono-Regular.ttf");
}

void EditorShutdown()
{
    ResFontUnload("JetBrainsMono-Regular.ttf");
    ImGui::SFML::Shutdown();
}

void EditorEvent(sf::Event event)
{
    ImGui::SFML::ProcessEvent(event);
}

void EditorTick(sf::Time deltaTime)
{
    ImGui::SFML::Update(*rWindow, deltaTime);
    DrawMouseCoordinates();
    DrawFrameTime(deltaTime.asSeconds());
    DrawTextureSelector();

    ImGui::SFML::Render(*rWindow);
}

void DrawOnScreenSpriteData(const sf::Sprite& drawable)
{
    sf::FloatRect drawableSize = drawable.getLocalBounds();
    sf::Vector2f drawablePos = drawable.getPosition();
    sf::RectangleShape collider(sf::Vector2f(drawableSize.width, drawableSize.height));
    collider.setFillColor(sf::Color::Transparent);
    collider.setOutlineColor(sf::Color::Red);
    collider.setOutlineThickness(1.0f);
    collider.setOrigin(drawable.getOrigin());
    collider.setPosition(drawable.getPosition());

    sf::Text text;
    text.setFont(ResFontGet("JetBrainsMono-Regular.ttf"));
    text.setFillColor(sf::Color::Yellow);
    text.setCharacterSize(10);
    text.setPosition(collider.getPosition().x + drawableSize.width + 5.0f, collider.getPosition().y);

    std::stringstream ss;
    ss << "Position: " << drawablePos.x << "x" << drawablePos.y << '\n';

    text.setString(ss.str());
    rWindow->draw(text);
    rWindow->draw(collider);
}

void DrawMouseCoordinates()
{
    sf::Vector2i mousePosScreen = sf::Mouse::getPosition();
    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*rWindow);
    sf::Vector2f mousePosView = rWindow->mapPixelToCoords(sf::Mouse::getPosition(*rWindow), view);
    sf::Vector2u mousePosGrid;
    mousePosGrid.x = mousePosView.x / gridSize;
    mousePosGrid.y = mousePosView.y / gridSize;

    sf::Text text;
    text.setFont(ResFontGet("JetBrainsMono-Regular.ttf"));
    text.setFillColor(textColor);
    text.setCharacterSize(characterSize);
    text.setPosition(boundaryOffsetX, boundaryOffsetY);

    std::stringstream ss;
    ss << "Screen: x=" << mousePosScreen.x << " y=" << mousePosScreen.y << '\n'
       << "Window: x=" << mousePosWindow.x << " y=" << mousePosWindow.y << '\n'
       << "View: x=" << mousePosView.x << " y=" << mousePosView.y << '\n'
       << "Grid: x=" << mousePosGrid.x << " y=" << mousePosGrid.y << '\n';

    text.setString(ss.str());
    rWindow->draw(text);
}

void DrawFrameTime(float deltaTime)
{
    sf::Text text;
    text.setFont(ResFontGet("JetBrainsMono-Regular.ttf"));
    text.setFillColor(textColor);
    text.setCharacterSize(characterSize);
    text.setOrigin(0.0f, text.getCharacterSize());
    text.setPosition(boundaryOffsetX, rWindow->getSize().y - boundaryOffsetY);

    std::stringstream ss;
    ss << "FrameTime: " << deltaTime * 100.0f << "ms";

    text.setString(ss.str());
    rWindow->draw(text);
}

void DrawTextureSelector()
{
    if (!ImGui::Begin("TileSelector", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
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
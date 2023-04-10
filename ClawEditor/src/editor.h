#pragma once

#include "SFML/System.hpp"

void EditorInit();
void EditorShutdown();
void EditorEvent(sf::Event event);
void EditorTick(sf::Time deltaTime);

void DrawMouseCoordinates();
void DrawFrameTime(float deltaTime);
void DrawOnScreenSpriteData(const sf::Sprite& drawable);
void DrawTilePainter();
#pragma once

#include "SFML/Graphics.hpp"

extern const int windowSizeX;
extern const int windowSizeY;
extern const int gridSize;

extern sf::RenderWindow* rWindow;
extern sf::View view;

void RendererInit();
void RenderWindow(sf::Time deltaTime);

void DrawCollider(const sf::Sprite& s);
void DrawOrigin(const sf::Sprite& s);
#pragma once

#include "SFML/Graphics.hpp"

#define MAX_GRID_SIZE 50

extern const int windowSizeX;
extern const int windowSizeY;
extern const int gridSize;

extern sf::RenderWindow* rWindow;
extern sf::View view;
extern sf::View defaultView;

void RendererInit(int width, int height);
void RenderWindow(sf::Time deltaTime);

void DrawCollider(const sf::Sprite& s);
void DrawOrigin(const sf::Sprite& s);
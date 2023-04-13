#pragma once

#include "SFML/Graphics.hpp"

#include "renderer.h"

void DrawMouseCoordinates(const render_context_t& renderContext);
void DrawFrameTime(const render_context_t& renderContext, float deltaTime);
void DrawOnScreenSpriteData(const render_context_t& renderContext, const sf::Sprite& drawable);
void DrawGridMouseHover(const render_context_t& renderContext);
void DrawCollider(const render_context_t& renderContext, const sf::Sprite& s);
void DrawOrigin(const render_context_t& renderContext, const sf::Sprite& s);
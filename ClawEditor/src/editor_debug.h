#pragma once

#include "SFML/Graphics.hpp"

#include "renderer.h"

void DrawMouseCoordinates(const render_context_t& renderContext);
void DrawFrameTime(const render_context_t& renderContext, float deltaTime);
void DrawOnScreenSpriteData(const render_context_t& renderContext, const entity_t* drawable);
void DrawGridMouseHover(const render_context_t& renderContext);
void DrawCollider(const entity_t* entity);
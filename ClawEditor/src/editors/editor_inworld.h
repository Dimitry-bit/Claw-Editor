#pragma once

#include "SFML/Graphics.hpp"

#include "renderer.h"

void EditorUpdateInWorldEditors(const render_context_t& renderContext, sf::Time deltaTime);

void DrawMouseCoordinates(const render_context_t& renderContext);
void DrawFrameTime(const render_context_t& renderContext, float deltaTime);
void DrawOnScreenSpriteData(const render_context_t& renderContext, const entity_t* entity);
void DrawGridMouseHover(const render_context_t& renderContext);
void DrawTileInfo(const entity_t* entity);
void DrawCollider(const entity_t* entity);
void DrawPivotPoint(const entity_t* entity);
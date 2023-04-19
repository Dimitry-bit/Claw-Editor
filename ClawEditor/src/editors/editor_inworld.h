#pragma once

#include "SFML/Graphics.hpp"

#include "editors/editor_internal.h"
#include "renderer.h"

void EditorUpdateInWorldEditors(const editor_context_t* editorContext, const scene_context_t* world,
                                sf::Time deltaTime);

void DrawOnScreenSpriteData(const entity_t* entity);
void DrawMouseCoordinates(const scene_context_t* world);
void DrawFrameTime(float deltaTime);
void DrawGridMouseHover(const scene_context_t* world);
void DrawTileInfo(const entity_t* entity);

void DrawCollider(const entity_t* entity);
void DrawPivotPoint(const entity_t* entity);
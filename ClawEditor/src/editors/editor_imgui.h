#pragma once

#include "editor_internal.h"

void EditorInitImGuiWindows(editor_context_t* editorContext);
void EditorUpdateImGuiEditors(editor_context_t* editorContext, render_context_t* renderContext,
                              scene_context_t* world, sf::Time deltaTime);
void DrawMainMenuBar(editor_context_t* editorContext, render_context_t& renderContext, scene_context_t* world);
void DrawStatusBar(editor_context_t* editorContext);
void DrawAboutWindow(scene_context_t* world, editorwindow_t& eWindow);

void DrawImageSet(scene_context_t* world, editorwindow_t& eWindow);
void DrawTilePainter(scene_context_t* world, editorwindow_t& eWindow);
void DrawTreasurePropertiesWindow(scene_context_t* world, editorwindow_t& eWindow);
void DrawPickupPropertiesWindow(scene_context_t* world, editorwindow_t& eWindow);
void DrawTimeObjPropertiesWindow(scene_context_t* world, editorwindow_t& eWindow);
void DrawCheckpointPropertiesWindow(scene_context_t* world, editorwindow_t& eWindow);
void DrawEyeCandyPropertiesWindow(scene_context_t* world, editorwindow_t& eWindow);
void DrawEnemyPropertiesWindow(scene_context_t* world, editorwindow_t& eWindow);
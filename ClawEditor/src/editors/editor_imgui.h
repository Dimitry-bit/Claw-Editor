#pragma once

#include "editor_internal.h"

void EditorInitImGuiWindows();
void EditorUpdateImGuiEditors(render_context_t& renderContext);
void DrawMainMenuBar(render_context_t& renderContext);
void DrawStatusBar();
void DrawAboutWindow(editorwindow_t& eWindow);

void DrawImageSet(editorwindow_t& eWindow);
void DrawTilePainter(editorwindow_t& eWindow);
void DrawTreasurePropertiesWindow(editorwindow_t& eWindow);
void DrawPickupPropertiesWindow(editorwindow_t& eWindow);
void DrawTimeObjPropertiesWindow(editorwindow_t& eWindow);
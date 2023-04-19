#pragma once

#include "SFML/System.hpp"

#include "editors/editor_internal.h"
#include "renderer.h"
#include "scene_manager.h"

void EditorInit(editor_context_t* editorContext);
void EditorShutdown();
void EditorEvent(editor_context_t* editorContext, sf::Event event);
void EditorUpdateImGuiEditors(editor_context_t* editorContext, render_context_t& renderContext,
                              scene_context_t* world, sf::Time deltaTime);
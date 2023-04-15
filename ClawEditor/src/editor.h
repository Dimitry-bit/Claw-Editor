#pragma once

#include "SFML/System.hpp"

#include "entity.h"
#include "renderer.h"

void EditorInit();
void EditorShutdown();
void EditorEvent(sf::Event event);
void UpdateAndRenderEditor(render_context_t& renderContext, sf::Time deltaTime);
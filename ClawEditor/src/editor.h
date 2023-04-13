#pragma once

#include <string>
#include "SFML/System.hpp"

#include "entity.h"
#include "renderer.h"

using std::string;

typedef void (* editorwindowCallback_t)(struct editorwindow_t& window);

struct editorwindow_t {
    string name;
    bool isOpen;
    sf::Keyboard::Key shortcutKey;
    editorwindowCallback_t callback;
};

enum editor_modes_t {
    EDITOR_MODE_TILE,
    EDITOR_MODE_OBJ,
};

struct editor_context_t {
    editor_modes_t mode;
    entity_t* entityHit;
};

void EditorInit();
void EditorShutdown();
void EditorEvent(sf::Event event);
void UpdateAndRenderEditor(render_context_t& renderContext, sf::Time deltaTime);
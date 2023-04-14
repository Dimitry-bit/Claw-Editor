#pragma once

#include <string>
#include <stack>
#include "SFML/System.hpp"

#include "entity.h"
#include "renderer.h"

using std::string;

typedef void (* editorwindowCallback_t)(struct editor_context_t& editorContext, struct editorwindow_t& window);

enum action_types_t {
    ACTION_NONE,
    ACTION_TILE_PLACEMENT,
    ACTION_OBJECT_PLACEMENT,
};

struct action_t {
    action_types_t type;
    entity_t modifiedEntityCache;
};

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

struct editor_hit_t {
    sf::Vector2u girdPosition;
    sf::Vector2f viewPosition;
    entity_t* entity;
};

struct editor_context_t {
    editor_modes_t mode;
    editor_hit_t editorHit;
    entity_t* selectedEntity;
    std::stack<action_t> undoStack;
    std::stack<action_t> redoStack;
};

void EditorInit();
void EditorShutdown();
void EditorEvent(sf::Event event);
void UpdateAndRenderEditor(render_context_t& renderContext, sf::Time deltaTime);
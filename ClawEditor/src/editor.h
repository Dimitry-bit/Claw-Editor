#pragma once

#include <string>
#include <stack>
#include "SFML/System.hpp"

#include "entity.h"
#include "editor_actions.h"
#include "renderer.h"

using std::string;

typedef void (* editorwindowCallback_t)(struct editorwindow_t& window);

struct editorwindow_t;
struct editor_hit_t;

enum editor_modes_t {
    EDITOR_MODE_TILE,
    EDITOR_MODE_OBJ,
};

struct editorwindow_t {
    string name;
    bool isOpen;
    sf::Keyboard::Key shortcutKey;
    editorwindowCallback_t callback;
};

struct editor_hit_t {
    entity_t* entity;
};

struct editor_context_t {
    std::map<string, std::vector<editorwindow_t>> editorsMap;
    editor_modes_t mode;
    editor_hit_t editorHit;
    entity_t* selectedEntity;
    std::stack<action_t> undoStack;
    std::stack<action_t> redoStack;
};

extern editor_context_t editorContext;

void EditorInit();
void EditorShutdown();
void EditorEvent(sf::Event event);
void UpdateAndRenderEditor(render_context_t& renderContext, sf::Time deltaTime);
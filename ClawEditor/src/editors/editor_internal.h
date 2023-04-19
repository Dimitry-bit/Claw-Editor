#pragma once

#include <string>
#include <stack>
#include "SFML/System.hpp"

#include "entity.h"
#include "editor_actions.h"
#include "editor_constants.h"

using std::string;

struct editor_context_t;
typedef void (* editorwindowCallback_t)(scene_context_t* world, struct editorwindow_t& window);

struct editorwindow_t;
struct editor_hit_t;

enum editor_modes_t {
    EDITOR_MODE_TILE,
    EDITOR_MODE_OBJ,
};

enum painter_brush_types_t {
    BRUSH_TYPE_CLICKY,
    BRUSH_TYPE_WHEE,
};

enum painter_brush_modes {
    BRUSH_MODE_NONE,
    BRUSH_MODE_PAINT,
    BRUSH_MODE_ERASE,
};

struct editorwindow_t {
    string name;
    bool isOpen;
    sf::Keyboard::Key shortcutKey;
    editorwindowCallback_t callback;
    editor_context_t* context;
};

struct editor_hit_t {
    sf::Vector2f mouseViewPos;
    entity_t* entity;
};

struct editor_context_t {
    std::map<string, std::vector<editorwindow_t>> editorsMap;
    editor_modes_t mode;
    painter_brush_modes brushMode;
    painter_brush_types_t brushType;
    editor_hit_t editorHit;
    std::stack<action_t> undoStack;
    std::stack<action_t> redoStack;
};
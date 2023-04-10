#pragma once

#include <string>

#include "SFML/System.hpp"

using std::string;

typedef void (* editorwindowCallback_t)(struct editorwindow_t& window);

struct editorwindow_t {
    string name;
    bool isOpen;
    sf::Keyboard::Key shortcutKey;
    editorwindowCallback_t callback;
};

void EditorInit();
void EditorShutdown();
void EditorEvent(sf::Event event);
void EditorTick(sf::Time deltaTime);

void EditorRegisterWindow(const char* tab, const char* name, editorwindowCallback_t callback,
                          sf::Keyboard::Key shortcutKey = sf::Keyboard::Unknown, bool defaultState = false);
void EditorUnRegisterWindow(editorwindowCallback_t callback);
#pragma once

#include "SFML/Window.hpp"

void InputEvent(sf::Event event);

void ClearKeyStatus();
bool isKeyPressed(sf::Keyboard::Key key);
bool isKeyReleased(sf::Keyboard::Key key);
bool isMousePressed(sf::Mouse::Button key);
bool isMouseReleased(sf::Mouse::Button key);
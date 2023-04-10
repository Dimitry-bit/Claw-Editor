#include <map>
#include "imgui.h"

#include "input.h"

enum keystatus_t {
    KEY_OFF = 0,
    KEY_PRESSED,
    KEY_RELEASED,
};

static std::map<sf::Keyboard::Key, keystatus_t> keyMap;
static std::map<sf::Mouse::Button, keystatus_t> mouseKeyMap;
sf::Event::MouseWheelScrollEvent scrollEvent;

void SetKeyStatus(sf::Keyboard::Key key, keystatus_t status);
void SetMouseKeyStatus(sf::Mouse::Button key, keystatus_t status);

bool isMouseWheelScrolled = false;

void InputEvent(sf::Event event)
{
    switch (event.type) {
        case sf::Event::MouseButtonPressed: {
            SetMouseKeyStatus(event.mouseButton.button, KEY_PRESSED);
        }
            break;
        case sf::Event::KeyPressed:
        case sf::Event::JoystickButtonPressed: {
            SetKeyStatus(event.key.code, KEY_PRESSED);
        }
            break;
        case sf::Event::MouseButtonReleased: {
            SetMouseKeyStatus(event.mouseButton.button, KEY_RELEASED);
        }
            break;
        case sf::Event::MouseWheelScrolled: {
            scrollEvent = event.mouseWheelScroll;
            isMouseWheelScrolled = true;
        }
            break;
        case sf::Event::KeyReleased:
        case sf::Event::JoystickButtonReleased: {
            SetKeyStatus(event.key.code, KEY_RELEASED);
        }
            break;
        default: break;
    }
}

void ClearKeyStatus()
{
    for (auto& key: keyMap) {
        key.second = KEY_OFF;
    }
    for (auto& key: mouseKeyMap) {
        key.second = KEY_OFF;
    }

    isMouseWheelScrolled = false;
}

void SetKeyStatus(sf::Keyboard::Key key, keystatus_t status)
{
    keyMap[key] = status;
}

void SetMouseKeyStatus(sf::Mouse::Button key, keystatus_t status)
{
    mouseKeyMap[key] = status;
}

bool isKeyPressed(sf::Keyboard::Key key)
{
    return (keyMap[key] == KEY_PRESSED);
}

bool isKeyReleased(sf::Keyboard::Key key)
{
    return (keyMap[key] == KEY_RELEASED);
}

bool isMousePressed(sf::Mouse::Button key)
{
    return (mouseKeyMap[key] == KEY_PRESSED);
}

bool isMouseReleased(sf::Mouse::Button key)
{
    return (mouseKeyMap[key] == KEY_RELEASED);
}

sf::Event::MouseWheelScrollEvent GetMouseWheelScroll()
{
    return scrollEvent;
}

bool IsMouseWheelScrolled()
{
    return isMouseWheelScrolled;
}
#pragma once

#include <vector>
#include "SFML/Graphics.hpp"

#include "scene_manager.h"

using std::string;

extern sf::RenderWindow* rWindow;

struct render_context_t {
    sf::View worldView;
    sf::View uiView;
    bool isDrawCollider;
    struct {
        std::vector<string> tileNames;
        std::vector<string> treasureNames;
        std::vector<string> pickUpNames;
    } assetContext;
};

void RendererInit(int rWidth, int rHeight);
void UpdateAndRenderWindow(sf::Time deltaTime);
render_context_t& GetRenderContext();
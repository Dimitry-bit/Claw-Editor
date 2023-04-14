#include "imgui.h"

#include "scene_navigation.h"
#include "renderer.h"
#include "input.h"

const float zoomFactor = 0.1f;
const float maxZoom = 2.0f;
float gameZoom = 1.0f;

sf::Cursor cursorHand;
sf::Cursor cursorArrow;

static void SceneMovement(render_context_t& renderContext);
static void SceneZoom(render_context_t& renderContext);

void SceneNavigationInit()
{
    cursorHand.loadFromSystem(sf::Cursor::Hand);
    cursorArrow.loadFromSystem(sf::Cursor::Arrow);
}

void SceneNavigationUpdate(render_context_t& renderContext)
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
        return;
    }

    SceneMovement(renderContext);
    SceneZoom(renderContext);
}

void SceneMovement(render_context_t& renderContext)
{
    static sf::Vector2i oldPos;
    sf::Vector2i currentPos;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        currentPos = sf::Mouse::getPosition(*rWindow);
        sf::Vector2f pos = rWindow->mapPixelToCoords(sf::Vector2i(oldPos.x - currentPos.x, oldPos.y - currentPos.y));
        renderContext.worldView.move(pos.x / gameZoom, pos.y / gameZoom);

        rWindow->setView(renderContext.worldView);
        oldPos = currentPos;
        rWindow->setMouseCursor(cursorHand);
    } else {
        oldPos = sf::Mouse::getPosition(*rWindow);
        rWindow->setMouseCursor(cursorArrow);
    }
}

void SceneZoom(render_context_t& renderContext)
{
    if (!IsMouseWheelScrolled() || GetMouseWheelScroll().wheel != sf::Mouse::Wheel::VerticalWheel) {
        return;
    }

    gameZoom += zoomFactor * GetMouseWheelScroll().delta;
    gameZoom = std::clamp(gameZoom, 1 / maxZoom, maxZoom);
    sf::Vector2f viewSize = sf::Vector2f(rWindow->getSize());
    viewSize.x /= gameZoom;
    viewSize.y /= gameZoom;
    renderContext.worldView.setSize(viewSize);
}
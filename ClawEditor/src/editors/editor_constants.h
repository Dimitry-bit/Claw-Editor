#pragma once

#define BASE_FONT_FILE_NAME "JetBrainsMono-Regular.ttf"
#define ABOUT_MESSAGE "Simple '.cscene' (Claw Scene) editor, written in C++ using SFML.\n"
#define ABOUT_AUTHOR_MESSAGE "Tony Medhat"

inline const float baseFontSize = 18.0f;
inline const float iconFontSize = 18.0f;
inline const int sfmlTextSize = 13;

inline const float boundaryOffsetX = 20.0f;
inline const float boundaryOffsetY = 45.0f;

inline const sf::Color frameTimeColor = sf::Color::White;
inline const sf::Color mouseCoordinatesColor = sf::Color::White;
inline const sf::Color spriteDebugColor = sf::Color::Yellow;
inline const sf::Color colliderColor = sf::Color::Red;
inline const sf::Color hoverFillColor = sf::Color(255, 255, 255, 35);
inline const sf::Color hoverFrameColor = sf::Color(255, 255, 255, 255);
inline const sf::Color colliderSolidColor = sf::Color(0, 0, 255, 80);
inline const sf::Color colliderGroundColor = sf::Color(0, 255, 0, 80);
inline const sf::Color colliderClimbColor = sf::Color(255, 255, 0, 80);
inline const sf::Color colliderDeathColor = sf::Color(255, 0, 0, 80);

inline const sf::Color colorLightGrey(sf::Color(190, 190, 190, 255));
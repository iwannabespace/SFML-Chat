#ifndef COMMON_HPP
#define COMMON_HPP

#include <SFML/Graphics.hpp>
#include <array>

namespace Functions
{
    void FitText(sf::Text& text, sf::Vector2f size);
    float GetMiddle(float object, float container, float start, float minus);
    std::string ConvertTimeToText(int32_t duration);
    sf::Color getColorFromTextbox(const std::string& r, const std::string& g, const std::string& b);
}

#endif
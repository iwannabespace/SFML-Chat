#ifndef COMMON_HPP
#define COMMON_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include "message.hpp"

namespace Functions
{
    void FitText(sf::Text& text, sf::Vector2f size);
    float GetMiddle(float object, float container, float start, float minus);
    std::string ConvertTimeToText(int32_t duration);
    sf::Color GetColorFromTextbox(const std::string& r, const std::string& g, const std::string& b);
    float GetTextMaxHeight(const sf::Text& text);
    Message CreateMessage(uint64_t id, uint8_t type, const std::string& data, const std::string& fileExtension);
    std::string GetFileName();
    void AddNewLinesToText(sf::Text& text, uint32_t maxWidth);
    std::string ReadFileAsBinary(const std::string& filename);
}

#endif
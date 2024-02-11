#include "../include/functions.hpp"

namespace Functions
{
    void FitText(sf::Text& text, sf::Vector2f size)
    {
        for (uint8_t i = 1; true; i++)
        {
            text.setCharacterSize(i);
            
            auto tSize = text.getGlobalBounds().getSize();
            
            if (tSize.x >= size.x || tSize.y >= size.y)
            {
                text.setCharacterSize(i - 24);
                break;
            }
        }
    }

    float GetMiddle(float object, float container, float start, float minus)
    {
        return std::round(start + ((container - object) / 2) - minus);
    }
    
    std::string ConvertTimeToText(int32_t duration)
    {
        std::string text;
        std::string minutes = std::to_string(duration / 60);
        std::string seconds = std::to_string(duration % 60);
        
        if (minutes.length() == 1)
            text += "0";
        text += (minutes + ":");

        if (seconds.length() == 1)
            text += "0";
        text += seconds;

        return text;
    }

    sf::Color getColorFromTextbox(const std::string& r, const std::string& g, const std::string& b)
    {
            std::uint8_t rc = 0, gc = 0, bc = 0;

            if (r.length())
                rc = std::stoul(r);
            
            if (g.length())
                gc = std::stoul(g);
            
            if (b.length())
                bc = std::stoul(b);

            return { rc, gc, bc };
    }
}
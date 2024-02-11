#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class FontManager
{
    public:
        FontManager();
        ~FontManager();
        bool load(const std::string& path, const std::string& name);
        std::optional<sf::Font*> get(const std::string& name);
    private:
        std::unordered_map<std::string, sf::Font> fonts;
};

#endif
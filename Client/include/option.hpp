#ifndef OPTION_HPP
#define OPTION_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include "RoundRect.hpp"

class Option : public sf::Drawable
{
    public:
        Option(sf::Font& font);
        ~Option();
        void on_window_resize(const sf::RenderWindow& window);
        void on_hover(const sf::RenderWindow& window);
        void on_click(const sf::RenderWindow& window);
        void setCallback(const std::function<void()>& _callback);
        void setString(const std::string& str);
        void setSize(sf::Vector2f size);
        void setPosition(sf::Vector2f position);
        void setFillColor(sf::Color color);
        void setTextColor(sf::Color color);
        void setHoverColor(sf::Color color);
        void setTextCharacterSize(uint32_t size);
        void place();
        sf::Vector2f getSize() const;
        sf::Vector2f getPosition() const;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        crd::RoundRect container;
        sf::Text text;
        sf::Color fillColor;
        sf::Color hoverColor;
        std::function<void()> callback;
        bool callbackSet = false;
};

#endif
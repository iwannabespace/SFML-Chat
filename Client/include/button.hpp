#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include "RoundRect.hpp"

class Button : public sf::Drawable
{
    public:
        Button(sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Color borderColor, sf::Color textColor, const std::string& textOf, sf::Font& font);
        ~Button();
        void on_hover(const sf::RenderWindow& window);
        void on_click(const sf::RenderWindow& window, std::function<void()> callback);
        void setPosition(sf::Vector2f position);
        void setSize(sf::Vector2f size);
        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        crd::RoundRect body;
        sf::Text text;
        sf::Color outlineColor;
};

#endif
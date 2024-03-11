#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "option.hpp"
#include "RoundRect.hpp"

class Options : public sf::Drawable
{
    private:
        using Callback = std::function<void()>;
    public:
        Options(sf::Font& _font, float width);
        ~Options();
        void on_hover(const sf::RenderWindow& window);
        void on_click(const sf::RenderWindow& window);
        void addOption(const std::string& optStr, const Callback& callback);
        void setWidth(float width);
        void setPosition(sf::Vector2f position);
        void setFillColor(sf::Color color);
        void setTextColor(sf::Color color);
        void setHoverColor(sf::Color color);
        void setFont(sf::Font& _font);
        void place();
        sf::Vector2f getSize() const;
        sf::Vector2f getPosition() const;
        sf::FloatRect getGlobalBounds() const;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        crd::RoundRect container;
        std::vector<Option> options;
        sf::Font& font;
        sf::Color fillColor;
        sf::Color textColor;
        sf::Color hoverColor;
};

#endif
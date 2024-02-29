#ifndef VALUEBAR_HPP
#define VALUEBAR_HPP

#include <SFML/Graphics.hpp>

class ValueBar : public sf::Drawable
{   
    public:
        ValueBar(sf::Vector2f size, sf::Vector2f position, sf::Color inner_color, sf::Color outer_color, float _max);
        ~ValueBar();
        void setMax(float _max);
        void setValue(float _value);
        float getValue() const;
        float getMax() const;
        void setSize(sf::Vector2f size);
        void setPosition(sf::Vector2f position);
        void setInnerColor(sf::Color color);
        void setOuterColor(sf::Color color);
        sf::Vector2f getSize() const;
        sf::Vector2f getPosition() const;
        void move(sf::Vector2f offset);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::RectangleShape inner;
        sf::RectangleShape outer;
        float value;
        float max;
};

#endif
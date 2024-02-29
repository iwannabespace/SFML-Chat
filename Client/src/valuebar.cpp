#include "../include/valuebar.hpp"

ValueBar::ValueBar(sf::Vector2f size, sf::Vector2f position, sf::Color inner_color, sf::Color outer_color, float _max)
    : value(0), max(_max)
{
    inner.setSize(size);
    inner.setPosition(position);
    inner.setFillColor(inner_color);

    outer.setSize({ 0, size.y });
    outer.setPosition(position);
    outer.setFillColor(outer_color);
}

ValueBar::~ValueBar()
{
}

void ValueBar::setMax(float _max)
{
    this->max = _max;
}

void ValueBar::setValue(float _value)
{
    if (_value <= max)
    {
        this->value = _value;
        float ratio = inner.getSize().x / max;
        float new_size = value * ratio;
        outer.setSize({ new_size, outer.getSize().y });
    }
}

float ValueBar::getValue() const
{
    return value;
}

void ValueBar::setSize(sf::Vector2f size)
{
    inner.setSize(size);
    outer.setSize({ 0, size.y });
}

void ValueBar::setPosition(sf::Vector2f position)
{
    inner.setPosition(position);
    outer.setPosition(position);
}

void ValueBar::setInnerColor(sf::Color color)
{
    inner.setFillColor(color);
}

void ValueBar::setOuterColor(sf::Color color)
{
    outer.setFillColor(color);
}

sf::Vector2f ValueBar::getSize() const
{
    return inner.getSize();
}

float ValueBar::getMax() const
{
    return max;
}

sf::Vector2f ValueBar::getPosition() const
{
    return inner.getPosition();
}

void ValueBar::move(sf::Vector2f offset)
{
    inner.move(offset);
    outer.move(offset);
}

void ValueBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(inner, states);
    target.draw(outer, states);
}
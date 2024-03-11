#include "../include/option.hpp"
#include "../include/functions.hpp"
#include <iostream>

Option::Option(sf::Font& font)
{
    text.setFont(font);
    container.setRadius(6);
}

Option::~Option()
{
}

void Option::on_window_resize(const sf::RenderWindow& window)
{
    window;
}

void Option::on_hover(const sf::RenderWindow& window)
{
    if (container.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
        container.setFillColor(hoverColor);
    
    else
        container.setFillColor(fillColor);
}

void Option::on_click(const sf::RenderWindow& window)
{
    if (container.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
        callback();
}

void Option::setCallback(const std::function<void()>& _callback)
{
    callback = _callback;
}

void Option::setString(const std::string& str)
{
    text.setString(str);
}

void Option::setSize(sf::Vector2f size)
{
    container.setSize(size);
}

void Option::setPosition(sf::Vector2f position)
{
    container.setPosition(position);
}

void Option::setFillColor(sf::Color color)
{
    fillColor = color;
    container.setFillColor(color);
}

void Option::setHoverColor(sf::Color color)
{
    hoverColor = color;
}

void Option::setTextColor(sf::Color color)
{
    text.setFillColor(color);
}

void Option::setTextCharacterSize(uint32_t size)
{
    text.setCharacterSize(size);
}

void Option::place()
{
    float textMaxHeight = Functions::GetTextMaxHeight(text);

    text.setPosition({
        container.getPosition().x + 10,
        Functions::GetMiddle(textMaxHeight, container.getSize().y, container.getPosition().y, text.getLocalBounds().top),
    });
}

sf::Vector2f Option::getSize() const
{
    return container.getSize();
}

sf::Vector2f Option::getPosition() const
{
    return container.getPosition();
}

void Option::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(container, states);
    target.draw(text, states);
}
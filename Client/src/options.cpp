#include "../include/options.hpp"

Options::Options(sf::Font& _font, float width)
    : font(_font)
{
    container.setSize({ width, 0 });
    container.setRadius(6);
}

Options::~Options()
{
}

void Options::on_hover(const sf::RenderWindow& window)
{
    for (Option& option : options)
        option.on_hover(window);
}

void Options::on_click(const sf::RenderWindow& window)
{
    for (Option& option : options)
        option.on_click(window);
}

void Options::addOption(const std::string& optStr, const Callback& callback)
{
    Option option(font);
    option.setString(optStr);
    option.setCallback(callback);
    options.push_back(option);
}

void Options::setWidth(float width)
{
    container.setSize({ width, 0 });
}

void Options::setPosition(sf::Vector2f position)
{
    container.setPosition(position);
}

void Options::setFillColor(sf::Color color)
{   
    fillColor = color;
    container.setFillColor(color);
}

void Options::setTextColor(sf::Color color)
{
    textColor = color;
}

void Options::setHoverColor(sf::Color color)
{
    hoverColor = color;
}

void Options::setFont(sf::Font& _font)
{
    font = _font;
}

void Options::place()
{
    sf::Vector2f size, position;
    size.x = container.getSize().x - 10;
    size.y = 20;
    position.x = container.getPosition().x + 5;
    position.y = container.getPosition().y + 5;

    float totalHeight = 10;

    for (Option& option : options)
    {
        option.setSize(size);
        option.setPosition(position);
        option.setFillColor(fillColor);
        option.setHoverColor(sf::Color::Blue);
        option.setTextCharacterSize(12);
        option.setTextColor(textColor);
        option.place();
        position.y += size.y;
        totalHeight += size.y;
    }

    container.setSize({ container.getSize().x, totalHeight });
}

sf::Vector2f Options::getSize() const
{
    return container.getSize();
}

sf::Vector2f Options::getPosition() const
{
    return container.getPosition();
}

sf::FloatRect Options::getGlobalBounds() const
{
    return container.getGlobalBounds();
}

void Options::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(container, states);
    
    for (const Option& option : options)
        target.draw(option, states);
}
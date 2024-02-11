#include "../include/activatable.hpp"
#include "../include/functions.hpp"

Activatable::Activatable(sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Color borderColor, sf::Color iconColor, const std::string& iconFile)
    : outlineColor(borderColor)
{
    body.setSize(size);
    body.setPosition(position);
    body.setFillColor(color);
    body.setOutlineColor(sf::Color::Transparent);
    body.setOutlineThickness(1);
    body.setRadius(6);

    iconTexture.loadFromFile(iconFile);
    icon.setTexture(iconTexture);

    float scaleX = size.x + 10 / icon.getLocalBounds().width;
    float scaleY = size.y + 10 / icon.getLocalBounds().height;
    icon.setScale(scaleX, scaleY);

    icon.setPosition({ 
        Functions::GetMiddle(icon.getGlobalBounds().width, size.x, position.x, 0),
        Functions::GetMiddle(icon.getGlobalBounds().height, size.y, position.y, 0),
    });

    icon.setColor(iconColor);
}

Activatable::~Activatable()
{
}

void Activatable::on_hover(const sf::RenderWindow& window)
{
    if (body.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
        body.setOutlineColor(outlineColor);
    
    else
        body.setOutlineColor(sf::Color::Transparent);
}

void Activatable::on_click(const sf::RenderWindow& window, std::function<void()> callback)
{
    if (body.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
    {    
        _activated = !_activated;

        if (_activated)
            callback();
    }
}

void Activatable::setPosition(sf::Vector2f position)
{
    body.setPosition(position);
    icon.setPosition({ 
        Functions::GetMiddle(icon.getGlobalBounds().width, body.getSize().x, position.x, 0),
        Functions::GetMiddle(icon.getGlobalBounds().height, body.getSize().y, position.y, 0),
    });
}

void Activatable::setSize(sf::Vector2f size)
{
    body.setSize(size);
    icon.setPosition({ 
        Functions::GetMiddle(icon.getGlobalBounds().width, size.x, body.getPosition().x, 0),
        Functions::GetMiddle(icon.getGlobalBounds().height, size.y, body.getPosition().y, 0),
    });
}

bool Activatable::activated() const
{
    return _activated;
}

sf::Vector2f Activatable::getPosition() const
{
    return body.getPosition();
}

sf::Vector2f Activatable::getSize() const
{
    return body.getSize();
}

void Activatable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(body, states);
    target.draw(icon, states);
}
#include "../include/button.hpp"
#include "../include/functions.hpp"

Button::Button(sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Color borderColor, sf::Color textColor, const std::string& textOf, sf::Font& font)
    : text(font), outlineColor(borderColor)
{
    body.setSize(size);
    body.setPosition(position);
    body.setFillColor(color);
    body.setOutlineColor(sf::Color::Transparent);
    body.setOutlineThickness(1);
    body.setRadius(6);

    text.setFont(font);
    text.setString(textOf);
    text.setCharacterSize(12);
    text.setFillColor(textColor);
    text.setPosition({
        Functions::GetMiddle(text.getGlobalBounds().size.x, size.x, position.x, 0),
        Functions::GetMiddle(text.getGlobalBounds().size.y, size.y, position.y, text.getGlobalBounds().position.y),
    });
}

Button::~Button()
{
}

void Button::on_hover(const sf::RenderWindow& window)
{
    if (body.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
        body.setOutlineColor(outlineColor);

    else
        body.setOutlineColor(sf::Color::Transparent);
}

void Button::on_click(const sf::RenderWindow& window, std::function<void()> callback)
{
    if (body.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
        callback();
}

void Button::setPosition(sf::Vector2f position)
{
    body.setPosition(position);
    text.setPosition({
        Functions::GetMiddle(text.getGlobalBounds().size.x, body.getSize().x, body.getPosition().x, 0),
        Functions::GetMiddle(text.getGlobalBounds().size.y, body.getSize().y, body.getPosition().y, text.getLocalBounds().position.y),
    });
}

void Button::setSize(sf::Vector2f size)
{
    body.setSize(size);
    text.setPosition({
        Functions::GetMiddle(text.getGlobalBounds().size.x, body.getSize().x, body.getPosition().x, 0),
        Functions::GetMiddle(text.getGlobalBounds().size.y, body.getSize().y, body.getPosition().y, text.getGlobalBounds().position.y),
    });
}

sf::Vector2f Button::getPosition() const
{
    return body.getPosition();
}

sf::Vector2f Button::getSize() const
{
    return body.getSize();
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(body, states);
    target.draw(text, states);
}

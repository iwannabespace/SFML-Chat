#include "../include/message_container.hpp"
#include "../include/functions.hpp"
#include "../include/theme.hpp"
#include <fstream>
#include <iostream>

MessageContainer::MessageContainer(const Message& message, sf::Font& font)
    : senderId(message.senderId), messageType(message.type), font(font)
{
    if (message.type == MessageType::Text) {
        std::string msg = message.data;
        text = sf::Text(msg, font, 14);
        Functions::AddNewLinesToText(text.value(), 240);
    } else {
        if (message.type == MessageType::File) {
            image = ImageViewer(message.data);
            image.value().setOutlineThickness(2);
        } else {
            sound = SoundPlayer(message.data, font);
        }
    }

    container.setFillColor(Theme::Secondary);

    senderName.setFont(font);
    senderName.setCharacterSize(14);
    senderName.setString(message.joiner.username);
}

MessageContainer::MessageContainer(const MessageContainer& rhs)
    : senderId(rhs.senderId), messageType(rhs.messageType), font(rhs.font)
{
    if (messageType == MessageType::Text) {
        std::string msg = rhs.text.value().getString();
        text = sf::Text(msg, font, 14);
        Functions::AddNewLinesToText(text.value(), 240);
    } else {
        if (messageType == MessageType::File) {
            image = ImageViewer(rhs.image.value().getFilename());
            image.value().setOutlineThickness(2);
        } else {
            sound = SoundPlayer(rhs.sound.value().getFilename(), font);
        }
    }

    container.setFillColor(Theme::Secondary);

    senderName.setFont(font);
    senderName.setCharacterSize(14);
    senderName.setString(rhs.senderName.getString());

    setSize();
    setPosition(rhs.getPosition());
    setTextColor(rhs.senderName.getFillColor());
    setImageOutlineColor(rhs.senderName.getFillColor());
    setPlayerHoverColor(sf::Color::Red);
    setPlayerColor(rhs.senderName.getFillColor());
    setPlayerInnerBarColor(sf::Color::Black);
    setPlayerOuterBarColor(sf::Color::Green);
    setPlayerTextColor(sf::Color::Black);
}

MessageContainer::~MessageContainer()
{
}

void MessageContainer::on_hover_objects(const sf::RenderWindow& window)
{
    if (messageType == MessageType::File) {
        image.value().on_hover(window);
    } else if (messageType == MessageType::Sound) {
        sound.value().on_hover(window);
    }   
}

void MessageContainer::on_click_objects(const sf::RenderWindow& window)
{
    if (messageType == MessageType::File) {
        image.value().on_click(window);
    } else if (messageType == MessageType::Sound) {
        sound.value().on_click(window);
    }   
}

void MessageContainer::updatePlayer()
{
    if (messageType == MessageType::Sound)
        sound.value().updateBar();
}

void MessageContainer::setSize()
{
    float width = 250;
    float height = 15 + senderName.getGlobalBounds().height + 10;

    if (messageType == MessageType::File) {
        image.value().setSize({ 240, 240 });
        height += 240;
    } else if (messageType == MessageType::Sound) {
        sound.value().setSize({ 240, 20 });
        height += 20;
    } else {
        height += text.value().getGlobalBounds().height + 5;
    }

    container.setSize({ width, height });
}

void MessageContainer::setPosition(sf::Vector2f position)
{
    container.setPosition(position);

    senderName.setPosition({ container.getPosition().x + 5, container.getPosition().y + 5 });

    if (messageType == MessageType::File) {
        image.value().setPosition({
            container.getPosition().x + 5,
            senderName.getPosition().y + senderName.getGlobalBounds().height + 15,
        });
    } else if (messageType == MessageType::Sound) {
        sound.value().setPosition({
            container.getPosition().x + 5,
            senderName.getPosition().y + senderName.getGlobalBounds().height + 15,
        });
    } else {
        text.value().setPosition({
            container.getPosition().x + 5,
            senderName.getPosition().y + senderName.getGlobalBounds().height + 15,
        });
    }
}

void MessageContainer::setTextColor(sf::Color color)
{
    senderName.setFillColor(color);

    if (messageType == MessageType::Text)
        text.value().setFillColor(color);
}

void MessageContainer::setPlayerInnerBarColor(sf::Color color)
{
    if (messageType == MessageType::Sound)
        sound.value().setBarInnerColor(color);
}

void MessageContainer::setPlayerOuterBarColor(sf::Color color)
{
    if (messageType == MessageType::Sound)
        sound.value().setBarOuterColor(color);
}

void MessageContainer::setImageOutlineColor(sf::Color color)
{
    if (messageType == MessageType::File)
        image.value().setOutlineColor(color);
}

void MessageContainer::setPlayerColor(sf::Color color)
{
    if (messageType == MessageType::Sound)
        sound.value().setFillColor(color); 
}

void MessageContainer::setPlayerTextColor(sf::Color color)
{
    if (messageType == MessageType::Sound)
        sound.value().setTextColor(color);
}

void MessageContainer::setPlayerHoverColor(sf::Color color)
{
    if (messageType == MessageType::Sound)
        sound.value().setHoverColor(color);
}

void MessageContainer::move(sf::Vector2f offset)
{
    container.move(offset);
    setPosition(container.getPosition());
}

sf::Vector2f MessageContainer::getSize() const
{
    return container.getSize();
}

sf::Vector2f MessageContainer::getPosition() const
{
    return container.getPosition();
}

uint64_t MessageContainer::getSenderId() const
{
    return senderId;
}

void MessageContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(container, states);
    target.draw(senderName, states);
    
    if (messageType == MessageType::File)
        target.draw(image.value(), states);
    
    else if (messageType == MessageType::Sound)
        target.draw(sound.value(), states);
    
    else
        target.draw(text.value(), states);
}
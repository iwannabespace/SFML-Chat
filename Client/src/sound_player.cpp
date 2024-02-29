#include "../include/sound_player.hpp"
#include "../include/functions.hpp"
#include <iostream>

SoundPlayer::SoundPlayer(const std::string& _filename, sf::Font& font)
    : valueBar({}, {}, sf::Color::White, sf::Color::Black, 0), font(font)
{
    filename = _filename;
    
    if (!music.openFromFile(filename))
        std::cout << filename << " coudln't be opened!" << std::endl;

    valueBar.setSize({ 60, 10 });
    valueBar.setMax(music.getDuration().asSeconds());

    currentTimeText.setFont(font);
    currentTimeText.setString("00:00");
    currentTimeText.setCharacterSize(10);
    currentTimeText.setFillColor(sf::Color::Black);

    soundLengthText.setFont(font);
    soundLengthText.setString(Functions::ConvertTimeToText(music.getDuration().asSeconds()));
    soundLengthText.setCharacterSize(10);
    soundLengthText.setFillColor(sf::Color::Black);
}

SoundPlayer::SoundPlayer(const SoundPlayer& rhs)
    : valueBar({}, {}, sf::Color::White, sf::Color::Black, 0), font(rhs.font)
{
    filename = rhs.filename;
    
    if (!music.openFromFile(filename))
        std::cout << filename << " coudln't be opened!" << std::endl;

    valueBar.setSize({ 60, 10 });
    valueBar.setMax(music.getDuration().asSeconds());

    currentTimeText.setFont(font);
    currentTimeText.setString("00:00");
    currentTimeText.setCharacterSize(10);
    currentTimeText.setFillColor(sf::Color::Black);

    soundLengthText.setFont(font);
    soundLengthText.setString(Functions::ConvertTimeToText(music.getDuration().asSeconds()));
    soundLengthText.setCharacterSize(10);
    soundLengthText.setFillColor(sf::Color::Black);
}

SoundPlayer::~SoundPlayer()
{
}

void SoundPlayer::on_hover(const sf::RenderWindow& window)
{
    if (container.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
        container.setFillColor(hoverColor);
    
    else
        container.setFillColor(fillColor);
}

void SoundPlayer::on_click(const sf::RenderWindow& window)
{
    if (container.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
    {
        auto status = music.getStatus();

        if (status == sf::Music::Status::Paused || status == sf::Music::Status::Stopped)
            music.play();
        else
            music.pause();
    }
}

void SoundPlayer::updateBar()
{
    auto status = music.getStatus();

    if (status == sf::Music::Status::Playing)
    {    
        float currentOffset = music.getPlayingOffset().asSeconds();
        currentTimeText.setString(Functions::ConvertTimeToText(currentOffset));
        valueBar.setValue(currentOffset);
    }
}

void SoundPlayer::setSize(sf::Vector2f size)
{
    container.setSize(size);
    container.setRadius(6);
}

void SoundPlayer::setPosition(sf::Vector2f position)
{
    float totalWidth = currentTimeText.getGlobalBounds().width + valueBar.getSize().x + soundLengthText.getGlobalBounds().width + 20; 
    container.setPosition(position);
    currentTimeText.setPosition({
        Functions::GetMiddle(totalWidth, container.getSize().x, container.getPosition().x, 0),
        Functions::GetMiddle(Functions::GetTextMaxHeight(currentTimeText), container.getSize().y, container.getPosition().y, 0)
    });
    valueBar.setPosition({
        currentTimeText.getPosition().x + currentTimeText.getGlobalBounds().width + 5,
        Functions::GetMiddle(valueBar.getSize().y, container.getSize().y, container.getPosition().y, 0),
    });
    soundLengthText.setPosition({
        valueBar.getPosition().x + valueBar.getSize().x + 5,
        Functions::GetMiddle(Functions::GetTextMaxHeight(soundLengthText), container.getSize().y, container.getPosition().y, 0)
    });
}

void SoundPlayer::setFillColor(sf::Color color)
{
    fillColor = color;
    container.setFillColor(color);
}

void SoundPlayer::setHoverColor(sf::Color color)
{
    hoverColor = color;
}

void SoundPlayer::setTextColor(sf::Color color)
{
    currentTimeText.setFillColor(color);
    soundLengthText.setFillColor(color);
}

void SoundPlayer::setBarInnerColor(sf::Color color)
{
    valueBar.setInnerColor(color);
}

void SoundPlayer::setBarOuterColor(sf::Color color)
{
    valueBar.setOuterColor(color);
}

sf::Vector2f SoundPlayer::getSize() const
{
    return container.getSize();
}

sf::Vector2f SoundPlayer::getPosition() const
{
    return container.getPosition();
}

const std::string& SoundPlayer::getFilename() const
{
    return filename;
}

SoundPlayer& SoundPlayer::operator=(const SoundPlayer& rhs)
{
    filename = rhs.filename;
    
    if (!music.openFromFile(filename))
        std::cout << filename << " coudln't be opened!" << std::endl;
    
    font = rhs.font;

    valueBar.setSize({ 60, 10 });
    valueBar.setMax(music.getDuration().asSeconds());

    currentTimeText.setFont(font);
    currentTimeText.setString("00:00");
    currentTimeText.setCharacterSize(10);
    currentTimeText.setFillColor(sf::Color::Black);

    soundLengthText.setFont(font);
    soundLengthText.setString(Functions::ConvertTimeToText(music.getDuration().asSeconds()));
    soundLengthText.setCharacterSize(10);
    soundLengthText.setFillColor(sf::Color::Black);

    return *this;
}

void SoundPlayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(container, states);
    target.draw(currentTimeText, states);
    target.draw(soundLengthText, states);
    target.draw(valueBar, states);
}
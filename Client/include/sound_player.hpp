#ifndef SOUND_PLAYER_HPP
#define SOUND_PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "valuebar.hpp"
#include "RoundRect.hpp"

class SoundPlayer : public sf::Drawable
{
    public:
        SoundPlayer(const std::string& _filename, sf::Font& font);
        SoundPlayer(const SoundPlayer& rhs);
        ~SoundPlayer();
        void on_hover(const sf::RenderWindow& window);
        void on_click(const sf::RenderWindow& window);
        void updateBar();
        void setSize(sf::Vector2f size);
        void setPosition(sf::Vector2f position);
        void setFillColor(sf::Color color);
        void setHoverColor(sf::Color color);
        void setTextColor(sf::Color color);
        void setBarInnerColor(sf::Color color);
        void setBarOuterColor(sf::Color color);
        sf::Vector2f getSize() const;
        sf::Vector2f getPosition() const;
        const std::string& getFilename() const;
        SoundPlayer& operator=(const SoundPlayer& rhs);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        crd::RoundRect container;
        ValueBar valueBar;
        sf::Text currentTimeText;
        sf::Text soundLengthText;
        sf::Color fillColor;
        sf::Color hoverColor;
        sf::Music music;
        sf::Font& font;
        std::string filename;
};

#endif
#ifndef MESSAGE_CONTAINER_HPP
#define MESSAGE_CONTAINER_HPP

#include <SFML/Graphics.hpp>
#include <optional>
#include "message.hpp"
#include "image_viewer.hpp"
#include "sound_player.hpp"

class MessageContainer : public sf::Drawable
{
    public:
        MessageContainer(const Message& message, sf::Font& font);
        MessageContainer(const MessageContainer& rhs);
        ~MessageContainer();
        void on_hover_objects(const sf::RenderWindow& window);
        void on_click_objects(const sf::RenderWindow& window);
        void updatePlayer();
        void setSize();
        void setPosition(sf::Vector2f position);
        void setTextColor(sf::Color color);
        void setPlayerInnerBarColor(sf::Color color);
        void setPlayerOuterBarColor(sf::Color color);
        void setImageOutlineColor(sf::Color color);
        void setPlayerColor(sf::Color color);
        void setPlayerTextColor(sf::Color color);
        void setPlayerHoverColor(sf::Color color);
        void move(sf::Vector2f offset);
        sf::Vector2f getSize() const;
        sf::Vector2f getPosition() const;
        uint64_t getSenderId() const;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        uint64_t senderId;
        MessageType messageType;
        sf::RectangleShape container;
        sf::Text senderName;
        std::optional<sf::Text> text;
        std::optional<ImageViewer> image;
        std::optional<SoundPlayer> sound;
        std::string path;
        sf::Font& font;
};

#endif
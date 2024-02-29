#ifndef CHAT_SCREEN_HPP
#define CHAT_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "textbox.hpp"
#include "button.hpp"
#include "client.hpp"
#include "activatable.hpp"
#include "message_container.hpp"

class ChatScreen : public sf::Drawable
{
    public:
        ChatScreen(const sf::RenderWindow& window, Client& client, sf::Font& font);
        ~ChatScreen();
        void on_window_resize(const sf::RenderWindow& window);
        void on_hover_items(const sf::RenderWindow& window);
        void on_click_items(const sf::RenderWindow& window);
        void on_event_click_items(const sf::RenderWindow& window);
        void on_text_entered(uint32_t character);
        void on_key_pressed(sf::Keyboard::Key key);
        void on_scrolled(float delta, const sf::RenderWindow& window);
        void on_recorder_started();
        void on_recorder_stopped();
        void updateSoundPlayer();
        void draw_rt_items();
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::RenderTexture messagesRt;
        sf::RenderTexture onlinesBarRT;
        sf::RectangleShape messageDrawContainer;
        sf::RectangleShape onlinesTitleBar;
        sf::RectangleShape onlinesBar;
        sf::Text onlinesBarText;
        Textbox inputBox;
        Button sendButton;
        Button sendFileButton;
        Activatable soundRecorderButton;
        sf::SoundBufferRecorder soundRecorder;
        Client& client;
        std::vector<sf::Text> onlines;
        std::vector<MessageContainer> messages;
};

#endif

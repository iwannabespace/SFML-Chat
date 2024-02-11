#ifndef LOGIN_SCREEN_HPP
#define LOGIN_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include "textbox.hpp"
#include "button.hpp"
#include "client.hpp"
#include "RoundRect.hpp"

class LoginScreen : public sf::Drawable
{
    public:
        LoginScreen(const sf::RenderWindow& window, Client& client, sf::Font& font);
        ~LoginScreen();
        void on_window_resize(const sf::RenderWindow& window);
        void on_hover_items(const sf::RenderWindow& window);
        void on_click_items(const sf::RenderWindow& window);
        void on_event_click_items(const sf::RenderWindow& window);
        void on_text_entered(uint32_t character);
        void on_key_pressed(sf::Keyboard::Key key);
        void draw_rt_items();
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        crd::RoundRect mainFrame;
        crd::RoundRect colorPreviewBox;
        Textbox usernameTextbox;
        Textbox rTextbox, gTextbox, bTextbox;
        sf::Text usernameText;
        sf::Text rgbText;
        Button button;
        bool _done = false;
        Client& client;

};

#endif
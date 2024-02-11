#include "../include/chat_screen.hpp"
#include "../include/theme.hpp"
#include "../include/functions.hpp"

ChatScreen::ChatScreen(const sf::RenderWindow& window, Client& client, sf::Font& font)
    : inputBox("text...", {}, {}, Theme::Secondary, Theme::Text, Theme::Text, Theme::Text, Theme::Text, font, 1, std::nullopt),
      sendButton({}, {}, Theme::Secondary, Theme::Text, Theme::Text, "send", font),
      sendFileButton({}, {}, Theme::Secondary, Theme::Text, Theme::Text, "choose", font),
      sendRecordingButton({}, {}, Theme::Secondary, Theme::Text, Theme::Text, "record", font),
      client(client)
{
    float winw = window.getSize().x;
    float winh = window.getSize().y;
    
    onlinesTitleBar.setSize({ 150, 30 });
    onlinesTitleBar.setPosition({ winw - onlinesTitleBar.getSize().x, 0 });
    onlinesTitleBar.setFillColor(Theme::Secondary);

    onlinesBar.setSize({ onlinesTitleBar.getSize().x, winh - onlinesTitleBar.getSize().y });
    onlinesBar.setPosition({ winw - onlinesBar.getSize().x, 30 });
    onlinesBar.setFillColor(Theme::Secondary);

    onlinesBarText.setFont(font);
    onlinesBarText.setCharacterSize(14);
    onlinesBarText.setString("Online Gays");
    onlinesBarText.setPosition({
        Functions::GetMiddle(onlinesBarText.getGlobalBounds().width, onlinesTitleBar.getSize().x, onlinesTitleBar.getPosition().x, 0),
        Functions::GetMiddle(onlinesBarText.getGlobalBounds().height, onlinesTitleBar.getSize().y, onlinesTitleBar.getPosition().y, 0),
    });
    onlinesBarText.setFillColor(Theme::Text);
    
    sendButton.setSize({ 60, 40 });
    sendButton.setPosition({ onlinesBar.getPosition().x - sendButton.getSize().x - 20, winh - sendButton.getSize().y - 10 });

    inputBox.setSize({ winw - onlinesBar.getSize().x - sendButton.getSize().x - 50, 40 });
    inputBox.setPosition({ 20, winh - inputBox.getSize().y - 10 });
}

ChatScreen::~ChatScreen()
{

}

void ChatScreen::on_window_resize(const sf::RenderWindow& window)
{   
    float winw = window.getSize().x;
    float winh = window.getSize().y;

    onlinesTitleBar.setPosition({ winw - onlinesTitleBar.getSize().x, 0 });

    onlinesBar.setSize({ onlinesTitleBar.getSize().x, winh - onlinesTitleBar.getSize().y });
    onlinesBar.setPosition({ winw - onlinesBar.getSize().x, 30 });
    
    
    onlinesBarText.setPosition({
        Functions::GetMiddle(onlinesBarText.getGlobalBounds().width, onlinesTitleBar.getSize().x, onlinesTitleBar.getPosition().x, 0),
        Functions::GetMiddle(onlinesBarText.getGlobalBounds().height, onlinesTitleBar.getSize().y, onlinesTitleBar.getPosition().y, 0),
    });
    
    sendButton.setPosition({ onlinesBar.getPosition().x - sendButton.getSize().x - 20, winh - inputBox.getSize().y - 10 });

    inputBox.setSize({ winw - onlinesBar.getSize().x - sendButton.getSize().x - 50, 40 });
    inputBox.setPosition({ 20, winh - inputBox.getSize().y - 10 });
}

void ChatScreen::on_hover_items(const sf::RenderWindow& window)
{
    inputBox.on_hover(window);
    sendButton.on_hover(window);
}

void ChatScreen::on_click_items(const sf::RenderWindow& window)
{
    inputBox.on_click(window);
}

void ChatScreen::on_event_click_items(const sf::RenderWindow& window)
{
    sendButton.on_click(window, []() {});
}

void ChatScreen::on_text_entered(uint32_t character)
{
    inputBox.on_input(character);
}

void ChatScreen::on_key_pressed(sf::Keyboard::Key key)
{
    inputBox.on_shortcuts();

    if (key == sf::Keyboard::Key::Left)
    {
        inputBox.on_text_left();
    }

    else if (key == sf::Keyboard::Key::Right)
    {
        inputBox.on_text_right();
    }
}

void ChatScreen::draw_rt_items()
{
    inputBox.draw_rt();
}

void ChatScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(onlinesTitleBar, states);
    target.draw(onlinesBar, states);
    target.draw(onlinesBarText, states);
    target.draw(inputBox, states);
    target.draw(sendButton, states);
}
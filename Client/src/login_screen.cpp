#include "../include/login_screen.hpp"
#include "../include/theme.hpp"
#include "../include/functions.hpp"
#include <iostream>

LoginScreen::LoginScreen(const sf::RenderWindow& window, Client& client, sf::Font& font)
    : client(client), 
      usernameTextbox("username", { 200, 25 }, {}, Theme::Primary, Theme::Text, Theme::Text, Theme::Text, Theme::Text, font, 1, 15),
      rTextbox("R", { 45, 25 }, {}, Theme::Primary, Theme::Text, Theme::Text, Theme::Text, Theme::Text, font, 1, 3, "", true),
      gTextbox("G", { 45, 25 }, {}, Theme::Primary, Theme::Text, Theme::Text, Theme::Text, Theme::Text, font, 1, 3, "", true),
      bTextbox("B", { 45, 25 }, {}, Theme::Primary, Theme::Text, Theme::Text, Theme::Text, Theme::Text, font, 1, 3, "", true),
      button({ 70, 30 }, { 30, 200 }, Theme::Primary, sf::Color::White, Theme::Text, "Connect", font)
{
    float winw = window.getSize().x;
    float winh = window.getSize().y;

    mainFrame.setSize({ 300, 300 });
    mainFrame.setPosition({ std::round((winw - mainFrame.getSize().x) / 2), std::round((winh - mainFrame.getSize().y) / 2) });
    mainFrame.setFillColor(Theme::Secondary);
    mainFrame.setRadius(6);

    float itemPosX = mainFrame.getPosition().x + 10;

    usernameText.setFont(font);
    usernameText.setString("Username");
    usernameText.setCharacterSize(14);
    usernameText.setFillColor(Theme::Text);
    usernameText.setPosition({ itemPosX, mainFrame.getPosition().y + 15 });

    usernameTextbox.setPosition({ itemPosX, usernameText.getPosition().y + usernameText.getGlobalBounds().height + 15 });

    rgbText.setFont(font);
    rgbText.setString("Pick a color");
    rgbText.setCharacterSize(14);
    rgbText.setFillColor(Theme::Text);
    rgbText.setPosition({ itemPosX, usernameTextbox.getPosition().y + usernameTextbox.getSize().y + 15 });

    rTextbox.setPosition({ itemPosX, rgbText.getPosition().y + rgbText.getGlobalBounds().height + 15 });
    gTextbox.setPosition({ rTextbox.getPosition().x + rTextbox.getSize().x + 5, rgbText.getPosition().y + rgbText.getGlobalBounds().height + 15 });
    bTextbox.setPosition({ gTextbox.getPosition().x + gTextbox.getSize().x + 5, rgbText.getPosition().y + rgbText.getGlobalBounds().height + 15 });

    colorPreviewBox.setSize({ 25, 25 });
    colorPreviewBox.setPosition({ bTextbox.getPosition().x + bTextbox.getSize().x + 25, bTextbox.getPosition().y });
    colorPreviewBox.setFillColor(sf::Color::Black);
    colorPreviewBox.setRadius(4);

    button.setPosition({ 
        Functions::GetMiddle(button.getSize().x, mainFrame.getSize().x, mainFrame.getPosition().x, 0),
        mainFrame.getPosition().y + mainFrame.getSize().y - button.getSize().y - 10,
    });
}

LoginScreen::~LoginScreen()
{
}

void LoginScreen::on_window_resize(const sf::RenderWindow& window)
{
    float winw = window.getSize().x;
    float winh = window.getSize().y;

    mainFrame.setPosition({ std::round((winw - mainFrame.getSize().x) / 2), std::round((winh - mainFrame.getSize().y) / 2) });

    float itemPosX = mainFrame.getPosition().x + 10;

    usernameText.setPosition({ itemPosX, mainFrame.getPosition().y + 15 });

    usernameTextbox.setPosition({ itemPosX, usernameText.getPosition().y + usernameText.getGlobalBounds().height + 15 });

    rgbText.setPosition({ itemPosX, usernameTextbox.getPosition().y + usernameTextbox.getSize().y + 15 });

    rTextbox.setPosition({ itemPosX, rgbText.getPosition().y + rgbText.getGlobalBounds().height + 15 });
    gTextbox.setPosition({ rTextbox.getPosition().x + rTextbox.getSize().x + 5, rgbText.getPosition().y + rgbText.getGlobalBounds().height + 15 });
    bTextbox.setPosition({ gTextbox.getPosition().x + gTextbox.getSize().x + 5, rgbText.getPosition().y + rgbText.getGlobalBounds().height + 15 });

    colorPreviewBox.setPosition({ bTextbox.getPosition().x + bTextbox.getSize().x + 25, bTextbox.getPosition().y });

    button.setPosition({ 
        Functions::GetMiddle(button.getSize().x, mainFrame.getSize().x, mainFrame.getPosition().x, 0),
        mainFrame.getPosition().y + mainFrame.getSize().y - button.getSize().y - 10,
    });
}

void LoginScreen::on_hover_items(const sf::RenderWindow& window)
{
    usernameTextbox.on_hover(window);
    rTextbox.on_hover(window);
    gTextbox.on_hover(window);
    bTextbox.on_hover(window);
    button.on_hover(window);
}

void LoginScreen::on_click_items(const sf::RenderWindow& window)
{
    usernameTextbox.on_click(window);
    rTextbox.on_click(window);
    gTextbox.on_click(window);
    bTextbox.on_click(window);
}

void LoginScreen::on_event_click_items(const sf::RenderWindow& window)
{
    button.on_click(window, [this]() {
        std::string username = this->usernameTextbox.value();
        size_t usernameLength = username.length();
        sf::Color color = Functions::getColorFromTextbox(rTextbox.value(), gTextbox.value(), bTextbox.value());

        this->client.join(username, color);
    });   
}   

void LoginScreen::on_text_entered(uint32_t character)
{
    usernameTextbox.on_input(character);
    rTextbox.on_input(character);
    gTextbox.on_input(character);
    bTextbox.on_input(character);

    colorPreviewBox.setFillColor(Functions::getColorFromTextbox(rTextbox.value(), gTextbox.value(), bTextbox.value()));
}

void LoginScreen::on_key_pressed(sf::Keyboard::Key key)
{
    usernameTextbox.on_shortcuts();
    rTextbox.on_shortcuts();
    gTextbox.on_shortcuts();
    bTextbox.on_shortcuts();

    if (key == sf::Keyboard::Key::Left)
    {
        usernameTextbox.on_text_left();
        rTextbox.on_text_left();
        gTextbox.on_text_left();
        bTextbox.on_text_left();
    }

    else if (key == sf::Keyboard::Key::Right)
    {
        usernameTextbox.on_text_right();
        rTextbox.on_text_right();
        gTextbox.on_text_right();
        bTextbox.on_text_right();
    }
}

void LoginScreen::draw_rt_items()
{
    usernameTextbox.draw_rt();
    rTextbox.draw_rt();
    gTextbox.draw_rt();
    bTextbox.draw_rt();
}


void LoginScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mainFrame, states);
    target.draw(usernameText, states);
    target.draw(usernameTextbox, states);
    target.draw(rgbText, states);
    target.draw(rTextbox, states);
    target.draw(gTextbox, states);
    target.draw(bTextbox, states);
    target.draw(colorPreviewBox, states);
    target.draw(button, states);
}
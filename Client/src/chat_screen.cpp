#include "../include/chat_screen.hpp"
#include "../include/theme.hpp"
#include "../include/functions.hpp"
#include "../include/portable-file-dialogs.h"
#include "../../Shared/shared.hpp"
#include <filesystem>

ChatScreen::ChatScreen(const sf::RenderWindow& window, Client& client, sf::Font& font)
    : inputBox("text...", { 10, 10 }, {}, Theme::Secondary, Theme::Text, Theme::Text, Theme::Text, Theme::Text, font, 1, std::nullopt),
      sendButton({}, {}, Theme::Secondary, Theme::Text, Theme::Text, "send", font),
      sendFileButton({}, {}, Theme::Secondary, Theme::Text, Theme::Text, "choose", font),
      soundRecorderButton({}, {}, Theme::Secondary, Theme::Text, Theme::Text, "files/microphone_24.png"),
      client(client)
{
    float winw = window.getSize().x;
    float winh = window.getSize().y;
    
    onlinesTitleBar.setSize({ 150, 30 });
    onlinesTitleBar.setPosition({ winw - onlinesTitleBar.getSize().x, 0 });
    onlinesTitleBar.setFillColor(Theme::Secondary);

    onlinesBar.setSize({ onlinesTitleBar.getSize().x, winh - onlinesTitleBar.getSize().y });
    onlinesBar.setPosition({ winw - onlinesBar.getSize().x, 30 });

    if (!onlinesBarRT.create(onlinesTitleBar.getSize().x, winh - onlinesTitleBar.getSize().y))
        throw "onlinesBarRT couldn't be created!";

    onlinesBarText.setFont(font);
    onlinesBarText.setCharacterSize(14);
    onlinesBarText.setString("Online Gays");
    onlinesBarText.setPosition({
        Functions::GetMiddle(onlinesBarText.getGlobalBounds().width, onlinesTitleBar.getSize().x, onlinesTitleBar.getPosition().x, 0),
        Functions::GetMiddle(onlinesBarText.getGlobalBounds().height, onlinesTitleBar.getSize().y, onlinesTitleBar.getPosition().y, 0),
    });
    onlinesBarText.setFillColor(Theme::Text);

    sf::Vector2f buttonSize = { 60, 40 };

    soundRecorderButton.setSize(buttonSize);
    soundRecorderButton.setPosition({ onlinesBar.getPosition().x - buttonSize.x - 20, winh - buttonSize.y - 10 });

    sendFileButton.setSize(buttonSize);
    sendFileButton.setPosition({ soundRecorderButton.getPosition().x - buttonSize.x - 10, winh - buttonSize.y - 10 });
    
    sendButton.setSize(buttonSize);
    sendButton.setPosition({ sendFileButton.getPosition().x - buttonSize.x - 10, winh - buttonSize.y - 10 });

    inputBox.setSize({ winw - onlinesBar.getSize().x - buttonSize.x * 3 - 70, 40 });
    inputBox.setPosition({ 20, winh - inputBox.getSize().y - 10 });

    messageDrawContainer.setSize({ winw - onlinesTitleBar.getSize().x, winh - (winh - inputBox.getPosition().y) - 50 });
    messageDrawContainer.setPosition({ 0, 0 });

    if (!messagesRt.create(messageDrawContainer.getSize().x, messageDrawContainer.getSize().y))
        throw "messageRt couldn't be created!";

    client.setOnJoinerJoinedCallback([this, &font](const Joiner& joiner) {
        sf::Text text(joiner.username, font, 14);
        sf::Vector2f pos = { 5, 5 };

        if (!this->onlines.empty()) {
            sf::Text last = this->onlines.back().second;
            pos.y = last.getPosition().y + last.getGlobalBounds().height + 20;
        }

        text.setPosition(pos);
        text.setFillColor(joiner.color);
        this->onlines.emplace_back(joiner.id, text);
    });
    
    client.setOnMessageReceivedCallback([this, &font](const Message& message) {
        sf::Vector2f pos = { 10, 10 };
        MessageContainer container(message, font);

        if (!this->messages.empty()) {
            pos.y += this->messages.back().getPosition().y + this->messages.back().getSize().y + 10;
        }

        if (!this->client.isMe(message.senderId)) {
            pos.x = this->messageDrawContainer.getSize().x - 250 - 10;
        }

        container.setSize();
        container.setPosition(pos);
        container.setTextColor(message.joiner.color);
        container.setImageOutlineColor(message.joiner.color);
        container.setPlayerHoverColor(sf::Color::Red);
        container.setPlayerColor(message.joiner.color);
        container.setPlayerInnerBarColor(sf::Color::Black);
        container.setPlayerOuterBarColor(sf::Color::Green);
        container.setPlayerTextColor(sf::Color::Black);

        this->messages.emplace_back(container);

        float diff = (container.getPosition().y + container.getSize().y) - this->messageDrawContainer.getSize().y;

        if (diff > 0) {
            for (auto& msg : this->messages)
                msg.move({ 0, -(diff + 10) });
        }
    });

    client.setOnJoinerRemovedCallback([this](const Joiner& joiner) {
        this->onlines.erase(std::remove_if(
            this->onlines.begin(), 
            this->onlines.end(),
            [&joiner](const std::pair<uint64_t, sf::Text>& online) { return online.first == joiner.id; }),
            this->onlines.end()
        );

        sf::Vector2f pos = { 5, 5 };

        for (auto& [id, online] : this->onlines) {
            online.setPosition(pos);
            pos.y += 20;
        }
    });

    soundRecorderButton.setCallback([this]() {
        this->soundRecorder.start();
    });
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

    if (!onlinesBarRT.create(onlinesTitleBar.getSize().x, winh - onlinesTitleBar.getSize().y))
        throw "onlinesBarRT couldn't be created!";
    
    onlinesBarText.setPosition({
        Functions::GetMiddle(onlinesBarText.getGlobalBounds().width, onlinesTitleBar.getSize().x, onlinesTitleBar.getPosition().x, 0),
        Functions::GetMiddle(onlinesBarText.getGlobalBounds().height, onlinesTitleBar.getSize().y, onlinesTitleBar.getPosition().y, 0),
    });
    
    sf::Vector2f buttonSize = { 60, 40 };

    soundRecorderButton.setPosition({ onlinesBar.getPosition().x - buttonSize.x - 20, winh - buttonSize.y - 10 });
    sendFileButton.setPosition({ soundRecorderButton.getPosition().x - buttonSize.x - 10, winh - buttonSize.y - 10 });
    sendButton.setPosition({ sendFileButton.getPosition().x - buttonSize.x - 10, winh - buttonSize.y - 10 });

    inputBox.setSize({ winw - onlinesBar.getSize().x - buttonSize.x * 3 - 70, 40 });
    inputBox.setPosition({ 20, winh - inputBox.getSize().y - 10 });

    messageDrawContainer.setSize({ winw - onlinesTitleBar.getSize().x, winh - (winh - inputBox.getPosition().y) - 50 });

    if (!messagesRt.create(messageDrawContainer.getSize().x, messageDrawContainer.getSize().y))
        throw "messageRt couldn't be created!";

    for (auto& message : messages) {
        if (client.isMe(message.getSenderId())) {
            message.setPosition({ 10, message.getPosition().y });
        } else {
            message.setPosition({ messageDrawContainer.getSize().x - 250 - 10, message.getPosition().y });
        }
        message.on_window_resize(window);
    }
}

void ChatScreen::on_hover_items(const sf::RenderWindow& window)
{
    inputBox.on_hover(window);
    sendButton.on_hover(window);
    sendFileButton.on_hover(window);
    soundRecorderButton.on_hover(window);

    for (auto& message : messages)
        message.on_hover_objects(window);
}

void ChatScreen::on_click_items(const sf::RenderWindow& window)
{
    inputBox.on_click(window);
}

void ChatScreen::on_right_click_items(const sf::RenderWindow& window)
{
    for (auto& message : messages)
        message.on_right_click_objects(window);
}

void ChatScreen::on_event_click_items(const sf::RenderWindow& window)
{
    sendButton.on_click(window, [this]() {
        if (this->client.sendMessage(Shared::TEXT, this->inputBox.value()))
        {   
            uint64_t id = this->client.getId();
            Message message = Functions::CreateMessage(id, Shared::TEXT, this->inputBox.value(), "");
            message.joiner = { this->client.getUsername(), id, this->client.getColor() };
            this->client.newMessage(message);
            this->inputBox.clear();
        }
    });

    sendFileButton.on_click(window, [this]() {
        auto result = pfd::open_file("Choose an Image", ".", { "Image Files", "*.bmp *.png *.tga *.jpg *.gif *.psd *.hdr *.pic *.pnm *.jpeg" }).result();
        auto content = Functions::ReadFileAsBinary(result[0]);

        auto extension = std::filesystem::path(result[0]).extension().string();
 
        if (this->client.sendMessage(Shared::FILE, content, extension))
        {
            auto joiner = Joiner();
            joiner.username = this->client.getUsername();
            joiner.color = this->client.getColor();
            joiner.id = this->client.getId();
            this->client.newMessage({ joiner.id, joiner, result[0], MessageType::File });
        }
    });
    
    soundRecorderButton.on_event_click(window);

    for (auto& message : messages)
        message.on_click_objects(window);
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

    else if (key == sf::Keyboard::Return)
    {
        if (inputBox.active())
        {
            if (client.sendMessage(Shared::TEXT, inputBox.value()))
            {    
                uint64_t id = this->client.getId();
                Message message = Functions::CreateMessage(id, Shared::TEXT, this->inputBox.value(), "");
                message.joiner = { this->client.getUsername(), id, this->client.getColor() };
                this->client.newMessage(message);
                this->inputBox.clear();
            }
        }
    }
}

void ChatScreen::on_scrolled(float delta, const sf::RenderWindow& window)
{
    if ((delta < 0 && (onlines.back().second.getPosition().y + onlines.back().second.getGlobalBounds().height > onlinesBar.getSize().y)) || 
        (delta > 0 && (onlines.front().second.getPosition().y < 5 )))
    {    
        if (onlinesBar.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
            for (auto& [id, online] : onlines)
                online.move({ 0, std::round(delta) });
    }
    
    else if ((delta < 0 && (messages.back().getPosition().y + messages.back().getSize().y > messageDrawContainer.getSize().y)) || 
        (delta > 0 && (messages.front().getPosition().y < 10 )))
    {    
        if (messageDrawContainer.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
            for (auto& message : messages)
                message.move({ 0, std::round(delta) });
    }
}

void ChatScreen::on_recorder_started()
{
    soundRecorderButton.on_active();
}

void ChatScreen::on_recorder_stopped()
{
    if (soundRecorderButton.deactivated())
    {
        soundRecorder.stop();
        const sf::SoundBuffer& buffer = soundRecorder.getBuffer();
        std::string filename = "files/" + Functions::GetFileName() + ".ogg";
        
        if (!buffer.saveToFile(filename))
            throw "Recording couldn't be saved!";
        
        std::string content = Functions::ReadFileAsBinary(filename);
        uint64_t id = client.getId();
        
        if (client.sendMessage(Shared::SOUND, content, ".ogg"))
        {
            auto joiner = Joiner();
            joiner.username = this->client.getUsername();
            joiner.color = this->client.getColor();
            joiner.id = this->client.getId();
            this->client.newMessage({ joiner.id, joiner, filename, MessageType::Sound });
        }
    }
}

void ChatScreen::updateSoundPlayer()
{
    for (auto& message : messages)
        message.updatePlayer();
}

void ChatScreen::draw_rt_items()
{
    inputBox.draw_rt();

    onlinesBarRT.clear(Theme::Secondary);
    
    for (const auto& [id, online] : onlines)
        onlinesBarRT.draw(online);
    
    onlinesBarRT.display();
    onlinesBar.setTexture(&onlinesBarRT.getTexture(), true);

    messagesRt.clear(Theme::Primary);

    for (const auto& message : messages) 
        messagesRt.draw(message);
    
    messagesRt.display();
    messageDrawContainer.setTexture(&messagesRt.getTexture(), true);
}

void ChatScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(messageDrawContainer);
    target.draw(onlinesTitleBar, states);
    target.draw(onlinesBar, states);
    target.draw(onlinesBarText, states);
    target.draw(inputBox, states);
    target.draw(sendButton, states);
    target.draw(sendFileButton, states);
    target.draw(soundRecorderButton, states);
}
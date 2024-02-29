#include <iostream>
#include <thread>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "../include/app.hpp"
#include "../include/client.hpp"
#include "../include/joiner.hpp"
#include "../include/button.hpp"
#include "../include/font_manager.hpp"
#include "../include/theme.hpp"
#include "../include/login_screen.hpp"
#include "../include/chat_screen.hpp"
#include "../include/activatable.hpp"
#include "../include/image_viewer.hpp"
#include "../include/option.hpp"
#include "../include/options.hpp"
#include "../include/sound_player.hpp"
#include "../include/message_container.hpp"
#include "../include/functions.hpp"
#include "../../Shared/shared.hpp"

App::App()
{
    settings.antialiasingLevel = 8;
    
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Chat", sf::Style::Default, settings);
    window->setVerticalSyncEnabled(true);

    sf::Vector2f win_size = { static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y) };

    view.setSize(win_size.x, win_size.y);
    view.setCenter({ win_size.x / 2, win_size.y / 2 });
}

App::~App()
{
}

bool App::run()
{
    Client client("138.2.137.174", 9472);

    if (client.connect())
    {
        auto manager = FontManager();
        manager.load("fonts/UbuntuMono-Regular.ttf", "monospace");

        sf::Font& monospaceFont = *manager.get("monospace").value();

        std::thread receiver(&App::receiver, this, std::ref(client));

        std::unique_ptr<LoginScreen> loginScreen = std::make_unique<LoginScreen>(*window, client, monospaceFont);
        std::unique_ptr<ChatScreen> chatScreen = std::make_unique<ChatScreen>(*window, client, monospaceFont);

        bool chatScreenOpened = false;

        while (window->isOpen() && client.connected())
        {
            sf::Event event;

            while (window->pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                        window->close();
                        break;
                    case sf::Event::Resized:
                    {
                        handleWindowSize();

                        float width = window->getSize().x;
                        float height = window->getSize().y;

                        view.setSize(width, height);
                        view.setCenter({ width / 2, height / 2 });

                        if (!client.joined())
                            loginScreen->on_window_resize(*window);
                        else
                            chatScreen->on_window_resize(*window);
                        break;
                    }
                    case sf::Event::MouseButtonReleased:
                        if (!client.joined())
                            loginScreen->on_event_click_items(*window);
                        else
                            chatScreen->on_event_click_items(*window);
                        break;
                    case sf::Event::MouseWheelScrolled:
                        if (client.joined())
                        {    
                            float delta = event.mouseWheelScroll.delta;
                            chatScreen->on_scrolled(delta, *window);
                        }
                        break;
                    case sf::Event::TextEntered:
                        if (!client.joined())
                            loginScreen->on_text_entered(event.text.unicode);
                        else
                            chatScreen->on_text_entered(event.text.unicode);
                        break;
                    case sf::Event::KeyPressed:
                        if (!client.joined())
                            loginScreen->on_key_pressed(event.key.code);
                        else
                            chatScreen->on_key_pressed(event.key.code);
                        break;
                    default:
                        break;
                }
            }

            if (!client.joined())
            {
                loginScreen->on_hover_items(*window);
                loginScreen->on_click_items(*window);
                loginScreen->draw_rt_items();
            }
            
            else
            {
                if (!chatScreenOpened)
                {    
                    chatScreen->on_window_resize(*window);
                    client.newJoiner({ client.getUsername(), client.getId(), client.getColor() });
                    chatScreenOpened = true;
                }

                chatScreen->on_hover_items(*window);
                chatScreen->on_click_items(*window);
                chatScreen->on_recorder_started();
                chatScreen->on_recorder_stopped();
                chatScreen->updateSoundPlayer();
                chatScreen->draw_rt_items();
            }
            
            window->setView(view);
            window->clear(Theme::Primary);
            if (!client.joined()) window->draw(*loginScreen);
            else window->draw(*chatScreen);
            window->display();
        }

        return true;
    }

    else
    {   
        std::cout << "Connection to server is failed!" << std::endl;
        return false;
    }
}

void App::handleWindowSize()
{
    uint32_t width = window->getSize().x;
    uint32_t height = window->getSize().y;

    sf::Vector2i pos = window->getPosition();

    if (width < 600)
    {
        window->setSize({ 600, height });
        width = 600;
    }

    if (height < 600)
        window->setSize({ width, 600 });
    
    window->setPosition(pos);
}

void App::receiver(Client& client)
{
    sf::Packet packet;
    uint8_t descriptor;

    while (true)
    {
        if (client.receive(packet))
        {
            packet >> descriptor;

            if (descriptor == Shared::ID)
            {
                uint64_t id;
                packet >> id;
                client.setId(id);
            }

            else if (descriptor == Shared::NEW_CLIENT)
            {
                std::string username;
                uint64_t id;
                uint8_t r, g, b;

                packet >> id >> username >> r >> g >> b;
                client.newJoiner({ username, id, { r, g, b } });
            }

            else if (descriptor == Shared::OTHER_CLIENTS)
            {
                uint64_t count;
                std::string username;
                uint64_t id;
                uint8_t r, g, b;

                packet >> count;

                for (uint64_t i = 0; i < count; i++)
                {
                    packet >> id >> username >> r >> g >> b;
                    client.newJoiner({ username, id, { r, g, b } });
                }
            }

            else if (descriptor == Shared::MESSAGE)
            {
                uint64_t id;
                uint8_t type;
                std::string data;
                std::string extension;

                packet >> id >> type >> data >> extension;
                Message message = Functions::CreateMessage(id, type, data, extension);
                message.joiner = client.getJoiners().at(id);

                client.newMessage(message);
            }
        }
    }
}
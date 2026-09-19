#include <filesystem>
#include <iostream>
#include <thread>
#include <unordered_map>
#include "../include/app.hpp"
#include "../include/client.hpp"
#include "../include/joiner.hpp"
#include "../include/font_manager.hpp"
#include "../include/theme.hpp"
#include "../include/login_screen.hpp"
#include "../include/chat_screen.hpp"
#include "../include/functions.hpp"
#include "../../Shared/shared.hpp"

App::App()
{
    settings.antiAliasingLevel = 8;

    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode::getDesktopMode(), "Chat", sf::Style::Default, sf::State::Windowed, settings);
    window->setVerticalSyncEnabled(true);

    sf::Vector2f win_size = { static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y) };

    view.setSize(win_size);
    view.setCenter({ win_size.x / 2, win_size.y / 2 });

    std::filesystem::create_directory("files");
}

App::~App()
{
}

bool App::run()
{
    Client client(sf::IpAddress(127, 0, 0, 1), 9472);

    if (client.connect())
    {
        auto manager = FontManager();
        manager.load("assets/fonts/UbuntuMono-Regular.ttf", "monospace");

        sf::Font& monospaceFont = *manager.get("monospace").value();

        std::thread receiver(&App::receiver, this, std::ref(client));

        std::unique_ptr<LoginScreen> loginScreen = std::make_unique<LoginScreen>(*window, client, monospaceFont);
        std::unique_ptr<ChatScreen> chatScreen = std::make_unique<ChatScreen>(*window, client, monospaceFont);

        bool chatScreenOpened = false;

        while (window->isOpen() && client.connected())
        {
            while (const std::optional event = window->pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window->close();
                }
                else if (event->is<sf::Event::Resized>())
                {
                    handleWindowSize();

                    const float width = static_cast<float>(window->getSize().x);
                    const float height = static_cast<float>(window->getSize().y);

                    view.setSize({ width, height });
                    view.setCenter({ width / 2, height / 2 });

                    if (!client.joined())
                        loginScreen->on_window_resize(*window);
                    else
                        chatScreen->on_window_resize(*window);
                }
                else if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonReleased>())
                {
                    if (!client.joined())
                        loginScreen->on_event_click_items(*window);
                    else if (mouseButton->button == sf::Mouse::Button::Left)
                        chatScreen->on_event_click_items(*window);
                    else
                        chatScreen->on_right_click_items(*window);
                }
                else if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>())
                {
                    if (client.joined())
                        chatScreen->on_scrolled(mouseWheel->delta, *window);
                }
                else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
                {
                    if (!client.joined())
                        loginScreen->on_text_entered(textEntered->unicode);
                    else
                        chatScreen->on_text_entered(textEntered->unicode);
                }
                else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (!client.joined())
                        loginScreen->on_key_pressed(keyPressed->code);
                    else
                        chatScreen->on_key_pressed(keyPressed->code);
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

            else if (descriptor == Shared::CLIENT_REMOVE)
            {
                uint64_t id;
                packet >> id;
                client.removeJoiner(id);
            }
        }
    }
}

#include <iostream>
#include <thread>
#include <unordered_map>
#include "../include/app.hpp"
#include "../include/client.hpp"
#include "../include/joiner.hpp"
#include "../include/button.hpp"
#include "../include/font_manager.hpp"
#include "../include/theme.hpp"
#include "../include/login_screen.hpp"
#include "../include/chat_screen.hpp"
#include "../include/activatable.hpp"

App::App()
{
    settings.antialiasingLevel = 8;

   // window = std::make_unique<sf::RenderWindow>(sf::VideoMode({ 600, 600 }), "Chat", sf::Style::Default, settings);
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Chat", sf::Style::Default, settings);
    window->setVerticalSyncEnabled(true);

    sf::Vector2f win_size = { (float)window->getSize().x, (float)window->getSize().y };

    view.setSize(win_size.x, win_size.y);
    view.setCenter({ win_size.x / 2, win_size.y / 2 });
}

App::~App()
{
}

bool App::run()
{
    Client client("127.0.0.1", 3000);

    if (client.connect())
    {
        auto manager = FontManager();
        manager.load("fonts/UbuntuMono-Regular.ttf", "monospace");

        std::thread receiver([&client]() {
            sf::Packet packet;
            std::string descriptor;

            while (true)
            {
                if (client.receive(packet))
                {
                    packet >> descriptor;

                    if (descriptor == "ID")
                    {
                        std::uint64_t id;
                        packet >> id;
                        client.setId(id);

                        std::cout << "Your id is set by the server as " << id << std::endl;
                    }

                    else if (descriptor == "NEW-CLIENT")
                    {
                        std::string username;
                        std::uint64_t id;
                        sf::Color color;

                        packet >> id >> username >> color.r << color.g << color.b;
                        client.addJoiner({ username, id, color });

                        std::cout << "New client joined -> " << id << " : " << username << " : " << (uint32_t)color.r << std::endl;
                    }

                    else if (descriptor == "OTHER-CLIENTS")
                    {
                        std::uint64_t count;
                        std::string username;
                        std::uint64_t id;
                        sf::Color color;

                        packet >> count;

                        for (auto i = 0; i < count; i++)
                        {
                            packet >> id >> username >> color.r >> color.g >> color.b;
                            client.addJoiner({ username, id, color });

                            std::cout << "Already joined client info: " << id << " -> " << username << " : " << (uint32_t)color.r << std::endl;
                        }
                    }
                }
            }
        });

        std::unique_ptr<LoginScreen> loginScreen = std::make_unique<LoginScreen>(*window, client, *manager.get("monospace").value());
        std::unique_ptr<ChatScreen> chatScreen = nullptr;

        auto a = Activatable({ 70, 40 }, { 20, 20 }, sf::Color::White, sf::Color::Black, sf::Color::Black, "images/mic_icon.png");

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
                if (!chatScreen)
                    chatScreen = std::make_unique<ChatScreen>(*window, client, *manager.get("monospace").value());

                chatScreen->on_hover_items(*window);
                chatScreen->on_click_items(*window);
                chatScreen->draw_rt_items();
            }
            
            window->setView(view);
            window->clear(Theme::Primary);
            if (!client.joined()) window->draw(*loginScreen);
            else window->draw(*chatScreen);
            //window->draw(a);
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
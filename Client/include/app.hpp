#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "client.hpp"

class App
{
    public:
        App();
        ~App();
        bool run();
    private:
        void handleWindowSize();
        void receiver(Client& client);
    private:
        std::unique_ptr<sf::RenderWindow> window;
        sf::ContextSettings settings;
        sf::View view;
};

#endif
#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include <memory>

class App
{
    public:
        App();
        ~App();
        bool run();
    private:
        void handleWindowSize();
    private:
        std::unique_ptr<sf::RenderWindow> window;
        sf::ContextSettings settings;
        sf::View view;
};

#endif
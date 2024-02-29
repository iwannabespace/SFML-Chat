#ifndef ACTIVATABLE_HPP
#define ACTIVATABLE_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include "RoundRect.hpp"

class Activatable : public sf::Drawable
{
    public:
        Activatable(sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Color borderColor, sf::Color iconColor, const std::string& iconFile);
        ~Activatable();
        void on_hover(const sf::RenderWindow& window);
        void on_event_click(const sf::RenderWindow& window);
        void on_active();
        void setCallback(const std::function<void()>& _callback);
        void setPosition(sf::Vector2f position);
        void setSize(sf::Vector2f size);
        bool activated() const;
        bool deactivated();
        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        crd::RoundRect body;
        sf::Sprite icon;
        sf::Texture iconTexture;
        sf::Color outlineColor;
        std::function<void()> callback;
        bool _activated = false;
        bool _deactivated = false;
        bool callbackCalled = false;
        bool callbackSet = false;
        int32_t clickCount = 0;

};

#endif
#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include <SFML/Graphics.hpp>
#include <optional>

class Textbox : public sf::Drawable
{
    public:
        Textbox(const sf::String& placeholder, sf::Vector2f size, sf::Vector2f position, sf::Color color, 
            sf::Color hovered_color, sf::Color focused_color, sf::Color text_color, sf::Color placeholder_color, 
            sf::Font& font, float border_width, std::optional<size_t> maxCharacter, const std::string& def = "", bool numberOnly = false);
        ~Textbox();
        void on_input(uint32_t character);
        void on_shortcuts();
        void on_text_left();
        void on_text_right();
        void on_hover(const sf::RenderWindow& window);
        void on_click(const sf::RenderWindow& window);
        void draw_rt();
        void setPosition(sf::Vector2f position);
        void setSize(sf::Vector2f size);
        void clear();
        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;
        std::string value() const;
        bool active() const;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        bool is_number(const std::string& str) const;
    private:
        sf::RenderTexture renderer;
        sf::RectangleShape container;
        sf::RectangleShape cover;
        sf::Text text;
        sf::Text placeholder_text;
        sf::String string;
        sf::String placeholder_str;
        sf::Font& font;
        sf::Color color;
        sf::Color hovered_color;
        sf::Color focused_color;
        sf::Vector2f text_pos;
        size_t cursor_index;
        std::optional<size_t> maxCharacter;
        float oneCharWidth;
        float oneCharHeight;
        bool focused;
        bool selected;
        bool numberOnly = false;
};

#endif
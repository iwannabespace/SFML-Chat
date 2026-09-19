#include "../include/textbox.hpp"
#include "../include/functions.hpp"
#include <iostream>
#include <algorithm>

namespace
{
    sf::Vector2f getCharacterPosition(const sf::Text& text, std::size_t index)
    {
        const auto& glyphs = text.getShapedGlyphs();
        const auto glyph = std::find_if(glyphs.begin(), glyphs.end(), [index](const sf::Text::ShapedGlyph& item) {
            return item.cluster >= index;
        });

        if (glyph != glyphs.end())
            return text.getTransform().transformPoint(glyph->position);

        if (!glyphs.empty())
        {
            const auto& last = glyphs.back();
            return text.getTransform().transformPoint(last.position + sf::Vector2f{last.glyph.advance, 0.f});
        }

        return text.getPosition();
    }
}

Textbox::Textbox(const sf::String& placeholder, sf::Vector2f size, sf::Vector2f position, sf::Color color,
        sf::Color hovered_color, sf::Color focused_color, sf::Color text_color, sf::Color placeholder_color,
        sf::Font& font, float border_width, std::optional<size_t> maxCharacter, const std::string& def, bool numberOnly)
    : text(font), placeholder_text(font), placeholder_str(placeholder), font(font), color(color), hovered_color(hovered_color), focused_color(focused_color),
        cursor_index(0), maxCharacter(maxCharacter), focused(false), selected(false), numberOnly(numberOnly)
{
    if (!renderer.resize({static_cast<unsigned>(size.x), static_cast<unsigned>(size.y)}))
        std::cerr << "Renderer couldn'be created!" << std::endl;

    container.setSize(size);
    container.setPosition(position);
    container.setOutlineColor(sf::Color::Transparent);
    container.setOutlineThickness(border_width);

    text.setFont(font);
    text.setFillColor(text_color);
    text.setString("qwertyuıopğüasdfghjklşizxcvbnmöçQWERTYUIOPĞÜASDFGHJKLŞİZXCVBNMÖÇ");
    text.setCharacterSize(14);

    oneCharHeight = text.getGlobalBounds().size.y;

    //Functions::FitText(text, size);

    text_pos = {
        5,
        Functions::GetMiddle(text.getGlobalBounds().size.y, container.getSize().y, 0, text.getGlobalBounds().position.y)
    };

    text.setPosition(text_pos);
    text.setString("W");

    oneCharWidth = text.getGlobalBounds().size.x;

    cover.setSize({ 50, text.getGlobalBounds().size.y + 10});
    cover.setPosition({ text_pos.x, std::round(((size.y - cover.getSize().y) / 2))});
    cover.setFillColor(sf::Color::Transparent);
    text.setString(def + "_");

    placeholder_text.setFont(font);
    placeholder_text.setString(placeholder_str);
    placeholder_text.setCharacterSize(text.getCharacterSize());
    placeholder_text.setPosition(text_pos);
    placeholder_text.setFillColor(placeholder_color);

    string = def + "_";
    cursor_index = string.getSize() - 1;
}

Textbox::~Textbox()
{
}

void Textbox::on_input(uint32_t character)
{
    if (focused)
    {
        bool added = false;
        bool deleted = false;

        if (std::isprint(character) || character > 127)
        {
            if (((maxCharacter.has_value() && string.getSize() - 1 < maxCharacter.value()) || (!maxCharacter.has_value())) &&
                ((numberOnly && std::isdigit(character)) || !(numberOnly)))
            {
                string.insert(cursor_index, static_cast<char32_t>(character));

                cursor_index++;
                added = true;

                if (selected)
                    selected = false;
            }
        }

        else if (character == 8)
        {
            if (selected)
            {
                string = "_";
                text.setString(string);
                cursor_index = 0;
                selected = false;
            }

            else
            {
                if (cursor_index > 0)
                {
                    string.erase(--cursor_index);
                    deleted = true;
                }
            }
        }

        text.setString(string);

        if (text.getGlobalBounds().size.x >= container.getSize().x)
        {
            if (added)
            {
                if (getCharacterPosition(text, cursor_index).x + oneCharWidth >= container.getSize().x)
                    text.move({ -oneCharWidth, 0 });

                added = false;
            }

            if (deleted)
            {
                if (text.getGlobalBounds().size.x >= container.getSize().x)
                    text.move({ oneCharWidth, 0});

                if (text.getPosition().x > text_pos.x)
                    text.setPosition({ text_pos.x, text.getPosition().y });

                deleted = false;
            }
        }

        else
            text.setPosition(text_pos);
    }
}

void Textbox::on_shortcuts()
{
    if (focused)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace))
        {
            if (string != "_")
            {
                string = "_";
                text.setString(string);
                text.setPosition(text_pos);
                cursor_index = 0;

                if (selected)
                    selected = false;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            if (!selected && string.getSize() > 1)
            {
                cover.setFillColor(sf::Color(72, 98, 136));
                cover.setPosition({ text.getPosition().x, cover.getPosition().y });
                cover.setSize({ text.getGlobalBounds().size.x + text.getLocalBounds().position.x, cover.getSize().y });
                selected = true;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
        {
            if (selected)
            {
                sf::String copy = string;
                copy.erase(cursor_index);
                sf::Clipboard::setString(copy);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V))
        {
            std::string to_copy = sf::Clipboard::getString().toAnsiString();

            if (selected)
            {
                string = "_";
                text.setString(string);
                cursor_index = 0;
                selected = false;
            }

            string.insert(cursor_index, to_copy);
            string.erase(cursor_index + to_copy.length());
            string.insert(cursor_index, "_");
            text.setString(string);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
        {
            if (selected)
            {
                string.erase(cursor_index);
                sf::Clipboard::setString(string);
                string = "_";
                text.setString(string);
                text.setPosition(text_pos);
                cursor_index = 0;
                selected = false;
            }
        }
    }
}

void Textbox::on_text_left()
{
    if (focused)
    {
        if (selected)
        {
            selected = false;
            // string.erase(cursor_index);
            // string.insert(0, "_");
            // text.setString(string);
            // text.setPosition(text_pos);
            // cursor_index = 0;
            // selected = false;
        }

        else if (cursor_index > 0)
        {
            string.erase(cursor_index--);
            string.insert(cursor_index, "_");
            text.setString(string);

            if (getCharacterPosition(text, cursor_index).x < 0)
                text.move({ oneCharWidth, 0});
        }
    }
}

void Textbox::on_text_right()
{
    if (focused)
    {
        if (selected)
        {
            selected = false;
            // string.erase(cursor_index);
            // string += "_";
            // cursor_index = string.getSize() - 1;
            // text.setString(string);
            // text.setPosition({ container.getSize().x - (text.getGlobalBounds().size.x), text_pos.y });
            // selected = false;
        }

        else if (cursor_index < string.getSize() - 1)
        {
            string.erase(cursor_index++);
            string.insert(cursor_index, "_");
            text.setString(string);

            if (getCharacterPosition(text, cursor_index).x + oneCharWidth >= container.getSize().x)
                text.move({ -oneCharWidth, 0});
        }
    }
}

void Textbox::on_hover(const sf::RenderWindow& window)
{
    if (!focused)
    {
        if (container.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
            container.setOutlineColor(hovered_color);

        else
            container.setOutlineColor(sf::Color::Transparent);
    }
}

void Textbox::on_click(const sf::RenderWindow& window)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        bool contains = container.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)));

        if (contains && !focused)
        {
            container.setOutlineColor(focused_color);
            focused = true;
        }

        if (!contains)
        {
            container.setOutlineColor(sf::Color::Transparent);
            focused = false;
        }
    }
}

void Textbox::draw_rt()
{
    renderer.clear(color);

    if (selected)
        renderer.draw(cover);

    if (!focused && string.getSize() == 1)
        renderer.draw(placeholder_text);

    if (focused || string.getSize() > 1)
        renderer.draw(text);

    renderer.display();
    container.setTexture(&renderer.getTexture(), true);
}

void Textbox::setPosition(sf::Vector2f position)
{
    container.setPosition(position);
}

void Textbox::setSize(sf::Vector2f size)
{
    container.setSize(size);
    if (!renderer.resize({static_cast<unsigned>(size.x), static_cast<unsigned>(size.y)}))
        std::cerr << "Renderer couldn't be resized!" << std::endl;

    text_pos = {
        5,
        Functions::GetMiddle(oneCharHeight, container.getSize().y, 0, 0)
    };

    placeholder_text.setPosition(text_pos);

    cover.setPosition({ text_pos.x, std::round(((size.y - cover.getSize().y) / 2))});
    text.setPosition(text_pos);
}

void Textbox::clear()
{
    string = "_";
    text.setString(string);
    text.setPosition(text_pos);
    cursor_index = 0;
}

sf::Vector2f Textbox::getPosition() const
{
    return container.getPosition();
}

sf::Vector2f Textbox::getSize() const
{
    return container.getSize();
}

std::string Textbox::value() const
{
    // This is what I've written when I first worked on the project
    // and it was working fine on macos but now for some reason it crashes on my linux machine.
    // I use arch btw.

    // std::string copy = text.getString().toAnsiString();

    // copy.erase(copy.begin() + cursor_index);

    // return copy;



    // This is the fix of AI.
    // It disgusts me...

    sf::String copy = string;

    if (cursor_index < copy.getSize())
        copy.erase(cursor_index);

    const sf::U8String utf8 = copy.toUtf8();

    return std::string(utf8.begin(), utf8.end());
}

bool Textbox::active() const
{
    return focused;
}

bool Textbox::is_number(const std::string& str) const
{
    return std::all_of(str.begin(), str.end(), [](char c) {
        return std::isdigit(c);
    });
}

void Textbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(container, states);
}

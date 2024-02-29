#ifndef IMAGE_VIEWER_HPP
#define IMAGE_VIEWER_HPP

#include <SFML/Graphics.hpp>

class ImageViewer : public sf::Drawable
{
    public:
        ImageViewer(const std::string& _filename);
        ImageViewer(const ImageViewer& rhs);
        ~ImageViewer();
        void on_hover(const sf::RenderWindow& window);
        void on_click(const sf::RenderWindow& window);
        void setSize(sf::Vector2f size);
        void setPosition(sf::Vector2f position);
        void setOutlineColor(sf::Color color);
        void setOutlineThickness(float thickness);
        sf::Vector2f getSize() const;
        sf::Vector2f getPosition() const;
        const std::string& getFilename() const;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::Image image;
        sf::RectangleShape imageContainer;
        sf::Texture imageTexture;
        sf::Color outlineColor;
        sf::Vector2f defaultSize;
        sf::Vector2f defaultPosition;
        std::string filename;
        bool _opened = false;
};

#endif
#include "../include/image_viewer.hpp"
#include "../include/functions.hpp"
#include <iostream>

ImageViewer::ImageViewer(const std::string& _filename)
{
    this->filename = _filename;

    if (!image.loadFromFile(filename))
        throw "Image couldn't be loaded";

    if (!imageTexture.loadFromImage(image))
        throw "Texture couldn't be loaded";
    
    imageContainer.setTexture(&imageTexture);
    imageContainer.setOutlineColor(sf::Color::Transparent);
}

ImageViewer::ImageViewer(const ImageViewer& rhs)
{
    this->filename = rhs.filename;

    if (!image.loadFromFile(filename))
        throw "Image couldn't be loaded";

    if (!imageTexture.loadFromImage(image))
        throw "Texture couldn't be loaded";
    
    imageContainer.setTexture(&imageTexture);
    imageContainer.setOutlineColor(sf::Color::Transparent);
}

ImageViewer::~ImageViewer()
{
}

void ImageViewer::on_hover(const sf::RenderWindow& window)
{
    if (!_opened)
    {
         if (imageContainer.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
            imageContainer.setOutlineColor(outlineColor);
        
        else
            imageContainer.setOutlineColor(sf::Color::Transparent);
    }
}

void ImageViewer::on_click(const sf::RenderWindow& window)
{
    if (imageContainer.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
    {
        if (!_opened)
        {
            float winw = static_cast<float>(window.getSize().x);
            float winh = static_cast<float>(window.getSize().y);
            float neww = defaultSize.x * 4;
            float newh = defaultSize.y * 4;

            imageContainer.setSize({ neww, newh });
            imageContainer.setOutlineColor(sf::Color::Transparent);
            imageContainer.setPosition({
                Functions::GetMiddle(neww, winw, 0, 0),
                Functions::GetMiddle(newh, winh, 0, 0),
            });
            _opened = true;
        }
    }

    else
    {
        if (_opened)
        {    
            imageContainer.setSize(defaultSize);
            imageContainer.setPosition(defaultPosition);
            _opened = false;
        }
    }
}

void ImageViewer::setSize(sf::Vector2f size)
{
    defaultSize = size;
    imageContainer.setSize(size);
}

void ImageViewer::setPosition(sf::Vector2f position)
{
    defaultPosition = position;
    imageContainer.setPosition(position);
}

void ImageViewer::setOutlineColor(sf::Color color)
{
    outlineColor = color;
}

void ImageViewer::setOutlineThickness(float thickness)
{
    imageContainer.setOutlineThickness(thickness);
}

sf::Vector2f ImageViewer::getSize() const
{
    return imageContainer.getSize();
}

sf::Vector2f ImageViewer::getPosition() const
{
    return imageContainer.getPosition();
}

const std::string& ImageViewer::getFilename() const
{
    return filename;
}

void ImageViewer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(imageContainer, states);
}
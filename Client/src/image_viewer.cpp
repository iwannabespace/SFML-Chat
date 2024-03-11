#include "../include/image_viewer.hpp"
#include "../include/functions.hpp"
#include "../include/theme.hpp"
#include <iostream>

ImageViewer::ImageViewer(const std::string& _filename, sf::Font& font)
    : font(font), options(font, 200)
{
    this->filename = _filename;

    if (!image.loadFromFile(filename))
        throw "Image couldn't be loaded";

    if (!imageTexture.loadFromImage(image))
        throw "Texture couldn't be loaded";
    
    imageContainer.setTexture(&imageTexture);
    imageContainer.setOutlineColor(sf::Color::Transparent);

    options.setFillColor({ 105, 105, 105 });
    options.setTextColor(Theme::Text);

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        options.addOption("Reveal in file explorer", [this]() {
            system(std::string("explorer.exe /select " + this->filename).c_str());
        });
    #elif __APPLE__
        options.addOption("Reveal in finder", [this]() {
            system(std::string("open -R " + this->filename).c_str());
        });
    #endif
}

ImageViewer::ImageViewer(const ImageViewer& rhs)
    : font(rhs.font), options(rhs.font, 200)
{
    this->filename = rhs.filename;

    if (!image.loadFromFile(filename))
        throw "Image couldn't be loaded";

    if (!imageTexture.loadFromImage(image))
        throw "Texture couldn't be loaded";
    
    imageContainer.setTexture(&imageTexture);
    imageContainer.setOutlineColor(sf::Color::Transparent);

    options.setFillColor({ 105, 105, 105 });
    options.setTextColor(Theme::Text);

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        options.addOption("Reveal in file explorer", [this]() {
            system(std::string("explorer.exe /select " + this->filename).c_str());
        });
    #elif __APPLE__
        options.addOption("Reveal in finder", [this]() {
            system(std::string("open -R " + this->filename).c_str());
        });
    #endif
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
    sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

    if (imageContainer.getGlobalBounds().contains(mousePosition) && !options.getGlobalBounds().contains(mousePosition))
    {
        if (!_opened)
        {
            float winw = static_cast<float>(window.getSize().x);
            float winh = static_cast<float>(window.getSize().y);
            float neww = defaultSize.x * 2;
            float newh = defaultSize.y * 2;

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

    if (!options.getGlobalBounds().contains(mousePosition))
        _showOptions = false;
}

void ImageViewer::on_hover_items(const sf::RenderWindow& window)
{
    if (_showOptions)
        options.on_hover(window);
}

void ImageViewer::on_event_click_items(const sf::RenderWindow& window)
{
    if (_showOptions)
    {   
        options.on_click(window);
        _showOptions = false;
    }
}

void ImageViewer::on_right_click(const sf::RenderWindow& window)
{
    sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

    if (imageContainer.getGlobalBounds().contains(mousePosition))
    {
        if (!_opened)
        {
            options.setPosition(mousePosition);
            options.place();
            _showOptions = true;
        }
    }
}

void ImageViewer::on_window_resize(const sf::RenderWindow& window)
{
    if (_opened)
    {
        float winw = static_cast<float>(window.getSize().x);
        float winh = static_cast<float>(window.getSize().y);
        float neww = defaultSize.x * 2;
        float newh = defaultSize.y * 2;

        imageContainer.setSize({ neww, newh });
        imageContainer.setOutlineColor(sf::Color::Transparent);
        imageContainer.setPosition({
            Functions::GetMiddle(neww, winw, 0, 0),
            Functions::GetMiddle(newh, winh, 0, 0),
        });
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
    options.setHoverColor(color);
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

ImageViewer& ImageViewer::operator=(const ImageViewer& rhs)
{
    this->font = rhs.font;
    this->filename = rhs.filename;

    if (!image.loadFromFile(filename))
        throw "Image couldn't be loaded";

    if (!imageTexture.loadFromImage(image))
        throw "Texture couldn't be loaded";
    
    imageContainer.setTexture(&imageTexture);
    imageContainer.setOutlineColor(sf::Color::Transparent);

    options.setFont(font);
    options.setWidth(200);
    options.setFillColor({ 105, 105, 105 });
    options.setTextColor(Theme::Text);

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        options.addOption("Reveal in file explorer", [this]() {
            system(std::string("explorer.exe /select " + this->filename).c_str());
        });
    #elif __APPLE__
        options.addOption("Reveal in finder", [this]() {
            system(std::string("open -R " + this->filename).c_str());
            std::cout << "Sa" << std::endl;
        });
    #endif

    return *this;
}

void ImageViewer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(imageContainer, states);

    if (_showOptions)
        target.draw(options, states);
}
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>

struct Client
{
    std::string username;
    sf::TcpSocket* socket;
    std::uint64_t id;
    sf::Color color;
};

#endif
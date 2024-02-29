#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>

struct Client
{
    std::string username;
    sf::TcpSocket* socket;
    sf::Uint64 id;
    sf::Color color;
    bool joined = false;
};

#endif
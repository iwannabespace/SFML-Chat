#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <unordered_map>
#include "joiner.hpp"

class Client
{
    public:
        Client(const sf::IpAddress& host, unsigned short port);
        ~Client();
        bool connect();
        bool join(const std::string& username, sf::Color color);
        bool send(sf::Packet& packet);
        bool receive(sf::Packet& packet);
        bool connected() const;
        bool joined() const;
        void setId(std::uint64_t id);
        void addJoiner(const Joiner& joiner);
        const std::unordered_map<std::uint64_t, Joiner>& getJoiners() const;
    private:
        sf::TcpSocket socket;
        sf::IpAddress host;
        unsigned short port;
        bool _connected = false;
        bool _joined = false;
        std::string username;
        std::uint64_t id;
        sf::Color color;
        std::unordered_map<std::uint64_t, Joiner> joiners;
};

#endif
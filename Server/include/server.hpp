#ifndef SERVER_HPP
#define SERVER_HPP

#include <SFML/Network.hpp>
#include <unordered_map>
#include "client.hpp"

class Server
{
    public:
        Server(unsigned short port);
        ~Server();
        bool listen();
        void run();
        void sendOnly(sf::Uint64 id, sf::Packet& packet);
        void sendExcept(sf::Uint64 id, sf::Packet& packet);
        void sendClientId(sf::Uint64 id);
        void sendAllNewClient(sf::Uint64 id);
        void sendOtherClientInfos(sf::Uint64 id);
    private:
        unsigned short port;
        sf::TcpListener listener;
        sf::SocketSelector selector;
        std::unordered_map<sf::Uint64, Client> clients;
        sf::Uint64 joinedCount = 0;
        size_t received;
};

#endif
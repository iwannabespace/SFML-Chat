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
        void sendOnly(std::uint64_t id, sf::Packet& packet);
        void sendExcept(std::uint64_t id, sf::Packet& packet);
        void sendClientId(std::uint64_t id);
        void sendAllNewClient(std::uint64_t id);
        void sendOtherClientInfos(std::uint64_t id);
    private:
        unsigned short port;
        sf::TcpListener listener;
        sf::SocketSelector selector;
        std::unordered_map<std::uint64_t, Client> clients;
        size_t received;
        std::uint64_t joinedCount = 0;
};

#endif
#include "../include/server.hpp"
#include "../../Shared/shared.hpp"
#include <SFML/Graphics.hpp>

Server::Server(unsigned short port)
    : port(port)
{
}

Server::~Server()
{
}

bool Server::listen()
{
    if (listener.listen(port) != sf::Socket::Status::Done)
        return false;

    return true;
}

void Server::run()
{
    std::uint64_t id = 0;

    if (this->listen())
    {
        selector.add(listener);

        while (true)
        {
            if (selector.wait())
            {
                if (selector.isReady(listener))
                {
                    sf::TcpSocket* socket = new sf::TcpSocket;

                    if (listener.accept(*socket) == sf::Socket::Status::Done)
                    {
                        clients[id] = { "", socket, id, {} };
                        selector.add(*socket);
                        id++;
                    }

                    else
                        delete socket;
                }

                else
                {
                    for (auto it = clients.begin(); it != clients.end();)
                    {
                        auto& client = it->second;
                        auto clientSocket = client.socket;
                        auto clientId = client.id;
                        auto clientUsername = client.username;
                        bool removed = false;

                        if (selector.isReady(*clientSocket))
                        {
                            sf::Packet recvPacket;
                            sf::Packet sendPacket;

                            sf::Socket::Status status = clientSocket->receive(recvPacket);

                            if (status == sf::Socket::Status::Done)
                            {
                                uint8_t descriptor;
                                recvPacket >> descriptor;

                                if (descriptor == Shared::SET_USERNAME)
                                {
                                    std::string username;
                                    sf::Color color;

                                    recvPacket >> username >> color.r >> color.g >> color.b;
                                    client.username = username;
                                    client.color = color;
                                    client.joined = true;
                                    joinedCount++;

                                    sendClientId(clientId);
                                    sendAllNewClient(clientId);
                                    sendOtherClientInfos(clientId);
                                }

                                else if (descriptor == Shared::MESSAGE)
                                {
                                    std::uint64_t incId;
                                    std::uint8_t type;
                                    std::string data;
                                    std::string extension;

                                    recvPacket >> incId >> type >> data >> extension;
                                    sendPacket << Shared::MESSAGE << incId << type << data << extension;
                                    sendExcept(clientId, sendPacket);
                                }
                            }

                            else if (status == sf::Socket::Status::Disconnected)
                            {
                                if (client.joined)
                                    sendClientRemoved(clientId);

                                selector.remove(*clientSocket);
                                delete clientSocket;
                                it = clients.erase(it);
                                joinedCount--;
                                removed = true;
                            }
                        }

                        if (!removed)
                            ++it;
                    }
                }
            }
        }
    }
}

void Server::sendOnly(std::uint64_t id, sf::Packet& packet)
{
    auto& client = clients[id];

    if (client.joined)
        (void)client.socket->send(packet);
}

void Server::sendExcept(std::uint64_t id, sf::Packet& packet)
{
    if (joinedCount > 1)
        for (auto [clientId, client] : clients)
            if (clientId != id && client.joined)
                (void)client.socket->send(packet);
}

void Server::sendClientId(std::uint64_t id)
{
    sf::Packet packet;

    packet << Shared::ID << id;

    sendOnly(id, packet);
}

void Server::sendClientRemoved(std::uint64_t id)
{
    sf::Packet packet;

    packet << Shared::CLIENT_REMOVE << id;

    sendExcept(id, packet);
}

void Server::sendAllNewClient(std::uint64_t id)
{
    sf::Packet packet;

    auto client = clients[id];
    packet << Shared::NEW_CLIENT << id << client.username << client.color.r << client.color.g << client.color.b;

    sendExcept(id, packet);
}

void Server::sendOtherClientInfos(std::uint64_t id)
{
    if (joinedCount > 1)
    {
        sf::Packet packet;

        packet << Shared::OTHER_CLIENTS << joinedCount - 1;

        for (auto [clientId, client] : clients)
            if (clientId != id && client.joined)
                packet << clientId << client.username
                    << client.color.r << client.color.g << client.color.b;

        (void)clients[id].socket->send(packet);
    }
}

#include "../include/server.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <thread>

Server::Server(unsigned short port)
    : port(port)
{
}

Server::~Server()
{
}

bool Server::listen()
{
    if (listener.listen(port) != sf::Socket::Done)
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
                    
                    if (listener.accept(*socket) == sf::Socket::Done)
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
                    for (auto it = clients.begin(), next_it = it; it != clients.end(); it = next_it)
                    {
                        auto& client = it->second;
                        auto clientSocket = client.socket;
                        auto clientId = client.id;
                        auto clientUsername = client.username;

                        if (selector.isReady(*clientSocket))
                        {
                            sf::Packet recvPacket;
                            sf::Packet sendPacket;

                            sf::Socket::Status status = clientSocket->receive(recvPacket);

                            if (status == sf::Socket::Done)
                            {
                                std::string descriptor;
                                recvPacket >> descriptor;

                                if (descriptor == "SET-USERNAME")
                                {
                                    if (clientUsername.empty())
                                    {
                                        std::string username;
                                        sf::Color color;

                                        recvPacket >> username >> color.r >> color.g >> color.b;
                                        client.username = username;
                                        client.color = color;

                                        joinedCount++;

                                        sendClientId(clientId);
                                        sendAllNewClient(clientId);
                                        sendOtherClientInfos(clientId);
                                    }
                                }
                            }

                            else if (status == sf::Socket::Disconnected)
                            {
                                selector.remove(*clientSocket);
                                delete clientSocket;
                                clients.erase(it);
                            }
                        }

                        ++next_it;
                    }
                }
            }
        }
    }
}

void Server::sendOnly(std::uint64_t id, sf::Packet& packet)
{
    clients[id].socket->send(packet);
}

void Server::sendExcept(std::uint64_t id, sf::Packet& packet)
{
    for (auto [clientId, client] : clients)
        if (clientId != id)
            client.socket->send(packet);
}

void Server::sendClientId(std::uint64_t id)
{
    sf::Packet packet;

    packet << "ID" << id;

    sendOnly(id, packet); 
}

void Server::sendAllNewClient(std::uint64_t id)
{
    sf::Packet packet;

    auto client = clients[id];

    packet << "NEW-CLIENT" << id << client.username << 
        client.color.r << client.color.g << client.color.b;
    
    sendExcept(id, packet);
}

void Server::sendOtherClientInfos(std::uint64_t id)
{
    if (joinedCount > 1)
    {
        sf::Packet packet;
        
        packet << "OTHER-CLIENTS" << joinedCount - 1;

        for (auto [clientId, client] : clients)
            if (clientId != id && !client.username.empty())
                packet << clientId << client.username
                    << client.color.r << client.color.g << client.color.b;

        clients[id].socket->send(packet);
    }
}     
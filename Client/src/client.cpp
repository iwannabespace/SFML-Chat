#include "../include/client.hpp"
#include "../../Shared/shared.hpp"
#include <thread>
#include <iostream>

Client::Client(const sf::IpAddress& host, unsigned short port)
    : host(host), port(port)
{
}

Client::~Client()
{
    socket.disconnect();
}

bool Client::connect()
{
    return (_connected = (socket.connect(host, port) == sf::Socket::Done));
}

bool Client::join(const std::string& _username, sf::Color _color)
{
    sf::Packet packet;

    packet << Shared::SET_USERNAME << _username << _color.r << _color.g << _color.b;

    if (send(packet))
    {
        this->username = _username;
        this->color = _color;
        this->_joined = true;
        return true;
    }

    return false;
}

bool Client::send(sf::Packet& packet)
{
    sf::Socket::Status status = socket.send(packet);

    if (status == sf::Socket::Status::Disconnected)
    {
        socket.disconnect();
        _connected = false;
    }

    return status == sf::Socket::Status::Done; 
}

bool Client::receive(sf::Packet& packet)
{
    sf::Socket::Status status = socket.receive(packet);

    if (status == sf::Socket::Status::Disconnected)
    {
        socket.disconnect();
        _connected = false;
    }

    return status == sf::Socket::Status::Done;
}

bool Client::sendMessage(uint8_t type, const std::string& data, const std::string& extension)
{
    sf::Packet packet;
    sf::Socket::Status status;

    packet << Shared::MESSAGE << id << type << data << extension;
    status = socket.send(packet);

    return status == sf::Socket::Status::Done;
}

bool Client::connected() const
{
    return _connected;
}

bool Client::joined() const
{
    return _joined;
}

void Client::setId(std::uint64_t _id)
{
    this->id = _id;
}

void Client::newJoiner(const Joiner& joiner)
{
    joiners[joiner.id] = joiner;

    if (joinerCallback) 
        joinerCallback(joiner);
    
    else {
        std::cout << "For joiner: " << joiner.username << " callback is not yet set!" << std::endl;
    }
}

void Client::newMessage(const Message& message)
{
    if (messageCallback)
        messageCallback(message);
}

const std::unordered_map<uint64_t, Joiner>& Client::getJoiners() const
{
    return joiners;
}

const std::string& Client::getUsername() const
{
    return username;
}

uint64_t Client::getId() const
{
    return id;
}

sf::Color Client::getColor() const
{
    return color;
}

bool Client::isMe(uint64_t _id) const 
{
    return id == _id;
}

void Client::setOnJoinerJoinedCallback(const OnJoinerJoinedCallback& _callback)
{
    joinerCallback = _callback;
}

void Client::setOnMessageReceivedCallback(const OnMessageReceivedCallback& _callback)
{
    messageCallback = _callback;
}
#include "../include/client.hpp"
#include <thread>

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

bool Client::join(const std::string& username, sf::Color color)
{
    sf::Packet packet;

    packet << "SET-USERNAME" << username << color.r << color.g << color.b;

    if (send(packet))
    {
        this->username = username;
        this->color = color;
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

bool Client::connected() const
{
    return _connected;
}

bool Client::joined() const
{
    return _joined;
}

void Client::setId(std::uint64_t id)
{
    this->id = id;
}

void Client::addJoiner(const Joiner& joiner)
{
    joiners[joiner.id] = joiner;
}

const std::unordered_map<std::uint64_t, Joiner>& Client::getJoiners() const
{
    return joiners;
}
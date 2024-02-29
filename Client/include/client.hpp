#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <unordered_map>
#include "joiner.hpp"
#include "message.hpp"

class Client
{
    public:
        using OnJoinerJoinedCallback = std::function<void(const Joiner& joiner)>;
        using OnMessageReceivedCallback = std::function<void(const Message& message)>;
    public:
        Client(const sf::IpAddress& host, unsigned short port);
        ~Client();
        bool connect();
        bool join(const std::string& _username, sf::Color _color);
        bool send(sf::Packet& packet);
        bool receive(sf::Packet& packet);
        bool sendMessage(uint8_t type, const std::string& data, const std::string& extension = "");
        bool connected() const;
        bool joined() const;
        void setId(std::uint64_t _id);
        void newJoiner(const Joiner& joiner);
        void newMessage(const Message& message);
        const std::unordered_map<uint64_t, Joiner>& getJoiners() const;
        const std::string& getUsername() const;
        uint64_t getId() const;
        sf::Color getColor() const;
        bool isMe(uint64_t _id) const;
        void setOnJoinerJoinedCallback(const OnJoinerJoinedCallback& _callback);
        void setOnMessageReceivedCallback(const OnMessageReceivedCallback& _callback);
    private:
        sf::TcpSocket socket;
        sf::IpAddress host;
        unsigned short port;
        bool _connected = false;
        bool _joined = false;
        std::string username;
        sf::Color color;
        uint64_t id;
        std::unordered_map<uint64_t, Joiner> joiners;
        OnJoinerJoinedCallback joinerCallback;
        OnMessageReceivedCallback messageCallback;
};

#endif
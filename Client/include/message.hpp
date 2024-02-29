#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include "joiner.hpp"

enum class MessageType
{
    Text,
    File,
    Sound,
};

struct Message
{
    std::uint64_t senderId;
    Joiner joiner;
    std::string data;
    MessageType type;
};

#endif
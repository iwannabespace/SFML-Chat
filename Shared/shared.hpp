#ifndef SHARED_HPP
#define SHARED_HPP

#include <cstdint>

namespace Shared
{
    const std::uint8_t ID = 0;
    const std::uint8_t NEW_CLIENT = 1;
    const std::uint8_t OTHER_CLIENTS = 2;
    const std::uint8_t MESSAGE = 3;
    const std::uint8_t SET_USERNAME = 4;
    const std::uint8_t FILE = 5;
    const std::uint8_t SOUND = 6;
    const std::uint8_t TEXT = 7;
    const std::uint8_t CLIENT_REMOVE = 8;
}

#endif

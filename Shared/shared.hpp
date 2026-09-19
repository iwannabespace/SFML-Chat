#ifndef SHARED_HPP
#define SHARED_HPP

#include <cstdint>

namespace Shared
{
    inline constexpr std::uint8_t ID = 0;
    inline constexpr std::uint8_t NEW_CLIENT = 1;
    inline constexpr std::uint8_t OTHER_CLIENTS = 2;
    inline constexpr std::uint8_t MESSAGE = 3;
    inline constexpr std::uint8_t SET_USERNAME = 4;
    inline constexpr std::uint8_t FILE = 5;
    inline constexpr std::uint8_t SOUND = 6;
    inline constexpr std::uint8_t TEXT = 7;
    inline constexpr std::uint8_t CLIENT_REMOVE = 8;
}

#endif

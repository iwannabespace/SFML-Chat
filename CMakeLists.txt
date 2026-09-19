cmake_minimum_required(VERSION 3.28)

project(SFMLChat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include(FetchContent)

# Sisteminizdeki Mbed TLS yerine SFML'nin uyumlu
# bağımlılıkları indirmesini sağlar.
set(SFML_USE_SYSTEM_DEPS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(SFML)

find_package(Threads REQUIRED)

# Client kaynakları
file(GLOB CLIENT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Client/src/*.cpp"
)

add_executable(client
    Client/main.cpp
    ${CLIENT_SOURCES}
)

target_include_directories(client PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Client/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Shared"
)

target_link_libraries(client PRIVATE
    SFML::Graphics
    SFML::Audio
    SFML::Network
    Threads::Threads
)

# Server kaynakları
file(GLOB SERVER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Server/src/*.cpp"
)

add_executable(server
    Server/main.cpp
    ${SERVER_SOURCES}
)

target_include_directories(server PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Server/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Shared"
)

target_link_libraries(server PRIVATE
    SFML::Graphics
    SFML::Network
    Threads::Threads
)

add_custom_command(
    TARGET client POST_BUILD

    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Client/assets"
        "$<TARGET_FILE_DIR:client>/assets"

    VERBATIM
)
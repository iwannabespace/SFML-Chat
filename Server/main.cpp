#include "include/server.hpp"

int main()
{
    Server server(9472);

    server.run();

    return 0;
}
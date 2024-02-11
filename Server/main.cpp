#include "include/server.hpp"

int main()
{
    Server server(3000);

    server.run();

    return 0;
}
#include "src/Server.hpp"

int main(int ac, char **av)
{
    (void)ac;
    
    Server *server = new Server(std::atoi(av[1]));
    server->serverLoop();
    return (0);
}
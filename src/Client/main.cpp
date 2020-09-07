#include "src/Client.hpp"

int main(int ac, char **av)
{
    (void)ac;
    Client *client = new Client(std::atoi(av[1]), std::string(av[2]), std::stoi(av[3]));
    client->clientLoop();
    return (0);
}
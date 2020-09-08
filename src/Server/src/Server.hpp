#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "./Entity/Entity.hpp"
#include "./World/World.hpp"

#include <unistd.h>
#include <random>

class Server
{
    private:

        int maxSockets;
        int playerNumber;

        std::string serverIp;
        int serverPort;

        ThreadPool threadPool;

        RakNet::SocketDescriptor socketDescriptor;
        RakNet::RakPeerInterface *mainPeerInterface;

        World *world;

        std::vector<World*> pastWorlds;

    public:
        Server(int port);
        ~Server();

        long getSinceEpochMs();
        void saveWorld();

        //SERVER METHODS
        void createServer();
        void startServer();
        void shutdownServer();
        void getPackets();
        void serverLoop();
        void startFrame();
        void endFrame();

        std::chrono::high_resolution_clock::time_point frameClock;

        unsigned char GetPacketIdentifier(RakNet::Packet *packet);
        void notifyAll(Player *player, SERVER_MESSAGES specifier);

        //PLAYER METHODS
        void addPlayerToWorld(RakNet::RakNetGUID packetGuid);
        void deletePlayerFromWorld(Player *player);
        void sendPlayer(Player *player, RakNet::RakNetGUID receiverGuid, SERVER_MESSAGES specifier);
        void sendGameinformations(Player *player);

        //PLAYER MOVEMENT METHODS

        void playerMove(Player *player, RakNet::Packet *packet);
        void makePlayersMove();
        void stopPlayerMoving(Player *player);
};

#endif  
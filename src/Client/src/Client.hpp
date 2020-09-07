#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include "./World/World.hpp"
#include "./Entity/Entity.hpp"

#include "../../../common/ThreadPool.hpp"
#include "../../../common/RaknetIncludes.hpp"

#include <iomanip>
#include <ctime>

class Client
{
    private:

        //COMMON
        ThreadPool threadPool;

        //CLIENT VARIABLES

        int maxSockets;
        int playerNumber;
        int serverPort;
        int clientPort;

        std::string serverIp;
        RakNet::RakNetGUID serverGuid;

        RakNet::SocketDescriptor socketDescriptor;
        RakNet::RakPeerInterface *mainPeerInterface;

        //GRAPHIC VARIABLES

        bool hasFocus;
        World *world;

        sf::RenderWindow *window;
        sf::View *view;
        sf::Event event;

        std::chrono::high_resolution_clock::time_point frameClock;

    public:
        
        Client(int _clientPort, std::string _serverIp, int _serverPort);
        ~Client();

        void setServerGuid(RakNet::RakNetGUID _serverGuid){serverGuid = _serverGuid;}
        long getSinceEpochMs();

        //PLAYER METHODS
        void createClient();
    
        void addPlayerToWorld(RakNet::Packet *p);
        void deletePlayerFromWorld(RakNet::Packet *p);
        void updatePlayerFromWorld(RakNet::Packet *p);

        void loadWorld(RakNet::Packet *p);
        void listPlayer();

        //CLIENT METHODS
        void startClient();
        void shutdownClient();
        void getPackets();
        void clientLoop();
        void ping();
        unsigned char GetPacketIdentifier(RakNet::Packet *packet);

        //GRAPHIC METHODS

        int createWindow();
        void setCamera(Player *player);
        void getEvent();

        void startFrame();
        void endFrame();

        bool getFocusState();
        void setFocus(bool _hasFocus);
};

#endif  
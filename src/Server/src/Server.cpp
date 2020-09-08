#include "./Server.hpp"

Server::~Server()
{
    delete world;
}

Server::Server(int _serverPort)
{
    threadPool.createPool(1);
    maxSockets = MAX_PLAYER_CONNEXION;
    serverPort = _serverPort;
    playerNumber = 0;
    world = new World(10);
}

long Server::getSinceEpochMs()
{
    std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();
    long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return (timestamp);
}

void Server::sendGameinformations(Player *player)
{
    std::vector<Player*> playerList = world->getPlayerList();
    int playerNumber = (int)playerList.size()-1;

    RakNet::BitStream out;
    out.Write((unsigned char) ID_GET_INFORMATIONS);
    out.Write(playerNumber);

    for (int i = 0; i < (int)playerList.size(); i++){

        if (playerList[i]->getRaknetGuid() != player->getRaknetGuid()){

            out.Write((float)playerList[i]->getPos().first);
            out.Write((float)playerList[i]->getPos().second);
            out.Write((int)playerList[i]->getDirection());
            out.Write((int)playerList[i]->getSpeed());
            out.Write(playerList[i]->getRaknetGuid());
        }
    }

    std::vector<Chunk*> worldChunks = world->getWorld();

    out.Write((int)worldChunks.size());

    for (int k = 0; k < (int)worldChunks.size(); k++){

        Chunk *chunk = worldChunks[k];
        out.Write((int)chunk->getHeight());
        out.Write((int)chunk->getWidth());

        for (int i = 0; i < chunk->getHeight(); i++){
            for (int j = 0; j < chunk->getWidth(); j++){

                Block *block = chunk->getBlock(i, j);

                out.Write(block->getBlockId());
                out.Write(block->getPos().first);
                out.Write(block->getPos().second);
            }
        }
    }

    mainPeerInterface->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, player->getRaknetGuid(), false);
}

unsigned char Server::GetPacketIdentifier(RakNet::Packet *packet)
{
    if (!packet){
        return (255);
    }
    if ((unsigned char)packet->data[0] == ID_TIMESTAMP)
        return (unsigned char) packet->data[sizeof(unsigned char) + sizeof(unsigned long)];
    else
        return (unsigned char) packet->data[0];
}

void Server::createServer()
{
    mainPeerInterface = RakNet::RakPeerInterface::GetInstance();

    if (!mainPeerInterface){
        std::cerr << "Error createServer()" << std::endl;
        exit(0);
    }

    mainPeerInterface->SetIncomingPassword("pwd", 4);
    mainPeerInterface->SetTimeoutTime(150000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
    world->createWorld();
}

void Server::startServer()
{
    socketDescriptor.port = serverPort;
    socketDescriptor.socketFamily = AF_INET;

    bool checkStart = mainPeerInterface->Startup(maxSockets, &socketDescriptor, 1);

    if (checkStart != RakNet::RAKNET_STARTED){
        std::cerr << "Error Start()" << std::endl;
        exit(0);
    }

    mainPeerInterface->SetMaximumIncomingConnections(maxSockets);
    mainPeerInterface->SetOccasionalPing(true);
    mainPeerInterface->SetUnreliableTimeout(1000);

    std::cout << "Server started" << std::endl;
}

void Server::shutdownServer()
{
    mainPeerInterface->Shutdown(300);
    RakNet::RakPeerInterface::DestroyInstance(mainPeerInterface);
}

void Server::getPackets()
{
    RakNet::Packet *packet;

    for (packet = mainPeerInterface->Receive(); packet; packet = mainPeerInterface->Receive()){

        unsigned char packetIdentifier = GetPacketIdentifier(packet);

        if (packetIdentifier != 255){

            Player *player = world->getPlayer(packet->guid);
            RakNet::BitStream in(packet->data, packet->length, false);

            switch (packetIdentifier){
                case ID_NEW_INCOMING_CONNECTION: {
                    std::cout << "ID_NEW_INCOMING_CONNECTION" << std::endl;
                    threadPool.enqueue(std::bind(&Server::addPlayerToWorld, this, packet->guid));
                    break;
                }
                case ID_DISCONNECTION_NOTIFICATION:
                case ID_CONNECTION_LOST: {
                    std::cout << "ID_CONNECTION_LOST" << std::endl;
                    threadPool.enqueue(std::bind(&Server::deletePlayerFromWorld, this, player));
                    break;
                }
                case ID_MYPING: {
                    std::cout << "ID_MYPING" << std::endl;
                    //threadPool.enqueue(std::bind(&Server::Test, this, strdup("MYPING")));
                    break;
                }
                case ID_ACTION_MOVE: {
                    std::cout << "ID_ACTION_MOVE" << std::endl;
                    threadPool.enqueue(std::bind(&Server::playerMove, this, player, packet));
                    break;
                }
                case ID_ACTION_STOP_MOVE: {
                    std::cout << "ID_ACTION_STOP_MOVE" << std::endl;
                    threadPool.enqueue(std::bind(&Server::stopPlayerMoving, this, player));
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void Server::saveWorld()
{
    if (pastWorlds.size() >= 10){

        std::cout << "delete" << std::endl;

        delete pastWorlds[0];
        pastWorlds.erase(pastWorlds.begin());
    }
    World *worldPtr = world;
    pastWorlds.push_back(worldPtr);
}


void Server::startFrame()
{
    frameClock = std::chrono::high_resolution_clock::now();
}

void Server::endFrame()
{
    std::chrono::_V2::system_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> work_time = now - frameClock;

    if (work_time.count() < 25.0){

        long delta_ms_duration = 25-work_time.count();

        //std::cout << work_time.count() << " " << std::to_string(delta_ms_duration) << std::endl;

        if (delta_ms_duration > 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration));
        }
    }
}

void Server::serverLoop()
{
    createServer();
    startServer();

    for (;;){

        startFrame();

        getPackets();

        endFrame();

        if (world){
            saveWorld();
        }
    }
}
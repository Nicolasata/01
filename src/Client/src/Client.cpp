#include "./Client.hpp"

bool exitStatus = false;

void signalHandler(int sig)
{
    signal(sig, SIG_IGN);
    exitStatus = true;
}

Client::~Client()
{
}

Client::Client(int _clientPort, std::string _serverIp, int _serverPort)
{
    threadPool.createPool(1);
    maxSockets = MAX_PLAYER_CONNEXION;
    playerNumber = 0;
    serverIp = _serverIp;
    clientPort = _clientPort;
    serverPort = _serverPort;
    world = new World();
    hasFocus = true;
}

unsigned char Client::GetPacketIdentifier(RakNet::Packet *packet)
{
    if (!packet){
        return (255);
    }

    if ((unsigned char)packet->data[0] == ID_TIMESTAMP)
        return (unsigned char) packet->data[sizeof(unsigned char) + sizeof(unsigned long)];
    else
        return (unsigned char) packet->data[0];
}

long Client::getSinceEpochMs()
{
    std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();
    long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return (timestamp);
}

void Client::setFocus(bool _hasFocus)
{
    hasFocus = _hasFocus;
}

bool Client::getFocusState()
{
    return (hasFocus);
}


void Client::createClient()
{
    mainPeerInterface = RakNet::RakPeerInterface::GetInstance();

    if (!mainPeerInterface){
        std::cerr << "Error createClient()" << std::endl;
        exit(0);
    }

    std::cout << "CREATED" << std::endl;
}

void Client::startClient()
{
    socketDescriptor = RakNet::SocketDescriptor(clientPort, 0);
    socketDescriptor.socketFamily = AF_INET;

    mainPeerInterface->Startup(maxSockets, &socketDescriptor, 1);
    mainPeerInterface->SetOccasionalPing(true);

    RakNet::ConnectionAttemptResult check = mainPeerInterface->Connect(serverIp.c_str(), serverPort, "pwd", 4);
    RakAssert(check == RakNet::CONNECTION_ATTEMPT_STARTED);
    (void)check;

    if (!createWindow()){
        std::cerr << "Error startClient() 'createWindow()' returned 0" << std::endl;
    }
}

void Client::shutdownClient()
{
    window->close();
    delete window;

    mainPeerInterface->Shutdown(300);
    RakNet::RakPeerInterface::DestroyInstance(mainPeerInterface);
}

void Client::ping()
{
    RakNet::BitStream out;
    out.Write((unsigned char)ID_MYPING);
    for (int i = 0; i < 10; i++){
        std::cout << "PUSH" << std::endl;
        mainPeerInterface->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, serverGuid, false);
    }
}

void listPlayer(std::vector<Player*> player)
{
    for (int i = 0; i < (int)player.size(); i++){
        std::cout << "Player: " << player[i]->getRaknetGuid().ToString() << std::endl;
    }
}

void Client::getPackets()
{
    RakNet::Packet *packet;

    for (packet = mainPeerInterface->Receive(); packet; packet = mainPeerInterface->Receive()){

        unsigned char packetIdentifier = GetPacketIdentifier(packet);

        if (packetIdentifier != 255){

            switch (packetIdentifier){
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    std::cout << "ID_CONNECTION_REQUEST_ACCEPTED" << std::endl;
                    setServerGuid(packet->guid);
                    break;
                case ID_CONNECTION_ATTEMPT_FAILED:
                    std::cout << "ID_CONNECTION_ATTEMPT_FAILED" << std::endl;
                    break;
                case ID_INVALID_PASSWORD:
                    std::cout << "ID_INVALID_PASSWORD" << std::endl;
                    break; 
                case ID_ADD_PLAYER: {
                    std::cout << "ID_ADD_PLAYER" << std::endl;
                    addPlayerToWorld(packet);
                    listPlayer();
                    break;
                }
                case ID_DELETE_PLAYER: {
                    std::cout << "DELETE PLAYER" << std::endl;
                    deletePlayerFromWorld(packet);
                    listPlayer();
                    break;
                }
                case ID_GET_INFORMATIONS: {
                    std::cout << "ID_GET_INFORMATIONS" << std::endl;
                    loadWorld(packet);
                    listPlayer();
                    break;
                }
                case ID_ACTION_STOP_MOVE:
                case ID_ACTION_MOVE: {
                    std::cout << "ID_ACTION_MOVE || ID_ACTION_STOP_MOVE" << std::endl;
                    threadPool.enqueue(std::bind(&Client::updatePlayerFromWorld, this, packet));
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void Client::clientLoop()
{
    createClient();
    startClient();
    signal(SIGINT, signalHandler);

    for (; !exitStatus && window->isOpen();){

        startFrame();

        getPackets();
        getEvent();
        world->checkPlayerFall();
        world->render(window);

        endFrame();
    }
    shutdownClient();
}
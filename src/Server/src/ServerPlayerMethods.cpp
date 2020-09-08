#include "./Server.hpp"

float getRandom()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(0, 10);
    return dis(e);
}

void Server::sendPlayer(Player *player, RakNet::RakNetGUID receiverGuid, SERVER_MESSAGES specifier)
{
    RakNet::BitStream out;

    out.Write((unsigned char) specifier);
    out.Write((float)player->getPos().first);
    out.Write((float)player->getPos().second);
    out.Write((int)player->getDirection());
    out.Write((float)player->getSpeed());
    out.Write((long)getSinceEpochMs());
    out.Write(player->getRaknetGuid());

    mainPeerInterface->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, receiverGuid, false);
}

void Server::notifyAll(Player *player, SERVER_MESSAGES specifier)
{
    std::vector<Player*> playerList = world->getPlayerList();
    
    for (int i = 0; i < (int)playerList.size(); i++){
        if (playerList[i]->getRaknetGuid() != player->getRaknetGuid()){
            sendPlayer(player, playerList[i]->getRaknetGuid(), specifier);
        }
    }
}

void Server::addPlayerToWorld(RakNet::RakNetGUID packetGuid)
{
    try {
        float x = 10;
        float y = 125;

        Player *player = new Player(x, y);

        player->setRaknetGuid(packetGuid);
        world->pushPlayer(player);

        std::cout << "PLAYER CREATED: " << packetGuid.ToString() << std::endl;

        sendPlayer(player, packetGuid, ID_ADD_PLAYER); //SEND ACTUAL PLAYER CREATED TO ACTUAL CLIENT
        sendGameinformations(player); //SEND OTHER PLAYERS TO ACTUAL CLIENT
        notifyAll(player, ID_ADD_PLAYER); // SEND ACTUAL PLAYER TO OTHER CLIENTS
    } catch(...){
        std::cerr << "Caught exception: addPlayerToWorld()" << std::endl;
    }
}

void Server::deletePlayerFromWorld(Player *player)
{
    if (!player){
        return;
    }

    RakNet::RakNetGUID playerGuid = player->getRaknetGuid();

    if (world->deletePlayer(player)){

        if (!world->getPlayerListSize()){
            std::cout << "NO PLAYER LEFT" << std::endl;
        } else {

            std::cout << "PLAYER DELETED: " << playerGuid.ToString() << std::endl;

            std::vector<Player*> playerList = world->getPlayerList();
            RakNet::BitStream out;

            out.Write((unsigned char)ID_DELETE_PLAYER);
            out.Write(playerGuid);

            for (int j = 0; j < (int)playerList.size(); j++){ // SEND EVENT ID_DELETE_PLAYER WITH DELETED PLAYER GUID
                mainPeerInterface->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, playerList[j]->getRaknetGuid(), false);
            }
        }
    } else {
        std::cerr << "deletePlayerFromWorld() Player: " << playerGuid.ToString() << " not found" << std::endl;
    }
}
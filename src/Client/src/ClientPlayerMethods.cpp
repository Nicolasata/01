#include "./Client.hpp"

void Client::addPlayerToWorld(RakNet::Packet *packet)
{
    RakNet::BitStream in(packet->data, packet->length, false);

    float x;
    float y;
    float speed;

    DIRECTION direction;
    long actionTimeMs;
    RakNet::RakNetGUID raknetGuid;

    in.IgnoreBytes(sizeof(RakNet::MessageID));
    in.Read(x);
    in.Read(y);
    in.Read(direction);
    in.Read(speed);
    in.Read(actionTimeMs);
    in.Read(raknetGuid);

    Player *player = new Player(x, y, direction, raknetGuid);
    player->setLastMsDelay(getSinceEpochMs()-actionTimeMs);
    player->setSpeed(speed);

    world->pushPlayer(player);

    if (world->getPlayerListSize() == 1){
        setCamera(world->getPlayer(0));
    }
}

void Client::deletePlayerFromWorld(RakNet::Packet *packet)
{
    RakNet::BitStream in(packet->data, packet->length, false);
    RakNet::RakNetGUID playerGuid;

    in.IgnoreBytes(sizeof(RakNet::MessageID));
    in.Read(playerGuid);

    if (!world->deletePlayer(playerGuid)){
        std::cerr << "deletePlayerFromWorld() Player: " << playerGuid.ToString() << " not found" << std::endl;
    } else {
        std::cout << "PLAYER DELETED: " << playerGuid.ToString() << std::endl;
    }
}

void Client::updatePlayerFromWorld(RakNet::Packet *packet)
{
    RakNet::BitStream in(packet->data, packet->length, false);

    float x;
    float y;
    float speed;

    DIRECTION direction;
    long actionTimeMs;
    RakNet::RakNetGUID raknetGuid;

    in.IgnoreBytes(sizeof(RakNet::MessageID));
    in.Read(x);
    in.Read(y);
    in.Read(direction);
    in.Read(speed);
    in.Read(actionTimeMs);
    in.Read(raknetGuid);

    //std::cout << "MS: " << getSinceEpochMs()-lastTimestamp << std::endl;

    Player *player = world->getPlayer(raknetGuid);

    if (!player){
        std::cerr << "updatePlayerFromWorld() Player not found" << std::endl;
    }

    std::cout << "POSADV: " << x << " " << y << std::endl;

    player->setPos(x, y);
    player->setDirection(direction);
    player->setSpeed(speed);
    player->setLastMsDelay(getSinceEpochMs()-actionTimeMs);
}

void Client::loadWorld(RakNet::Packet *packet)
{
    RakNet::BitStream in(packet->data, packet->length, false);

    int actualPlayerMax = 0;

    in.IgnoreBytes(sizeof(RakNet::MessageID));
    in.Read(actualPlayerMax);

    for (int i = 0; i < actualPlayerMax; i++){

        float x;
        float y;
        float speed;

        DIRECTION direction;
        RakNet::RakNetGUID raknetGuid;

        in.Read(x);
        in.Read(y);
        in.Read(direction);
        in.Read(speed);
        in.Read(raknetGuid);

        Player *player = new Player(x, y, direction, raknetGuid);
        player->setSpeed(speed);
        world->pushPlayer(player);
    }

    int chunkNumber = 0;
    in.Read(chunkNumber);

    for (int k = 0; k < chunkNumber; k++){

        int height = 0;
        int width = 0;

        in.Read(height);
        in.Read(width);

        Chunk *chunk = new Chunk(height, width);

        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){

                int blockId;
                float x;
                float y;

                in.Read(blockId);
                in.Read(x);  
                in.Read(y);

                Block *block = NULL;

                switch(blockId){
                    case 1:
                        block = new DirtBlock(x, y);
                        break;
                    default:
                        break;
                }

                chunk->setBlock(block, i, j);
            }
        }
        world->addChunk(chunk);
    }
}

void Client::listPlayer()
{
    std::vector<Player*> playerList = world->getPlayerList();

    for (int i = 0; i < (int)playerList.size(); i++){
        std::cout << "Player: " << playerList[i]->getRaknetGuid().ToString() << std::endl;
    }
}
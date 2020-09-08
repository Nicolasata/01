#include "./World.hpp"

World::World(int _chunkNumber)
{
    chunkNumber = _chunkNumber;
}

void World::createWorld()
{
    int height = 5;
    int width = 5;

    int x = 0;
    int y = 200;

    for (int i = 0; i < chunkNumber; i++){
        world.push_back(new Chunk(x, y, height, width));
        x+=50*width;
    }
}

std::vector<Chunk*> World::getWorld()
{
    return (world);
}

std::vector<Player*> World::getPlayerList()
{
    return (playerList);
}

int World::getPlayerListSize()
{
    return (playerList.size());
}


Player *World::getPlayer(RakNet::RakNetGUID _raknetGuid)
{
    for (int i = 0; i < (int)playerList.size(); i++){

        if (playerList[i]->getRaknetGuid() == _raknetGuid){
            return (playerList[i]);
        }
    }
    return (NULL);
}

void World::pushPlayer(Player *player)
{
    playerList.push_back(player);
}

int World::deletePlayer(Player *player)
{
    for (int i = 0; i < (int)playerList.size(); i++){

        if (playerList[i]->getRaknetGuid() == player->getRaknetGuid()){

            delete playerList[i];
            playerList.erase(playerList.begin()+i);

            return (1);
        }
    }
    return (0);
}

int World::countMovingPlayers()
{
    int count = 0;

    for (int i = 0; i < (int)playerList.size(); i++){
        if (playerList[i]->isEntityMoving()){
            count++;
        }
    }
    return (count);
}
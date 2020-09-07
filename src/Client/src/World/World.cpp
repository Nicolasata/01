#include "./World.hpp"

World::World()
{
}

World::~World()
{
    for (int i = 0; i < (int)world.size(); i++){

        delete world[i];
        world.erase(world.begin()+i);
    }
    std::vector<Chunk*>().swap(world);
}

void World::addChunk(Chunk *chunk)
{
    world.push_back(chunk);
}

void World::pushPlayer(Player *player)
{
    playerList.push_back(player);
}

int World::deletePlayer(RakNet::RakNetGUID raknetGuid)
{
    for (int i = 0; i < (int)playerList.size(); i++){

        if (playerList[i]->getRaknetGuid() == raknetGuid){

            delete playerList[i];
            playerList.erase(playerList.begin()+i);

            return (1);
        }
    }
    return (0);
}

void World::render(sf::RenderWindow *window)
{
    window->clear(sf::Color::White);

    for (int k = 0; k < (int)world.size(); k++){

        Chunk *chunk = world[k];

        for (int i = 0; i < (int)chunk->getHeight(); i++){
            for (int j = 0; j < (int)chunk->getWidth(); j++){

                window->draw(*chunk->getBlock(i, j)->getSprite());
            }
        }
    }

    for (int i = 0; i < (int)playerList.size(); i++){

        window->draw(*playerList[i]->getSprite());
    }
    window->display();
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

Player *World::getPlayer(int index)
{

    if (!playerList.size()){
        return (NULL);
    }

    if ((int)playerList.size() < index){
        return (NULL);
    }

    return (playerList[index]);
}

void World::checkPlayerFall()
{
    /*if (!getPlayerListSize()){
        return;
    }

    Player *player = getPlayer(0);

    float _x = player->getPos()->x;
    float _y = 0.0;

    int chunkIndex = 0;

    for (int testChunk = 0; _x > testChunk;){
        chunkIndex++;
        testChunk += 50*5;
    }

    _x = round(_x);
    _y = round(_y);

    Block *block = world[chunkIndex-1]->getBlock(_x, _y);

    if (player->getPos()->y+50 < block->getPos()->y){
        std::cout << "FALL" << std::endl;
    }

    std::cout << "CHUNK INDEX: " << chunkIndex << std::endl;*/
}

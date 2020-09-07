#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <math.h>

#include "./Chunk.hpp"
#include "../Entity/Entity.hpp"

class World
{
    private:
        int chunkNumber;
        std::vector<Chunk*> world;
        std::vector<Player*> playerList;

    public:

        Player *getPlayer(RakNet::RakNetGUID _raknetGuid);
        Player *getPlayer(int index);

        std::vector<Chunk*> getWorld();
        std::vector<Player*> getPlayerList();

        void render(sf::RenderWindow *window);

        void checkPlayerFall();

        void addChunk(Chunk *chunk);
        void pushPlayer(Player *player);
        int deletePlayer(RakNet::RakNetGUID raknetGuid);

        int getPlayerListSize();

        World();
        ~World();
};

#endif
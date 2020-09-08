#ifndef WORLD_HPP_
#define WORLD_HPP_

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
        std::vector<Chunk*> getWorld();
        std::vector<Player*> getPlayerList();
        int getPlayerListSize();
        int countMovingPlayers();

        void pushPlayer(Player *player);
        int deletePlayer(Player *player);

        void createWorld();

        World(int _chunkNumber);
        //~World();
};

#endif
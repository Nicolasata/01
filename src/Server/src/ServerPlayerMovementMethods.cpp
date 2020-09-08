#include "./Server.hpp"

void Server::playerMove(Player *player, RakNet::Packet *packet)
{
    RakNet::BitStream in(packet->data, packet->length, false);

    DIRECTION direction;
    long actionTimeMs;

    in.IgnoreBytes(sizeof(RakNet::MessageID));
    in.Read(direction);
    in.Read(actionTimeMs);

    player->setDirection((DIRECTION) direction);
    player->setMoving(true);

    player->addSpeed(0.50);

    if (player->getSpeed() > 16.0){
        player->setSpeed(16.0);
    }

    float _x = 0.0;

    switch(direction){
        case DIRECTION::RIGHT:{
            if (player->getDirection() != DIRECTION::RIGHT){
                player->setSpeed(0);
            }
            _x = 1*player->getSpeed();
            player->setDirection(DIRECTION::RIGHT);
            break;
        }
        case DIRECTION::LEFT:{
            if (player->getDirection() != DIRECTION::LEFT){
                player->setSpeed(0);
            }
            _x = -1*player->getSpeed();
            player->setDirection(DIRECTION::LEFT);
            break;
        }
        default:
            break;
    }
    player->computePos(_x, 0);
    std::cout << "PPos_x: " << player->getPos().first << std::endl;

    notifyAll(player, ID_ACTION_MOVE);
}

void Server::stopPlayerMoving(Player *player)
{
    if (!player){
        return;
    }

    player->setMoving(false);
    player->setDirection(DIRECTION::UNDEFINED);
    player->setSpeed(0);

    notifyAll(player, ID_ACTION_STOP_MOVE);
}

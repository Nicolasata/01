#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "../../../../common/RaknetIncludes.hpp"

#include <utility>
#include <chrono>
#include <ctime>

enum DIRECTION {
    UNDEFINED = -1,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

class Entity
{
    protected:
        DIRECTION direction;
        bool isMoving;
        float speed;

        float y;
        float x;

    public:

        void computePos(float _x, float _y);
        void setPos(float _x, float _y);
        std::pair<float, float> getPos();

        void addSpeed(float toAdd);
        void setSpeed(float _speed);
        float getSpeed();
    
        void setDirection(DIRECTION _direction);
        DIRECTION getDirection();

        void setMoving(bool _isMoving);
        bool isEntityMoving();

        Entity();
        Entity(float _x, float _y);
};

class Player : public Entity
{
    private:
        RakNet::RakNetGUID raknetGuid;
        long lastDelay;

    public:
        void setRaknetGuid(RakNet::RakNetGUID _raknetGuid);
        RakNet::RakNetGUID getRaknetGuid();
        
        Player(float _x, float _y);
};

#endif
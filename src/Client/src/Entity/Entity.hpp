#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "../../../../common/RaknetIncludes.hpp"
#include <SFML/Graphics.hpp>

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
        sf::Sprite *sprite;
        sf::Texture *texture;
        sf::Vector2f *position;

        float speed;

        bool isMoving;
        bool isFalling;
        bool isJumping;

    public:

        sf::Vector2f *getPos();
        sf::Sprite *getSprite();

        void computePos(float _x, float _y);
        void setPos(float _x, float _y);

        void move();

        void addSpeed(float toAdd);
        void setSpeed(float _speed);
        float getSpeed();

        void setDirection(DIRECTION _direction);
        DIRECTION getDirection();

        void setMoving(bool _isMoving);
        bool isEntityMoving();

        Entity(float _x, float _y, DIRECTION _direction);
        ~Entity();
};

class Player : public Entity
{
    private:
        RakNet::RakNetGUID raknetGuid;
        long lastDelay;

    public:

        RakNet::RakNetGUID getRaknetGuid();
        void setRaknetGuid(RakNet::RakNetGUID _raknetGuid);

        void setLastMsDelay(long _lastDelay);
        long getLastMsDelay();

        Player(float _x, float _y, DIRECTION _direction, RakNet::RakNetGUID raknetGuid);
};

#endif
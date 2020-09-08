#include "./Entity.hpp"

Entity::Entity()
{
    x = 0;
    y = 0;
    direction = DIRECTION::UNDEFINED;
    isMoving = false;
    speed = 0.0;
}

Entity::Entity(float _x, float _y)
{
    x = _x;
    y = _y;
    direction = DIRECTION::UNDEFINED;
    isMoving = 0;
    speed = 0.0;
}

DIRECTION Entity::getDirection()
{
    return (direction);
}

std::pair<float, float> Entity::getPos()
{
    return (std::pair<float, float>(x, y));
}

void Entity::setPos(float _x, float _y)
{
    x = _x;
    y = _y;
}

void Entity::addSpeed(float toAdd)
{
    speed += toAdd;
}

void Entity::setSpeed(float _speed)
{
    speed = _speed;
}

float Entity::getSpeed()
{
    return (speed);
}

void Entity::computePos(float _x, float _y)
{
    x += _x;
    y += _y;
}

void Entity::setDirection(DIRECTION _direction)
{
    direction=_direction;
}

bool Entity::isEntityMoving()
{
    return (isMoving);
}

void Entity::setMoving(bool _isMoving)
{
    isMoving = _isMoving;
}

void Player::setRaknetGuid(RakNet::RakNetGUID _raknetGuid)
{
    raknetGuid = _raknetGuid;
}

RakNet::RakNetGUID Player::getRaknetGuid()
{
    return (raknetGuid);
}

Player::Player(float _x, float _y) : Entity(_x, _y)
{
    lastDelay = 0;
}
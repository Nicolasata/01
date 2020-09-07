#include "./Entity.hpp"

Entity::Entity(float _x, float _y, DIRECTION _direction)
{
    sprite = new sf::Sprite();
    texture = new sf::Texture();
    position = new sf::Vector2f(_x, _y);

    speed = 0;
    direction = _direction;

    isMoving = false;
    isFalling = false;
    isJumping = false;
}

Entity::~Entity()
{
    delete sprite;
    delete position;
    delete texture;
}

void Entity::move()
{
    if (!isMoving){
        return;
    }

    float _x = 0.0;
    float _y = 0.0;

    switch(direction){
        case DIRECTION::RIGHT:{
            _x = 1*speed;
            break;
        }
        case DIRECTION::LEFT:{
            _x = -1*speed;
            break;
        }
        case DIRECTION::DOWN:{
            _y = -1*speed;
            break;
        }
        default:
            break;
    }
    computePos(_x, _y);
}

void Entity::setPos(float _x, float _y)
{
    position->x = _x;
    position->y = _y;

    sprite->setPosition(*position);
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
    position->x += _x;
    position->y += _y;

    sprite->move(sf::Vector2f(_x, _y));
}

sf::Vector2f *Entity::getPos()
{
    return (position);
}

sf::Sprite *Entity::getSprite()
{
    return (sprite);
}

void Entity::setDirection(DIRECTION _direction)
{
    direction=_direction;
}

DIRECTION Entity::getDirection()
{
    return (direction);
}


void Entity::setMoving(bool _isMoving)
{
    isMoving = _isMoving;
}

bool Entity::isEntityMoving()
{
    return (isMoving);
}

void Player::setRaknetGuid(RakNet::RakNetGUID _raknetGuid)
{
    raknetGuid = _raknetGuid;
}

RakNet::RakNetGUID Player::getRaknetGuid()
{
    return (raknetGuid);
}

Player::Player(float _x, float _y, DIRECTION _direction, RakNet::RakNetGUID _raknetGuid) : Entity(_x, _y, _direction)
{
    raknetGuid = _raknetGuid;
    lastDelay = 0;

    if (!texture->loadFromFile("./src/Entity/Img/player.png")){
        std::cerr << "Faild to load 'player.png' texture" << std::endl;
    }

    sprite->setTexture(*texture);
    sprite->setPosition(*position);
}

void Player::setLastMsDelay(long _lastDelay)
{
    lastDelay = _lastDelay;
}

long Player::getLastMsDelay()
{
    return (lastDelay);
}
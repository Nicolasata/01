#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

class Block
{
    protected:

        float InitialBlockResistance;
        float blockResistance;
        int blockId;

        sf::Sprite *sprite;
        sf::Texture *texture;
        sf::Vector2f *position;

    public:

        virtual void onClick() = 0;

        int getBlockId()
        {
            return (blockId);
        }

        float getBlockResistance()
        {
            return (blockResistance);
        }

        sf::Sprite *getSprite()
        {
            return (sprite);
        }

        sf::Vector2f *getPos()
        {
            return (position);
        }

        int dammageBlock(float dammage)
        {
            blockResistance -= dammage;

            if (!blockResistance){
                return (0);
            }
            return (1);
        }

        void resetBlockResistance(float _newBlockResistance)
        {
            blockResistance = _newBlockResistance;
        }

        void resetBlockResistance()
        {
            blockResistance = InitialBlockResistance;
        }

        Block(float _x, float _y)
        {
            position = new sf::Vector2f(_x, _y);
        }

        virtual ~Block()
        {
            free(sprite);
            free(texture);
            free(position);
        }
};

class DirtBlock : public Block 
{

    public:

        DirtBlock(float _x, float _y) : Block(_x, _y)
        {
            blockId = 1;
            InitialBlockResistance = 1.0;
            blockResistance = InitialBlockResistance;

            texture = new sf::Texture();
            sprite = new sf::Sprite();

            if (!texture->loadFromFile("./src/World/Block/Img/dirt.jpg")){
                std::cerr << "Faild to load 'dirt.png' texture" << std::endl;
            }

            sprite->setTexture(*texture);
            sprite->setPosition(*position);
        }

        void onClick()
        {
            return;
        }
};

#endif

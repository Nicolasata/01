#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <iostream>
#include <vector>
#include <utility>

class Block
{
    protected:

        float InitialBlockResistance;
        float blockResistance;
        int blockId;

        float x;
        float y;

    public:

        int getBlockId()
        {
            return (blockId);
        }

        float getBlockResistance()
        {
            return (blockResistance);
        }

        std::pair<float, float> getPos()
        {
            return (std::pair<float, float>(x, y));
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
            x = _x;
            y = _y;
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
        }
};

#endif

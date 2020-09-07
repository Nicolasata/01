#ifndef CHUNK_HPP_
#define CHUNK_HPP_

#include "./Block/Block.hpp"

class Chunk
{
    private:

        float height;
        float width;

        float startPos;

        std::vector<std::vector<Block*>> chunk;

    public:

        Block *getBlock(int i, int j){return (chunk[i][j]);}
        void setBlock(Block *block, int i, int j);

        float getHeight(){return(height);}
        float getWidth(){return(width);}
        float getStartX(){return (startPos);}

        Chunk(float _width, float _height);
        Chunk(float _x, float _y, float _width, float _height);
        ~Chunk();
};

#endif
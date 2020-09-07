#include "./Chunk.hpp"

Chunk::Chunk(float _width, float _height)
{
    height = _height;
    width = _width;

    for (int i = 0; i < height; i++){

        std::vector<Block*> lineBlock;

        for (int j = 0; j < width; j++){

            lineBlock.push_back(NULL);
        }
        chunk.push_back(lineBlock);
    }
}

Chunk::Chunk(float _x, float _y, float _width, float _height)
{
    height = _height;
    width = _width;

    startPos = _x;

    float x = _x;
    float y = _y;

    for (int i = 0; i < height; i++, y+=50){

        std::vector<Block*> lineBlock;

        for (int j = 0; j < width; j++, x+=50){

            lineBlock.push_back(new DirtBlock(x, y));
        }
        x=_x;
        chunk.push_back(lineBlock);
    }
}

void Chunk::setBlock(Block *block, int i, int j)
{
    chunk[i][j] = block;
}

Chunk::~Chunk()
{
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            delete chunk[i][j];
            chunk[i].erase(chunk[i].begin()+j);
        }
        std::vector<Block*>().swap(chunk[i]);
    }
    std::vector<std::vector<Block*>>().swap(chunk);
}
//#pragma once
#ifndef MAIN_LEVEL_H
#define MAIN_LEVEL_H

//#include<Tile.h>

#include <string>
#include <iostream>
#include <vector>
//

struct Level{
public:
    Level(const std::string& path);
    Tile GetTile(int coordX, int coordsY) { return tiles[coordsX / TILE_WIDTH + (coords.Y / TILE_HEIGHT * CNT_WIDTH)]; }
    //Tile GetTileCoords(int coordX, int coordsY)
    //void PutTile(int coordX, int coordsY, Tile& cur) { tiles[coordsX / TILE_WIDTH + (coords.Y / TILE_HEIGHT * CNT_WIDTH)] = cur; }

private:
    std::vector<Tile> tiles;
};



#endif //MAIN_LEVEL_H

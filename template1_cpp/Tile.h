#pragma once
#ifndef MAIN_TILE_H
#define MAIN_TILE_H

#include "Image.h"
#include <map>
#include <string>
#include <iostream>

enum class TileType
{
    FLOOR,
    WALL,
    MAN,
    THORN,
    EXIT,
    FINAL_EXIT
};


class Tile {
public:
    Tile(char type_o);
    //Tile(char type_o, int tileWidth_o, int tileHeight_o) : tileWidth(tileWidth_o), tileHeight(tileHeight_o) {
 
    TileType GetType() { return type; }
    Image* GetImage(std::string str) { return images[str]; }
    //static map<std::string, Image*> images; // clear
    //static void addImage(TileType type, const std::string& path) { images[type] = Image(path); }
private:
    TileType type;
    //int tileWidth;
    //int tileHeight;

};

#endif //MAIN_TILE_H
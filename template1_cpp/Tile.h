#pragma once
#ifndef MAIN_TILE_H
#define MAIN_TILE_H

#include "Image.h"
#include <map>
#include <string>
#include <iostream>

enum TileType
{
    FLOOR,
    WALL,
    MAN,
    THORN,
    EXIT,
    FINAL_EXIT
};


struct Tile {
    Tile(char type_o);
    inline static std::map<TileType, Image*> images{}; // = nullptr; clear
    static void addImage(TileType type, const std::string& path) { images[type] = new Image(path); }
    //Tile(char type_o, int tileWidth_o, int tileHeight_o) : tileWidth(tileWidth_o), tileHeight(tileHeight_o) {
 
    TileType GetType() { return type; }
    Image* GetImage(TileType type) { return images[type]; }
    
    //static void addImage(TileType type, const std::string& path) { images[type] = Image(path); }
private:
    TileType type;
    //int tileWidth;
    //int tileHeight;
    //static map<std::string, Image*> images;
};

#endif //MAIN_TILE_H
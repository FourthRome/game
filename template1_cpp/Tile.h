#ifndef MAIN_TILE_H
#define MAIN_TILE_H

#include <string>
#include <iostream>

#include<Image.h>

enum class TileType
{
    FLOOR,
    WALL,
    MAN,
    THORN,
    EXIT,
    FINAL_EXIT;
};


struct Tile {
public:
    //Tile(char type_o, int tileWidth_o, int tileHeight_o) : tileWidth(tileWidth_o), tileHeight(tileHeight_o) {
    Tile(char type_o);
    TileType GetType() { return type; }
    Image* GetImage(std::string str) { return images[str]; }
    static std::map<std::string, Image*> images{};
    static addImage(TileType type, std::string& path) { images[type] = Image(path);}
private:
    TileType type;
    //int tileWidth;
    //int tileHeight;

};

#endif //MAIN_TILE_H
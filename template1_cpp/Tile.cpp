#include"Tile.h"

Tile::Tile(char type_o) {
    switch (type_o) {
    case '.':
        type = TileType::FLOOR;
        break;
    case '#':
        type = TileType::WALL;
        break;
    case '@':
        type = TileType::MAN;
        break;
    case 'T':
        type = TileType::THORN;
        break;
    case 'x':
        type = TileType::EXIT;
        break;
    case 'Q':
        type = TileType::FINAL_EXIT;
        break;
    default: break;
    }
}
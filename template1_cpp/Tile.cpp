#include<Tile.h>

Tile::Tile(char type_o) {
    switch (type_o) {
    case '.':
        type = FLOOR;
        break;
    case '#':
        type = WALL;
        break;
    case '@':
        type = MAN;
        break;
    case 'T':
        type = THORN;
        break;
    case 'x':
        type = EXIT;
        break;
    case 'Q':
        type = FINAL_EXIT;
        break;
    default: break;
    }

#pragma once
#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Tile.h"
#include "Level.h"

struct Point
{
    int x;
    int y;
};

enum class MovementDir
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Player
{
    explicit Player(Point pos = { .x = 10, .y = 10 }) :
        coords(pos), old_coords(coords), player_tile(TileType::MAN) { player_tile = Tile(TileType::MAN); };

    void ProcessInput(MovementDir dir, Level& level, bool& flag_stop, bool& flag_fall, bool& flag_exit, bool& flag_final_exit);
    void MyProcessInput(MovementDir dir, Level& level, bool& flag_stop, bool& flag_fall, bool& flag_exit, bool& flag_final_exit);
    void DrawOfPlayer(Image& screen);
    // char Elem(Level& level, int a_obst, int b_obst);
    bool CheckCoords(Level& level, int obstX, int obstY, int futureX, int futureY);
    void UpdateFlags(bool& local_flag_break, bool& flag_fall, bool& flag_exit, bool& flag_FinalExit,
        Level& level, int obstX, int obstY);
    
    
    Point GetCoords() { return coords; }
    Point GetOldCoords() { return old_coords; }
    void PutCoords(int x, int y) { coords.x = x; coords.y = y; } // void PutCoords(int x, int y, Point coords) {...}
    //void PutStartCoords(int x, int y) { start_coords.x = x; start_coords.y = y; } //void PutStartCoords(int x, int y, Point coords) { ... }

private:
    //int startX{};
    //int startY{};
    //Point start_coords{ .x = 150, .y = 150 };
    Point coords{ .x = 150, .y = 150 };
    Point old_coords{ .x = 150, .y = 150 };
    //Pixel color{ .r = 255, .g = 0, .b = 0, .a = 1 }; //here, color of game_square
    int move_speed = 1; // here, speed of game_square
    Tile player_tile;
};

#endif //MAIN_PLAYER_H

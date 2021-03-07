#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"

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
        coords(pos), old_coords(coords) {};

    bool Moved() const;
    void ProcessInput(MovementDir dir, std::string& chars, bool& flag_prick, bool& flag_exit, bool& flag_FinalExit);
    void DrawOfPlayer(Image& screen, Image& floor, Image& man);
    char Elem(std::string& str, int a_obst, int b_obst, int a_future, int b_future);
    bool CheckCoords(std::string& chars, int obstX, int obstY, int futureX, int futureY);
    bool Crached(std::string& str, int a_obst, int b_obst, int a_future, int b_future);
    bool Pricked(std::string& str, int a_obst, int b_obst, int a_future, int b_future);
    bool Exited(std::string& str, int a_obst, int b_obst, int a_future, int b_future);
    bool FinalExited(std::string& str, int a_obst, int b_obst, int a_future, int b_future);
    Point GetOldCoords() { return old_coords; }

private:
    Point coords{ .x = 150, .y = 150 };
    Point old_coords{ .x = 150, .y = 150 };
    Pixel color{ .r = 255, .g = 0, .b = 0, .a = 1 }; //here, color of game_square
    int move_speed = 1; // here, speed of game_square
};

#endif //MAIN_PLAYER_H

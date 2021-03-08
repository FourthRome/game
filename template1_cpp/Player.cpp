#include <GLFW/glfw3.h>
#include "Player.h"
#include <iostream>

//bool Player::Moved() const
//{
//  if(coords.x == old_coords.x && coords.y == old_coords.y)
//    return false;
//  else
//    return true;
//}
int obstX, obstY, futureX, futureY;

char Player::Elem(std::string& chars, int obstX, int obstY) {
    try { return chars[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX]; }
    catch (...) { std::cout << "wrong indexes obstX = " << obstX << " obstY = " << obstY << std::endl; }
}

bool Player::CheckCoords(std::string& chars, int obstX, int obstY, int futureX, int futureY) {
    if (futureX <= obstX * TILE_WIDTH + TILE_WIDTH &&
        futureX >= obstX * TILE_WIDTH &&
        futureY <= obstY * TILE_HEIGHT + TILE_HEIGHT &&
        futureY >= obstY * TILE_HEIGHT)
        return true;
    else return false;
}

void Player::UpdateFlags(bool& local_flag_break, bool& flag_fall, bool& flag_exit, bool& flag_final_exit,
    std::string& chars, int obstX, int obstY) {

    if (Elem(chars, obstX, obstY) == '#') {
        local_flag_break = true;
    }
    else if (Elem(chars, obstX, obstY) == 'T' || Elem(chars, obstX, obstY) == ' ') {
        flag_fall = true;
    }
    else if (Elem(chars, obstX, obstY) == 'x') {
        flag_exit = true;
    }
    else if (Elem(chars, obstX, obstY) == 'Q') {
        flag_final_exit = true;
    }

}

void Player::ProcessInput(MovementDir dir, std::string& chars, bool& flag_fall, bool& flag_exit, bool& flag_final_exit)
{
    int move_dist = 1;

    bool local_flag_break{};
    switch (dir) {
    case MovementDir::UP:
        futureX = coords.x;
        futureY = coords.y + move_dist;
        obstX = coords.x / TILE_WIDTH;
        obstY = coords.y / TILE_HEIGHT + 2; // important
        if (CheckCoords(chars, obstX, obstY, futureX, futureY + TILE_HEIGHT)) {
            if (coords.x % 32 == 0) {
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX, obstY);
            }
            else {
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX, obstY);
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX + 1, obstY);

            }
        }
        if (!local_flag_break) {
            old_coords.y = coords.y;
            coords.y += move_dist;
        }
        break;


    case MovementDir::DOWN:
        futureX = coords.x;
        futureY = coords.y - move_dist;
        obstX = coords.x / TILE_WIDTH;
        obstY = coords.y / TILE_HEIGHT - 1; // checj for window_size
        if (CheckCoords(chars, obstX, obstY, futureX, futureY)) {
            if (coords.x % 32 == 0) {
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX, obstY);
            }
            else {
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX, obstY);
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX + 1, obstY);

            }
        }
        if (!local_flag_break) {
            old_coords.y = coords.y;
            coords.y -= move_dist;
        }
        break;


    case MovementDir::LEFT:
        futureX = coords.x - move_dist;
        futureY = coords.y;
        obstX = coords.x / TILE_WIDTH - 1; // important
        obstY = coords.y / TILE_HEIGHT;
        if (CheckCoords(chars, obstX, obstY, futureX, futureY)) {
            if (coords.y % 32 == 0) {
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX, obstY);
            }
            else {
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX, obstY);
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX, obstY + 1);

            }
        }
        if (!local_flag_break) {
            old_coords.x = coords.x;
            coords.x -= move_dist;
        }
        break;


    case MovementDir::RIGHT:
        futureX = coords.x + move_dist;
        futureY = coords.y;
        obstX = coords.x / TILE_WIDTH + 2; // important
        obstY = coords.y / TILE_HEIGHT;
        if (CheckCoords(chars, obstX, obstY, futureX + TILE_WIDTH, futureY)) {
            if (coords.y % 32 == 0) {
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX, obstY);
            }
            else {
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX, obstY);
                UpdateFlags(local_flag_break, flag_fall, flag_exit, flag_final_exit, chars, obstX, obstY + 1);

            }
        }
        if (!local_flag_break) {
            old_coords.x = coords.x;
            coords.x += move_dist;
        }
        break;
    default:break;
    }
}


void Player::DrawOfPlayer(Image& screen, Image& floor, Image& man) {

    for (int y = coords.y; y <= coords.y + man.Height(); ++y) {
        for (int x = coords.x; x <= coords.x + man.Width(); ++x) {
            screen.PutPixel(x, y, man.GetPixel(x - coords.x, (y - coords.y)));
            //screen.PutPixel(x, y, color);
        }
    }
}
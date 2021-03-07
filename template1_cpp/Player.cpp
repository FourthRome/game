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

char Player::Elem(std::string& chars, int obstX, int obstY, int futureX, int futureY) {
    return chars[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX];
}

bool Player::CheckCoords(std::string& chars, int obstX, int obstY, int futureX, int futureY) {
    if (futureX <= obstX * TILE_WIDTH + TILE_WIDTH &&
        futureX >= obstX * TILE_WIDTH &&
        futureY <= obstY * TILE_HEIGHT + TILE_HEIGHT &&
        futureY >= obstY * TILE_HEIGHT)
        return true;
    else return false;
}


bool Player::Crached(std::string& chars, int obstX, int obstY, int futureX, int futureY) {
    if (chars[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX] == '#' &&
        futureX <= obstX * TILE_WIDTH + TILE_WIDTH &&
        futureX >= obstX * TILE_WIDTH &&
        futureY <= obstY * TILE_HEIGHT + TILE_HEIGHT &&
        futureY >= obstY * TILE_HEIGHT)
        return true;
    else return false;
}

bool Player::Pricked(std::string& chars, int obstX, int obstY, int futureX, int futureY) {
    if ((chars[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX] == 'T' ||
        chars[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX] == '#') &&
        futureX <= obstX * TILE_WIDTH + TILE_WIDTH &&
        futureX >= obstX * TILE_WIDTH &&
        futureY <= obstY * TILE_HEIGHT + TILE_HEIGHT &&
        futureY >= obstY * TILE_HEIGHT)
        return true;
    else return false;
}

bool Player::Exited(std::string& chars, int obstX, int obstY, int futureX, int futureY) {
    if (chars[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX] == 'x' &&
        futureX <= obstX * TILE_WIDTH + TILE_WIDTH &&
        futureX >= obstX * TILE_WIDTH &&
        futureY <= obstY * TILE_HEIGHT + TILE_HEIGHT &&
        futureY >= obstY * TILE_HEIGHT)
        return true;
    else return false;
}

bool Player::FinalExited(std::string& chars, int obstX, int obstY, int futureX, int futureY) {
    if (chars[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX] == 'Q' &&
        futureX <= obstX * TILE_WIDTH + TILE_WIDTH &&
        futureX >= obstX * TILE_WIDTH &&
        futureY <= obstY * TILE_HEIGHT + TILE_HEIGHT &&
        futureY >= obstY * TILE_HEIGHT)
        return true;
    else return false;
}

void Player::ProcessInput(MovementDir dir, std::string& chars, bool& flag_prick, bool& flag_exit, bool& flag_FinalExit)
{
    int move_dist = 1;
    int obstX, obstY, futureX, futureY;
    switch (dir) {
    case MovementDir::UP:
        futureX = coords.x;
        futureY = coords.y + move_dist + TILE_HEIGHT;
        obstX = coords.x / TILE_WIDTH;
        obstY = coords.y / TILE_HEIGHT + 2;
        if (Crached(chars, obstX, obstY, futureX, futureY)) {
            break;
        }
        else if (Pricked(chars, obstX, obstY, futureX, futureY)) {
            flag_prick = true;
        }
        else if (Exited(chars, obstX, obstY, futureX, futureY)) {
            flag_exit = true;
        }
        else if (FinalExited(chars, obstX, obstY, futureX, futureY)) {
            flag_FinalExit = true;
        }
        else {
            old_coords.y = coords.y;
            coords.y += move_dist;
        }
        break;
    case MovementDir::DOWN:
        futureX = coords.x;
        futureY = coords.y - move_dist;
        obstX = coords.x / TILE_WIDTH;
        obstY = coords.y / TILE_HEIGHT;
        if (Crached(chars, obstX, obstY, futureX, futureY)) {
            break;
        }
        else if (Pricked(chars, obstX, obstY, futureX, futureY)) {
            flag_prick = true;
        }
        else if (Exited(chars, obstX, obstY, futureX, futureY)) {
            flag_exit = true;
        }
        else if (FinalExited(chars, obstX, obstY, futureX, futureY)) {
            flag_FinalExit = true;
        }
        else {
            old_coords.y = coords.y;
            coords.y -= move_dist;
        }
        break;
    case MovementDir::LEFT:
        futureX = coords.x - move_dist;
        futureY = coords.y;
        obstX = coords.x / TILE_WIDTH;
        obstY = coords.y / TILE_HEIGHT;
        if (CheckCoords(chars, obstX, obstY, futureX, futureY)) {
            if (Elem(chars, obstX, obstY, futureX, futureY) == '#' || Elem(chars, obstX, obstY, futureX, futureY) == ' ') {
                break;
            }
            else if (Elem(chars, obstX, obstY, futureX, futureY) == 'T') {
                flag_prick = true;
            }
            else if (Elem(chars, obstX, obstY, futureX, futureY) == 'x') {
                flag_exit = true;
            }
            else if (Elem(chars, obstX, obstY, futureX, futureY) == 'Q') {
                flag_FinalExit = true;
            }
        }
        old_coords.x = coords.x;
        coords.x -= move_dist;
        break;
        /*if (Elem(chars, obstX, obstY, futureX, futureY) == '#' &&
                    CheckedCoords(chars, obstX, obstY, futureX, futureY)) {
            break;
        }
        else if (Pricked(chars, obstX, obstY, futureX, futureY)) {
            flag_prick = true;
        }
        else if (Exited(chars, obstX, obstY, futureX, futureY)) {
            flag_exit = true;
        }
        else if (FinalExited(chars, obstX, obstY, futureX, futureY)) {
            flag_FinalExit = true;
        }
        else {
            old_coords.x = coords.x;
            coords.x -= move_dist;
        }
        break;*/
    case MovementDir::RIGHT:
        futureX = coords.x + move_dist + TILE_WIDTH;
        futureY = coords.y;
        obstX = coords.x / TILE_WIDTH + 2;
        obstY = coords.y / TILE_HEIGHT;
        if (Crached(chars, obstX, obstY, futureX, futureY)) {
            break;
        }
        else if (Pricked(chars, obstX, obstY, futureX, futureY)) {
            flag_prick = true;
        }
        else if (Exited(chars, obstX, obstY, futureX, futureY)) {
            flag_exit = true;
        }
        else if (FinalExited(chars, obstX, obstY, futureX, futureY)) {
            flag_FinalExit = true;
        }
        else {
            old_coords.x = coords.x;
            coords.x += move_dist;
        }
        break;
    default:break;
    }
}


void Player::DrawOfPlayer(Image& screen, Image& floor, Image& man) {
    // screen in main - it is screenBuffer

        //if (Moved()){
            //for (int y = old_coords.y; y <= old_coords.y + floor.Height(); ++y) {
            //    for (int x = old_coords.x; x <= old_coords.x + floor.Width(); ++x) {
            //        int first = x - old_coords.x;
            //        int second = y - old_coords.y;
            //        screen.PutPixel(x, y, floor.GetPixel(first, second));
            //        //std::cout << first << " " << second << std::endl;
            //    }
            //}
            //old_coords = coords;

        //}

    for (int y = coords.y; y <= coords.y + man.Height(); ++y) {
        for (int x = coords.x; x <= coords.x + man.Width(); ++x) {
            screen.PutPixel(x, y, man.GetPixel(x - coords.x, (y - coords.y)));
            //screen.PutPixel(x, y, color);
        }
    }
}
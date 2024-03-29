#include <GLFW/glfw3.h>
#include <iostream>

#include "Player.h"

// char Player::Elem(Level& level, int obstX, int obstY) {
//     try { return level[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX]; }
//     catch (...) { std::cout << "wrong indexes obstX = " << obstX << " obstY = " << obstY << std::endl; }
// }

bool Player::CheckCoords(Level& level, int obstX, int obstY, int futureX, int futureY) {
    if (futureX < obstX * TILE_WIDTH + TILE_WIDTH &&
        futureX >= obstX * TILE_WIDTH &&
        futureY < obstY * TILE_HEIGHT + TILE_HEIGHT &&
        futureY >= obstY * TILE_HEIGHT)
        return true;
    else return false;
}

void Player::UpdateFlags(bool& flag_stop, bool& flag_fall, bool& flag_exit, bool& flag_final_exit,
    Level& level, int obstX, int obstY) {
    
    auto tile_type = level.GetTile(obstX, obstY).GetType();

    if (tile_type == TileType::WALL) {
        flag_stop = true;
    }
    else if (tile_type == TileType::THORN || tile_type == TileType::EMPTY) {
        flag_fall = true;
    }
    else if (tile_type == TileType::EXIT) {
        flag_exit = true;
    }
    else if (tile_type == TileType::FINAL_EXIT) {
        flag_final_exit = true;
    }

}

// void Player::ProcessInput(MovementDir dir, Level& level, bool& flag_stop, bool& flag_fall, bool& flag_exit, bool& flag_final_exit)
// {
//     int move_dist = 1;
//     int obstX, obstY, futureX, futureY;
//     flag_stop = false;
//     //flag_final_exit = false;
//     switch (dir) {
//     case MovementDir::UP:
        
   
//         futureX = coords.x;
//         futureY = coords.y + move_dist;
//         obstX = coords.x / TILE_WIDTH;
//         obstY = coords.y / TILE_HEIGHT + 2; // check for window_size
//         if (CheckCoords(level, obstX, obstY, futureX, futureY + TILE_HEIGHT)) {
//             if (coords.x % 32 == 0) {
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX, obstY);
//                 std::cout << level[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX] << std::endl;
//             }
//             else {
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX, obstY);
//                 std::cout << level[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX];
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX + 1, obstY);
//                 std::cout << level[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX] << std::endl;

//             }
//             if (flag_stop) { flag_fall = false; flag_exit = false; flag_final_exit = false;  }
//             if (flag_fall) { flag_exit = false; flag_final_exit = false; }
//             if (flag_exit) { flag_final_exit = false; }
//         }
//         if (!flag_stop) {
//             old_coords.y = coords.y;
//             coords.y += move_dist;
//         }
//         break;


//     case MovementDir::DOWN:
//         futureX = coords.x;
//         futureY = coords.y - move_dist;
//         obstX = coords.x / TILE_WIDTH;
//         obstY = coords.y / TILE_HEIGHT - 1; // check for window_size
//         if (CheckCoords(level, obstX, obstY, futureX, futureY)) {
//             if (coords.x % 32 == 0) {
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX, obstY);
//             }
//             else {
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX, obstY);
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX + 1, obstY);

//             }
//             if (flag_stop) { flag_fall = false; flag_exit = false; flag_final_exit = false; }
//             if (flag_fall) { flag_exit = false; flag_final_exit = false; }
//             if (flag_exit) { flag_final_exit = false; }
//         }
//         if (!flag_stop) {
//             old_coords.y = coords.y;
//             coords.y -= move_dist;
//         }
//         break;


//     case MovementDir::LEFT:
//         futureX = coords.x - move_dist;
//         futureY = coords.y;
//         obstX = coords.x / TILE_WIDTH - 1; // check for window_size
//         obstY = coords.y / TILE_HEIGHT;
//         if (CheckCoords(level, obstX, obstY, futureX, futureY)) {
//             if (coords.y % 32 == 0) {
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX, obstY);
//             }
//             else {
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX, obstY);
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX, obstY + 1);

//             }
//             if (flag_stop) { flag_fall = false; flag_exit = false; flag_final_exit = false; }
//             if (flag_fall) { flag_exit = false; flag_final_exit = false; }
//             if (flag_exit) { flag_final_exit = false; }
//         }
//         if (!flag_stop) {
//             old_coords.x = coords.x;
//             coords.x -= move_dist;
//         }
//         break;


//     case MovementDir::RIGHT:
//         futureX = coords.x + move_dist;
//         futureY = coords.y;
//         obstX = coords.x / TILE_WIDTH + 2; // check for window_size
//         obstY = coords.y / TILE_HEIGHT;
//         if (CheckCoords(level, obstX, obstY, futureX + TILE_WIDTH, futureY)) {
//             if (coords.y % 32 == 0) {
//                 std::cout << level[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX] << std::endl;
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX, obstY);
//             }
//             else {
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX, obstY);
//                 std::cout << level[(CNT_HEIGHT - 1 - obstY) * (CNT_WIDTH + 1) + obstX] << std::endl;
//                 UpdateFlags(flag_stop, flag_fall, flag_exit, flag_final_exit, level, obstX, obstY + 1);
//                 std::cout << level[(CNT_HEIGHT - 1 - obstY - 1) * (CNT_WIDTH + 1) + obstX] << std::endl;

//             }
//             if (flag_stop) { flag_fall = false; flag_exit = false; flag_final_exit = false; }
//             if (flag_fall) { flag_exit = false; flag_final_exit = false; }
//             if (flag_exit) { flag_final_exit = false; }
//         }
//         if (!flag_stop) {
//             old_coords.x = coords.x;
//             coords.x += move_dist;
//         }
//         break;
//     default:break;
//     }
// }


void Player::MyProcessInput(MovementDir dir, Level& level, bool& flag_stop, bool& flag_fall, bool& flag_exit, bool& flag_final_exit)
{
   int move_dist = 1;
   //int obstX, obstY, futureX, futureY;
   flag_stop = false;
   //flag_final_exit = false;

   switch (dir) {
   case MovementDir::UP:
       switch (level.GetTile(coords.x, coords.y + move_dist + TILE_HEIGHT).GetType()) {
       case TileType::FLOOR: old_coords.y = coords.y; coords.y += move_dist; break;
       case TileType::WALL: flag_stop = true; break;
       case TileType::THORN: flag_fall = true; break;
       case TileType::EXIT: old_coords.y = coords.y; coords.y += move_dist; flag_exit; break;
       case TileType::FINAL_EXIT: flag_final_exit = true; break;

       }
       break;


   case MovementDir::DOWN:
       switch (level.GetTile(coords.x, coords.y + move_dist).GetType()) {
       case TileType::FLOOR: old_coords.y = coords.y; coords.y -= move_dist; break;
       case TileType::WALL: flag_stop = true; break;
       case TileType::THORN: flag_fall = true; break;
       case TileType::EXIT: old_coords.y = coords.y; coords.y -= move_dist; flag_exit; break;
       case TileType::FINAL_EXIT: flag_final_exit = true; break;

       }
       
       break;


   case MovementDir::LEFT:
       switch (level.GetTile(coords.x, coords.y + move_dist).GetType()) {
       case TileType::FLOOR: old_coords.x = coords.x; coords.x -= move_dist; break;
       case TileType::WALL: flag_stop = true; break;
       case TileType::THORN: flag_fall = true; break;
       case TileType::EXIT: old_coords.x = coords.x; coords.x -= move_dist; flag_exit; break;
       case TileType::FINAL_EXIT: flag_final_exit = true; break;

       }
      
       break;


   case MovementDir::RIGHT:
       
       switch (level.GetTile(coords.x, coords.y + move_dist).GetType()) {
       case TileType::FLOOR: old_coords.x = coords.x; coords.x += move_dist; break;
       case TileType::WALL: flag_stop = true; break;
       case TileType::THORN: flag_fall = true; break;
       case TileType::EXIT: old_coords.x = coords.x; coords.x += move_dist; flag_exit; break;
       case TileType::FINAL_EXIT: flag_final_exit = true; break;

       }
       
       break;
   default:break;
   }
}



void Player::DrawOfPlayer(Image& screen) {

    for (int y = coords.y; y < coords.y + TILE_HEIGHT; ++y) { // or <=
        for (int x = coords.x; x < coords.x + TILE_WIDTH; ++x) { // or <=
            screen.PutPixel(x, y, (Tile::GetImage(TileType::MAN)->GetPixel(x - coords.x, y - coords.y)));
        }
    }
}
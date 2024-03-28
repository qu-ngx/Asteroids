/***********************************
 * CSC 122 Sample Raylib application..
 * name: Bekam Guta and Quang Nugyen
 */

#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include "player.hpp"

using namespace std;

void draw_player(Player *player)
{
    Vector2 p1 = player->position;
    Vector2 left = {-10, 10};
    Vector2 right = {10, 10};

    left = Vector2Rotate(left, player->rotation);
    right = Vector2Rotate(right, player->rotation);

    DrawLineV(p1, Vector2Add(left, p1), RED);
    DrawLineV(p1, Vector2Add(right, p1), RED);
    DrawLineV(Vector2Add(left, p1), Vector2Add(right, p1), RED);
}

int main(int argc, char **argv)
{
    // Declare the player in the main game
    Player player;
    player.position.x = 400;
    player.position.y = 300;
    player.rotation = PI / 2.0;

    // Init the window
    InitWindow(800, 450, "Welcome to DodFight");

    // Set the FPS to 60
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 mouse_position = GetMousePosition();
        Vector2 to_cursor = Vector2Subtract(mouse_position, player.position);

        float angle = Vector2Angle((Vector2){0, -1}, to_cursor);

        BeginDrawing();

        // Drawing out the player
        draw_player(&player);

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
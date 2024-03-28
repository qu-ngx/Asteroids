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
    Vector2 top = {0, -10};
    Vector2 left = {-10, 10};
    Vector2 right = {10, 10};

    top = Vector2Rotate(top, player->rotation);
    left = Vector2Rotate(left, player->rotation);
    right = Vector2Rotate(right, player->rotation);

    Vector2 t = Vector2Add(top, player->position);
    Vector2 l = Vector2Add(left, player->position);
    Vector2 r = Vector2Add(right, player->position);

    DrawLineV(t, l, RED);
    DrawLineV(t, r, RED);
    DrawLineV(l, r, RED);
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
        player.rotation = angle;

        BeginDrawing();

        DrawText(TextFormat("%.2f", angle), 0, 0, 30, RED);

        ClearBackground(BLACK);
        // Drawing out the player
        draw_player(&player);

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
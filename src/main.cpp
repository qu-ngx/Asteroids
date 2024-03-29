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

#define MAX_PLAYER_VELOCITY 100

void init_player(Player *player)
{
    player->position.x = 400;
    player->position.y = 400;
    player->rotation = PI / 2.0;

    player->acceleration = 100.0;
    player->velocity = Vector2Zero();
}

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
    init_player(&player);

    // Init the window
    InitWindow(800, 600, "Welcome to DodFight");

    while (!WindowShouldClose())
    {
        Vector2 mouse_position = GetMousePosition();
        Vector2 to_cursor = Vector2Subtract(mouse_position, player.position);

        float angle = Vector2Angle((Vector2){0, -1}, to_cursor);
        player.rotation = angle;

        // TO DO: make sure to cursor is not zero vector
        float a = player.acceleration * GetFrameTime();

        Vector2 dir = Vector2Normalize(to_cursor);

        player.velocity = Vector2Add(Vector2Scale(dir, a), player.velocity);

        if (Vector2Length(player.velocity) > MAX_PLAYER_VELOCITY)
        {
            player.velocity = Vector2Scale(Vector2Normalize(player.velocity), MAX_PLAYER_VELOCITY);
        }

        player.position = Vector2Add(Vector2Scale(player.velocity, GetFrameTime()), player.position);

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
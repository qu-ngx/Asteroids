/***********************************
 * CSC 122 Raylib Major Project
 * name: Bekam Guta and Quang Nugyen
 * *********************************/

#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include "player.hpp"
#include "bullet.h"

// Define some constants
#define MAX_PLAYER_VELOCITY 250
#define MAX_BULLET_COUNT 100
#define BULLET_VELOCITY 1000.0

void init_player(Player *player)
{
    player->position.x = 400;
    player->position.y = 300;
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

    DrawLineV(t, l, GREEN);
    DrawLineV(t, r, GREEN);
    DrawLineV(l, r, GREEN);
}

Bullet bullets[MAX_BULLET_COUNT];

void init_bullets()
{
    for (size_t i = 0; i < MAX_BULLET_COUNT; i++)
    {
        bullets[i].dead = true;
    }
}

Bullet *spawn_bullet()
{
    for (size_t i = 0; i < MAX_BULLET_COUNT; i++)
    {
        Bullet *bullet = (bullets + i);
        if (!bullet->dead)
        {
            continue;
        }
        bullet->dead = false;
        return bullet;
    }
    return NULL;
}

void draw_bullets()
{
    for (size_t i = 0; i < MAX_BULLET_COUNT; i++)
    {
        Bullet *bullet = (bullets + i);
        if (bullet->dead)
        {
            continue;
        }
        DrawLineV(bullet->position, Vector2Add(bullet->position, Vector2Scale(bullet->direction, -10)), RED);
    }
}

int main(int argc, char **argv)
{
    // Declare the player in the main game
    Player player = {0};
    init_player(&player);
    init_bullets();

    // Init the window
    InitWindow(800, 600, "Welcome to DodFight");

    while (!WindowShouldClose())
    {
        Vector2 mouse_position = GetMousePosition();
        Vector2 to_cursor = Vector2Subtract(mouse_position, player.position);
        Vector2 player_dir = Vector2Normalize(to_cursor);

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            // TODO: make sure to cursor is not zero vector
            float a = player.acceleration * GetFrameTime();
            player.velocity = Vector2Add(Vector2Scale(player_dir, a), player.velocity);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Bullet *bullet = spawn_bullet();
            if (bullet)
            {
                bullet->position = player.position;
                bullet->direction = player_dir;
            }
        }

        float angle = Vector2Angle((Vector2){0, -1}, to_cursor);
        player.rotation = angle;

        if (Vector2Length(player.velocity) > MAX_PLAYER_VELOCITY)
        {
            player.velocity = Vector2Scale(Vector2Normalize(player.velocity), MAX_PLAYER_VELOCITY);
        }

        player.position = Vector2Add(Vector2Scale(player.velocity, GetFrameTime()), player.position);

        for (size_t i = 0; i < MAX_BULLET_COUNT; i++)
        {
            Bullet *bullet = (bullets + i);
            Vector2 velocity = Vector2Scale(bullet->direction, BULLET_VELOCITY);
            bullet->position = Vector2Add(bullet->position, Vector2Scale(velocity, GetFrameTime()));

            if (bullet->position.x < -20 || bullet->position.x > GetScreenWidth() + 20 || bullet->position.y < -20 || bullet->position.y > GetScreenHeight() + 20)
            {
                bullet->dead = true;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);
        // Drawing out the player
        draw_player(&player);
        draw_bullets();

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
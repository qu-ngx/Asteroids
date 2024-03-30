/***********************************
 * CSC 122 Raylib Major Project
 * name: Bekam Guta and Quang Nguyen
 * *********************************/

#include "lib/raylib.h"
#include "lib/raymath.h"
#include "Player/Player.hpp"
#include "Bullet/Bullet.hpp"
#include "Asteroid/AsteroidList.hpp"
#include "Asteroid/Asteroid.hpp"

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
        DrawLineV(bullet->position, Vector2Add(bullet->position, Vector2Scale(bullet->direction, -10)), YELLOW);
    }
}

int main(int argc, char **argv)
{
    // Declare the player in the main game
    Player player;
    init_bullets();
    int score = 0;
    float asteroid_spawn_sec = ASTEROID_SPAWN_SEC;

    // Initialize asteroids
    AsteroidList asteroidslist;
    for (int i = 0; i < MAX_ASTEROIDS_COUNT; i++)
    {
        Asteroid asteroid;
        asteroidslist.append(asteroid);
    }

    // Init the window
    InitWindow(800, 600, "Welcome to DodFight");

    while (!WindowShouldClose())
    {
        Vector2 mouse_position = GetMousePosition();
        Vector2 to_cursor = Vector2Subtract(mouse_position, player.position);
        Vector2 player_dir = Vector2Normalize(to_cursor);
        bool gameover = false;

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

        asteroid_spawn_sec -= GetFrameTime();
        if (asteroid_spawn_sec < 0)
        {
            asteroid_spawn_sec = ASTEROID_SPAWN_SEC + asteroid_spawn_sec;
            asteroidslist.spawnAll();
        }

        // Player update
        float angle = Vector2Angle((Vector2){0, -1}, to_cursor);
        player.rotation = angle;

        if (Vector2Length(player.velocity) > MAX_PLAYER_VELOCITY)
        {
            player.velocity = Vector2Scale(Vector2Normalize(player.velocity), MAX_PLAYER_VELOCITY);
        }

        player.position = Vector2Add(Vector2Scale(player.velocity, GetFrameTime()), player.position);

        // Bullet update
        for (size_t i = 0; i < MAX_BULLET_COUNT; i++)
        {
            Bullet *bullet = (bullets + i);
            if (bullet->dead)
                continue;

            Vector2 velocity = Vector2Scale(bullet->direction, BULLET_VELOCITY);
            bullet->position = Vector2Add(bullet->position, Vector2Scale(velocity, GetFrameTime()));

            if (bullet->position.x < -20 || bullet->position.x > GetScreenWidth() + 20 || bullet->position.y < -20 || bullet->position.y > GetScreenHeight() + 20)
            {
                bullet->dead = true;
            }

            if (bullet->dead)
                continue;

            for (size_t j = 0; j < MAX_ASTEROIDS_COUNT; j++)
            {
                Asteroid *asteroid = (asteroidslist.asteroids + j);
                if (!asteroid->alive)
                    continue;

                Rectangle asteroid_rect = asteroidslist.get_asteroid_bounds(asteroid);
                if (!CheckCollisionPointRec(bullet->position, asteroid_rect))
                    continue;

                bullet->dead = true;
                asteroid->alive = false;

                score++;

                if (asteroid->is_big)
                {
                    float velocity_magnitude = Vector2Length(asteroid->velocity);
                    for (int i = 0; i < 4; i++)
                    {
                        Asteroid *child_asteroid = asteroidslist.spawn_asteroid(asteroid->position.x, asteroid->position.y, false);
                        if (child_asteroid == NULL)
                            break;

                        Vector2 velocity = {0, -1};
                        velocity = Vector2Rotate(velocity, (GetRandomValue(0, 100) / 100.0f) * PI * 2.0);
                        velocity = Vector2Scale(velocity, velocity_magnitude);
                        child_asteroid->velocity = velocity;
                    }
                }

                break;
            }
        }

        // Asteroid update
        for (size_t i = 0; i < MAX_ASTEROIDS_COUNT; i++)
        {
            Asteroid *asteroid = (asteroidslist.asteroids + i);
            if (!asteroid->alive)
                continue;

            asteroid->position = Vector2Add(asteroid->position, Vector2Scale(asteroid->velocity, GetFrameTime()));

            Rectangle asteroid_bounds = asteroidslist.get_asteroid_bounds(asteroid);

            if (CheckCollisionCircleRec(player.position, 5.0f, asteroid_bounds))
                gameover = true;
        }

        if (gameover)
            break;

        BeginDrawing();

        ClearBackground(BLACK);
        // Drawing out the player, bullets, asteroids
        player.draw_player();
        draw_bullets();
        asteroidslist.draw_asteroids();

        DrawText(TextFormat("Score: %d", score), 0, 0, 64, RED);

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
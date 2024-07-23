#include "lib/raylib.h"
#include "lib/raymath.h"
#include "Player/Player.hpp"
#include "Bullet/Bullet.hpp"
#include "Asteroid/AsteroidList.hpp"
#include "Asteroid/Asteroid.hpp"
#include "Bullet/Bullet.hpp"
#include "Bullet/BulletStack.hpp"
#include "game.hpp"

bool simulate(Game *game)
{
    Player *player = &game->player;
    Vector2 mouse_position = GetMousePosition();
    Vector2 to_cursor = Vector2Subtract(mouse_position, player->position);
    Vector2 player_dir = Vector2Normalize(to_cursor);
    bool gameover = false;
    bool spawned = false;

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        // TODO: make sure to cursor is not zero vector
        float a = player->acceleration * GetFrameTime();
        player->velocity = Vector2Add(Vector2Scale(player_dir, a), player->velocity);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Bullet *bullet = game->bulletstack.spawn_bullet();
        if (bullet)
        {
            bullet->position = player->position;
            bullet->direction = player_dir;
        }
    }

    game->asteroid_spawn_sec -= GetFrameTime();
    if (!spawned && game->asteroid_spawn_sec < 0)
    {
        spawned = true;
        game->asteroid_spawn_sec = ASTEROID_SPAWN_SEC + game->asteroid_spawn_sec;
        game->asteroidslist.spawnAll();
    }

    // Player update
    float angle = Vector2Angle((Vector2){0, -1}, to_cursor);
    player->rotation = angle;

    if (Vector2Length(player->velocity) > MAX_PLAYER_VELOCITY)
    {
        player->velocity = Vector2Scale(Vector2Normalize(player->velocity), MAX_PLAYER_VELOCITY);
    }

    player->position = Vector2Add(Vector2Scale(player->velocity, GetFrameTime()), player->position);

    // Wrap player
    if (player->position.x < 0)
    {
        player->position.x += GetScreenWidth();
    }
    else if (player->position.x >= GetScreenWidth())
    {
        player->position.x -= GetScreenWidth();
    }

    if (player->position.y < 0)
    {
        player->position.y += GetScreenHeight();
    }
    else if (player->position.y >= GetScreenHeight())
    {
        player->position.y -= GetScreenHeight();
    }

    // Bullet update
    for (size_t i = 0; i < MAX_BULLET_COUNT; i++)
    {
        Bullet *bullet = game->bulletstack.bullets;
        if (bullet->dead)
            continue;

        Vector2 velocity = Vector2Scale(bullet->direction, BULLET_VELOCITY);
        bullet->position = Vector2Add(bullet->position, Vector2Scale(velocity, GetFrameTime()));

        if (bullet->position.x < -20 || bullet->position.x > GetScreenWidth() + 20 ||
            bullet->position.y < -20 || bullet->position.y > GetScreenHeight() + 20)
        {
            bullet->dead = true;
        }

        if (bullet->dead)
            continue;

        for (size_t j = 0; j < game->asteroidslist.asteroid_count; j++)
        {
            Asteroid *asteroid = (game->asteroidslist.asteroids + j);

            bool bullet_in_asteroid = game->asteroidslist.is_colliding_with_asteroids(bullet->position, asteroid);
            if (!bullet_in_asteroid)
                continue;

            bullet->dead = true;

            game->score++;

            game->asteroidslist.asteroid_split(asteroid);
            game->asteroidslist.asteroid_die(j);
            break;
        }
    }

    // Asteroid update
    for (size_t i = 0; i < game->asteroidslist.asteroid_count; i++)
    {
        Asteroid *asteroid = (game->asteroidslist.asteroids + i);

        for (size_t j = 0; j < game->asteroidslist.asteroid_count; ++j)
        {
            if (j == i)
                continue;

            Asteroid *other = (asteroid + j);

            Vector2 collision_point;
            for (int p1 = 0; p1 < asteroid->point_count - 1; ++p1)
            {
                Vector2 a1 = Vector2Add(asteroid->points[p1], asteroid->position);
                Vector2 b1 = Vector2Add(asteroid->points[p1 + 1], asteroid->position);

                for (int p2 = 0; p2 < other->point_count - 1; ++p2)
                {
                    Vector2 a2 = Vector2Add(other->points[p2], other->position);
                    Vector2 b2 = Vector2Add(other->points[p2 + 1], other->position);
                    bool is_colliding = CheckCollisionLines(a1, b1, a2, b2, &collision_point);

                    if (is_colliding)
                    {
                        Vector2 relative = Vector2Subtract(asteroid->position, collision_point);
                        relative = Vector2Normalize(relative);

                        float speed = Vector2Length(asteroid->velocity);
                        asteroid->velocity = Vector2Scale(relative, speed);
                    }
                }
            }
        }

        asteroid->position = Vector2Add(asteroid->position, Vector2Scale(asteroid->velocity, GetFrameTime()));

        game->asteroidslist.wrap_asteroid(asteroid);
        bool player_hit_asteroid = game->asteroidslist.is_colliding_with_asteroids(game->player.position, asteroid);

        if (player_hit_asteroid)
        {
            gameover = true;
            game->asteroidslist.destroy_asteroids();
        }
    }

    return gameover;
}

int main(int argc, char **argv)
{
    // Declare the player in the main game
    Game game;

    game.score = 0;

    // Init the window
    InitWindow(800, 600, "Asteroids by Quang D. Nguyen");

    bool paused = false;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_P))
        {
            paused = !paused;
        }

        if (!paused)
        {
            bool gameover = simulate(&game);
            if (gameover)
            {
                break;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);
        // Drawing out the player, bullets, asteroids
        game.player.draw_player();
        game.bulletstack.draw_bullets();
        game.asteroidslist.draw_asteroids();

        DrawText(TextFormat("Score: %d", game.score), 0, 0, 64, RED);

        if (paused)
        {
            int text_size = 64;
            int text_width = MeasureText("Paused", text_size);
            DrawText("Paused", GetScreenWidth() / 2 - text_width / 2.0, GetScreenHeight() / 2 - text_size / 2.0, text_size, RED);
        }

        EndDrawing();
    }
    CloseWindow(); // Close window and OpenGL context

    return 0;
}

#include "lib/raylib.h"
#include "lib/raymath.h"
#include "Player/Player.hpp"
#include "Bullet/Bullet.hpp"
#include "Asteroid/AsteroidList.hpp"
#include "Asteroid/Asteroid.hpp"
#include "Bullet/Bullet.hpp"
#include "Bullet/BulletStack.hpp"

int main(int argc, char **argv)
{
    // Declare the player in the main game
    Player player;
    int score = 0;
    float asteroid_spawn_sec = ASTEROID_SPAWN_SEC;
    
    // Initialize asteroids
    AsteroidList asteroidslist;
    for (int i = 0; i < MAX_ASTEROIDS_COUNT; i++)
    {
        Asteroid asteroid;
        asteroidslist.append(asteroid);
    }

    // Initialize bullets
    BulletStack bulletstack;
    for (int i = 0; i < MAX_BULLET_COUNT; i++)
    {
        Bullet bullet;
        bulletstack.push(bullet);
    }

    // Init the window
    InitWindow(800, 600, "Asteroids by Quang D. Nguyen");

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
            Bullet *bullet = bulletstack.spawn_bullet();
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

        // Wrap player
        if (player.position.x < 0)
        {
            player.position.x += GetScreenWidth();
        }
        else if (player.position.x >= GetScreenWidth())
        {
            player.position.x -= GetScreenWidth();
        }

        if (player.position.y < 0)
        {
            player.position.y += GetScreenHeight();
        }
        else if (player.position.y >= GetScreenHeight())
        {
            player.position.y -= GetScreenHeight();
        }

        // Bullet update
        for (size_t i = 0; i < MAX_BULLET_COUNT; i++)
        {
            Bullet *bullet = bulletstack.bullets;
            if (bullet->dead)
                continue;

            Vector2 velocity = Vector2Scale(bullet->direction, BULLET_VELOCITY);
            bullet->position = Vector2Add(bullet->position, Vector2Scale(velocity, GetFrameTime()));

            if (bullet->position.x < -20 || bullet->position.x > GetScreenWidth() + 20 ||
                bullet->position.y < -20 || bullet->position.y > GetScreenHeight() + 20){
                bullet->dead = true;
            }

            if (bullet->dead)
                continue;

            for (size_t j = 0; j < asteroidslist.asteroid_count; j++)
            {
                Asteroid *asteroid = (asteroidslist.asteroids + j);

                bool bullet_in_asteroid = asteroidslist.is_colliding_with_asteroids(bullet->position, asteroid);
                if (!bullet_in_asteroid)
                    continue;

                bullet->dead = true;

                score++;
                
                asteroidslist.asteroid_split(asteroid);
                asteroidslist.asteroid_die(j);
                break;
            }
        }

        // Asteroid update
        for (size_t i = 0; i < asteroidslist.asteroid_count; i++)
        {
            Asteroid *asteroid = (asteroidslist.asteroids + i);

            for (size_t j = 0; j < asteroidslist.asteroid_count; ++j) {
              if (j == i) continue;

              Asteroid *other = (asteroid + j);

              Vector2 collision_point;
              for (int p1 = 0; p1< asteroid->point_count - 1; ++p1) {
                Vector2 a1 = Vector2Add(asteroid->points[p1], asteroid->position);
                Vector2 b1 = Vector2Add(asteroid->points[p1 + 1], asteroid->position);

                for (int p2 = 0; p2 < other->point_count - 1; ++p2) {
                  Vector2 a2 = Vector2Add(other->points[p2], other->position);
                  Vector2 b2 = Vector2Add(other->points[p2 + 1], other->position);
                  bool is_colliding = CheckCollisionLines( a1, b1, a2, b2, &collision_point);

                  if (is_colliding) {
                    Vector2 relative = Vector2Subtract(asteroid->position, collision_point);
                    relative = Vector2Normalize(relative);

                    float speed = Vector2Length(asteroid->velocity);
                    asteroid->velocity = Vector2Scale(relative, speed);  
                    
                  }
                }
              }
            }


            asteroid->position = Vector2Add(asteroid->position, Vector2Scale(asteroid->velocity, GetFrameTime()));

            asteroidslist.wrap_asteroid(asteroid);
            bool player_hit_asteroid = asteroidslist.is_colliding_with_asteroids(player.position, asteroid);

            if (player_hit_asteroid)
                gameover = true;
        }

        if (gameover)
            break;

        BeginDrawing();

        ClearBackground(BLACK);
        // Drawing out the player, bullets, asteroids
        player.draw_player();
        bulletstack.draw_bullets();
        asteroidslist.draw_asteroids();

        DrawText(TextFormat("Score: %d", score), 0, 0, 64, RED);

        EndDrawing();
    }
    CloseWindow(); // Close window and OpenGL context

    return 0;
}

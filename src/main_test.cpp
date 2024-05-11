#include "lib/raylib.h"
#include "lib/raymath.h"
#include "Player/Player.hpp"
#include "Bullet/Bullet.hpp"
#include "Asteroid/AsteroidList.hpp"
#include "Asteroid/Asteroid.hpp"
#include "Bullet/Bullet.hpp"
#include "Bullet/BulletStack.hpp"
#include "game.hpp"

typedef enum GameScreen
{
    TITLE,
    GAMEPLAY,
    ENDING
} GameScreen;

typedef enum transition
{
    SHOW,
    HIDE
} transition;

bool simulate(Game *game)
{
    Player *player = &game->player;

    Vector2 mouse_position = GetMousePosition();
    Vector2 to_cursor = Vector2Subtract(mouse_position, player->position);
    Vector2 player_dir = Vector2Normalize(to_cursor);
    bool gameover = false;

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
    if (game->asteroid_spawn_sec < 0)
    {
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

        if (bullet->position.x < -20 || bullet->position.x > GetScreenWidth() + 20 || bullet->position.y < -20 || bullet->position.y > GetScreenHeight() + 20)
        {
            bullet->dead = true;
        }

        if (bullet->dead)
            continue;

        for (size_t j = 0; j < game->asteroidslist.asteroid_count; j++)
        {
            Asteroid *asteroid = (game->asteroidslist.asteroids + j);

            Rectangle asteroid_rect = game->asteroidslist.get_asteroid_bounds(asteroid);
            if (!CheckCollisionPointRec(bullet->position, asteroid_rect))
                continue;

            bullet->dead = true;

            game->score++;

            if (asteroid->is_big)
            {
                float velocity_magnitude = Vector2Length(asteroid->velocity);
                for (int i = 0; i < 4; i++)
                {
                    Asteroid *child_asteroid = game->asteroidslist.spawn_asteroid(asteroid->position.x, asteroid->position.y, false);
                    if (child_asteroid == NULL)
                        break;

                    Vector2 velocity = {0, -1};
                    velocity = Vector2Rotate(velocity, (GetRandomValue(0, 100) / 100.0f) * PI * 2.0);
                    velocity = Vector2Scale(velocity, velocity_magnitude);
                    child_asteroid->velocity = velocity;
                }
            }
            game->asteroidslist.asteroid_die(j);
            break;
        }
    }

    // Asteroid update
    for (size_t i = 0; i < game->asteroidslist.asteroid_count; i++)
    {
        Asteroid *asteroid = (game->asteroidslist.asteroids + i);

        asteroid->position = Vector2Add(asteroid->position, Vector2Scale(asteroid->velocity, GetFrameTime()));

        Rectangle asteroid_bounds = game->asteroidslist.get_asteroid_bounds(asteroid);

        if (CheckCollisionCircleRec(player->position, 5.0f, asteroid_bounds))
            gameover = true;
    }

    return gameover;
}

int main(int argc, char **argv)
{
    // Create game object + Initialize windows
    bool gameover = false;
    int screenWidth = 800;
    int screenHeight = 600;
    GameScreen currentScreen = TITLE;
    int framesCounter = 0;
    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "Asteroids");
    int highScore;

    Game game;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        bool gameOver;

        switch (currentScreen)
        {
        case TITLE:
        {
            if (IsKeyPressed(KEY_SPACE))
                currentScreen = GAMEPLAY;
        }
        break;
        case GAMEPLAY:
        {
            gameOver = simulate(&game);
            if (gameOver == true)
            {
                highScore = game.score;
                currentScreen = ENDING;
            }
        }
        break;
        case ENDING:
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                game.score = 0;
                currentScreen = GAMEPLAY;
                gameOver = false;
                game.reset();
            }

            if (IsKeyPressed(KEY_H))
            {
                game.score = 0;
                currentScreen = TITLE;
                gameOver = false;
                game.reset();
            }
        }
        break;
        }
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentScreen)
        {
        case TITLE:
        {

            ClearBackground(BLACK);
            transition trans1;
            switch (trans1)
            {
            case SHOW:
            {
                DrawText("ASTEROIDS", 250, 200, 50, Color{210, 4, 45, 191});
                DrawText("PRESS SPACE TO START THE GAMEPLAY", 180, 300, 20, Color{170, 74, 68, 100});
                DrawText("PRESS SPACE TO START THE GAMEPLAY", 180, 300, 20, Color{170, 74, 68, 30});
                DrawText("PRESS SPACE TO START THE GAMEPLAY", 180, 300, 20, Color{170, 74, 68, 100});
                trans1 = HIDE;
            }
            break;
            case HIDE:
            {
                DrawText("ASTEROIDS", 250, 200, 50, Color{210, 4, 45, 191});
                DrawText("PRESS SPACE TO START THE GAMEPLAY", 180, 300, 20, Color{170, 74, 68, 30});
                DrawText("PRESS SPACE TO START THE GAMEPLAY", 180, 300, 20, Color{170, 74, 68, 100});
                trans1 = SHOW;
            }
            break;
            }
        }
        break;
        case GAMEPLAY:
        {
            ClearBackground(BLACK);
            game.player.draw_player();
            game.bulletstack.draw_bullets();
            game.asteroidslist.draw_asteroids();
            DrawText(TextFormat("Score: %d", game.score), 0, 0, 45, RED);
        }
        break;
        case ENDING:
        {
            transition trans1;
            switch (trans1)
            {
            case SHOW:
            {
                ClearBackground(BLACK);
                DrawText(TextFormat("YOUR HIGH SCORE IS: %d", highScore), 150, 250, 40, GREEN);
                DrawText(TextFormat("PRESS H TO COME BACK TO GO BACK TO HOME MENU"), 130, 320, 20, GRAY);
                DrawText(TextFormat("PRESS H TO COME BACK TO GO BACK TO HOME MENU"), 130, 320, 20, GRAY);
                DrawText(TextFormat("PRESS SPACE TO REPLAY THE GAME"), 210, 350, 20, GRAY);
                DrawText(TextFormat("PRESS SPACE TO REPLAY THE GAME"), 210, 350, 20, GRAY);
                trans1 = HIDE;
            }
            break;
            case HIDE:
            {
                ClearBackground(BLACK);
                DrawText(TextFormat("YOUR HIGH SCORE IS: %d", highScore), 150, 250, 40, GREEN);
                DrawText(TextFormat("PRESS H TO COME BACK TO GO BACK TO HOME MENU"), 130, 320, 20, GRAY);
                DrawText(TextFormat("PRESS H TO COME BACK TO GO BACK TO HOME MENU"), 130, 320, 20, GRAY);
                DrawText(TextFormat("PRESS SPACE TO REPLAY THE GAME"), 210, 350, 20, GRAY);
                DrawText(TextFormat("PRESS SPACE TO REPLAY THE GAME"), 210, 350, 20, GRAY);
                trans1 = SHOW;
            }
            break;
            }
        }
        break;
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "lib/raylib.h"
#include "lib/raymath.h"
#define MAX_PLAYER_VELOCITY 150

class Player
{
public:
    Vector2 position;
    float rotation;
    float acceleration;
    Vector2 velocity;

    Player()
    {
        position.x = 400;
        position.y = 300;
        rotation = PI / 2.0;

        acceleration = 100.0;
        velocity = Vector2Zero();
    }

    void draw_player()
    {
        Vector2 top = {0, -10};
        Vector2 left = {-10, 10};
        Vector2 right = {10, 10};

        top = Vector2Rotate(top, rotation);
        left = Vector2Rotate(left, rotation);
        right = Vector2Rotate(right, rotation);

        Vector2 t = Vector2Add(top, position);
        Vector2 l = Vector2Add(left, position);
        Vector2 r = Vector2Add(right, position);

        static Vector2 copy_postion[] = {
            (Vector2){0.0f, 0.0f},
            (Vector2){-static_cast<float>(GetScreenWidth()), 0.0f},
            (Vector2){static_cast<float>(GetScreenWidth()), 0.0f},
            (Vector2){0.0f, -static_cast<float>(GetScreenHeight())},
            (Vector2){0.0f, static_cast<float>(GetScreenHeight())},
        };

        for (int i = 0; i < sizeof(copy_postion) / sizeof(copy_postion[0]); i++)
        {
            Vector2 relative = copy_postion[i];

            DrawLineEx(Vector2Add(t, relative), Vector2Add(l, relative), 3.0f, BLUE);
            DrawLineEx(Vector2Add(t, relative), Vector2Add(r, relative), 3.0f, BLUE);
            DrawLineEx(Vector2Add(l, relative), Vector2Add(r, relative), 3.0f, BLUE);
        }

        DrawLineV(t, l, BLUE);
        DrawLineV(t, r, BLUE);
        DrawLineV(l, r, BLUE);
    }

    void reset()
    {
        position.x = 400;
        position.y = 300;
        rotation = PI / 2.0;
        acceleration = 100.0;
        velocity = Vector2Zero();
    }
};

#endif

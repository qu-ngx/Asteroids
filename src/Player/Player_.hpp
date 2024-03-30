#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "lib/raylib.h"
#include "lib/raymath.h"
#define MAX_PLAYER_VELOCITY 150

class Player_
{
public:
    Vector2 position;
    float rotation;
    float acceleration;
    Vector2 velocity;

    Player_()
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

        DrawLineV(t, l, GREEN);
        DrawLineV(t, r, GREEN);
        DrawLineV(l, r, GREEN);
    }
};

#endif
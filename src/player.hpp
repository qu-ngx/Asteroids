#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raymath.h"
class Player
{
private:
public:
    Vector2 position;
    float rotation;
    float acceleration;
    Vector2 velocity;

    // Testing feature (Refactor)
    // void init_player()
    // {
    //     Player player;
    //     player.position.x = 400;
    //     player.position.y = 300;
    //     player.rotation = PI / 2.0;

    //     player.acceleration = 100.0;
    //     player.velocity = Vector2Zero();
    // }

    // void draw_player()
    // {
    //     Player player;
    //     Vector2 top = {0, -10};
    //     Vector2 left = {-10, 10};
    //     Vector2 right = {10, 10};

    //     top = Vector2Rotate(top, player.rotation);
    //     left = Vector2Rotate(left, player.rotation);
    //     right = Vector2Rotate(right, player.rotation);

    //     Vector2 t = Vector2Add(top, player.position);
    //     Vector2 l = Vector2Add(left, player.position);
    //     Vector2 r = Vector2Add(right, player.position);

    //     DrawLineV(t, l, GREEN);
    //     DrawLineV(t, r, GREEN);
    //     DrawLineV(l, r, GREEN);
    // }
};

#endif
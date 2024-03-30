#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "raymath.h"
#include "raylib.h"
#define MAX_ASTEROIDS_COUNT 256
#define ASTEROIDS_BIG_WH 32
#define ASTEROIDS_SMALL_WH 16

class Asteroid_
{
public:
    Vector2 velocity;
    Vector2 position;
    bool alive;
    bool is_big;

    Asteroid_()
    {
        alive = false;
        is_big = false;
        position = {0.0f, 0.0f};
        velocity = {0.0f, 0.0f};
    }
};

#endif
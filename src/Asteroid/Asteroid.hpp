#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "lib/raymath.h"
#define MAX_ASTEROIDS_COUNT 256
#define ASTEROIDS_BIG_WH 32
#define ASTEROIDS_SMALL_WH 16
#define ASTEROID_SPAWN_SEC 0.5f

class Asteroid
{
public:
    Vector2 velocity;
    Vector2 position;
    bool alive;
    bool is_big;

    Asteroid()
    {
        alive = false;
        is_big = false;
        position = Vector2Zero();
        velocity = Vector2Zero();
    }
};

#endif
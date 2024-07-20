#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "lib/raymath.h"
#define MAX_ASTEROIDS_COUNT 256
#define ASTEROIDS_BIG_WH 32
#define ASTEROIDS_SMALL_WH 16
#define ASTEROID_SPAWN_SEC 0.5f
#define MAX_ASTEROIDS_VERTS 12

class Asteroid
{
public:
    Vector2 velocity;
    Vector2 position;
    int point_count;
   Vector2 points[MAX_ASTEROIDS_VERTS];

    Asteroid()
    {
        position = Vector2Zero();
        velocity = Vector2Zero();
    }
};

#endif

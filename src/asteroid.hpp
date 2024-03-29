#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "raymath.h"

class Asteroid
{

public:
    Vector2 position;
    Vector2 velocity;
    bool is_big;
    bool alive;
};

#endif
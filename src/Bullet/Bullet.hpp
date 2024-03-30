#ifndef BULLET_HPP
#define BULLET_HPP

#include "lib/raymath.h"
// Define some constants
#define MAX_BULLET_COUNT 100
#define BULLET_VELOCITY 1000.0

class Bullet
{
public:
    bool dead;
    Vector2 position;
    Vector2 direction;
};

#endif
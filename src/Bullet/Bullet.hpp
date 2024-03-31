#ifndef BULLET_HPP
#define BULLET_HPP

#include "lib/raymath.h"

class Bullet
{
public:
    bool dead;
    Vector2 position;
    Vector2 direction;

    Bullet()
    {
        position = Vector2Zero();
        direction = Vector2Zero();
        dead = true;
    }
};

#endif
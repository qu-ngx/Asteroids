#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <raymath.h>
class Player
{
private:
public:
    Vector2 position;
    float rotation;
    float acceleration;
    Vector2 velocity;
};

#endif
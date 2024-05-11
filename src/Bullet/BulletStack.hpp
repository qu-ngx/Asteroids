#ifndef BULLET_STACK_HPP
#define BULLET_STACK_HPP

#include <iostream>
#include "Bullet.hpp"
#include "lib/raymath.h"
#include "lib/raylib.h"
using namespace std;

#define MAX_BULLET_COUNT 100
#define BULLET_VELOCITY 40.0

class BulletStack
{
private:
    int topIndex;
    int stackSize;
    int capacity;
    void increaseCapacity()
    {
        capacity *= 2;
        Bullet *newStack = new Bullet[capacity];
        for (int i = 0; i < stackSize; i++)
        {
            newStack[i] = bullets[i];
        }
        delete[] bullets;
        bullets = newStack;
    }

public:
    Bullet *bullets;
    int bullet_count = 0;

    BulletStack()
    {
        capacity = MAX_BULLET_COUNT;
        bullets = new Bullet[capacity];
        topIndex = 0;
        stackSize = 0;
    }

    ~BulletStack()
    {
        delete[] bullets;
    }

    void push(const Bullet &bullet)
    {
        if (stackSize == capacity)
        {
            increaseCapacity();
        }
        bullets[topIndex++] = bullet;
        stackSize++;
    }

    Bullet pop()
    {
        if (stackSize == 0)
        {
            return Bullet(); // This will return a default-constructed card
        }

        Bullet poppedCard = bullets[stackSize - 1]; // Get the top card
        stackSize--;                                // Update stackSize
        return poppedCard;
        return *bullets;
    }

    Bullet *spawn_bullet()
    {
        for (size_t i = 0; i < capacity; i++)
        {
            if (bullets)
            {
                if (!bullets->dead)
                    continue;
                bullets->dead = false;
                return bullets;
            }
            pop();
        }
        return NULL;
    }

    void draw_bullets()
    {
        for (size_t i = 0; i < capacity; i++)
        {
            Bullet bullet = *bullets;
            if (bullets)
            {
                if (bullets->dead)
                    continue;
            }
            DrawLineV(bullet.position, Vector2Add(bullet.position, Vector2Scale(bullet.direction, -10)), WHITE);
            pop();
        }
    }
};

#endif
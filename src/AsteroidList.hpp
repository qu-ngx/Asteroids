#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Asteroid.hpp"

#define MAX_ASTEROIDS_COUNT 256
#define ASTEROIDS_BIG_WH 32
#define ASTEROIDS_SMALL_WH 16
#define ASTEROID_SPAWN_SEC 0.5f

class AsteroidList
{

private:
    int capacity;
    int size;
    void increaseCapacity()
    {
        int new_capacity = 2 * capacity;
        Asteroid *new_array = new Asteroid[new_capacity]();
        for (int i = 0; i < size; ++i)
        {
            new_array[i] = asteroids[i];
        }
        delete[] asteroids;
        asteroids = new_array;
        capacity = new_capacity;
    }

public:
    Asteroid *asteroids;

    AsteroidList()
    {
        capacity = 256; // Initial capacity
        asteroids = new Asteroid[capacity]();
        size = 0;
    }

    ~AsteroidList()
    {
        delete[] asteroids;
    }

    void insert(Asteroid value, int index)
    {
        size++;
        if (size == capacity)
        {
            increaseCapacity();
        }
        for (int i = size - 1; i >= index; i--)
        {
            asteroids[i + 1] = asteroids[i];
        }
        asteroids[index] = value;
    }

    void append(Asteroid value)
    {
        if (size == capacity)
        {
            increaseCapacity();
        }
        asteroids[size] = value;
        size++;
    }

    Rectangle get_asteroid_bounds(Asteroid *asteroid)
    {
        float asteroid_size = ASTEROIDS_SMALL_WH;
        if (asteroid->is_big)
            asteroid_size = ASTEROIDS_BIG_WH;

        return (Rectangle){
            .x = asteroid->position.x - asteroid_size / 2,
            .y = asteroid->position.y - asteroid_size / 2,
            .width = asteroid_size,
            .height = asteroid_size,
        };
    }

    void draw_asteroids()
    {
        for (size_t i = 0; i < capacity; i++)
        {
            Asteroid *asteroid = (asteroids + i);
            if (asteroid)
            {
                if (!asteroid->alive)
                    continue;

                Rectangle asteroid_rect = get_asteroid_bounds(asteroid);

                DrawRectangleLinesEx(asteroid_rect, 1.0, RED);
            }
        }
    }

    Asteroid *spawn_asteroid(int x, int y, bool is_big)
    {
        Asteroid *asteroid = NULL;
        for (size_t i = 0; i < MAX_ASTEROIDS_COUNT; i++)
        {
            Asteroid *asteroid_ = (asteroids + i);
            if (asteroid_->alive)
                continue;
            asteroid = asteroid_;
            break;
        }

        if (asteroid == NULL)
            return NULL;

        asteroid->alive = true;
        asteroid->position.x = x;
        asteroid->position.y = y;
        asteroid->is_big = is_big;
        return asteroid;
    }

    void spawnAll()
    {
        bool is_big = GetRandomValue(0, 1);

        Vector2 random_direction = Vector2Rotate((Vector2){0, -1}, PI * 2 * GetRandomValue(0, 100) / 100.0);
        Vector2 signs = {static_cast<float>(random_direction.x >= 0 ? 1 : -1), static_cast<float>(random_direction.y >= 0 ? 1 : -1)};
        Vector2 relative = (Vector2){
            (GetScreenWidth() / 2 + 50) * -signs.x,
            (GetScreenHeight() / 2 + 50) * -signs.y};

        Vector2 screen_middle = {static_cast<float>(GetScreenWidth() / 2.0f), static_cast<float>(GetScreenHeight() / 2.0f)};
        Vector2 spawn_postion = Vector2Add(screen_middle, relative);

        Asteroid *asteroid = spawn_asteroid(spawn_postion.x, spawn_postion.y, is_big);

        if (asteroid != NULL)
        {
            asteroid->velocity = Vector2Scale(random_direction, 100.0f);
        }
    }
};

#endif
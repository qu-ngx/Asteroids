#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Asteroid_.hpp"

class AsteroidList
{

private:
    int capacity;
    int size;
    void increaseCapacity()
    {
        int new_capacity = 2 * capacity;
        Asteroid_ *new_array = new Asteroid_[new_capacity]();
        for (int i = 0; i < size; ++i)
        {
            new_array[i] = asteroids[i];
        }
        delete[] asteroids;
        asteroids = new_array;
        capacity = new_capacity;
    }

public:
    Asteroid_ *asteroids;

    AsteroidList()
    {
        capacity = 256; // Initial capacity
        asteroids = new Asteroid_[capacity]();
        size = 0;
    }

    ~AsteroidList()
    {
        delete[] asteroids;
    }

    void insert(Asteroid_ value, int index)
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

    void append(Asteroid_ value)
    {
        if (size == capacity)
        {
            increaseCapacity();
        }
        asteroids[size] = value;
        size++;
    }

    float getX(int j)
    {
        Asteroid_ *asteroid = (asteroids + j);
        return asteroid->position.x;
    }

    float getY(int j)
    {
        Asteroid_ *asteroid = (asteroids + j);
        return asteroid->position.y;
    }

    Rectangle get_asteroid_bounds(Asteroid_ *asteroid)
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
            Asteroid_ *asteroid = (asteroids + i);
            if (asteroid)
            {
                if (!asteroid->alive)
                    continue;

                Rectangle asteroid_rect = get_asteroid_bounds(asteroid);

                DrawRectangleLinesEx(asteroid_rect, 1.0, RED);
            }
        }
    }

    Asteroid_ *spawn_asteroid(int x, int y, bool is_big)
    {
        Asteroid_ *asteroid = NULL;
        for (size_t i = 0; i < MAX_ASTEROIDS_COUNT; i++)
        {
            Asteroid_ *asteroid_ = (asteroids + i);
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

    void changeVelo(int j, Vector2 random_direction)
    {
        Asteroid_ *asteroid = (asteroids + j);
        asteroid->alive = false;
        if (&asteroid != NULL)
        {
            asteroid->velocity = Vector2Scale(random_direction, 100.0f);
        }
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

        Asteroid_ *asteroid = spawn_asteroid(spawn_postion.x, spawn_postion.y, is_big);

        if (asteroid != NULL)
        {
            asteroid->velocity = Vector2Scale(random_direction, 100.0f);
        }
    }
};

#endif
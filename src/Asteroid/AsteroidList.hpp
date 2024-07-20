#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <iostream>
#include "lib/raylib.h"
#include "lib/raymath.h"
#include "Asteroid.hpp"
#include "../Player/Player.hpp"
#include <cassert>

#define MAX_ASTEROIDS_COUNT 256
#define ASTEROIDS_BIG_WH 32
#define ASTEROIDS_SMALL_WH 16
#define ASTEROID_SPAWN_SEC 0.5f
#define MAX_ASTEROIDS_VERTS 12

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
    int asteroid_count = 0;
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

    void asteroid_die(int index)
    {
        asteroids[index] = asteroids[asteroid_count - 1];
        asteroid_count--;
    }

    bool is_colliding_with_asteroids(Vector2 position , Asteroid *asteroid)
    {
     Vector2 local_to_asteroid = Vector2Subtract(position, asteroid->position);
     return CheckCollisionPointPoly(local_to_asteroid, asteroid->points, asteroid->point_count);
    }


    void draw_asteroids()
    {
        Vector2 copy_positions[] = {
          (Vector2) { 0.0f, 0.0f },
          // (Vector2) { - static_cast<float>(GetScreenWidth()), 0.0f },
          // (Vector2) { static_cast<float>(GetScreenWidth()), 0.0f },
          // (Vector2) { 0.0f, - static_cast<float>(GetScreenHeight()) },
          // (Vector2) { 0.0f, static_cast<float>(GetScreenHeight()) },
        };

        for (size_t i = 0; i < asteroid_count; i++)
        {
            Asteroid *asteroid = (asteroids + i);
            if (asteroid)
            {
                for (int p = 0; p < asteroid->point_count - 1; ++p) {
                  for (int j = 0; j < sizeof(copy_positions)/sizeof(copy_positions[0]); ++j) {
                    Vector2 p1 = Vector2Add(asteroid->points[p] , asteroid->position);
                    Vector2 p2 = Vector2Add(asteroid->points[p + 1], asteroid->position);
                    DrawLineEx(Vector2Add(p1, copy_positions[j]), Vector2Add(p2, copy_positions[j]), 3.0, RED);
                  }
                }
            }
        }

    }

    Asteroid *spawn_asteroid(int x, int y, bool is_big)
    {
        if (asteroid_count >= MAX_ASTEROIDS_COUNT)
            return NULL;

        Asteroid *asteroid = (asteroids + asteroid_count);
        asteroid_count++;

        if (asteroid == NULL)
            return NULL;

        asteroid->position.x = x;
        asteroid->position.y = y;

        int sides = GetRandomValue(5, MAX_ASTEROIDS_VERTS - 1);
        asteroid->point_count = sides + 1;
        float r = is_big ? ASTEROIDS_BIG_WH : ASTEROIDS_SMALL_WH;

        // Get the angles and the edges of the polygon
        for (int i = 0; i < sides; ++i) {
          asteroid->points[i].x = r * cosf(2.0f*PI*i/sides) + x;
          asteroid->points[i].y = r * sin(2.0f*PI*i/sides) + y;

          asteroid->points[i].x += (GetRandomValue(-100, 100) / 100.0) * 5.0f;
          asteroid->points[i].y += (GetRandomValue(-100, 100) / 100.0) * 5.0f;
        }

        asteroid->points[sides] = asteroid->points[0];

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

    void asteroid_reclaim(int index)
    {
        assert(index < asteroid_count && "should only be called on live asteroids");
        asteroids[index] = asteroids[asteroid_count - 1];
        asteroid_count--;
    }

    void destroy_asteroids(int i)
    {
        Asteroid *asteroid = (asteroids + i);
        // Boom animatio (To be added later)
        asteroid_reclaim(i);
    }

    void destroy_asteroids()
    {
        for (size_t i = 0; i < asteroid_count; i++)
        {
            destroy_asteroids(i);
        }
    }
};

#endif

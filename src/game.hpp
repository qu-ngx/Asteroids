#include "lib/raylib.h"
#include "lib/raymath.h"
#include "Player/Player.hpp"
#include "Bullet/Bullet.hpp"
#include "Asteroid/AsteroidList.hpp"
#include "Asteroid/Asteroid.hpp"
#include "Bullet/Bullet.hpp"
#include "Bullet/BulletStack.hpp"

class Game
{
public:
    Player player;
    int score = 0;
    float asteroid_spawn_sec;
    BulletStack bulletstack;
    AsteroidList asteroidslist;

    Game()
    {
        score = 0;
        asteroid_spawn_sec = ASTEROID_SPAWN_SEC;

        for (int i = 0; i < MAX_BULLET_COUNT; i++)
        {
            Bullet bullet;
            bulletstack.push(bullet);
        }

        for (int i = 0; i < MAX_ASTEROIDS_COUNT; i++)
        {
            Asteroid asteroid;
            asteroidslist.append(asteroid);
        }
    }

    void reset()
    {
        score = 0;

        // reinit players, asteroids,
        player.position.x = 400;
        player.position.y = 300;
        player.rotation = PI / 2.0;

        player.acceleration = 100.0;
        player.velocity = Vector2Zero();
    }
};
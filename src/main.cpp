/***********************************
 * CSC 122 Sample Raylib application.
 * A game object chases the points at the front of a queue.
 *
 * name: Bekam Guta and Quang Nugyen
 */
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    Color depauwBlack = Color{35, 31, 32, 255};
    Color depauwGold = Color{253, 221, 6, 255};

    InitWindow(screenWidth, screenHeight, "Welcome to DodFight");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        BeginDrawing();
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
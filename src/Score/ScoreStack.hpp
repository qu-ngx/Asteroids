#ifndef Score_STACK_HPP
#define Score_STACK_HPP

#include <iostream>
#include "Score.hpp"

using namespace std;

class ScoreStack
{
private:
    int topIndex;
    int stackSize;
    int capacity;
    void increaseCapacity()
    {
        capacity *= 2;
        Score *newStack = new Score[capacity];
        for (int i = 0; i < stackSize; i++)
        {
            newStack[i] = stack[i];
        }
        delete[] stack;
        stack = newStack;
    }

public:
    Score *stack;

    ScoreStack()
    {
        capacity = 10;
        stack = new Score[capacity];
        topIndex = 0;
        stackSize = 0;
    }

    ~ScoreStack()
    {
        delete[] stack;
    }

    void push(const Score &card)
    {
        if (stackSize == capacity)
        {
            increaseCapacity();
        }
        stack[topIndex++] = card;
        stackSize++;
    }

    Score pop()
    {
        if (stackSize == 0)
        {
            return Score(0, ""); // This will return a default-constructed card
        }

        // It will shift elements to the left
        for (int i = topIndex; i < stackSize; i++)
        {
            stack[i] = stack[i + 1];
        }
        stackSize--;   // Update stackSize
        return *stack; // Return the popped card
    }

    bool isEmpty() const
    {
        return stackSize == 0;
    }

    bool isFull() const
    {
        return stackSize == capacity;
    }

    int size() const
    {
        return stackSize;
    }
};

#endif
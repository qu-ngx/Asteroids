#ifndef SCORE_HPP
#define SCORE_HPP

#include <iostream>

using namespace std;

class Score
{
public:
    int number;
    string suit;

    Score()
    {
        number = -1;
        suit = "";
    }

    Score(int newNumber, string newSuit)
    {
        number = newNumber;
        suit = newSuit;
    }

    void setNumber(int anotherNumber)
    {
        number = anotherNumber;
    }

    void setSuit(string anotherSuit)
    {
        suit = anotherSuit;
    }

    int getNumber()
    {
        return number;
    }

    string getSuit()
    {
        return suit;
    }

    void display() const
    {
        cout << "Number: " << number << ", Suit: " << suit << endl;
    }
};

#endif

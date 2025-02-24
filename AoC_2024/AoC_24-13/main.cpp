#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#define MAX_INPUTS 100
#define LOC_MODIFIER 10000000000000

void Part1();
void Part2();

int main()
{
    Part1();
    Part2();
    return 0;
}

void Part1()
{
    using namespace std;
    // read inputs 3 lines at a time to pull values.

    // Process inputs trying to figure out if it's possible to reach the prize location.

    // Try to get as close to the location as possible with 'B' button, then iterate through possible combinations with 'A' to see if we can reach the location.

    int Ax = 0, Bx = 0, Ay = 0, By = 0, Targetx = 0, Targety = 0, APresses = 0, BPresses = 0;
    int Testx = 0, Testy = 0;
    int TotalCoins = 0;
    bool bFoundMatch = false;

    regex ValueRegex("\\d+");
    sregex_iterator RegBegin;

    string inputLine;
    ifstream data("input.txt");
    while (getline(data, inputLine))
    {
        RegBegin = sregex_iterator(inputLine.begin(), inputLine.end(), ValueRegex);
        Ax = stoi((*RegBegin).str());
        Ay = stoi((*++RegBegin).str());

        getline(data, inputLine);
        RegBegin = sregex_iterator(inputLine.begin(), inputLine.end(), ValueRegex);
        Bx = stoi((*RegBegin).str());
        By = stoi((*++RegBegin).str());

        getline(data, inputLine);
        RegBegin = sregex_iterator(inputLine.begin(), inputLine.end(), ValueRegex);
        Targetx = stoi((*RegBegin).str());
        Targety = stoi((*++RegBegin).str());

        BPresses = min(Targety / By, MAX_INPUTS);
        bFoundMatch = false;
        while (BPresses >= 0)
        {
            APresses = 0;
            while (APresses < MAX_INPUTS)
            {
                Testx = BPresses * Bx + APresses * Ax;
                Testy = BPresses * By + APresses * Ay;

                if (Testx == Targetx && Testy == Targety)
                {
                    TotalCoins += BPresses + APresses * 3;
                    bFoundMatch = true;
                    break;
                }
                else if (Testx > Targetx || Testy > Targety)
                    break;

                ++APresses;
            }

            if (bFoundMatch)
                break;

            --BPresses;
        }
        
        // Empty line
        getline(data, inputLine);
    }

    cout << "Part 1 coins: " << TotalCoins << "\n";
}

void Part2 ()
{
    using namespace std;

    int Ax = 0, Bx = 0, Ay = 0, By = 0;
    long long Targetx = 0, Targety = 0, Testx = 0, Testy = 0, Det = 0, TotalCoins = 0;
    bool bFoundMatch = false;

    regex ValueRegex("\\d+");
    sregex_iterator RegBegin;

    string inputLine;
    ifstream data("input.txt");
    while (getline(data, inputLine))
    {
        RegBegin = sregex_iterator(inputLine.begin(), inputLine.end(), ValueRegex);
        Ax = stoi((*RegBegin).str());
        Ay = stoi((*++RegBegin).str());

        getline(data, inputLine);
        RegBegin = sregex_iterator(inputLine.begin(), inputLine.end(), ValueRegex);
        Bx = stoi((*RegBegin).str());
        By = stoi((*++RegBegin).str());

        getline(data, inputLine);
        RegBegin = sregex_iterator(inputLine.begin(), inputLine.end(), ValueRegex);
        Targetx = stoi((*RegBegin).str()) + LOC_MODIFIER;
        Targety = stoi((*++RegBegin).str()) + LOC_MODIFIER;

        Det = Ax * By - Ay * Bx;
        if (Det == 0)
            continue;

        Testx = (Targetx * By - Targety * Bx);
        Testy = (Targety * Ax - Targetx * Ay);

        if (Testx % Det == 0 && Testy % Det == 0)
        {
            Testx /= Det;
            Testy /= Det;
            if (Testx > 0 && Testy > 0)
                TotalCoins += Testx * 3 + Testy;
        }
        
        // Empty line
        getline(data, inputLine);
    }

    cout << "Part 2 coins: " << TotalCoins << "\n";
}
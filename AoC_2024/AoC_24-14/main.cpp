#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <cstdlib>

#define ITERATIONS 100
#define WIDTH 101
#define HEIGHT 103
#define BOTLISTSIZE WIDTH * HEIGHT * 4

int* ReadData(int* const BotList);
void PredictBots(int* const BotList, int* const BotListEnd, int* Quadrants, int (&Grid)[WIDTH][HEIGHT], int Iterations = ITERATIONS);
bool ScanGrid(int (&Grid)[WIDTH][HEIGHT]);

int main()
{
    int BotList[BOTLISTSIZE];
    int Quadrants[4] = {0};
    int Grid[WIDTH][HEIGHT] = {0};

    int* ListEnd = ReadData(BotList);
    PredictBots(BotList, ListEnd, Quadrants, Grid);

    int SafetyFactor = 1;
    for (int i = 0; i < 4; ++i)
        SafetyFactor *= Quadrants[i];

    std::cout << "Safety Factor: " << SafetyFactor;

    int i = -1;
    do
    {
        ++i;
        std::memset(Grid, 0, sizeof(Grid));
        PredictBots(BotList, ListEnd, Quadrants, Grid, i);
        
    } while (!ScanGrid(Grid) && i < BOTLISTSIZE);

    for (int y = 0; y < HEIGHT; ++y)
    {
        std::cout << "\n";
        for (int x = 0; x < WIDTH; ++x)
        {
            std::cout << (Grid[x][y] == 1 ? "0" : " ");
        }
    }

    std::cout << "\nFound Image at: " << i << " seconds";
    
    return 0;
}

int* ReadData(int* const BotList)
{
    using namespace std;

    int* CurrentLoc = BotList - 1;

    regex ValueRegex("-?\\d+");
    sregex_iterator RegBegin;

    string inputLine;
    ifstream data("input.txt");
    while (getline(data, inputLine))
    {
        if (CurrentLoc - BotList > BOTLISTSIZE - 1)
        {
            cout << "Tried to read too many bots!";
            return CurrentLoc;
        }
        
        // pull our four values for each bot
        RegBegin = sregex_iterator(inputLine.begin(), inputLine.end(), ValueRegex);
        *++CurrentLoc = stoi((*RegBegin).str());
        *++CurrentLoc = stoi((*++RegBegin).str());
        *++CurrentLoc = stoi((*++RegBegin).str());
        *++CurrentLoc = stoi((*++RegBegin).str());
    }

    return CurrentLoc;
}

void PredictBots(int* const BotList, int* const BotListEnd, int* Quadrants, int (&Grid)[WIDTH][HEIGHT], int Iterations)
{
    int NumBots = (BotListEnd - BotList + 1) / 4;
    int BotX = 0, BotY = 0;
    int BotIdx = 0;
    int QuadWidth = WIDTH / 2;
    int QuadHeight = HEIGHT / 2;

    for (int i = 0; i < NumBots; ++i)
    {
        BotIdx = i * 4;
        BotX = ((BotList[BotIdx] + BotList[BotIdx + 2] * Iterations) % WIDTH + WIDTH) % WIDTH;
        BotY = ((BotList[BotIdx + 1] + BotList[BotIdx + 3] * Iterations) % HEIGHT + HEIGHT) % HEIGHT;

        if (BotX == QuadWidth ||
            BotY == QuadHeight)
            continue;

        ++Quadrants[(BotX < QuadWidth) * 1 + (BotY < QuadHeight) * 2];

        ++Grid[BotX][BotY];
    }
}

bool ScanGrid(int (&Grid)[WIDTH][HEIGHT])
{
    for (int j = 0; j < WIDTH; ++j)
    {
        for (int k = 0; k < HEIGHT; ++k)
        {
            if (Grid[j][k] > 1)
            {
                return false;
            }
        }
    }

    return true;
}